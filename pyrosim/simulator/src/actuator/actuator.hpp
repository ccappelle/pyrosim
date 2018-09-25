#ifndef _ACTUATOR_HPP
#define _ACTUATOR_HPP

#pragma once

#include <ode/ode.h>
#include "entity.hpp"

class Actuator : public Entity{
// Base Class for actuators

protected:
    dReal lastInput; // stores the last input signal to the actuator
    dReal nextInput; // stores the current input signal to send to the actuator in the next time step
public:
    virtual void actuate(dReal input, dReal timeStep) =0;

    void actuate(dReal timeStep){
        // run the actuators local actuation function for the specified value and duration
        this->actuate(this->nextInput, timeStep);
        this->resetNextInput(); // set lastInput to nextInput
    }

    virtual void setNextInput(dReal input){
        this->nextInput = input;
    }

    virtual void resetNextInput(){
        this->lastInput = nextInput;
        this->nextInput = 0.0;
    }

    virtual void takeStep(int timeStep, dReal dt) { this->actuate(dt); }

    virtual EntityType getEntityType(void){ return ACTUATOR; }
};

#endif