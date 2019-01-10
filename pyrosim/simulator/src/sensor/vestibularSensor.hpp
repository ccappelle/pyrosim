#ifndef __VESTIBULAR_SENSOR_HPP__
#define __VESTIBULAR_SENSOR_HPP__

#pragma once
#include "sensor/sensor.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"

class QuaternionSensor : public Sensor
{
protected:
    RigidBody *body;
    int bodyID;
    int whichDimension;
public:
    virtual void readFromPython(void){
        readValueFromPython<int>(&this->bodyID);
        readValueFromPython<int>(&this->whichDimension);
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
        const dReal * quat = this->body->getQuaternion();
        // const dReal * quat = dBodyGetQuaternion(this->bodyID);
        this->currentSensorValue = quat[this->whichDimension];
        this->sensorValues.push_back(this->currentSensorValue);
    }
};

// class PositionSensor : public Sensor
// {
// protected:
//     RigidBody *body;
//     int bodyID;
//     int whichDimension;
// public:
//     virtual void readFromPython(void){
//         readValueFromPython<int>(&this->bodyID);
//         readValueFromPython<int>(&this->whichDimension);
//     }

//     virtual void create(Environment *environment){
//         try {
//             this->body = (RigidBody *) environment->getEntity(this->bodyID);
//         }
//         catch (...){
//             std::cerr << "ERROR:Cannot convert ID " << this->bodyID << " to rigid body."
//                       << "Please check that this ID is appropriate" << std::endl;

//             exit(0);
//         }
//     }

//     void sense(){
//         const dReal* pos = this->body->getPosition();
//         this->currentSensorValue = pos[this->whichDimension];
//         this->sensorValues.push_back(this->currentSensorValue);
//     }
// };

#endif