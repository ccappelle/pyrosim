#ifndef _JOINT_MOTOR_HPP
#define _JOINT_MOTOR_HPP

#pragma once

#include <ode/ode.h>
#include <string>

#include "actuator.hpp"
#include "pythonReader.hpp"
#include "joint/joint.hpp"

class JointMotor : public Actuator{
protected:
    float maxForce, speed;
    int jointID;
    std::string control;
    dJointID joint;
public:
    JointMotor(){};

    virtual void create(Environment *environment) =0;
    virtual void actuate(void) =0;
    virtual void actuate(dReal input) =0;

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

class Rotary : public JointMotor{

    void create(Environment *environment){
        HingeJoint *jointObject = (HingeJoint *) environment->getEntity(this->jointID);
        this->joint = jointObject->getJoint();

        if (this->maxForce < 0){
            dJointSetHingeParam(this->joint, dParamFMax, dInfinity);
        }
        else{
            dJointSetHingeParam(this->joint, dParamFMax, this->maxForce);
        }
        
    }
    void actuate(void){
        this->actuate(this->nextInput);
        this->resetNextInput();
    }

    void actuate(dReal input){
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

        dReal highStop = dJointGetHingeParam(this->joint, dParamHiStop);
        dReal lowStop = dJointGetHingeParam(this->joint, dParamLoStop);
        dReal desiredTarget = position * ( highStop - lowStop ) + lowStop;

        dReal currentTarget;
        currentTarget = dJointGetHingeAngle(joint);
        dReal diff = desiredTarget - currentTarget;

        dJointSetHingeParam(this->joint, dParamVel, this->speed * diff);
    }
};

#endif