#include "Device.h"

void 
Device::init() {
}

void 
Device::update() {
}

void 
Device::render() {
}

void 
Device::destroy() {
}

HRESULT
Device::CreateRenderTargetView(ID3D11Resource* pResource, 
                               D3D11_RENDER_TARGET_VIEW_DESC* pDesc, 
                               ID3D11RenderTargetView** ppRTView) {
  HRESULT hr = S_OK;
  if (pResource != nullptr || ppRTView != nullptr) {
    hr = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);
    WARNING("Device::CreateRenderTargetView : [CREATION OF RESOURCE : OK] \n");
  }
  else {
    WARNING("ERROR: Device::CreateRenderTargetView : Error in data from params [CHECK FOR METHOD INITIALIZATION] \n");
    exit(1);
    //return hr;
  }
  return hr;
}

HRESULT 
Device::CreateTexture2D(D3D11_TEXTURE2D_DESC* pDesc, 
                        const D3D11_SUBRESOURCE_DATA* pInitialData, 
                        ID3D11Texture2D** ppTexture2D) {
  HRESULT hr = S_OK;
  if (pDesc != nullptr || ppTexture2D != nullptr) {
    hr = m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
    WARNING("Device::CreateTexture2D : [CREATION OF RESOURCE : OK] \n");
  }
  else {
    WARNING("ERROR: Device::CreateTexture2D : Error in data from params [CHECK FOR METHOD INITIALIZATION] \n");
    exit(1);
  }
  return hr;
}

HRESULT 
Device::CreateDepthStencilView(ID3D11Resource* pResource, 
                               const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, 
                               ID3D11DepthStencilView** ppDepthStencilView) {
  HRESULT hr = S_OK;

  if (pResource == nullptr) {
    WARNING("ERROR: Device::CreateDepthStencilView : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11Resource* pResource] \n");
    exit(1);
  }
  else if (ppDepthStencilView == nullptr) {
    WARNING("ERROR: Device::CreateDepthStencilView : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11DepthStencilView** ppDepthStencilView] \n");
    exit(1);
  }
  else {
    hr = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
    WARNING("Device::CreateDepthStencilView : [CREATION OF RESOURCE : OK] \n");
  }

  return hr;
}

HRESULT 
Device::CreateVertexShader(const void* pShaderBytecode, 
                           unsigned int BytecodeLength, 
                           ID3D11ClassLinkage* pClassLinkage, 
                           ID3D11VertexShader** ppVertexShader) {
  HRESULT hr = S_OK;
  
  if (pShaderBytecode == nullptr) {
    WARNING("ERROR: Device::CreateVertexShader : [CREATION OF RESOURCE : FALSE] [CHECK FOR const void* pShaderBytecode] \n");
    exit(1);
  }
  else if (ppVertexShader == nullptr) {
    WARNING("ERROR: Device::CreateVertexShader : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11VertexShader** ppVertexShader] \n");
    exit(1);
  }
  else {
    hr = m_device->CreateVertexShader(pShaderBytecode, 
                                      BytecodeLength, 
                                      pClassLinkage, 
                                      ppVertexShader);
    WARNING("Device::CreateVertexShader : [CREATION OF RESOURCE : OK] \n");
  }
  return hr;
}

HRESULT 
Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, 
                          unsigned int NumElements, 
                          const void* pShaderBytecodeWithInputSignature, 
                          unsigned int BytecodeLength, 
                          ID3D11InputLayout** ppInputLayout) {
  HRESULT hr = S_OK;
  if (pInputElementDescs == nullptr) {
    WARNING("ERROR: Device::CreateInputLayout : [CREATION OF RESOURCE : FALSE] [CHECK FOR const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs] \n");
    exit(1);
  }
  else if (pShaderBytecodeWithInputSignature == nullptr) {
    WARNING("ERROR: Device::CreateInputLayout : [CREATION OF RESOURCE : FALSE] [CHECK FOR const void* pShaderBytecodeWithInputSignature] \n");
    exit(1);
  }
  else if (ppInputLayout == nullptr) {
    WARNING("ERROR: Device::CreateInputLayout : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11InputLayout** ppInputLayout] \n");
    exit(1);
  }
  else {
    hr = m_device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    WARNING("Device::CreateInputLayout : [CREATION OF RESOURCE : OK] \n");
  }
  return hr;
}

HRESULT 
Device::CreatePixelShader(const void* pShaderBytecode, 
                          unsigned int BytecodeLength, 
                          ID3D11ClassLinkage* pClassLinkage, 
                          ID3D11PixelShader** ppPixelShader) {
  HRESULT hr = S_OK;
  if (pShaderBytecode == nullptr) {
    WARNING("ERROR: Device::CreatePixelShader : [CREATION OF RESOURCE : FALSE] [CHECK FOR const void* pShaderBytecode] \n");
    exit(1);
  }
  else if (ppPixelShader== nullptr) {
    WARNING("ERROR: Device::CreatePixelShader : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11PixelShader** ppPixelShader] \n");
    exit(1);
  }
  else {
    hr = m_device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    WARNING("Device::CreatePixelShader : [CREATION OF RESOURCE : OK] \n");
  }
  return hr;
}

HRESULT 
Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, 
                     const D3D11_SUBRESOURCE_DATA* pInitialData, 
                     ID3D11Buffer** ppBuffer) {
  HRESULT hr = S_OK;
  if (pDesc == nullptr) {
    WARNING("ERROR: Device::CreateBuffer : [CREATION OF RESOURCE : FALSE] [CHECK FOR const D3D11_BUFFER_DESC* pDesc] \n");
    exit(1);
  }
  //else if (pInitialData == nullptr) {
  //  WARNING("ERROR: Device::CreateBuffer : [CREATION OF RESOURCE : FALSE] [CHECK FOR const D3D11_SUBRESOURCE_DATA* pInitialData] \n");
  //  exit(1);
  //}
  else if (ppBuffer == nullptr) {
    WARNING("ERROR: Device::CreateBuffer : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11Buffer** ppBuffer] \n");
    exit(1);
  }
  else {
    hr = m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);
    WARNING("Device::CreateBuffer : [CREATION OF RESOURCE : OK] \n");
  }
  return hr;
}

HRESULT Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, 
                                   ID3D11SamplerState** ppSamplerState) {
  HRESULT hr = S_OK;
  if (pSamplerDesc == nullptr) {
    WARNING("ERROR: Device::CreateSamplerState : [CREATION OF RESOURCE : FALSE] [CHECK FOR const D3D11_SAMPLER_DESC* pSamplerDesc] \n");
    exit(1);
  }
  else if (ppSamplerState== nullptr) {
    WARNING("ERROR: Device::CreateSamplerState : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11SamplerState** ppSamplerState] \n");
    exit(1);
  }
  else {
    hr = m_device->CreateSamplerState(pSamplerDesc, ppSamplerState);
    WARNING("Device::CreateSamplerState : [CREATION OF RESOURCE : OK] \n");
  }
  return hr;
}
