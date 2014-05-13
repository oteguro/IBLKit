// ----------------------------------------------------------------------------
// iblkit.cpp
// ----------------------------------------------------------------------------
#include "iblkit.h"
#include <DirectXMath.h>

#include <cassert>
#include <algorithm>

#include "./_IMPL/scope_exit.h"

#include "../shaders/compiled/cubemap_filter.h"

#define  TILE_SIZE_X        (8)
#define  TILE_SIZE_Y        (8)

namespace iblkit
{
    namespace
    {
        void GetCubemapWidthHeightMipCount(ID3D11Texture2D* cubemap, UINT& width, UINT& height, UINT& mipCount)
        {
            if (!cubemap)
            {
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            cubemap->GetDesc(&desc);

            width    = desc.Width;
            height   = desc.Height;

            UINT texel = std::max<UINT>(width, height);
            while(texel) { mipCount++; texel>>=1; }
        }

        bool IsCubemap(ID3D11Texture2D* cubemap)
        {
            if (!cubemap)
            {
                return false;
            }

            D3D11_TEXTURE2D_DESC desc;
            cubemap->GetDesc(&desc);

            if(desc.ArraySize!=6)
            {
                return false;
            }

            return (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) ? true : false;
        }

        void FillSubResourceDataFp16Cubemap(D3D11_SUBRESOURCE_DATA*    initData,
                                            UINT                       width,
                                            UINT                       height,
                                            UINT                       arrayCount,
                                            UINT                       mipCount,
                                            void*                      pMem,
                                            size_t                     poolSize)
        {
            UINT index=0;
            void* pM = pMem;
            size_t consumeSize = 0;

            for(UINT a=0; a<arrayCount; ++a)
            {
                UINT w = width;
                UINT h = height;

                for(UINT m=0; m<mipCount; ++m)
                {
                    UINT RowBytes = sizeof(uint16_t) * 4 * w;
                    UINT NumBytes = RowBytes * h;

                    initData[index].pSysMem             = (const void*)(pM);
                    initData[index].SysMemPitch         = RowBytes;
                    initData[index].SysMemSlicePitch    = NumBytes;
                    index++;

                    consumeSize += NumBytes;
                    if(consumeSize>poolSize)
                    {
                        __debugbreak();
                    }

                    w >>= 1;
                    h >>= 1;

                    if(w==0)
                    {
                        w=1;
                    }
                    if(h==0)
                    {
                        h=1;
                    }

                    pM = (void*)((uintptr_t)pM + (uintptr_t)NumBytes);
                }
            }
        }

        HRESULT CreateComputeShader(ID3D11Device* device, const BYTE* buffer, size_t bufferSize, ID3D11ComputeShader** cs)
        {
            HRESULT hr;

            hr = device->CreateComputeShader(buffer, bufferSize, NULL, cs);
            if (FAILED(hr))
            {
                return hr;
            }

            return hr;
        }

    } // unnamed namespace 

    struct BufferParameter
    {
        float m_faceVector  [4];
        float m_faceTangent [4];
        float m_faceBinormal[4];
        float m_texelOffset [4];
        float m_roughness   [4];
        UINT  m_uavSize[4];

    }; // struct BufferParameter 

    // Start. 
    bool FilteringCubemap(Context*                       context,
                          ID3D11Texture2D*               inCubemap,
                          ID3D11Texture2D**              outCubemap)
    {
        if(!context)
        {
            return false;
        }
        if(!context->m_d3dDevice || !context->m_d3dImContext)
        {
            return false;
        }
        if (context->m_mode != kMode_None)
        {
            return false;
        }
        if (context->m_state != kState_Ready)
        {
            return false;
        }
        if(!inCubemap)
        {
            return false;
        }
        if(!outCubemap)
        {
            return false;
        }
        if(!IsCubemap(inCubemap))
        {
            return false;
        }

        D3D11_TEXTURE2D_DESC tex2DDesc;
        inCubemap->GetDesc ( &tex2DDesc);

        if(context->m_filterCS == nullptr)
        {
            HRESULT hr = CreateComputeShader(context->m_d3dDevice, &cubemap_filter[0], sizeof(cubemap_filter), &context->m_filterCS);
            if(FAILED(hr))
            {
                return false;
            }
        }

        UINT width = 0, height = 0, mipCount = 0;
        const UINT arrayCount = 6;
        GetCubemapWidthHeightMipCount(inCubemap, width, height, mipCount);
        if(width==0 || height==0 || mipCount==0)
        {
            return 0;
        }

        ID3D11Device* d = context->m_d3dDevice;
        assert(d);

        if(*outCubemap == nullptr)
        {
            D3D11_TEXTURE2D_DESC desc;
            desc.Width              = width;
            desc.Height             = height;
            desc.MipLevels          = mipCount;
            desc.ArraySize          = 6;
            desc.Format             = DXGI_FORMAT_R16G16B16A16_FLOAT; // FIXED. 
            desc.SampleDesc.Count   = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage              = D3D11_USAGE_DEFAULT;
            desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
            desc.CPUAccessFlags     = 0;
            desc.MiscFlags          = D3D11_RESOURCE_MISC_TEXTURECUBE;

            const size_t poolSize   = sizeof(uint16_t) * 4 * width * height * arrayCount * 2;

            void* pMem = malloc(poolSize);
            SCOPE_EXIT(free(pMem););
            ZeroMemory(pMem, poolSize);

            D3D11_SUBRESOURCE_DATA* initData = new D3D11_SUBRESOURCE_DATA [arrayCount * mipCount];
            SCOPE_EXIT(delete [] initData;);
            FillSubResourceDataFp16Cubemap(initData, width, height, arrayCount, mipCount, pMem, poolSize);

            HRESULT hr = d->CreateTexture2D(&desc, initData, outCubemap);
            if(FAILED(hr))
            {
                return false;
            }
        }

        context->m_uavCount = mipCount * arrayCount;
        context->m_outCubemap = new ID3D11UnorderedAccessView* [context->m_uavCount];

        for(UINT i=0; i<context->m_uavCount; ++i)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
            desc.Format                         = DXGI_FORMAT_R16G16B16A16_FLOAT;
            desc.ViewDimension                  = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
            desc.Texture2DArray.MipSlice        = i / arrayCount;
            desc.Texture2DArray.FirstArraySlice = i % arrayCount;
            desc.Texture2DArray.ArraySize       = 1;

            HRESULT hr = d->CreateUnorderedAccessView(*outCubemap, &desc, &context->m_outCubemap[i]);
            if (FAILED(hr))
            {
                return false;
            }
        }

        if(context->m_inCubemapSRV == nullptr)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC desc;

            desc.Format                         = tex2DDesc.Format;
            desc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURECUBE;
            desc.TextureCube.MipLevels          = tex2DDesc.MipLevels;
            desc.TextureCube.MostDetailedMip    = 0;

            HRESULT hr = d->CreateShaderResourceView(inCubemap, &desc, &context->m_inCubemapSRV);
            if (FAILED(hr))
            {
                return false;
            }
        }

        if(context->m_inCubemapSampler == nullptr)
        {
            D3D11_SAMPLER_DESC desc;

            ZeroMemory(&desc, sizeof(desc));
            desc.Filter         = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            desc.MinLOD         = 0;
            desc.MaxLOD         = D3D11_FLOAT32_MAX;

            HRESULT hr = d->CreateSamplerState(&desc, &context->m_inCubemapSampler);
            if (FAILED(hr))
            {
                return false;
            }
        }

        if(context->m_parameter == nullptr)
        {
            D3D11_BUFFER_DESC desc;

            ZeroMemory(&desc, sizeof(desc));
            desc.Usage          = D3D11_USAGE_DEFAULT;
            desc.ByteWidth      = sizeof(BufferParameter);
            desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = 0;

            HRESULT hr = d->CreateBuffer(&desc, nullptr, &context->m_parameter);
            if (FAILED(hr))
            {
                return false;
            }
        }

        (*outCubemap)->GetDesc(&context->m_desc);

        context->m_jobIndex  = 0;
        context->m_jobSize   = 6 * mipCount;
        context->m_inCubemap = inCubemap;

        context->m_mode      = kMode_FilterCubemap;
        context->m_state     = kState_Processing;


        return true;
    }

    // Tick. 
    bool ProcessFiltering(Context* context)
    {
        // error check. 
        if(!context)
        {
            return false;
        }
        if(!context->m_d3dDevice || !context->m_d3dImContext)
        {
            return false;
        }
        if(context->m_mode  == kMode_None)
        {
            return false;
        }
        if(context->m_state == kState_Ready)
        {
            return false;
        }
        if(context->m_jobIndex>=context->m_jobSize)
        {
            return false;
        }

        // 
        UINT jobIndex  = context->m_jobIndex;
        UINT mipIndex  = jobIndex / context->m_desc.ArraySize;
        UINT faceIndex = jobIndex % context->m_desc.ArraySize;

        const UINT kTile = 8;
        UINT w = context->m_desc.Width;
        UINT h = context->m_desc.Height;

        w >>= mipIndex;
        if(w==0)
        {
            w = 1;
        }
        h >>= mipIndex;
        if(h==0)
        {
            h = 1;
        }

        UINT threadGroupX = (w + (kTile-1)) / kTile;
        UINT threadGroupY = (h + (kTile-1)) / kTile;

        // http://msdn.microsoft.com/en-us/library/windows/desktop/bb204881(v=vs.85).aspx
        const float faceVector[]   = // Z 
        {
             1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             0.0f,-1.0f, 0.0f,
             0.0f, 0.0f, 1.0f,
             0.0f, 0.0f,-1.0f,
        };

        const float faceTangent[]  = // X 
        {
             0.0f, 0.0f,-1.0f,
             0.0f, 0.0f, 1.0f,
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
        };

        const float faceBinormal[] = // Y : flip texcoord. 
        {
            0.0f,-1.0f, 0.0f,
            0.0f,-1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f,-1.0f,
            0.0f,-1.0f, 0.0f,
            0.0f,-1.0f, 0.0f,
        };

        BufferParameter param;
        param.m_faceVector[0]   = faceVector  [faceIndex * 3 + 0];
        param.m_faceVector[1]   = faceVector  [faceIndex * 3 + 1];
        param.m_faceVector[2]   = faceVector  [faceIndex * 3 + 2];
        param.m_faceVector[3]   = 0;

        param.m_faceTangent[0]  = faceTangent [faceIndex * 3 + 0];
        param.m_faceTangent[1]  = faceTangent [faceIndex * 3 + 1];
        param.m_faceTangent[2]  = faceTangent [faceIndex * 3 + 2];
        param.m_faceTangent[3]  = 0;

        param.m_faceBinormal[0] = faceBinormal[faceIndex * 3 + 0];
        param.m_faceBinormal[1] = faceBinormal[faceIndex * 3 + 1];
        param.m_faceBinormal[2] = faceBinormal[faceIndex * 3 + 2];
        param.m_faceBinormal[3] = 0;

        param.m_texelOffset [0] = 1.0f / (float)(w);
        param.m_texelOffset [1] = 1.0f / (float)(h);
        param.m_texelOffset [2] = 0;
        param.m_texelOffset [3] = 0;

        float fMipIndex  = (float)(mipIndex);
        float fMipLevels = (float)(context->m_desc.MipLevels);

//      param.m_roughness   [0] = (float)(mipIndex) / (float)(context->m_desc.ArraySize - 1);
        param.m_roughness   [0] = powf(2.0f, (fMipIndex-fMipLevels+3.0f));
        param.m_roughness   [1] = 0.0f;
        param.m_roughness   [2] = 0.0f;
        param.m_roughness   [3] = 0.0f;

        param.m_uavSize[0] = w;
        param.m_uavSize[1] = h;
        param.m_uavSize[2] = w / 2; if(param.m_uavSize[2])
        param.m_uavSize[3] = h / 2;

        ID3D11UnorderedAccessView* uav = context->m_outCubemap[jobIndex];
        ID3D11ShaderResourceView*  srv = context->m_inCubemapSRV;
        ID3D11SamplerState*        smp = context->m_inCubemapSampler;
        ID3D11Buffer*              buf = context->m_parameter;
        ID3D11DeviceContext* imContext = context->m_d3dImContext;

        ID3D11UnorderedAccessView* ppUAV[1]         = { uav };
        ID3D11ShaderResourceView*  ppSRV[1]         = { srv };
        ID3D11SamplerState*        ppSMP[1]         = { smp };
        ID3D11Buffer*              ppBUF[1]         = { buf };

        imContext->ClearState();

        imContext->UpdateSubresource(context->m_parameter, 0, nullptr, &param, 0, 0);

        imContext->CSSetShader(context->m_filterCS, nullptr, 0);
        imContext->CSSetUnorderedAccessViews(0, 1, ppUAV, nullptr);
        imContext->CSSetShaderResources     (0, 1, ppSRV);
        imContext->CSSetSamplers            (0, 1, ppSMP);
        imContext->CSSetConstantBuffers     (0, 1, ppBUF);

        imContext->Dispatch                 (threadGroupX, threadGroupY, 1);

        //  
        ID3D11UnorderedAccessView* ppUAVNULL   [1] = { nullptr };
        ID3D11ShaderResourceView*  ppSRVNULL   [1] = { nullptr };
        ID3D11SamplerState*        ppSMPNULL   [1] = { nullptr };
        ID3D11Buffer*              ppBUFNULL   [1] = { nullptr };

        imContext->CSSetShader              (nullptr, nullptr, 0);
        imContext->CSSetUnorderedAccessViews(0, 1, ppUAVNULL, nullptr);
        imContext->CSSetShaderResources     (0, 1, ppSRVNULL);
        imContext->CSSetSamplers            (0, 1, ppSMPNULL);
        imContext->CSSetConstantBuffers     (0, 1, ppBUFNULL);

        //  
        context->m_jobIndex++;

        return true;
    }




} // namespace iblkit 
