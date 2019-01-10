#ifndef __RAY_SENSOR_HPP__
#define __RAY_SENSOR_HPP__

#pragma once
#include "sensor/sensor.hpp"
#include "body/ray.hpp"
#include "pythonReader.hpp"


class RaySensor : public Sensor
{
protected:
    Ray *ray;
    int rayID;
    float range;
    int whichSense;

public:
    virtual void readFromPython(void){
        readValueFromPython<int>(&this->rayID, "Ray ID" );
        readValueFromPython<int>(&this->whichSense, "Which Sense");
    }

    virtual void create(Environment *environment){
        try {
            this->ray = (Ray *) environment->getEntity(this->rayID);
        }
        catch (...){
            std::cerr << "ERROR:Cannot convert ID " << this->rayID << " to ray."
                      << "Please check that this ID is appropriate" << std::endl;

            exit(0);
        }
        
    }

    void sense(){
        if (this->whichSense == 0){
            this->currentSensorValue = ray->getDistance();
        }
        else{
            this->currentSensorValue = ray->getColorComponent(this->whichSense - 1);
        }
        this->sensorValues.push_back(this->currentSensorValue);
        // const dReal* pos = this->body->getPosition();
        // this->currentSensorValue = pos[this->whichDimension];
        // this->sensorValues.push_back(this->currentSensorValue);
    }
};

#endif