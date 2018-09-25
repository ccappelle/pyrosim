#ifndef __TOUCH_SENSOR_HPP__
#define __TOUCH_SENSOR_HPP__

#pragma once

#include <vector>
#include <utility>
#include "sensor/sensor.hpp"
#include "pythonReader.hpp"

class TouchSensor : public Sensor
{
protected:
    int bodyID;
    std::vector< std::pair<int, int> >* collisions;

public:
    TouchSensor(){};

    void sense(void){
        std::cerr << "Sensing " << std::endl;
        for (auto const &collisionPair : *this->collisions){
            if (collisionPair.first == this->bodyID or
                collisionPair.second == this->bodyID){

                this->sensorValues.push_back(1.0);
                return;
            }
        }
        this->sensorValues.push_back(0.0f);
    };

    void create(Environment *environment){
        this->collisions = &environment->collisions;
    };

    void readFromPython(void){
        readValueFromPython<int>(&this->bodyID, "Body ID");
    }
};

#endif