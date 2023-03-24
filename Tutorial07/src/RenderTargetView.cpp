#include "RenderTargetView.h"
#include "Device.h"
#include "Texture.h"
#include "DeviceContext.h"
#include "DepthStencilView.h"
void 
RenderTargetView::init(Device device, Texture backBuffer, DXGI_FORMAT Format) {
  if (device.m_device == nullptr) {
    WARNING("ERROR: RenderTargetView::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
    exit(1);
  }
  else if (backBuffer.m_texture == nullptr) {
    WARNING("ERROR: RenderTargetView::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Texture backBuffer] \n");
    exit(1);
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc = {};
  desc.Format = Format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

  device.CreateRenderTargetView(backBuffer.m_texture, &desc, &m_renderTargetView);
}

void  
RenderTargetView::update() {
}

void 
RenderTargetView::render(DeviceContext & deviceContext, DepthStencilView & depthStencilView, unsigned int numViews, float ClearColor[4])
{
  deviceContext.ClearRenderTargetView(m_renderTargetView, ClearColor);
  deviceContext.OMSetRenderTargets(numViews, &m_renderTargetView, depthStencilView.m_depthStencilView);
}

void 
RenderTargetView::destroy() {
  SAFE_RELEASE(m_renderTargetView);
}
