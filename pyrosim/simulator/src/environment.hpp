#ifndef _ENVIRONMENT_HPP
#define _ENVIRONMENT_HPP

#pragma once

// standard
#include <iostream>
#include <string>
#include <vector>

// ode
#include <ode/ode.h>

// local
#include "entity.hpp"

class Environment{
public:
    dWorldID world;
    dSpaceID space;
    std::vector<Entity*> entities;

    Environment(dWorldID world, dSpaceID topspace, int numEntities = 50);
    ~Environment();

    // add to an existing entity using input from python
    void addToEntityFromPython(void);

    // add new uninitialized entity to entities
    // and read in contents from python to that entity
    void readEntityFromPython(void);

    // create entities in simualiton
    void createEntities(void);

    // draw entities. Toggles for joints and spaces
    void draw(int drawJoints, int drawSpaces);

    dWorldID getWorld(){return this->world;};
    // returns spaces
    // in the future will be a map of space names to allow for using subspaces
    dSpaceID getSpace(std::string name){return this->space;}; // currently returns topspace

    dBodyID getBody(int i); 
    // write entities to python
    void writeToPython(void);
};

#endif