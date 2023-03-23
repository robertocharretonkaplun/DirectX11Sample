#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"

void 
DepthStencilView::init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT Format) {
  if (device.m_device == nullptr) {
    WARNING("ERROR: DepthStencilView::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
    exit(1);
  }
  else if (depthStencil == nullptr) {
    WARNING("ERROR: DepthStencilView::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3D11Resource* depthStencil] \n");
    exit(1);
  }
  else {
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    memset(&descDSV, 0, sizeof(descDSV));
    descDSV.Format = Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    
    device.CreateDepthStencilView(depthStencil, &descDSV, &m_depthStencilView);
  }
}

void 
DepthStencilView::update() {
}

void 
DepthStencilView::render(DeviceContext& deviceContext) {
  deviceContext.ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void 
DepthStencilView::destroy() {
  SAFE_RELEASE(m_depthStencilView);
}
