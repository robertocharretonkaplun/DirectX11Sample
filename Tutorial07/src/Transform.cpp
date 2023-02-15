#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::init()
{
  m_pos = new float[3];
}

void Transform::update()
{
}

void Transform::render()
{
}

void Transform::destroy()
{
}

void Transform::ui()
{
  ImGui::Begin("Transform");
  // Get the position
  m_pos[0] = Position.x;
  m_pos[1] = Position.y;
  m_pos[2] = Position.z;

  // Draw the structure
  UI.vec3Control("Position", m_pos);

  // Set the position 
  Position.x = m_pos[0];
  Position.y = m_pos[1];
  Position.z = m_pos[2];
  ImGui::End();
}
