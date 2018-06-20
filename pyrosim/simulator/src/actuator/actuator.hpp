#ifndef _ACTUATOR_HPP
#define _ACTUATOR_HPP

#pragma once

#include <ode/ode.h>
#include "entity.hpp"

class Actuator : public Entity{
protected:
    dReal lastInput;
    dReal nextInput;
public:
    virtual void actuate(dReal input) =0;
    virtual void actuate(void) =0;

    virtual void setNextInput(dReal input){
        this->nextInput = input;
    }

    virtual void resetNextInput(){
        this->lastInput = nextInput;
        this->nextInput = 0.0;
    }

    virtual void takeStep(int timeStep, dReal dt) {}
};

#endif