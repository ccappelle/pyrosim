#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#pragma once

#include <drawstuff/drawstuff.h>
// drawing necessity
#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawTriangle dsDrawTriangleD
#endif

class Environment;

class Entity{
protected:
    int entityID;
    std::string drawName; // currently unused name to specify drawing toggle. Currently just body and joint
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

    // writes output to cerr (may be unused)
    virtual void writeToPython(void){};

    // sets id (aka placement in entity list in env)
    virtual void setID(int myID){this->entityID = myID;};
    virtual int getID(){return this->entityID;};
    virtual std::string getDrawName(){return this->drawName;};
};
#endif