#ifndef _JOINT_MOTOR_HPP
#define _JOINT_MOTOR_HPP

#pragma once

#include <ode/ode.h>
#include <string>

#include "entity.hpp"
#include "pythonReader.hpp"
#include "joint/joint.hpp"

class JointMotor{
protected:
    float maxForce;
    int jointID;
    std::string control;

public:
    JointMotor(){};

    void readFromPython(void){
        // read in entity id of joint
        readValueFromPython<int>(&this->jointID, "Joint ID");
        // read in max speed
        
        // read in max force
        readValueFromPython<float>(&this->maxForce, "Max Force");

        readStringFromPython(control, "Control Scheme");
    }
    virtual void create(Environment *environment) =0;
}

class HingeMotor : public JointMotor{
    void create(Environment *environment){
        Hinge *jointObject = (Hinge *) this->getEntity(this->jointID);
        dJointID joint = jointObject->getJoint();

        dJointSetHingeParam(joint, dParamFMax, this->maxForce);
    }
}