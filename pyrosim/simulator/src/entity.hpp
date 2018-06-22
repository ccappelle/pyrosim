#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#pragma once

#include <drawstuff/drawstuff.h>
#include <ode/ode.h>
// drawing necessity
#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawTriangle dsDrawTriangleD
#endif

enum EntityType {ENTITY=0, ACTUATOR, JOINT, NEURON, SYNAPSE, BODY, SENSOR};

class Environment;

class Entity{
protected:
    int entityID;
public:
    // addition function prototype 
    // allows extra python inputs to act on entity
    // specifically useful for adding bodies to composite body
    virtual void readAdditionFromPython(void) {}; 

    // main reading in function 
    virtual void readFromPython(void) =0;

    // creates entity in environent
    virtual void create(Environment *environment) =0;

    // draws entity (may be unused)
    virtual void draw(void){};
    virtual void takeStep(int timeStep, dReal dt) {};

    // writes output to cerr (may be unused)
    virtual void writeToPython(void){};

    // sets id (aka placement in entity list in env)
    virtual void setID(int myID){this->entityID = myID;};
    virtual int getID(){return this->entityID;};
    virtual EntityType getEntityType(void){return ENTITY;};
};

#endif