#pragma once
#include "Prerequisites.h"
#include "Commons.h"
class Device;
class DeviceContext;

class 
VertexBuffer {
public:
	VertexBuffer() = default;
  ~VertexBuffer() {};

	void 
  init(Device device, LoadData loadData);

  void 
  update();
  
  void 
  render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumBuffers);
  
  void 
  destroy();

private:
	ID3D11Buffer* m_vertexBuffer = nullptr;
  unsigned int m_stride = sizeof(SimpleVertex);
  unsigned int m_offset = 0;
};
