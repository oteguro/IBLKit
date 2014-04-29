// ----------------------------------------------------------------------------
// ibk_kit_gui_app.cpp
// ----------------------------------------------------------------------------
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3d11shader.h>
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

#pragma comment(lib, "d3d11.lib")

// 
HWND                    hWnd;
HINSTANCE               hInst;
TCHAR                   szTitle         [MAX_LOADSTRING];
TCHAR                   szWindowClass   [MAX_LOADSTRING];

D3D_DRIVER_TYPE         driverType          = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       featureLevel        = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           d3dDevice           = nullptr;
ID3D11DeviceContext*    immediateContext    = nullptr;
IDXGISwapChain*         swapChain           = nullptr;
ID3D11RenderTargetView* renderTargetView    = nullptr;

XMMATRIX                worldMatrix;
XMMATRIX                viewMatrix;
XMMATRIX                projectionMatrix;


// 
ATOM                    MyRegisterClass (HINSTANCE hInstance);
BOOL                    InitInstance    (HINSTANCE hInstance, int nCmdShow);
LRESULT     CALLBACK    WndProc         (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR     CALLBACK    About           (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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

//  while(GetMessage(&msg, NULL, 0, 0))
    while(WM_QUIT != msg.message)
    {
//      if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
        wmId = LOWORD(wParam);
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
    sd.BufferCount          = 1;
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

    return hr;
}

void CleanupDevice()
{
    if( immediateContext)
    {
        immediateContext->ClearState();
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
    // geometry. 



    // uniform. 
    UINT width;
    UINT height;
    ClientRectSize(width, height);

    XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
    XMVECTOR At  = XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f);
    XMVECTOR Up  = XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f);

    viewMatrix          = XMMatrixLookAtLH(Eye, At, Up);
    projectionMatrix    = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
    worldMatrix         = XMMatrixIdentity();
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

    // Swap. 
    swapChain->Present(0, 0);
}


