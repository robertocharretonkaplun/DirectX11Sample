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
  Loader.LoadFile(objFileName);

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
    LD.vertex[i].Tex.y = Loader.LoadedVertices[i].TextureCoordinate.Y;
  }

  // Load and set data to custom structure (Indices)
  LD.index.resize(Loader.LoadedIndices.size());
  for (int i = 0; i < LD.index.size(); i++) {
    LD.index[i] = Loader.LoadedIndices[i];
  }

  LD.numVertex = LD.vertex.size();
  LD.numIndex = LD.index.size();
  return LD;
}
