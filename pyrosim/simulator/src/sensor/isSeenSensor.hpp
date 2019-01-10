#ifndef __IS_SEEN_SENSOR_HPP__
#define __IS_SEEN_SENSOR_HPP__

#pragma once
#include "sensor/sensor.hpp"
#include "pythonReader.hpp"

class IsSeenSensor : public Sensor
{
protected:
    RigidBody *body;
    int bodyID;
public:
    virtual void readFromPython( void ){
        readValueFromPython<int>( &this->bodyID, "Body ID" );
    }

    virtual void create( Environment *environment ){
        this->body = (RigidBody *) environment->getEntity( this->bodyID );
    }

    void sense(){
        this->currentSensorValue = this->body->getIsSeen();
        this->sensorValues.push_back( this->currentSensorValue );
        this->body->setIsSeen( false );
    }
};


#endif