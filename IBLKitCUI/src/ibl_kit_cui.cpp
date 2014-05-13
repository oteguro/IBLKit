#define  _CRT_SECURE_NO_WARNINGS
#define  _CRT_NON_CONFORMING_SWPRINTFS
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

#include <stdio.h>
#include <tchar.h>

#include "../../IBLKit/source/_IMPL/scope_exit.h"
#include "../../IBLKitGUI/external/DirectXTex/DDSTextureLoader/DDSTextureLoader.h"

#include <iblkit.h>

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

D3D_DRIVER_TYPE             driverType       = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL           featureLevel     = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*               d3dDevice        = nullptr;
ID3D11DeviceContext*        immediateContext = nullptr;

ID3D11Texture2D*            baseIBL = nullptr;
ID3D11ShaderResourceView*   baseSRV = nullptr;
ID3D11Texture2D*            filteredIBL = nullptr;

iblkit::Context             iblContext;

void SaveTextureToDDSFile(ID3D11Resource* tex, LPCWSTR filename)
{   // from http://directxtex.codeplex.com/wikipage?title=CaptureTexture 
    DirectX::ScratchImage image;

    HRESULT hr;
    hr = DirectX::CaptureTexture(d3dDevice, immediateContext, tex, image);
    if (FAILED(hr))
    {
        return;
    }
    hr = DirectX::SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DDS_FLAGS_NONE, filename);
    if (FAILED(hr))
    {
        return;
    }
}

HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // feature level : DX11 class only. 
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        driverType = driverTypes[driverTypeIndex];

        hr = D3D11CreateDevice(NULL,
                               driverType,
                               NULL,
                               createDeviceFlags,
                               featureLevels,
                               numFeatureLevels,
                               D3D11_SDK_VERSION,
                               &d3dDevice,
                               &featureLevel,
                               &immediateContext);
        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    if(FAILED(hr))
    {
        return hr;
    }

    return hr;
}

void CleanupDevice()
{
    if (immediateContext)
    {
        immediateContext->ClearState();
    }

    iblContext.Release();

    if (filteredIBL)
    {
        filteredIBL->Release();
    }
    if (baseSRV)
    {
        baseSRV->Release();
    }
    if (baseIBL)
    {
        baseIBL->Release();
    }
    if (immediateContext)
    {
        immediateContext->Release();
    }
    if (d3dDevice)
    {
        d3dDevice->Release();
    }
}

void Usage()
{
    printf("================\nIBLKitCUI : usage\n>IBLKitCUI.exe [inFile]\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc < 2)
    {
        Usage();
        return 0;
    }

    HRESULT hr = InitDevice();
    if(SUCCEEDED(hr))
    {
        SCOPE_EXIT(CleanupDevice(););

        hr = CreateDDSTextureFromFile(d3dDevice, argv[1], (ID3D11Resource**)&baseIBL, &baseSRV);
        if(FAILED(hr))
        {
            Usage();
            return 0;
        }
        // IBLkit 
        iblContext.m_d3dDevice    = d3dDevice;
        iblContext.m_d3dImContext = immediateContext;

        iblkit::FilteringCubemap(&iblContext, baseIBL, &filteredIBL);


        while(iblContext.Processing())
        {
            if(!ProcessFiltering(&iblContext))
            {
                TCHAR DRV[256];
                TCHAR DIR[256];
                TCHAR FNAME[256];
                TCHAR EXT[256];
                TCHAR FULLPATH[1024];
                _tsplitpath(argv[1], DRV, DIR, FNAME, EXT);
                _stprintf(FULLPATH, L"%s%s%s_IBL%s", DRV, DIR, FNAME, EXT);

                SaveTextureToDDSFile(filteredIBL, FULLPATH);
                wprintf(L"done.\nSave file[%s]\n", FULLPATH);
                break;
            }
            printf(".");
            immediateContext->Flush();
        }

    }
    return 0;
}