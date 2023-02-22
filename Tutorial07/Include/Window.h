#include "Prerequisites.h"
#pragma once

class 
Window {
public:
	Window();
	~Window();

	HRESULT 
  init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();


public:
  HWND m_hWnd = nullptr;
  HINSTANCE m_hInst = nullptr;
  RECT m_rect;
  unsigned int m_width;
  unsigned int m_height;
  std::string m_windowName = "HeatBlast Engine";
};
