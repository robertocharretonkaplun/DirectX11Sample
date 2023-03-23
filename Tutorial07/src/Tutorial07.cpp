#include "Prerequisites.h"
#include "Commons.h"
#include "CTime.h"
#include "UserInterface.h"
#include "Transform.h"
#include "Window.h"
#include "Device.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "SamplerState.h"
//#include "InputLayout.h"
#include "ModelLoader.h"
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

//ID3D11VertexShader*                 g_pVertexShader = nullptr;
//ID3D11PixelShader*                  g_pPixelShader = nullptr;
ID3D11Buffer*                       g_pVertexBuffer = nullptr;
ID3D11Buffer*                       g_pIndexBuffer = nullptr;
ID3D11Buffer*                       g_Camera = nullptr;
ID3D11Buffer*                       g_pCBChangesEveryFrame = nullptr;
//ID3D11SamplerState*                 g_pSamplerLinear = nullptr;
//XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(1, 1, 1, 1);
// Custom Classes
Window                              g_window;
Device                              g_device;
SwapChain                           g_swapChain;
DeviceContext                       g_deviceContext;
RenderTargetView                    g_renderTargetView;
DepthStencilView                    g_depthStencilView;
Viewport                            g_viewport;
Texture                             g_depthStencil;
Texture                             g_ModelTexture;
Texture                             g_backBuffer;
//InputLayout                         g_inputLayout;
ShaderProgram                       g_shaderProgram;
SamplerState                        g_samplerLineal;
Camera                              g_cam;
CTime                               g_Time;
UserInterface                       g_UI;
Transform                           g_transform;
float                               g_movementSpeed = 5.0f;
float                               g_speed;
LoadData LD;
ModelLoader                         g_modelLoader;
//D3D11_VIEWPORT vp;
unsigned int stride = sizeof(SimpleVertex);
unsigned int offset = 0;
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
  g_swapChain.init(g_device, g_deviceContext, g_backBuffer, g_window);
  
  g_renderTargetView.init(g_device, g_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
  
  g_depthStencil.init(g_device, 
                      g_window.m_width, 
                      g_window.m_height,
                      DXGI_FORMAT_D24_UNORM_S8_UINT, 
                      D3D11_BIND_DEPTH_STENCIL);

  g_depthStencilView.init(g_device, 
                          g_depthStencil.m_texture, 
                          DXGI_FORMAT_D24_UNORM_S8_UINT);
  
  g_viewport.init(g_window);

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

  
  g_shaderProgram.init(g_device, "Tutorial07.fx", Layout);
  
  // Load Model
  LD = g_modelLoader.Load("Pistol.obj");

  // Create vertex buffer
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
  g_ModelTexture.init(g_device, "GunAlbedo.dds");

  g_samplerLineal.init(g_device);

  // Initialize the view matrix
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, -30.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  g_View = XMMatrixLookAtLH(Eye, At, Up);

  // Initialize the projection matrix
  g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 
                                          g_window.m_width / (FLOAT)g_window.m_height, 
                                          0.01f, 
                                          100.0f);

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
  if (ImGui::Button("screenshot"))
  {
  }
  ImGui::Image(g_ModelTexture.m_textureFromImg, ImVec2(50, 50));
  ImGui::End();
  g_transform.ui();
  g_modelLoader.ui();


  
  // Update variables that change once per frame
  //speed += .0002f;
  // Rotate cube around the origin
  
  g_transform.update();
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose(g_transform.m_matrix);
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
  g_samplerLineal.destroy();
  g_ModelTexture.destroy();
  g_modelLoader.destroy();
  if (g_Camera) g_Camera->Release();
  if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
  if (g_pVertexBuffer) g_pVertexBuffer->Release();
  if (g_pIndexBuffer) g_pIndexBuffer->Release();
  g_shaderProgram.destroy();
  g_depthStencil.destroy();
  g_depthStencilView.destroy();
  g_renderTargetView.destroy();
  g_swapChain.destroy();
  g_UI.destroy();
  g_device.destroy();
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
    break;
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
  g_depthStencilView.render(g_deviceContext);
  g_renderTargetView.render(g_deviceContext, g_depthStencilView, ClearColor);
  
  // Configurar el viewport
  g_deviceContext.RSSetViewports(1, &g_viewport.m_viewport);

  // Establecer el estado de entrada
  g_deviceContext.IASetInputLayout(g_shaderProgram.m_inputLayout.m_inputLayout);
  g_deviceContext.IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
  g_deviceContext.IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  g_deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Establecer los shaders
  g_deviceContext.VSSetShader(g_shaderProgram.m_VertexShader, nullptr, 0);
  g_deviceContext.PSSetShader(g_shaderProgram.m_PixelShader, nullptr, 0);

  // Establecer los constant buffers
  g_deviceContext.VSSetConstantBuffers(0, 1, &g_Camera);
  g_deviceContext.VSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);
  g_deviceContext.PSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);

  // Establecer las texturas y samplers
  g_deviceContext.PSSetShaderResources(0, 1, &g_ModelTexture.m_textureFromImg);
  g_deviceContext.PSSetSamplers(0, 1, &g_samplerLineal.m_sampler);

  // Dibujar
  g_deviceContext.DrawIndexed(LD.numIndex, 0, 0);

  // Renderizar la UI
  g_UI.render();

  // Presentar el back buffer
  g_swapChain.present();

}
