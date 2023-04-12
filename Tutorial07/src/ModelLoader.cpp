#include "ModelLoader.h"
#include "OBJ_Loader.h"

ModelLoader::ModelLoader() {
}

ModelLoader::~ModelLoader() {
}

void 
ModelLoader::init() {
}

void 
ModelLoader::update() {
  
}

void 
ModelLoader::render() {
}

void 
ModelLoader::destroy() {
}

LoadData 
ModelLoader::Load(std::string objFileName) {
  // Temporal Load Data
  LoadData LD;
  objl::Loader Loader;

  bool Model = Loader.LoadFile(objFileName);

  if (Model) {
    int totalVertices = Loader.LoadedVertices.size();
    int loadedVertices = 0;

    LD.name = objFileName;

    WARNING("ModelLoader::Load() [Load of file: " << objFileName.c_str() << "] \n");

    // Load and set data to custom structure (SimpleVertex)
    LD.vertex.resize(Loader.LoadedVertices.size());

    for (int i = 0; i < LD.vertex.size(); i++) {
      // Load Positions
      LD.vertex[i].Pos.x = Loader.LoadedVertices[i].Position.X;
      LD.vertex[i].Pos.y = Loader.LoadedVertices[i].Position.Y;
      LD.vertex[i].Pos.z = Loader.LoadedVertices[i].Position.Z;

      // Load Textures
      LD.vertex[i].Tex.x = Loader.LoadedVertices[i].TextureCoordinate.X;
      LD.vertex[i].Tex.y = 1.0f - Loader.LoadedVertices[i].TextureCoordinate.Y;
      loadedVertices++;
      percentage = (float)loadedVertices / (float)totalVertices * 100;
      WARNING("Cargando modelo: " << percentage << "% \n");
    }

    // Load and set data to custom structure (Indices)
    LD.index.resize(Loader.LoadedIndices.size());
    for (int i = 0; i < LD.index.size(); i++) {
      LD.index[i] = Loader.LoadedIndices[i];
    }

    LD.numVertex = LD.vertex.size();
    LD.numIndex = LD.index.size();
  }
  return LD;
}

LoadData 
ModelLoader::LoadV2(std::string objFileName) {
  LoadData ld;
  objl::Loader loader;

  if (loader.LoadFile(objFileName)) {
    ld.name = objFileName;

    // Reserve capacity for the vectors
    ld.vertex.reserve(loader.LoadedVertices.size());
    ld.index.reserve(loader.LoadedIndices.size());

    // Load the vertices
    for (auto& vertex : loader.LoadedVertices) {
      ld.vertex.emplace_back(SimpleVertex {
        { vertex.Position.X, vertex.Position.Y, vertex.Position.Z },
        { vertex.TextureCoordinate.X, 1.0f - vertex.TextureCoordinate.Y }
      });
    }

    // Load the indices
    for (auto index : loader.LoadedIndices) {
      ld.index.push_back(index);
    }

    ld.numVertex = ld.vertex.size();
    ld.numIndex =  ld.index.size();
  }

  return ld;
}

void 
ModelLoader::ui() {
  ImGui::Begin("Activity Progress");
  ImGui::Text("OBJ");
  ImGui::Text("0/0");
  ImGui::SameLine(50.0f);
  ImGui::ProgressBar(percentage, ImVec2(0.0f, 0.0f));
  ImGui::End();
}
