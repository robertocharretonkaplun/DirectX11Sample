#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class 
DepthStencilView {
public:
	DepthStencilView() = default;
	~DepthStencilView() { SAFE_RELEASE(m_depthStencilView); };

	void
	init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT Format);

	void
	update();

	void
	render(DeviceContext& deviceContext);

	void
	destroy();

public:
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
