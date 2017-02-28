#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plyparse.h"

/*
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
*/

ply new_ply(FILE * f){
  ply p;
  int vsize = 0, fsize = 0;
  int i, j, len = 1;
  char buffer[256];
  f = fopen("bunny.ply", "r");
  fscanf(f, "%s", buffer);
  while(strcmp(buffer, "end_header")){
    if(!strcmp(buffer, "element")){
      fscanf(f, "%s", buffer);
      if(!strcmp(buffer, "vertex")){
        fscanf(f, "%d", &vsize);
      }
      else if(!strcmp(buffer, "face")){
        fscanf(f, "%d", &fsize);
      }
    }
    fscanf(f, "%s", buffer);
  }
  p.vsize = vsize;
  p.fsize = fsize;
  p.vert = (float *) malloc(sizeof(float[vsize*3]));
  p.norm = (float *) malloc(sizeof(float[vsize*3]));
  for(i = 0; i < vsize; i++){
    for(j = 0; j < 3; j++){
      fscanf(f, "%f", &p.vert[i*3 + j]);
    }
    for(j = 0; j < 3; j++){
      fscanf(f, "%f", &p.norm[i*3 + j]);
    }
  }
  fscanf(f, "%d", &len);
  p.face = (int *) malloc(sizeof(int[fsize*len]));
  for(i = 0; i < fsize; i++){
    for(j = 0; j < len; j++){
      fscanf(f, "%d", &p.face[i*len + j]);
    }
    fscanf(f, "%d", &len);
  }
  fclose(f);
  return p;
}

/*
int main(void){
  FILE * f;
  ply p;
  int i;
  p = new_ply(f);
  printf("Number of Vertices and Normals: %d\n", p.vsize);
  printf("Number of Surfaces: %d\n", p.fsize);
}
*/
