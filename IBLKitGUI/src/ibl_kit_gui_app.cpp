// ----------------------------------------------------------------------------
// ibk_kit_gui_app.cpp
// ----------------------------------------------------------------------------
// Description : IBLkit sample GUI application. 
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "../external/DirectXTex/DDSTextureLoader/DDSTextureLoader.h"
#include "./resource/resource.h"

using namespace DirectX;

#define MAX_LOADSTRING 100

#ifndef PIf
#define PIf (3.141592653f)
#endif

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct SimpleVertex
{
    XMFLOAT3            Pos;
};

struct SimpleSphere
{
    XMFLOAT3            Pos;
    XMFLOAT2            Cd;
};

struct TfxCB
{
    XMMATRIX            World;
    XMMATRIX            View;
    XMMATRIX            Projection;
};

struct EyeCB
{
    XMVECTOR            Eye;
};

struct InvViewCB
{
    XMMATRIX            InvView;
};

// 
HWND                        hWnd;
HINSTANCE                   hInst;
TCHAR                       szTitle         [MAX_LOADSTRING];
TCHAR                       szWindowClass   [MAX_LOADSTRING];

D3D_DRIVER_TYPE             driverType          = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL           featureLevel        = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*               d3dDevice           = nullptr;
ID3D11DeviceContext*        immediateContext    = nullptr;
IDXGISwapChain*             swapChain           = nullptr;
ID3D11RenderTargetView*     renderTargetView    = nullptr;

ID3D11VertexShader*         backgroundVS        = nullptr;
ID3D11PixelShader*          backgroundPS        = nullptr;
ID3D11InputLayout*          backgroundIL        = nullptr;

ID3D11VertexShader*         sphereVS            = nullptr;
ID3D11PixelShader*          spherePS            = nullptr;
ID3D11InputLayout*          sphereIL            = nullptr;

ID3D11Buffer*               bgVB                = nullptr;
ID3D11Buffer*               bgIB                = nullptr;

ID3D11Buffer*               ssVB                = nullptr;
ID3D11Buffer*               ssIB                = nullptr;

ID3D11Buffer*               tfxCB               = nullptr;
ID3D11Buffer*               eyeCB               = nullptr;
ID3D11Buffer*               ivCB                = nullptr;

ID3D11Texture2D*            baseIBL             = nullptr;
ID3D11ShaderResourceView*   baseSRV             = nullptr;
ID3D11SamplerState*         baseSMP             = nullptr;

ID3D11BlendState*           blendState          = nullptr;

EyeCB                       eyecb;
TfxCB                       tfxcb;
TfxCB                       sscb;
InvViewCB                   ivcb;


float                       rad                 = 0.0f;

// 
ATOM                    MyRegisterClass (HINSTANCE hInstance);
BOOL                    InitInstance    (HINSTANCE hInstance, int nCmdShow);
LRESULT     CALLBACK    WndProc         (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR     CALLBACK    About           (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT                 CompileShaderFromFile(WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
HRESULT                 InitDevice      ();
void                    CleanupDevice   ();
void                    Update          ();
void                    Render          ();


//! GUI entry. 
int APIENTRY _tWinMain(HINSTANCE    hInstance,
                       HINSTANCE    hPrevInstance,
                       LPTSTR       lpCmdLine,
                       int          nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg = {0};
    HACCEL hAccelTable;

    LoadString(hInstance, IDS_APP_TITLE, szTitle,       MAX_LOADSTRING);
    LoadString(hInstance, IDC_IBLKITGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IBLKITGUI));

    if(FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }

    while(WM_QUIT != msg.message)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage (&msg);
        }
        else
        {
            Update();
            Render();
        }
    }

    CleanupDevice();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IBLKITGUI));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_IBLKITGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    RECT rc = { 0, 0, 1280, 720 };

    hWnd = CreateWindow(szWindowClass,
                        szTitle,
                        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        rc.right  - rc.left,
                        rc.bottom - rc.top,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    if(!hWnd)
    {
        return FALSE;
    }

    ShowWindow  (hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

RECT ClientRectSize(UINT& width, UINT& height)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    width  = rc.right  - rc.left;
    height = rc.bottom - rc.top;
    return rc;
}

HRESULT CompileShaderFromFile(WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(fileName,           // Filename 
                            NULL,               // Defines 
                            NULL,               // Include 
                            entryPoint,         // Entry point 
                            shaderModel,        // Target 
                            dwShaderFlags,      // Flag1 
                            0,                  // Flag2 
                            blobOut,            // Code blob 
                            &errorBlob);        // Error blob 
    if(FAILED(hr))
    {
        if(errorBlob != NULL)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        if(errorBlob)
        {
            errorBlob->Release();
        }
        return hr;
    }
    if (errorBlob)
    {
        errorBlob->Release();
    }
    return S_OK;
}

HRESULT CreateVertexShaderFromFile(WCHAR*                   fileName,
                                   D3D11_INPUT_ELEMENT_DESC desc[],
                                   UINT                     numElements,
                                   ID3D11VertexShader**     vs,
                                   ID3D11InputLayout**      il)
{
    HRESULT hr;
    ID3DBlob* pVSBlob = nullptr;

    hr = CompileShaderFromFile(fileName, "VS", "vs_5_0", &pVSBlob);
    if(FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, vs);
    if(FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    hr = d3dDevice->CreateInputLayout(desc, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), il);
    
    pVSBlob->Release();

    if(FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

HRESULT CreatePixelShaderFromFile(WCHAR*                   fileName,
                                  ID3D11PixelShader**      ps)
{
    HRESULT hr;
    ID3DBlob* pPSBlob = nullptr;

    hr = CompileShaderFromFile(fileName, "PS", "ps_5_0", &pPSBlob);
    if(FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = d3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, ps);

    pPSBlob->Release();

    if(FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

HRESULT CreateBuffer(const void* data, UINT bufferSize, UINT bindFlags, ID3D11Buffer** buffer)
{
    HRESULT hr;

    D3D11_BUFFER_DESC bd;

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage          = D3D11_USAGE_DEFAULT;
    bd.ByteWidth      = bufferSize;
    bd.BindFlags      = bindFlags;
    bd.CPUAccessFlags = 0;

    if(data)
    {
        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = data;
        hr = d3dDevice->CreateBuffer(&bd, &sd, buffer);
    }
    else
    {
        hr = d3dDevice->CreateBuffer(&bd, nullptr, buffer);
    }
    if (FAILED(hr))
    {
        return hr;
    }
    return S_OK;
}

HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    UINT width;
    UINT height;
    RECT rc = ClientRectSize(width, height);

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
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount          = 2;
    sd.BufferDesc.Width     = width;
    sd.BufferDesc.Height    = height;
    sd.BufferDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator   = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow         = hWnd;
    sd.SampleDesc.Count     = 1;
    sd.SampleDesc.Quality   = 0;
    sd.Windowed             = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        driverType = driverTypes[driverTypeIndex];

        hr = D3D11CreateDeviceAndSwapChain(NULL,
                                           driverType,
                                           NULL,
                                           createDeviceFlags,
                                           featureLevels,
                                           numFeatureLevels,
                                           D3D11_SDK_VERSION,
                                           &sd,
                                           &swapChain,
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

    // Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = d3dDevice->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    backBuffer->Release();
    if(FAILED(hr))
    {
        return hr;
    }

    // Create shaders. 
    D3D11_INPUT_ELEMENT_DESC descBG[] = 
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    D3D11_INPUT_ELEMENT_DESC descSS[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = CreateVertexShaderFromFile(L".\\datas\\shaders\\basic_background.hlsl", descBG, 1, &backgroundVS, &backgroundIL);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreatePixelShaderFromFile (L".\\datas\\shaders\\basic_background.hlsl",            &backgroundPS);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreateVertexShaderFromFile(L".\\datas\\shaders\\basic_sphere.hlsl",     descSS, 2, &sphereVS,     &sphereIL);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreatePixelShaderFromFile(L".\\datas\\shaders\\basic_sphere.hlsl",                 &spherePS);
    if(FAILED(hr))
    {
        return hr;
    }

    // Create vertex buffers & index buffers. 
    const float bgRadius = 50.0f;
    SimpleVertex bgVertices[] = 
    {
        { XMFLOAT3(-bgRadius,  bgRadius, -bgRadius) },
        { XMFLOAT3( bgRadius,  bgRadius, -bgRadius) },
        { XMFLOAT3( bgRadius,  bgRadius,  bgRadius) },
        { XMFLOAT3(-bgRadius,  bgRadius,  bgRadius) },
        { XMFLOAT3(-bgRadius, -bgRadius, -bgRadius) },
        { XMFLOAT3( bgRadius, -bgRadius, -bgRadius) },
        { XMFLOAT3( bgRadius, -bgRadius,  bgRadius) },
        { XMFLOAT3(-bgRadius, -bgRadius,  bgRadius) },
    };
    WORD bgIndices[] =
    {
        0, 1, 3,
        3, 1, 2,

        4, 5, 0,
        0, 5, 1,

        7, 4, 3,
        3, 4, 0,

        5, 6, 1,
        1, 6, 2,

        6, 7, 2,
        2, 7, 3,

        5, 4, 6,
        6, 4, 7,

    };

    hr = CreateBuffer((const void*)&bgVertices[0], sizeof(SimpleVertex)*8,  D3D11_BIND_VERTEX_BUFFER,    &bgVB);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreateBuffer((const void*)&bgIndices[0],  sizeof(WORD) * 36,       D3D11_BIND_INDEX_BUFFER,     &bgIB);
    if(FAILED(hr))
    {
        return hr;
    }

    const float ssRadius = 2.0f;
    SimpleSphere ssVertices[] =
    {
        { XMFLOAT3(-ssRadius, -ssRadius, 0.0f), XMFLOAT2(-1, -1), },
        { XMFLOAT3( ssRadius, -ssRadius, 0.0f), XMFLOAT2( 1, -1), },
        { XMFLOAT3(-ssRadius,  ssRadius, 0.0f), XMFLOAT2(-1,  1), },
        { XMFLOAT3( ssRadius,  ssRadius, 0.0f), XMFLOAT2( 1,  1), },
    };
    WORD ssIndices[] =
    {
        0, 2, 1,
        2, 3, 1,
    };

    hr = CreateBuffer((const void*)&ssVertices[0], sizeof(SimpleSphere)*4,  D3D11_BIND_VERTEX_BUFFER,    &ssVB);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreateBuffer((const void*)&ssIndices[0],  sizeof(WORD) * 6,        D3D11_BIND_INDEX_BUFFER,     &ssIB);
    if(FAILED(hr))
    {
        return hr;
    }


    // Constant buffers. 
    hr = CreateBuffer(nullptr,                   sizeof(TfxCB),             D3D11_BIND_CONSTANT_BUFFER,  &tfxCB);
    if(FAILED(hr))
    {
        return hr;
    }

    hr = CreateBuffer(nullptr,                   sizeof(EyeCB),             D3D11_BIND_CONSTANT_BUFFER,  &eyeCB);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = CreateBuffer(nullptr,                   sizeof(InvViewCB),         D3D11_BIND_CONSTANT_BUFFER,  &ivCB);
    if (FAILED(hr))
    {
        return hr;
    }

    // Texture(IBL)
    hr = CreateDDSTextureFromFile(d3dDevice, L".\\datas\\ibl\\Ennis House.dds", (ID3D11Resource**)&baseIBL, &baseSRV);
    if(FAILED(hr))
    {
        return hr;
    }

    // Create sampler. 
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD   = 0;
    sampDesc.MaxLOD   = D3D11_FLOAT32_MAX;
    hr = d3dDevice->CreateSamplerState(&sampDesc, &baseSMP);
    if(FAILED(hr))
    {
        return hr;
    }

    // Blend factor. 
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable  = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable   = TRUE;
    blendDesc.RenderTarget[0].BlendOp       = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha  = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend     = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].SrcBlend      = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = d3dDevice->CreateBlendState(&blendDesc, &blendState);
    if(FAILED(hr))
    {
        return hr;
    }

    return hr;
}

void CleanupDevice()
{
    if( immediateContext)
    {
        immediateContext->ClearState();
    }

    if(blendState)
    {
        blendState      ->Release();
    }
    if( baseSMP)
    {
        baseSMP         ->Release();
    }
    if( baseSRV)
    {
        baseSRV         ->Release();
    }
    if( baseIBL)
    {
        baseIBL         ->Release();
    }
    if( ivCB)
    {
        ivCB            ->Release();
    }
    if( eyeCB)
    {
        eyeCB           ->Release();
    }
    if( tfxCB)
    {
        tfxCB           ->Release();
    }
    if( bgVB)
    {
        bgVB            ->Release();
    }
    if( bgIB)
    {
        bgIB            ->Release();
    }
    if( ssVB)
    {
        ssVB            ->Release();
    }
    if( ssIB)
    {
        ssIB            ->Release();
    }
    if( backgroundIL)
    {
        backgroundIL    ->Release();
    }
    if( backgroundPS)
    {
        backgroundPS    ->Release();
    }
    if( backgroundVS)
    {
        backgroundVS    ->Release();
    }
    if( sphereIL)
    {
        sphereIL        ->Release();
    }
    if( spherePS)
    {
        spherePS        ->Release();
    }
    if( sphereVS)
    {
        sphereVS        ->Release();
    }
    if( renderTargetView)
    {
        renderTargetView->Release();
    }
    if( swapChain)
    {
        swapChain       ->Release();
    }
    if( immediateContext)
    {
        immediateContext->Release();
    }
    if( d3dDevice)
    {
        d3dDevice       ->Release();
    }
}

void Update()
{
    //XMVECTOR iv;

    // uniform. 
    UINT width;
    UINT height;
    ClientRectSize(width, height);

    const float R = 5.0f;
    XMVECTOR Eye = XMVectorSet(R*sinf(rad), 0.0f, R*cosf(rad), 0.0f);
    XMVECTOR At  = XMVectorSet(0.0f, 0.0f,  0.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f);


    XMMATRIX worldMatrix         = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    XMMATRIX viewMatrix          = XMMatrixLookAtLH(Eye, At, Up);
    XMMATRIX projectionMatrix    = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 120.0f);

    tfxcb.World                  = XMMatrixTranspose(worldMatrix);
    tfxcb.View                   = XMMatrixTranspose(viewMatrix);
    tfxcb.Projection             = XMMatrixTranspose(projectionMatrix);

    sscb                         = tfxcb;
    sscb.World                   = viewMatrix; // transpose(transpose()) = E 

    eyecb.Eye                    = Eye;

    ivcb.InvView                 = viewMatrix;

    rad += 0.0001f;
    if(rad > 2.0f*PIf)
    {
        rad -= 2.0f*PIf;
    }
}

void Render()
{
    assert(d3dDevice       !=nullptr);
    assert(immediateContext!=nullptr);
    assert(swapChain       !=nullptr);
    assert(renderTargetView!=nullptr);

    // 
    UINT width;
    UINT height;
    RECT rc = ClientRectSize(width, height);

    // Clear renderstate. 
    immediateContext->ClearState();

    // Rendertarget. 
    immediateContext->OMSetRenderTargets(1, &renderTargetView, NULL);

    // Viewport. 
    D3D11_VIEWPORT vp;
    vp.Width    = (FLOAT)width;
    vp.Height   = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    immediateContext->RSSetViewports(1, &vp);

    // Clear RT. 
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // R, G, B, A 
    immediateContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Update CB. 
    immediateContext->UpdateSubresource(ivCB,  0, nullptr, &ivcb,  0, 0);
    immediateContext->UpdateSubresource(eyeCB, 0, nullptr, &eyecb, 0, 0);
    immediateContext->UpdateSubresource(tfxCB, 0, nullptr, &tfxcb, 0, 0);

    // Clear background. 
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;


    // Draw BG. 
    immediateContext->IASetInputLayout          (backgroundIL);
    immediateContext->VSSetShader               (backgroundVS, NULL, 0);
    immediateContext->VSSetConstantBuffers      (0, 1, &tfxCB);
    immediateContext->PSSetShader               (backgroundPS, NULL, 0);
    immediateContext->PSSetConstantBuffers      (1, 1, &eyeCB);

    immediateContext->PSSetShaderResources      (0, 1, &baseSRV);
    immediateContext->PSSetSamplers             (0, 1, &baseSMP);

    immediateContext->IASetPrimitiveTopology    (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediateContext->IASetVertexBuffers        (0, 1, &bgVB, &stride, &offset);
    immediateContext->IASetIndexBuffer          (bgIB, DXGI_FORMAT_R16_UINT, 0);

    immediateContext->DrawIndexed               (36, 0, 0);


    // Draw sphere. 
    immediateContext->UpdateSubresource(tfxCB, 0, nullptr, &sscb, 0, 0);
    stride = sizeof(SimpleSphere);
    offset = 0;

    const float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
    immediateContext->OMSetBlendState           (blendState, blendFactor, 0xffffffff);

    immediateContext->IASetInputLayout          (sphereIL);
    immediateContext->VSSetShader               (sphereVS, NULL, 0);
    immediateContext->VSSetConstantBuffers      (0, 1, &tfxCB);
    immediateContext->PSSetShader               (spherePS, NULL, 0);
    immediateContext->PSSetConstantBuffers      (1, 1, &eyeCB);
    immediateContext->PSSetConstantBuffers      (2, 1, &ivCB);

    immediateContext->PSSetShaderResources      (0, 1, &baseSRV);
    immediateContext->PSSetSamplers             (0, 1, &baseSMP);

    immediateContext->IASetPrimitiveTopology    (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediateContext->IASetVertexBuffers        (0, 1, &ssVB, &stride, &offset);
    immediateContext->IASetIndexBuffer          (ssIB, DXGI_FORMAT_R16_UINT, 0);

    immediateContext->DrawIndexed               (6, 0, 0);


    // Swap. 
    swapChain->Present(0, 0);
}


