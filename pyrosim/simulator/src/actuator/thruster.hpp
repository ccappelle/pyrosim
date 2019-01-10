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
    dBodyID body; // the ode given id
    int bodyID;   // the pyrosim entity id
    dReal dt;
    dReal lastForce[3];
    dReal lastMagnitude;
    dMatrix3 RInit;

public:
    ThrusterActuator(){};

    void create(Environment *environment){
        RigidBody *bodyObj = (RigidBody *) environment->getEntity(this->bodyID);
        this->body = bodyObj->getBody();
        this->lastMagnitude = 0.0;
        dBodyCopyRotation( this->body, this->RInit );
    }

    void readFromPython(void){
        readValueFromPython<int>(&this->bodyID, "Body ID");
        readValueFromPython<float>(&this->low_force, "Low Force");
        readValueFromPython<float>(&this->high_force, "High Force");
        readValueFromPython<float>(this->direction, 3, "Thruster Direction");
    }

    void actuate(dReal input, dReal dt){
        const dReal *R = dBodyGetRotation(this->body);

        // invert by taking transform of initial rotation to ensure direction
        // is in global coordinates then modified by rotation due to simulation
        const dReal xDirTemp = RInit[0] * direction[0] + RInit[4] * direction[1] + RInit[8]*direction[2];
        const dReal yDirTemp = RInit[1] * direction[0] + RInit[5] * direction[1] + RInit[9]*direction[2];
        const dReal zDirTemp = RInit[2] * direction[0] + RInit[6] * direction[1] + RInit[10]*direction[2];

        const dReal xDir = R[0] * xDirTemp + R[1] * yDirTemp + R[2] * zDirTemp;
        const dReal yDir = R[4] * xDirTemp + R[5] * yDirTemp + R[6] * zDirTemp;
        const dReal zDir = R[8] * xDirTemp + R[9] * yDirTemp + R[10] * zDirTemp;

        dReal normalizedInput = (input + 1.0) / 2.0;
        dReal forceMagnitude = normalizedInput * (high_force - low_force) + low_force;
        this->lastMagnitude = forceMagnitude;

        // std::cerr << forceMagnitude << " " << high_force << " " << std::endl;
        const dReal dir[3] = {xDir * forceMagnitude * dt,
                              yDir * forceMagnitude * dt,
                              zDir * forceMagnitude * dt};
        // save the direction of the force for drawing purposes
        this->lastForce[0] = normalizedInput * xDir;
        this->lastForce[1] = normalizedInput * yDir;
        this->lastForce[2] = normalizedInput * zDir;

        dBodyAddForce(this->body, dir[0], dir[1], dir[2]);
    }

    void actuateByMagnitude(dReal input, dReal dt){
        // actuate so that 0 corresponds to the low range of force
        dReal normalizedInput = abs(input);
        dReal forceMagnitude = normalizedInput * (high_force - low_force) + low_force;

        const dReal dir[3] = {direction[0] * forceMagnitude * dt,
                              direction[1] * forceMagnitude * dt,
                              direction[2] * forceMagnitude * dt};
        this->lastForce[0] = normalizedInput + 0.5 / 2.0 * direction[0] * dt;
        this->lastForce[1] = normalizedInput + 0.5 / 2.0 * direction[1] * dt;
        this->lastForce[2] = normalizedInput + 0.5 / 2.0 * direction[2] * dt;


        dBodyAddForce(this->body, dir[0], dir[1], dir[2]);
    }

    void draw(void){
        // std::cerr << "Drawing thruster" << std::endl;
        // std::cerr << this->lastForce[0] << " " << this->lastForce[1] << " " << this->lastForce[2] << std::endl;
        // float length = 1.0;
        const dReal *pos1 = dBodyGetPosition(this->body);

        const dReal pos2[3] = {pos1[0] - this->lastForce[0],
                               pos1[1] - this->lastForce[1],
                               pos1[2] - this->lastForce[2]};

        dsSetColor(1, 0.75, 0.2);
        // std::cerr << pos1[0] << " " << pos1[1] << " " << pos1[2] << std::endl;
        // std::cerr << pos2[0] << " " << pos2[1] << " " << pos2[2] << std::endl;
        dsDrawLine(pos1, pos2);
    }
};

#endif