#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"
class Device;

class 
ShaderProgram {
public:
	ShaderProgram() = default;
	~ShaderProgram() {};

  void
  init(Device device, 
       std::string fileName, 
       std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

  HRESULT 
  CompileShaderFromFile(char* szFileName, 
                        LPCSTR szEntryPoint, 
                        LPCSTR szShaderModel, 
                        ID3DBlob** ppBlobOut); 

  void
  CreateVertexShader(Device device);

  void
  CreateInputLayout(Device device, std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

  void
  CreatePixelShader(Device device);
public:
  ID3D11VertexShader* m_VertexShader = nullptr;
  ID3D11PixelShader* m_PixelShader = nullptr;
  InputLayout m_inputLayout;
private:
  std::string m_shaderFileName;
  ID3DBlob* m_vertexShaderData = nullptr;
  ID3DBlob* m_pixelShaderData = nullptr;
};