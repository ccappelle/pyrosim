#ifndef _ENVIRONMENT_HPP
#define _ENVIRONMENT_HPP

#pragma once

// standard
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

// ode
#include <ode/ode.h>

// local
#include "entity.hpp"

class Environment{
public:
    dWorldID world;
    dSpaceID topspace;
    // useful to split this into seperate maps
    std::vector<Entity*> entities; // general entities (bodies, joints, etc)

    // ENTITY, ACTUATOR, JOINT, NEURON, SYNAPSE, BODY, SENSOR
    std::vector< std::vector<int> > entityVectors;

    std::map<std::string, dSpaceID> subspaces;
    std::vector< std::pair <int, int> > collisions;

    Environment(dWorldID world, dSpaceID topspace, int numEntities = 50);
    ~Environment();

    // add to an existing entity using input from python
    void addToEntityFromPython(void);

    void addCollisionPair(int firstID, int secondID);
    void emptyCollisionPairs(void);
    
    // add new uninitialized entity to entities
    // and read in contents from python to that entity
    void readEntityFromPython(void);

    // create entities in simualiton
    void createInODE(void);

    // draw entities. Toggles for joints and spaces
    void draw(int drawJoints, int drawSpaces);

    dWorldID getWorld(){return this->world;};
    // returns spaces
    // in the future will be a map of space names to allow for using subspaces
    dSpaceID getSpace(std::string name);
    void createSpace(std::string name);

    Entity* getEntity(int i);
    // Actuator* getActuator(int i);
    
    void takeStep(int timeStep, dReal dt);
    void takeStep(std::vector<int> entityIDs, int timeStep, dReal dt);
    // write entities to python
    void writeToPython(void);

};

#endif