#include "SamplerState.h"
#include "Device.h"

void 
SamplerState::init(Device device) {
  if (device.m_device == nullptr) {
    WARNING("ERROR: SamplerState::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
    exit(1);
  }

  HRESULT hr = S_OK;
  D3D11_SAMPLER_DESC sampDesc;
  memset(&sampDesc, 0, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  hr = device.CreateSamplerState(&sampDesc, &m_sampler);

  if (FAILED(hr)) {
    WARNING("ERROR: SamplerState::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR CreateSamplerState] \n");
    exit(1);
  }
}

void 
SamplerState::update() {
}

void 
SamplerState::render() {
}

void 
SamplerState::destroy() {
  SAFE_RELEASE(m_sampler);
}
