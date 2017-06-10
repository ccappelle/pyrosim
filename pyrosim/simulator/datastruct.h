#ifndef DATASTRUCT_H
#define DATASTRUCT_H

struct Data
{
  char texturePathStr[100];
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
};

#endif