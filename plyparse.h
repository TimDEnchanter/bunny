#ifndef PLYPARSE_H
#define PLYPARSE_H

typedef struct ply{
  //Number of vertices and normals
  int vsize;

  //Number of surfaces
  int fsize;

  //Array for vertices
  float * vert;

  //Array for normals
  float * norm;

  //Array for surfaces
  int * face;
} ply;

ply new_ply(FILE * f);

#endif
