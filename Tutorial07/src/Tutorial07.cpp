#include "Prerequisites.h"
#include "Commons.h"
#include "CTime.h"
#include "UserInterface.h"
#include "Transform.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "DepthStencilView.h"
#include "ModelLoader.h"
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*                       g_device.m_device = nullptr;
IDXGISwapChain*                     g_pSwapChain = nullptr;
ID3D11RenderTargetView*             g_pRenderTargetView = nullptr;
ID3D11Texture2D*                    g_pDepthStencil = nullptr;
//ID3D11DepthStencilView*             g_pDepthStencilView = nullptr;
ID3D11VertexShader*                 g_pVertexShader = nullptr;
ID3D11PixelShader*                  g_pPixelShader = nullptr;
ID3D11InputLayout*                  g_pVertexLayout = nullptr;
ID3D11Buffer*                       g_pVertexBuffer = nullptr;
ID3D11Buffer*                       g_pIndexBuffer = nullptr;
ID3D11Buffer*                       g_Camera = nullptr;
ID3D11Buffer*                       g_pCBChangesEveryFrame = nullptr;
ID3D11ShaderResourceView*           g_pTextureRV = nullptr;
ID3D11SamplerState*                 g_pSamplerLinear = nullptr;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(1, 1, 1, 1);
ID3D11Texture2D*                    pBackBuffer = nullptr;

Window                              g_window;
Device                              g_device;
DeviceContext                       g_deviceContext;
DepthStencilView                    g_depthStencilView;
Camera                              g_cam;
CTime                               g_Time;
UserInterface                       g_UI;
Transform                           g_transform;
float                               g_movementSpeed = 5.0f;
float                               g_speed;
LoadData LD;
ModelLoader                         g_modelLoader;
D3D11_VIEWPORT vp;
UINT stride = sizeof(SimpleVertex);
UINT offset = 0;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();
void update(float deltaTime);
void destroy();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI 
wWinMain(HINSTANCE hInstance, 
         HINSTANCE hPrevInstance, 
         LPWSTR lpCmdLine,                                                                         
         int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  //if (FAILED(InitWindow(hInstance, nCmdShow)))
  //  return 0;
  
  if (FAILED(g_window.init(hInstance, nCmdShow, WndProc)))
    return 0;

  if (FAILED(InitDevice())) {
    destroy();
    return 0;
  }
  // Initialize the time
  g_Time.init();
  

  // Main message loop
  MSG msg = { 0 };
  while (WM_QUIT != msg.message) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {

      g_Time.update();
      update(g_Time.m_deltaTime);
      Render();

    }
  }
  destroy();

  return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT 
CompileShaderFromFile(char* szFileName, 
                      LPCSTR szEntryPoint, 
                      LPCSTR szShaderModel, 
                      ID3DBlob** ppBlobOut) {
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;
  hr = D3DX11CompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
    dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
  if (FAILED(hr)) {
    if (pErrorBlob != nullptr)
      OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
    if (pErrorBlob) pErrorBlob->Release();
    return hr;
  }
  if (pErrorBlob) pErrorBlob->Release();

  return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT 
InitDevice() {
  HRESULT hr = S_OK;
  

  unsigned int createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };


  unsigned int numFeatureLevels = ARRAYSIZE(featureLevels);

  DXGI_SWAP_CHAIN_DESC sd;
  memset(&sd, 0, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width =  g_window.m_width;
  sd.BufferDesc.Height = g_window.m_height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_window.m_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(nullptr, 
                                       g_driverType, 
                                       nullptr, 
                                       createDeviceFlags, 
                                       featureLevels, 
                                       numFeatureLevels,
                                       D3D11_SDK_VERSION, 
                                       &sd, 
                                       &g_pSwapChain, 
                                       &g_device.m_device,
                                       &g_featureLevel, 
                                       &g_deviceContext.m_deviceContext);
    if (SUCCEEDED(hr))
      break;
  }
  if (FAILED(hr))
    return hr;

  // Create a render target view

  hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr))
    return hr;

  hr = g_device.CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr))
    return hr;

  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  memset(&descDepth, 0, sizeof(descDepth));
  descDepth.Width =  g_window.m_width;
  descDepth.Height = g_window.m_height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  hr = g_device.CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
  if (FAILED(hr))
  {
    WARNING("Engine - Error in the Depth Stencil Texture Creation");
    return hr;
  }

  // Create the depth stencil view

  g_depthStencilView.init(g_device, g_pDepthStencil, descDepth.Format);


  // Setup the viewport
  
  vp.Width = (FLOAT) g_window.m_width;
  vp.Height = (FLOAT)g_window.m_height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  // Compile the vertex shader
  ID3DBlob* pVSBlob = nullptr;
  hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
  if (FAILED(hr))
  {
    MessageBox(nullptr,
      "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
    return hr;
  }

  // Create the vertex shader
  hr = g_device.CreateVertexShader(pVSBlob->GetBufferPointer(), 
                                   pVSBlob->GetBufferSize(), 
                                   nullptr, 
                                   &g_pVertexShader);
  if (FAILED(hr))
  {
    pVSBlob->Release();
    return hr;
  }

  // Define the input layout
  std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;

  D3D11_INPUT_ELEMENT_DESC position;
  position.SemanticName = "POSITION";
  position.SemanticIndex = 0;
  position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  position.InputSlot = 0;
  position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
  position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  position.InstanceDataStepRate = 0;
  Layout.push_back(position);

  D3D11_INPUT_ELEMENT_DESC texcoord;
  texcoord.SemanticName = "TEXCOORD";
  texcoord.SemanticIndex = 0;
  texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
  texcoord.InputSlot = 0;
  texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
  texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  texcoord.InstanceDataStepRate = 0;
  Layout.push_back(texcoord);

  // Create the input layout
  hr = g_device.CreateInputLayout(Layout.data(), 
                                       Layout.size(), 
                                       pVSBlob->GetBufferPointer(),
                                       pVSBlob->GetBufferSize(), 
                                       &g_pVertexLayout);
  pVSBlob->Release();
  if (FAILED(hr))
    return hr;

  // Set the input layout

  // Compile the pixel shader
  ID3DBlob* pPSBlob = nullptr;
  hr = CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
  if (FAILED(hr))
  {
    MessageBox(nullptr,
      "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
    return hr;
  }

  // Create the pixel shader
  hr = g_device.CreatePixelShader(pPSBlob->GetBufferPointer(), 
                                       pPSBlob->GetBufferSize(), 
                                       nullptr, 
                                       &g_pPixelShader);
  pPSBlob->Release();
  if (FAILED(hr))
    return hr;

  
  // Load Model
  LD = g_modelLoader.Load("Pistol.obj");

  // Create vertex buffer
  /*
  SimpleVertex vertices[] =
  {
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
  };

  */
  D3D11_BUFFER_DESC bd;
  memset(&bd,0, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  //bd.ByteWidth = sizeof(SimpleVertex) * 24;
  bd.ByteWidth = sizeof(SimpleVertex) * LD.numVertex;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData;
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = LD.vertex.data();
  hr = g_device.CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  if (FAILED(hr))
    return hr;

  // Set vertex buffer
  

  // Create index buffer
  // Create vertex buffer
  /*
  WORD indices[] =
  {
      3,1,0,
      2,1,3,

      6,4,5,
      7,4,6,

      11,9,8,
      10,9,11,

      14,12,13,
      15,12,14,

      19,17,16,
      18,17,19,

      22,20,21,
      23,20,22
  };
  */

  D3D11_BUFFER_DESC ib;
  memset(&ib, 0, sizeof(ib));
  ib.Usage = D3D11_USAGE_DEFAULT;
  //bd.ByteWidth = sizeof(WORD) * 36;
  ib.ByteWidth = sizeof(unsigned int) * LD.numIndex;
  ib.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ib.CPUAccessFlags = 0;
  InitData.pSysMem = LD.index.data();
  hr = g_device.CreateBuffer(&ib, &InitData, &g_pIndexBuffer);
  if (FAILED(hr))
    return hr;

  // Set index buffer

  // Set primitive topology

  // Create the constant buffers
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(Camera);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  hr = g_device.CreateBuffer(&bd, nullptr, &g_Camera);
  if (FAILED(hr))
    return hr;

  bd.ByteWidth = sizeof(CBChangesEveryFrame);
  hr = g_device.CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
  if (FAILED(hr))
    return hr;

  // Load the Texture
  hr = D3DX11CreateShaderResourceViewFromFile(g_device.m_device, 
                                              "DefaultTexture.dds", 
                                              nullptr, 
                                              nullptr, 
                                              &g_pTextureRV, 
                                              nullptr);
  if (FAILED(hr))
    return hr;

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  memset(&sampDesc, 0, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = g_device.CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  if (FAILED(hr))
    return hr;

  // Initialize the world matrices
  g_World = XMMatrixIdentity();

  // Initialize the view matrix
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, -30.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  g_View = XMMatrixLookAtLH(Eye, At, Up);

  // Initialize the projection matrix
  g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (FLOAT)g_window.m_height, 0.01f, 100.0f);

  g_cam.mView = XMMatrixTranspose(g_View);
  g_cam.mProjection = XMMatrixTranspose(g_Projection);


  // Initialize Classes
  g_UI.init(g_window.m_hWnd, g_device.m_device, g_deviceContext.m_deviceContext);
  g_transform.init();
  return S_OK;
}

void 
Input(float deltaTime) {
  if (GetAsyncKeyState('W') & 0x8000) {
    // move object forward
    g_transform.m_position.y += g_movementSpeed * deltaTime;
  }
  if (GetAsyncKeyState('S') & 0x8000) {
    // move object backward
    g_transform.m_position.y -= g_movementSpeed * deltaTime;
  }
  if (GetAsyncKeyState('A') & 0x8000) {
    // move object left
    g_transform.m_position.x -= g_movementSpeed * deltaTime;
  }
  if (GetAsyncKeyState('D') & 0x8000) {
    // move object right
    g_transform.m_position.x += g_movementSpeed * deltaTime;
  }

  if (GetAsyncKeyState('E') & 0x8000) {
    // move object right
    g_transform.m_position.z += g_movementSpeed * deltaTime;
  }
  if (GetAsyncKeyState('Q') & 0x8000) {
    // move object right
    g_transform.m_position.z -= g_movementSpeed * deltaTime;
  }

}

// Esta funcion esta encargada de actualizar la LOGICA del programa
void 
update(float deltaTime) {
  g_UI.update();
  Input(deltaTime);

  bool show_demo_window = true;
  ImGui::ShowDemoWindow(&show_demo_window);
  ImGui::Begin("Textures");
  ImGui::Image(g_pTextureRV, ImVec2(50, 50));
  ImGui::End();
  g_transform.ui();
  // Update variables that change once per frame
  //speed += .0002f;
  // Rotate cube around the origin
  g_World = XMMatrixScaling(g_transform.m_scale.x, 
                            g_transform.m_scale.y, 
                            g_transform.m_scale.z) * 
            XMMatrixRotationRollPitchYaw(g_transform.m_rotation.x,
                                         g_transform.m_rotation.y,
                                         g_transform.m_rotation.z) *
            XMMatrixTranslation(g_transform.m_position.x, 
                                g_transform.m_position.y, 
                                g_transform.m_position.z);
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose(g_World);
  cb.vMeshColor = g_vMeshColor;
  // Update Data
  // Update Mesh buffers
  g_deviceContext.UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);
  // Update Camera Buffers
  g_deviceContext.UpdateSubresource(g_Camera, 0, nullptr, &g_cam, 0, 0);
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void 
destroy() {
  g_deviceContext.destroy();

  if (g_pSamplerLinear) g_pSamplerLinear->Release();
  if (g_pTextureRV) g_pTextureRV->Release();
  //if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
  //if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
  if (g_Camera) g_Camera->Release();
  if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
  if (g_pVertexBuffer) g_pVertexBuffer->Release();
  if (g_pIndexBuffer) g_pIndexBuffer->Release();
  if (g_pVertexLayout) g_pVertexLayout->Release();
  if (g_pVertexShader) g_pVertexShader->Release();
  if (g_pPixelShader) g_pPixelShader->Release();
  if (g_pDepthStencil) g_pDepthStencil->Release();
  //if (g_pDepthStencilView) g_pDepthStencilView->Release();
  if (g_pRenderTargetView) g_pRenderTargetView->Release();
  if (g_pSwapChain) g_pSwapChain->Release();
  // Destroy Device
  //if (g_device.m_device) g_device.m_device->Release();
  g_UI.destroy();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT 
ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK 
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    return true;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_KEYDOWN:
    //// detectar entrada de teclado
    switch (wParam)
    {
    case 'C':
    {
    }
    break;
    case 'S':
      break;
    case 'A':
      break;
    case 'D':
      break;
    }
    break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;
  case  WM_IME_KEYUP:

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_SIZE:
  {
    // Obtener las nuevas dimensiones de la ventana

    UINT nWidth = LOWORD(lParam);
    UINT nHeight = HIWORD(lParam);

    // Hacer resize de la ventana
    //OnResize(nWidth, nHeight);
    break;

  }


  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void 
Render() {
  // Clear the back buffer
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  g_deviceContext.ClearRenderTargetView(g_pRenderTargetView, ClearColor);
  // Clear the depth buffer to 1.0 (max depth)
  g_deviceContext.ClearDepthStencilView(g_depthStencilView.m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
  
  g_deviceContext.OMSetRenderTargets(1, &g_pRenderTargetView, g_depthStencilView.m_depthStencilView);
  g_deviceContext.RSSetViewports(1, &vp);
  g_deviceContext.IASetInputLayout(g_pVertexLayout);
  g_deviceContext.IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
  g_deviceContext.IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  g_deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  

  // Render the cube
  g_deviceContext.VSSetShader(g_pVertexShader, nullptr, 0);
  g_deviceContext.VSSetConstantBuffers(0, 1, &g_Camera);
  g_deviceContext.VSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);
  g_deviceContext.PSSetShader(g_pPixelShader, nullptr, 0);
  g_deviceContext.PSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);
  g_deviceContext.PSSetShaderResources(0, 1, &g_pTextureRV);
  g_deviceContext.PSSetSamplers(0, 1, &g_pSamplerLinear);
  g_deviceContext.DrawIndexed(LD.numIndex, 0, 0);

  g_UI.render();
  // Present our back buffer to our front buffer
  g_pSwapChain->Present(0, 0);
}
