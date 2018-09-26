#ifndef __PROPRIOCEPTIVE_SENSOR_HPP__
#define __PROPRIOCEPTIVE_SENSOR_HPP__

#pragma once
#include "sensor/sensor.hpp"
#include "joint/joint.hpp"
#include "pythonReader.hpp"


class ProprioceptiveSensor : public Sensor
{
protected:
    Joint *joint;
    int jointID;
public:
    virtual void readFromPython(void){
        readValueFromPython<int>(&this->jointID);
    }

    virtual void create(Environment *environment){
        try {
            this->joint = (Joint *) environment->getEntity(this->jointID);
        }
        catch (...){
            std::cerr << "ERROR:Cannot convert ID " << this->jointID << " to joint."
                      << "Please check that this ID is appropriate" << std::endl;

            exit(0);
        }
    }

    void sense(){
        const dReal prop = this->joint->getProprioception();
        this->currentSensorValue = prop;
        this->sensorValues.push_back(this->currentSensorValue);
    }
};

#endif