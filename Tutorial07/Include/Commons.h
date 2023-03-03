#pragma once
#include "Prerequisites.h"

// Esta estructura esta encargada de almacenar la informacion que tendran nuestros objetos para
// despues dibujarse en pantalla.
struct 
SimpleVertex {
  XMFLOAT3 Pos; // Posiciones
  XMFLOAT2 Tex; // Coordenas de Textura

};

struct
LoadData {
  std::string name;
  std::vector <SimpleVertex> vertex;
  std::vector <unsigned int> index;
  int numVertex;
  int numIndex;
};

struct 
Camera {
  XMMATRIX mView;
  XMMATRIX mProjection;
};

struct 
CBChangesEveryFrame {
  XMMATRIX mWorld; // Modelo / mesh / objeto 3D -> mMesh -> Posiciones, Rotaciones y escalas de nuestro objeto
  XMFLOAT4 vMeshColor; // Texturas (R,G,B,A) (1,0,3,1)
};

struct 
Vector3f {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};
