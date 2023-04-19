#pragma once
#include "Prerequisites.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "UserInterface.h"
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
  update(DeviceContext& deviceContext, std::string wName);
  
  void 
  render(DeviceContext& deviceContext, unsigned int StartSlot,unsigned int NumBuffers);
  
  void 
  destroy();
  
  void
  ui(std::string wName);

  void 
  setTransform(Transform t);

  Transform
  getTransform();

  bool isActive() const { return m_active; }
  void setActive(bool active) { m_active = active; }
  void setStatic(bool _static) { m_static = _static; }

public:
  VertexBuffer                        g_vertexBuffer;
  IndexBuffer                         g_indexBuffer;
  ConstantBuffer                      g_modelBuffer;
  LoadData m_loadData;
  Transform m_transform;
  bool m_active = true;
  bool m_static = false;
};