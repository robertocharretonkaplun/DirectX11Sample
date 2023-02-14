#pragma once

// Esta estructura esta encargada de almacenar la informacion que tendran nuestros objetos para
// despues dibujarse en pantalla.
struct 
SimpleVertex {
  XMFLOAT3 Pos; // Posiciones
  XMFLOAT2 Tex; // Coordenas de Textura

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

struct 
Transform {
  Vector3f Position;
  Vector3f Rotation;
  Vector3f Scale;
};
