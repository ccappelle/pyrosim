#ifndef _JOINT_MOTOR_HPP
#define _JOINT_MOTOR_HPP

#pragma once

#include <ode/ode.h>
#include <string>

#include "actuator.hpp"
#include "pythonReader.hpp"
#include "joint/joint.hpp"

class JointActuator : public Actuator{
protected:
    float maxForce, speed;
    int jointID;
    std::string control;
    dJointID joint;
    
public:
    JointActuator(){};

    virtual void create(Environment *environment) =0;
    virtual void actuate(dReal input, dReal timeStep) =0;

    void readFromPython(void){
        // read in entity id of joint
        readValueFromPython<int>(&this->jointID, "Joint ID");
        // read in max speed
        
        // read in max force
        readValueFromPython<float>(&this->maxForce, "Max Force");
        readValueFromPython<float>(&this->speed, "Speed");
        readStringFromPython(control, "Control Scheme");
    }
};

class RotaryActuator : public JointActuator{

// actuator which acts about an axis
// C.C. Currently only specified for single hinge axis
public:
    RotaryActuator(){};

    void create(Environment *environment){
        HingeJoint *jointObject = (HingeJoint *) environment->getEntity(this->jointID);
        this->joint = jointObject->getJoint();

        // set hinge params
        // C.C. can add more params (erp, bounciness, etc.) when needed
        if (this->maxForce < 0){
            dJointSetHingeParam(this->joint, dParamFMax, dInfinity);
        }
        else{
            dJointSetHingeParam(this->joint, dParamFMax, this->maxForce);
        }
        
    }

    void actuate(dReal input, dReal dt){
        if (this->control == "positional"){
            this->actuateByPosition(input);
        }
        else if (this->control == "velocity"){
            this->actuateByVelocity(input);
        }
    }
    void actuateByVelocity(dReal velocity){
        dJointSetHingeParam(this->joint, dParamVel, this->speed * velocity);
    }

    void actuateByPosition(dReal position){
        // position variable is assumed to be in [-1, 1]

        // squash position to be in [0, 1]
        dReal normalizedPosition = (position + 1.0) / 2.0;
        dReal highStop = dJointGetHingeParam(this->joint, dParamHiStop);
        dReal lowStop = dJointGetHingeParam(this->joint, dParamLoStop);
        dReal desiredTarget = normalizedPosition * ( highStop - lowStop ) + lowStop;

        dReal currentTarget;
        currentTarget = dJointGetHingeAngle(this->joint);
        dReal diff = desiredTarget - currentTarget;

        // C.C. don't know why its necessary to flip when body1 is the world but
        // just how it is. There is probably a bug somewhere but I can't find it
        // This was not necessary in the last version.
        float mult = 1.0;
        if (dJointGetBody(this->joint, 0) == (dBodyID) 0){
            mult = -1.0;
        }
        dJointSetHingeParam(this->joint, dParamVel, this->speed * diff * mult);
    }
};



class LinearActuator : public JointActuator{
public:
    LinearActuator(){};

    void create(Environment *environment){
        SliderJoint *jointObject = (SliderJoint *) environment->getEntity(this->jointID);
        this->joint = jointObject->getJoint();

        if (this->maxForce < 0){
            dJointSetSliderParam(this->joint, dParamFMax, dInfinity);
        }
        else{
            dJointSetSliderParam(this->joint, dParamFMax, this->maxForce);
        }
    }

    void actuate(dReal input, dReal dt){
        if (this->control == "positional"){
            this->actuateByPosition(input);
        }
        else if (this->control == "velocity"){
            this->actuateByVelocity(input);
        }
    }

    void actuateByVelocity(dReal velocity){
        dJointSetSliderParam(this->joint, dParamVel, this->speed * velocity);
    }

    void actuateByPosition(dReal position){
        // squash value between [0, 1]
        dReal normalizedPosition = (position + 1.0) / 2.0;

        dReal highStop = dJointGetSliderParam(this->joint, dParamHiStop);
        dReal lowStop = dJointGetSliderParam(this->joint, dParamLoStop);
        dReal desiredTarget = normalizedPosition * ( highStop - lowStop ) + lowStop;

        dReal currentTarget;
        currentTarget = dJointGetSliderPosition(this->joint);
        dReal diff = desiredTarget - currentTarget;

        dJointSetSliderParam(this->joint, dParamVel, this->speed * diff);     
    }
};
#endif