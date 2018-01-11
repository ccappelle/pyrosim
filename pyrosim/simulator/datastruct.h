#ifndef DATASTRUCT_H
#define DATASTRUCT_H

const int MAX_GROUPS = 1000;
struct Data
{
  char texturePathStr[1000];
  //simulator parameters
  int runBlind;
  float gravity;
  float dt;
  int evaluationTime;
  //user option
  int debug;
  //camera parameters
  float xyz[3];
  float hpr[3];
  int trackBody;
  int followBody;
  int collisionMatrix[MAX_GROUPS][MAX_GROUPS];
  int numCollisionGroups;
  int capture;

  int windowWidth = 750;
  int windowHeight = 450;
};

#endif