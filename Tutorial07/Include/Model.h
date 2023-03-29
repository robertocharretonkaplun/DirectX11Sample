#pragma once
#include "Prerequisites.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
class Device;
class DeviceContext;

class 
Model {
public:
	Model() = default;
	~Model() {};

	void 
  init(Device device, LoadData loadData);

  void 
  update(DeviceContext& deviceContext,
    unsigned int DstSubresource,
    const D3D11_BOX* pDstBox,
    const void* pSrcData,
    unsigned int SrcRowPitch,
    unsigned int SrcDepthPitch);
  
  void 
  render(DeviceContext& deviceContext, unsigned int StartSlot);
  
  void 
  destroy();

public:
  VertexBuffer                        g_vertexBuffer;
  IndexBuffer                         g_indexBuffer;
  ConstantBuffer                      g_modelBuffer;
  LoadData m_loadData;
  Transform m_transform;
};