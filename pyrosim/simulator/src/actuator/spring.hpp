#ifndef __SPRING_HPP__
#define __SPRING_HPP__

#pragma once

#include "math.h"

#include "actuator/actuator.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"

class SpringActuator : public Actuator
{
protected:
    float restingLength;
    float stiffness;
    float damping;
    // dBodyID body1, body2;
    int body1ID, body2ID;
    RigidBody *body1Obj, *body2Obj;
    float currentLength;
    // float low_force, high_force;
    // float direction[3];
    // dBodyID body; // the ode given id
    // int bodyID;   // the pyrosim entity id
    // dReal dt;
    // dReal lastForce[3];
    // dReal lastMagnitude;
    // dMatrix3 RInit;

public:
    SpringActuator(){};

    void create(Environment *environment){
        this->body1Obj = (RigidBody *) environment->getEntity( this->body1ID );
        this->body2Obj = (RigidBody *) environment->getEntity( this->body2ID );
       // body1 = environment->getEntity( this->body1ID )->body;
        // body2 = environment->getEntity( this->body2ID )->body;
        // RigidBody *bodyObj = (RigidBody *) environment->getEntity(this->bodyID);
        // this->body = bodyObj->getBody();
        // this->lastMagnitude = 0.0;
        // dBodyCopyRotation( this->body, this->RInit );
    }

    void readFromPython(void){
        readValueFromPython<int>(&this->body1ID, "Body 1 ID");
        readValueFromPython<int>(&this->body2ID, "Body 2 ID");
        readValueFromPython<float>(&this->restingLength, "Resting Length" );
        readValueFromPython<float>(&this->stiffness, "Stiffness" );
        readValueFromPython<float>(&this->damping, "Damping Coefficient" );
        // readValueFromPython<float>(&this->low_force, "Low Force");
        // readValueFromPython<float>(&this->high_force, "High Force");
        // readValueFromPython<float>(this->direction, 3, "Thruster Direction");
    }

    void actuate(dReal input, dReal dt){
        dBodyID body1 = body1Obj->getBody();
        dBodyID body2 = body2Obj->getBody();

        const dReal *pos1 = dBodyGetPosition( body1 );
        const dReal *pos2 = dBodyGetPosition( body2 );

        dReal direction[3] = { pos2[0] - pos1[0],
                               pos2[1] - pos1[1],
                               pos2[2] - pos1[2] };

        this->currentLength = sqrt( pow( direction[0], 2.0 ) +
                              pow( direction[1], 2.0 ) +
                              pow( direction[2], 2.0 ) );
        direction[0] /= this->currentLength;
        direction[1] /= this->currentLength;
        direction[2] /= this->currentLength;

        dReal difference = restingLength - this->currentLength;

        const dReal *velocity1 = dBodyGetLinearVel( body1 );
        const dReal *velocity2 = dBodyGetLinearVel( body2 );
        dBodyAddForce( body1,
                      -direction[0] * difference * this->stiffness + direction[0] * this->damping,
                      -direction[1] * difference * this->stiffness + direction[1] * this->damping,
                      -direction[2] * difference * this->stiffness + direction[2] * this->damping );
        dBodyAddForce( body2,
                       direction[0] * difference * this->stiffness - direction[0] * this->damping,
                       direction[1] * difference * this->stiffness - direction[1] * this->damping,
                       direction[2] * difference * this->stiffness - direction[2] * this->damping );
    }

    void draw(void){
        // const dReal *pos1 = 
    }
};

#endif