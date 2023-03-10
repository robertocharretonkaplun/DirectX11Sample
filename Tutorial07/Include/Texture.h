#pragma once
#include "Prerequisites.h"
#include "Commons.h"
class Device;

class 
Texture {
public:
	Texture() = default;
  ~Texture();

	void 
  init(Device device, std::string textureName);

  void
  init(Device device,
       unsigned int width, 
       unsigned int height, 
       DXGI_FORMAT Format, 
       unsigned int BindFlags);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();
public:
  // This variable is in charge of handle a texture resource as data
  ID3D11Texture2D* m_texture = nullptr;
  // This variable is in charge of handle a texture resource as image data
  ID3D11ShaderResourceView* m_textureFromImg; // m_textureSRV
};
