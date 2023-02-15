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
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale;
  UserInterface UI;
private:
  // UI data
  float* m_pos;
};