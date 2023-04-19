#include "Model.h"
#include "Device.h"
#include "DeviceContext.h"

void
Model::init(Device device, LoadData loadData) {
  m_loadData = loadData;
  g_vertexBuffer.init(device, loadData);

  g_indexBuffer.init(device, loadData);

  g_modelBuffer.init(device, sizeof(CBChangesEveryFrame));

  m_transform.init();
}

void
Model::update(DeviceContext& deviceContext, std::string wName) {
  if (!m_static) {
    m_transform.update();
    CBChangesEveryFrame cb2;
    cb2.mWorld = XMMatrixTranspose(m_transform.m_matrix);
    XMFLOAT4                            g_vMeshColor(1, 1, 1, 1);
    cb2.vMeshColor = g_vMeshColor;
    g_modelBuffer.update(deviceContext, 0, nullptr, &cb2, 0, 0);
  }
}

void
Model::render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumBuffers) {
  if (m_active) {
    g_vertexBuffer.render(deviceContext, StartSlot, NumBuffers);
    g_indexBuffer.render(deviceContext);
    g_modelBuffer.VSSetConstantBuffers(deviceContext, 1, NumBuffers);
    g_modelBuffer.PSSetConstantBuffers(deviceContext, 1, NumBuffers);

    deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext.DrawIndexed(m_loadData.numIndex, 0, 0);
  }
}

void
Model::destroy() {
  g_modelBuffer.destroy();
  g_vertexBuffer.destroy();
  g_indexBuffer.destroy();
}

void
Model::setTransform(Transform t) {
  m_transform = t;
}

Transform
Model::getTransform() {
  return m_transform;
}
