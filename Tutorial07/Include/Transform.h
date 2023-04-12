#pragma once
#include "Prerequisites.h"
#include "Commons.h"
#include "UserInterface.h"

class 
Transform {
public:
	Transform();
	~Transform();

	void 
  init();

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

  void 
  ui(std::string wName);
public:
	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scale;
  XMMATRIX m_matrix;
  UserInterface UI;
  float* m_pos;
private:
  // UI data
  float* m_rot;
  float* m_sca;
};