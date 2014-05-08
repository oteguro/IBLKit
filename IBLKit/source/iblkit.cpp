// ----------------------------------------------------------------------------
// iblkit.cpp
// ----------------------------------------------------------------------------
#include "iblkit.h"
#include <d3dcompiler.h>
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

    } // unnamed namespace 

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
            desc.Format             = DXGI_FORMAT_R16G16B16A16_FLOAT;
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
//         if(*outUAV     == nullptr)
//         {
//             for(UINT i=0; i<6; ++i)
//             {
//                 D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
//                 desc.Format                         = DXGI_FORMAT_R16G16B16A16_FLOAT;
//                 desc.ViewDimension                  = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
//                 desc.Texture2DArray.MipSlice        = mipCount - 1;
//                 desc.Texture2DArray.FirstArraySlice = i;
//                 desc.Texture2DArray.ArraySize       = 6;
// 
//                 HRESULT hr = d->CreateUnorderedAccessView(*outCubemap, &desc, &outUAV[i]);
//                 if (FAILED(hr))
//                 {
//                     return false;
//                 }
//             }
//         }

        context->m_jobIndex  = 0;
        context->m_jobSize   = 6 * mipCount;
        context->m_inCubemap = inCubemap;
        context->m_outCubemap = nullptr;


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
        if(context->m_mode == kMode_None)
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





        return true;
    }




} // namespace iblkit 
