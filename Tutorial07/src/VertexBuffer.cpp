#include "VertexBuffer.h"
#include "Device.h"
#include "DeviceContext.h"
void 
VertexBuffer::init(Device device, LoadData loadData) {
  if (device.m_device == nullptr) {
    WARNING("ERROR: VertexBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
    exit(1);
  }
  else if (loadData.vertex.size() == 0) {
    WARNING("ERROR: VertexBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Load loadData.vertex.Size()] \n");
    exit(1);
  }
  else if (loadData.numVertex == 0) {
    WARNING("ERROR: VertexBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Load loadData.numVertex] \n");
    exit(1);
  }

  HRESULT hr = S_OK;
  D3D11_BUFFER_DESC vertexBufferDesc;
  memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = m_stride * loadData.numVertex;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = loadData.vertex.data();
  hr = device.CreateBuffer(&vertexBufferDesc, &InitData, &m_vertexBuffer);

  if (FAILED(hr)) {
    WARNING("ERROR: VertexBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR CreateBuffer()] \n");
    exit(1);
  }
}

void 
VertexBuffer::update() {
}

void 
VertexBuffer::render(DeviceContext & deviceContext, unsigned int StartSlot) {
  deviceContext.IASetVertexBuffers(StartSlot, 1, &m_vertexBuffer, &m_stride, &m_offset);
}

void 
VertexBuffer::destroy() {
  SAFE_RELEASE(m_vertexBuffer);
}
