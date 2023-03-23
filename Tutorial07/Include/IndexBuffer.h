#pragma once
#include "Prerequisites.h"
#include "Commons.h"
class Device;
class DeviceContext;

class
IndexBuffer {
public:
  IndexBuffer() = default;
  ~IndexBuffer() {};

  void
  init(Device device, LoadData loadData);

  void
  update();

  void
  render(DeviceContext& deviceContext);

  void
  destroy();

private:
  ID3D11Buffer* m_indexBuffer = nullptr;
  unsigned int m_stride = sizeof(SimpleVertex);
};
