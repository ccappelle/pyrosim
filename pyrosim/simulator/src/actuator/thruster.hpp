#ifndef __THRUSTER_HPP__
#define __THRUSTER_HPP__

#pragma once

#include "actuator/actuator.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"

class ThrusterActuator : public Actuator
{
protected:
    float low_force, high_force;
    float direction[3];
    dBodyID body;
    int bodyID;
    dReal dt;
    dReal lastForce[3];
public:
    ThrusterActuator(){};

    void create(Environment *environment){
        RigidBody *bodyObj = (RigidBody *) environment->getEntity(this->bodyID);
        this->body = bodyObj->getBody();
    }

    void readFromPython(void){
        readValueFromPython<int>(&this->bodyID, "Body ID");
        readValueFromPython<float>(&this->low_force, "Low Force");
        readValueFromPython<float>(&this->high_force, "High Force");
        readValueFromPython<float>(this->direction, 3, "Thruster Direction");
    }

    void actuate(dReal input, dReal dt){
        this->actuateByMagnitude(input, dt);
        // dReal normalizedInput = (input + 1.0) / 2.0;
        // dReal forceMagnitude = normalizedInput * (high_force - low_force) + low_force;

        // std::cerr << forceMagnitude << " " << high_force << " "
        // const dReal dir[3] = {direction[0] * forceMagnitude * timeStep,
        //                       direction[1] * forceMagnitude * timeStep,
        //                       direction[2] * forceMagnitude * timeStep};

        // dBodyAddForce(this->body, dir[0], dir[1], dir[2]);
    }

    void actuateByMagnitude(dReal input, dReal dt){
        // actuate so that 0 corresponds to the low range of force
        dReal normalizedInput = abs(input);
        dReal forceMagnitude = normalizedInput * (high_force - low_force) + low_force;
        const dReal dir[3] = {direction[0] * forceMagnitude * dt,
                              direction[1] * forceMagnitude * dt,
                              direction[2] * forceMagnitude * dt};
        this->lastForce[0] = dir[0] / dt;
        this->lastForce[1] = dir[1] / dt;
        this->lastForce[2] = dir[2] / dt;

        dBodyAddForce(this->body, dir[0], dir[1], dir[2]);
    }

    void draw(void){
        std::cerr << "Drawing thruster" << std::endl;
        std::cerr << this->lastForce[0] << " " << this->lastForce[1] << " " << this->lastForce[2] << std::endl;
        float length = 100.0;
        const dReal *pos1 = dBodyGetPosition(this->body);
        const dReal pos2[3] = {pos1[0] + log(this->lastForce[0]* length),
                               pos1[1] + log(this->lastForce[1]* length),
                               pos1[2] + log(this->lastForce[2]* length)};

        dsSetColor(1, 0, 0);
        dsDrawLine(pos1, pos2);
    }
};

#endif