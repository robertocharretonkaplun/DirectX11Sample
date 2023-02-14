#pragma once
#include "Prerequisites.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
class UserInterface
{
public:
  UserInterface();
  ~UserInterface();

  void 
  init(void* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
  void 
  update();
  void 
  render();
  void 
  destroy();
private:

};

