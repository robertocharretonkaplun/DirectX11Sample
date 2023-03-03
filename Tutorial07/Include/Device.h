#pragma once
#include "Prerequisites.h"

class 
Device {
public:
	Device() = default;
	~Device() { SAFE_RELEASE(m_device); };

	void 
  init();

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

  HRESULT
  CreateRenderTargetView(ID3D11Resource *pResource,
                         D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
                         ID3D11RenderTargetView **ppRTView);

  HRESULT
  CreateTexture2D(D3D11_TEXTURE2D_DESC *pDesc,
                  const D3D11_SUBRESOURCE_DATA *pInitialData,
                  ID3D11Texture2D **ppTexture2D);

  HRESULT
  CreateDepthStencilView(ID3D11Resource *pResource,
                         const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
                         ID3D11DepthStencilView **ppDepthStencilView);


  HRESULT 
  CreateVertexShader(const void *pShaderBytecode,
                     unsigned int BytecodeLength,
                     ID3D11ClassLinkage *pClassLinkage,
                     ID3D11VertexShader **ppVertexShader);

  HRESULT
  CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
                    unsigned int NumElements,
                    const void *pShaderBytecodeWithInputSignature,
                    unsigned int  BytecodeLength,
                    ID3D11InputLayout **ppInputLayout);

  HRESULT
  CreatePixelShader(const void *pShaderBytecode,
                    unsigned int BytecodeLength,
                    ID3D11ClassLinkage *pClassLinkage,
                    ID3D11PixelShader **ppPixelShader);

  HRESULT
  CreateBuffer(const D3D11_BUFFER_DESC *pDesc,
               const D3D11_SUBRESOURCE_DATA *pInitialData,
               ID3D11Buffer **ppBuffer);

  HRESULT
  CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc,
                     ID3D11SamplerState **ppSamplerState);

public:
  ID3D11Device* m_device = nullptr;
};
