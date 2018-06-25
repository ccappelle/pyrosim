#ifndef __SENSOR__HPP
#define __SENSOR__HPP

#pragma once

#include <vector>

#include "entity.hpp"

class Sensor : public Entity
{
protected:
    float currentSensorValue;
    std::vector<float> sensorValues;
public:
    float getSensorValue(){ return this->currentSensorValue; }
    virtual EntityType getEntityType(void){ return SENSOR; }
    virtual void sense() =0;
    virtual void takeStep(int timeStep, dReal dt){ this->sense(); }
    virtual void writeToPython(void){
        std::cout << " " << this->entityID;
        for (auto val : this->sensorValues){
            std::cout << " " << val;
        }
    };
};

#endif