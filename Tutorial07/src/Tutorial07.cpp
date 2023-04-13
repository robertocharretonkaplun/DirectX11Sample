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
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Screenshot.h"
#include "Model.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

//ID3D11VertexShader*                 g_pVertexShader = nullptr;
//ID3D11PixelShader*                  g_pPixelShader = nullptr;
//ID3D11Buffer*                       g_pVertexBuffer = nullptr;
//ID3D11Buffer*                       g_pIndexBuffer = nullptr;
ID3D11Buffer*                       g_Camera = nullptr;
//ID3D11Buffer*                       g_pCBChangesEveryFrame = nullptr;
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
Texture                             g_ModelTexture2;
std::vector<Texture> g_textures;
Texture                             g_backBuffer;
//VertexBuffer                        g_vertexBuffer;
//IndexBuffer                         g_indexBuffer;
//ConstantBuffer                      g_modelBuffer;
//InputLayout                         g_inputLayout;
//Model                               g_model;
//Model                               g_model2;

std::vector<Model> g_models;
ShaderProgram                       g_shaderProgram;
SamplerState                        g_samplerLineal;
Camera                              g_cam;
CTime                               g_Time;
UserInterface                       g_UI;
Transform                           g_transform;
Transform                           g_transform2;
float                               g_movementSpeed = 5.0f;
float                               g_speed;
LoadData LD;
LoadData LD2;
ModelLoader                         g_modelLoader;
ModelLoader                         g_modelLoader2;
Screenshot                          g_captureScreenshot;
//D3D11_VIEWPORT vp;
unsigned int stride = sizeof(SimpleVertex);
unsigned int offset = 0;
int currentModel = 0;
int activeModelIndex = 0;

ID3D11Texture2D* imguiTexture;
ID3D11RenderTargetView* imguiRTV;
ID3D11ShaderResourceView* imguiSRV = nullptr;

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

  

  // Dibuja en el render target

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
  // Load the Texture
  g_ModelTexture.init(g_device, "GunAlbedo.dds");
  g_ModelTexture2.init(g_device, "DefaultTexture.dds");
  g_textures.push_back(g_ModelTexture);
  g_textures.push_back(g_ModelTexture2);
  g_samplerLineal.init(g_device);
  // Load Model
  // Vertex e Index Buffer se van a trasladar a la clase Actor
  
  

  //g_vertexBuffer.init(g_device, LD);
  //
  //g_indexBuffer.init(g_device, LD);
  

  D3D11_TEXTURE2D_DESC textureDesc;
  ZeroMemory(&textureDesc, sizeof(textureDesc));
  textureDesc.Width = g_window.m_width;
  textureDesc.Height = g_window.m_height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  g_device.m_device->CreateTexture2D(&textureDesc, NULL, &imguiTexture);

  // Crear una vista de render target para la textura de IMGUI
  g_device.m_device->CreateRenderTargetView(imguiTexture, NULL, &imguiRTV);
  
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  srvDesc.Texture2D.MostDetailedMip = 0;
  g_device.m_device->CreateShaderResourceView(imguiTexture, &srvDesc, &imguiSRV);

  // Create the constant buffers
  D3D11_BUFFER_DESC CamBufferDesc;
  memset(&CamBufferDesc, 0, sizeof(CamBufferDesc));
  CamBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  CamBufferDesc.ByteWidth = sizeof(Camera);
  CamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  CamBufferDesc.CPUAccessFlags = 0;
  hr = g_device.CreateBuffer(&CamBufferDesc, nullptr, &g_Camera);
  if (FAILED(hr))
    return hr;

  //D3D11_BUFFER_DESC ModelBufferDesc;
  //memset(&ModelBufferDesc, 0, sizeof(ModelBufferDesc));
  //ModelBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  //ModelBufferDesc.ByteWidth = sizeof(CBChangesEveryFrame);
  //ModelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  //ModelBufferDesc.CPUAccessFlags = 0;
  //hr = g_device.CreateBuffer(&ModelBufferDesc, nullptr, &g_pCBChangesEveryFrame);
  //if (FAILED(hr))
  //  return hr;

  //g_modelBuffer.init(g_device, sizeof(CBChangesEveryFrame));
  //g_model.init(g_device, LD);
  //g_model2.init(g_device, LD);
  LD = g_modelLoader.LoadV2("Pistol.obj");
  LD2 = g_modelLoader2.LoadV2("Pistol.obj");
  Model Model1;
  Model1.init(g_device, LD);
  g_models.push_back(Model1);
  Model Model2;
  Model2.init(g_device, LD2);
  g_models.push_back(Model2);

  

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
  g_transform2.init();
  g_transform2.m_position.x = 1;
  g_models[currentModel].setActive(true);
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
  if (GetAsyncKeyState('P') & 0x8000) {
    currentModel = 0;
  }
  if (GetAsyncKeyState('O') & 0x8000) {
    currentModel = 1;
  }

}

void UI() {
  g_UI.update();
  
  bool show_demo_window = true;
  ImGui::ShowDemoWindow(&show_demo_window);
  ImGui::Begin("Textures");
  g_UI.menuBar(g_window, g_swapChain, g_backBuffer);
  g_captureScreenshot.ui(g_window, g_swapChain, g_backBuffer);
  if (ImGui::Button("New Model")) {
    LD2 = g_modelLoader2.LoadV2("Pistol.obj");
    Model Model1;
    Model1.init(g_device, LD2);
    g_models.push_back(Model1);
  }
  ImGui::End();
  g_modelLoader.ui();
  bool Stage = true;

  g_UI.Renderer(g_window, imguiSRV);
  

  
  ImGui::Begin("Stage", &Stage, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
  
  ImGui::End();

  ImGui::Begin("Properties", &Stage);
  ImGui::End();
  //ImGui::Begin("Facebook Style Window");

  // Imagen de perfil
  //ImGui::Image((void*)(intptr_t)my_image_texture_id, ImVec2(100, 100));

  // Nombre de usuario y estado
 // ImGui::Text("Nombre de Usuario");
  //ImGui::Text("Estado");

  // Cuadro de texto para publicación
  //static char post_text[1024] = "";
  //ImGui::InputTextMultiline("##Publicacion", post_text, IM_ARRAYSIZE(post_text), ImVec2(-1, 100));

  // Botón de publicación
 // if (ImGui::Button("Publicar", ImVec2(-1, 30)))
  //{
    // Agregar código aquí para publicar el mensaje
  //}

  //ImGui::End();
}
// Esta funcion esta encargada de actualizar la LOGICA del programa
void 
update(float deltaTime) {
  Input(deltaTime);

  UI();

  g_transform.update();

  for (int i = 0; i < g_models.size(); i++)
  {
    std::string n = "Transform" + i;
    g_models[i].update(g_deviceContext, n.c_str());
    g_models[i].ui(n.c_str());
  }
  
  
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
  //g_ModelTexture.destroy();
  for (int i = 0; i < g_textures.size(); i++)
  {
    g_textures[i].destroy();
  }

  //g_ModelTexture2.destroy();
  g_modelLoader.destroy();
  if (g_Camera) g_Camera->Release();
  for (int i = 0; i < g_models.size(); i++)
  {
    g_models[i].destroy();
  }

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
  // Configurar la textura IMGUI como la vista de renderizado
  g_deviceContext.m_deviceContext->OMSetRenderTargets(1, &imguiRTV, NULL);

  // Clear the back buffer
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  g_depthStencilView.render(g_deviceContext);
  g_renderTargetView.render(g_deviceContext, g_depthStencilView, 1, ClearColor);

  // Configurar el viewport
  g_viewport.render(g_deviceContext);

  // Establecer los shaders
  g_shaderProgram.render(g_deviceContext);

  // Establecer los constant buffers
  g_deviceContext.VSSetConstantBuffers(0, 1, &g_Camera);

  unsigned int startSlot = 0;
  for (int i = 0; i < g_models.size(); i++) {
    g_samplerLineal.render(g_deviceContext, 0, 1);
    g_models[i].render(g_deviceContext, 0, 1);
    startSlot++;
  }
  
  ID3D11ShaderResourceView* srvs[] = { imguiSRV };
  g_deviceContext.m_deviceContext->PSSetShaderResources(0, 1, srvs);

  g_textures[0].render(g_deviceContext, 0);

  // Dibujar
  // Copiar el back buffer a la textura IMGUI
  g_swapChain.m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&g_backBuffer.m_texture);
  g_deviceContext.m_deviceContext->CopyResource(imguiTexture, g_backBuffer.m_texture);
  g_backBuffer.m_texture->Release();

  // Renderizar la UI
  g_UI.render();

  // Presentar el back buffer
  g_swapChain.present();

}
