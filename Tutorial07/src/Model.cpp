#include "Model.h"
#include "Device.h"
#include "DeviceContext.h"

void
Model::init(Device device, LoadData loadData) {
  m_loadData = loadData;
  g_vertexBuffer.init(device, loadData);

  g_indexBuffer.init(device, loadData);

  g_modelBuffer.init(device, sizeof(CBChangesEveryFrame));

  m_transform.init();
}

void
Model::update(DeviceContext& deviceContext, std::string wName) {
  m_transform.update();
  CBChangesEveryFrame cb2;
  cb2.mWorld = XMMatrixTranspose(m_transform.m_matrix);
  XMFLOAT4                            g_vMeshColor(1, 1, 1, 1);
  cb2.vMeshColor = g_vMeshColor;
  g_modelBuffer.update(deviceContext, 0, nullptr, &cb2, 0, 0);
  //m_transform.ui(wName.c_str());
}

void
Model::render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumBuffers) {
  g_vertexBuffer.render(deviceContext, StartSlot, NumBuffers);
  g_indexBuffer.render(deviceContext);
  g_modelBuffer.VSSetConstantBuffers(deviceContext, 1, NumBuffers);
  g_modelBuffer.PSSetConstantBuffers(deviceContext, 1, NumBuffers);

  deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  deviceContext.DrawIndexed(m_loadData.numIndex, 0, 0);
}

void
Model::destroy() {
  g_modelBuffer.destroy();
  g_vertexBuffer.destroy();
  g_indexBuffer.destroy();
}

void
Model::ui(std::string wName) {
  static char post_text[1024] = "";
  char inputValue[64] = "GameObject";
  bool checkboxValue;
  bool checkboxValue2;
  ImGui::Begin(wName.c_str());
  // Agregar el checkbox a la izquierda del input text
  ImGui::Checkbox("", &checkboxValue);
  ImGui::SameLine();
  ImGui::InputText("", inputValue, IM_ARRAYSIZE(inputValue), 64);

  // Agregar el texto "Static" a la derecha del input text
  //ImGui::SameLine(0, ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 100);
  ImGui::SameLine();
  ImGui::Checkbox("Static", &checkboxValue2);
  static const char* items[] = { "Item 1", "Item 2", "Item 3" };
  static int currentItem = 0;
  ImGui::Text("Tag");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(258);
  ImGui::Combo("##TagCombo", &currentItem, items, IM_ARRAYSIZE(items));
  ImGui::SameLine();
  ImGui::Text("Layer");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(258);
  ImGui::Combo("##LayerCombo", &currentItem, items, IM_ARRAYSIZE(items));


  ImGui::Separator();
  // Transform Component
  ImGui::SetNextItemWidth(200);
  if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
    m_transform.ui_noWindow("");

  }
  ImGui::Separator();
  // Mesh Component
  if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Vertices: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string( m_loadData.numVertex).c_str());
    ImGui::Text("Indices: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string( m_loadData.numIndex).c_str());
    ImGui::Text("VERTICES");
    if (ImGui::Button("Load Vertex Data")) {
      std::string vertexPositions;
      for (int i = 0; i < m_loadData.vertex.size(); i++) {
        vertexPositions += "(" + std::to_string(m_loadData.vertex[i].Pos.x) + ", " +
          std::to_string(m_loadData.vertex[i].Pos.y) + ", " +
          std::to_string(m_loadData.vertex[i].Pos.z) + ") \n ";
      }
    }


  }
  ImGui::Separator();
  ImGui::End();
}

void
Model::setTransform(Transform t) {
  m_transform = t;
}

Transform
Model::getTransform() {
  return m_transform;
}
