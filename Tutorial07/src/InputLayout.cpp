#include "InputLayout.h"
#include "Device.h"

void 
InputLayout::init(Device device, 
  std::vector<D3D11_INPUT_ELEMENT_DESC> Layout, 
  ID3DBlob* VertexShaderData) {
	if (Layout.size() == 0) {
    WARNING("ERROR: InputLayout::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR std::vector<D3D11_INPUT_ELEMENT_DESC> Layout Size()] \n");
    exit(1);
	}
  else if (VertexShaderData == nullptr) {
    WARNING("ERROR: InputLayout::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR ID3DBlob* VertexShaderData] \n");
    exit(1);
  }

	device.CreateInputLayout(Layout.data(), 
                           Layout.size(), 
                           VertexShaderData->GetBufferPointer(),
                           VertexShaderData->GetBufferSize(), 
                           &m_inputLayout);
}

void 
InputLayout::update() {
}

void 
InputLayout::render() {
}

void 
InputLayout::destroy() {
  SAFE_RELEASE(m_inputLayout);
}
