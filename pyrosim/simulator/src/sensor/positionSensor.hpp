#ifndef __POSITION_SENSOR_HPP__
#define __POSITION_SENSOR_HPP__

#pragma once
#include "sensor/sensor.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"


class PositionSensor : public Sensor
{
protected:
    RigidBody *body;
    int bodyID;
    int whichDimension;
public:
    virtual void readFromPython(void){
        readValueFromPython<int>(&this->bodyID, "Body ID");
        readValueFromPython<int>(&this->whichDimension, "Wich Dimension (0:x, 1:y, 2:z)");
    }

    virtual void create(Environment *environment){
        try {
            this->body = (RigidBody *) environment->getEntity(this->bodyID);
        }
        catch (...){
            std::cerr << "ERROR:Cannot convert ID " << this->bodyID << " to rigid body."
                      << "Please check that this ID is appropriate" << std::endl;

            exit(0);
        }
    }

    void sense(){
        const dReal* pos = this->body->getPosition();
        this->currentSensorValue = pos[this->whichDimension];
        this->sensorValues.push_back(this->currentSensorValue);
    }
};

#endif