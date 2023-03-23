#include "Transform.h"

Transform::Transform() {
}

Transform::~Transform() {
}

void 
Transform::init() {
  m_pos = new float[3];
  m_rot = new float[3];
  m_sca = new float[3];

  m_scale.x = 1;
  m_scale.y = 1;
  m_scale.z = 1;

  m_matrix = XMMatrixIdentity();
}

void 
Transform::update() {
  m_matrix = XMMatrixScaling(m_scale.x, 
                             m_scale.y, 
                             m_scale.z) * 
            XMMatrixRotationRollPitchYaw(m_rotation.x,
                                         m_rotation.y,
                                         m_rotation.z) *
            XMMatrixTranslation(m_position.x, 
                                m_position.y, 
                                m_position.z);
}

void 
Transform::render() {
}

void 
Transform::destroy() {
}

void 
Transform::ui() {
  ImGui::Begin("Transform");
  // Get the position
  m_pos[0] = m_position.x;
  m_pos[1] = m_position.y;
  m_pos[2] = m_position.z;
  
  m_rot[0] = m_rotation.x;
  m_rot[1] = m_rotation.y;
  m_rot[2] = m_rotation.z;

  m_sca[0] = m_scale.x;
  m_sca[1] = m_scale.y;
  m_sca[2] = m_scale.z;


  // Draw the structure
  UI.vec3Control("Position", m_pos);
  UI.vec3Control("Rotation", m_rot);
  UI.vec3Control("Scale", m_sca);

  // Set the position 
  m_position.x = m_pos[0];
  m_position.y = m_pos[1];
  m_position.z = m_pos[2];
  
  m_rotation.x = m_rot[0];
  m_rotation.y = m_rot[1];
  m_rotation.z = m_rot[2];
  
  m_scale.x = m_sca[0];
  m_scale.y = m_sca[1];
  m_scale.z = m_sca[2];

  ImGui::End();
}
