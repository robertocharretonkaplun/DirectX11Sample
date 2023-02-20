#pragma once
#include "Prerequisites.h"
#include "Commons.h"
#include "UserInterface.h"

class Transform
{
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
  ui();
public:
	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_scale;
  UserInterface UI;
private:
  // UI data
  float* m_pos;
  float* m_rot;
  float* m_sca;
};