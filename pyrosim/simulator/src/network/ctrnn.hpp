#ifndef _ANN_HPP
#define _ANN_HPP

#pragma once

#include <math.h>
#include <vector>

#include "entity.hpp"
#include "pythonReader.hpp"
#include "actuator/actuator.hpp"
#include "sensor/sensor.hpp"

class Synapse;

class Neuron : public Entity
{
protected:
    int lastUpdated;
    std::vector<Synapse *> outSynapses;
    float value;
    float cachedValue;
public:
    virtual void addSynapticInput(float inputValue, float weight) {};
    virtual void readFromPython(void) =0;
    virtual void create(Environment *environment) =0;

    virtual void addSynapse(Synapse *synapse){
        // adds synapse to vector of synapses
        this->outSynapses.push_back(synapse);
    }

    virtual void takeStep(int timeStep, dReal dt){
        if (this->lastUpdated != timeStep){
            this->fireStep();
            this->lastUpdated = timeStep;
        }
        else{
            this->updateStep();
        }
    }

    virtual EntityType getEntityType(void){return NEURON;};
    virtual void fireStep() =0;
    virtual void updateStep() =0;
};


class Synapse : public Entity
{
protected:
    int sourceNeuronID, targetNeuronID;
    Neuron *sourceNeuron;
    Neuron *targetNeuron;
    float weight;
public:
    Synapse(){};
    virtual void readFromPython(){
        readValueFromPython<int>(&sourceNeuronID, "Source Neuron");
        readValueFromPython<int>(&targetNeuronID, "Target Neuron");
        readValueFromPython<float>(&weight, "Weight");
    }

    void create(Environment *environment){
        // add to source neuron
        this->sourceNeuron = (Neuron *) environment->getEntity(this->sourceNeuronID);
        this->targetNeuron = (Neuron *) environment->getEntity(this->targetNeuronID);
        this->sourceNeuron->addSynapse(this);
    }

    virtual EntityType getEntityType(void){return SYNAPSE;};
    Neuron* getSourceNeuron(void){ return this->sourceNeuron;}
    Neuron* getTargetNeuron(void){ return this->targetNeuron;}
    float getWeight(void){ return this->weight;}
};

// INPUT NEURONS ---- Bias, Sensor, User --------------------------
class InputNeuron : public Neuron
{
public:
    virtual void create(Environment* environment) {this->lastUpdated=-1;};
    virtual void fire(){
        for (Synapse* synapse : this->outSynapses){
            Neuron* targetNeuron = synapse->getTargetNeuron();
            targetNeuron->addSynapticInput(this->value, synapse->getWeight()); 
        }
    }
    virtual void fireStep(){ this->fire(); }
    virtual void readFromPython(void) =0;
    virtual void updateStep()=0;
};


class BiasNeuron : public InputNeuron
{
public:
    BiasNeuron(){};
    virtual void readFromPython(void){
        readValueFromPython(&this->value, "Bias Value");
    }
    virtual void updateStep(){cachedValue = 0.0f; };
};

class SensorNeuron : public InputNeuron
{
protected:
    Sensor *sensor;
    int sensorID;
public:
    SensorNeuron(){};
    virtual void updateStep(){
        cachedValue = 0.0f;
        this->value = this->sensor->getSensorValue();
    }

    virtual void create(Environment *environment){
        sensor = (Sensor *) environment->getEntity(this->sensorID);
        this->lastUpdated = -1;
    }

    virtual void readFromPython(){
        // read in motor's entity ID
        readValueFromPython<int>(&sensorID, "Sensor ID");
    }
};


class UserNeuron : public InputNeuron
{
protected:
    std::vector<float> inputValues;
    int indexValue;
public:
    UserNeuron(){};
    virtual void readFromPython(void){
        // read in num inputs
        int n;
        readValueFromPython<int>(&n, "Size of Input");

        // create array to store input
        float inValues[n];
        readValueFromPython<float>(inValues, n, "Value Inputs");

        for (int i=0; i<n; i++){
            this->inputValues.push_back(inValues[i]);
        }
        this->indexValue = 0;
    }
    virtual void updateStep(){
        this->cachedValue = 0.0f;
        // set value from inputValues
        this->value = this->inputValues[this->indexValue];
        // set next index value
        this->indexValue = (this->indexValue + 1) % this->inputValues.size();
    };
};


// Tagetable Neurons -------- Hidden, Motor ----------------------
class TargetableNeuron : public Neuron
{
protected:
    float lastValue;
    float cachedValue;
    float alpha, tau;
    float startingValue;
public:
    virtual void addSynapticInput(float neuronValue, float weight){
        // sets cached value to the sum of the synaptic weight multiplied
        // by source neuron value
        this->cachedValue += neuronValue * weight;
    }

    void updateNeuronFromCache(){
        // ctrnn update. then reset cache
        this->value = this->alpha * this->value + this->tau * this->cachedValue;
        this->cachedValue = 0.0f;
    }

    void readParamsFromPython(){
        // read in necessary params for CTRNN neurons
        readValueFromPython<float>(&this->alpha, "Alpha");
        readValueFromPython<float>(&this->tau, "Tau");
        readValueFromPython<float>(&this->startingValue, "Starting Value");
    }
    virtual void threshold(){
        // use tanh to threshold
        this->value = tanh(this->value);
    }

    virtual void fire(){
        // loop through synapse and update cach
        for (Synapse* synapse : this->outSynapses){
            Neuron* targetNeuron = synapse->getTargetNeuron();
            targetNeuron->addSynapticInput(this->value, synapse->getWeight()); 
        }
    }

    virtual void fireStep(){
        this->fire();
    }

    virtual void updateStep(){
        this->updateNeuronFromCache();
        this->threshold();
    }
};


class HiddenNeuron : public TargetableNeuron
{
public:
    HiddenNeuron(){};
    virtual void create(Environment *environment){
        this->value = 0.0;
        this->cachedValue = 0.0;
        this->lastUpdated = -1;
    }
    virtual void readFromPython(){
        this->readParamsFromPython();
    }
};


class MotorNeuron : public TargetableNeuron
{
protected:
    Actuator *motor;
    int motorID;
public:
    MotorNeuron(){};

    virtual void updateStep(){
        this->updateNeuronFromCache();
        this->threshold();
        motor->setNextInput(this->value);
    }

    virtual void create(Environment *environment){
        motor = (Actuator *) environment->getEntity(this->motorID);
        this->value = 0.0;
        this->cachedValue = 0.0;
        this->lastUpdated = -1;
    }

    virtual void readFromPython(){
        // read in motor's entity ID
        readValueFromPython<int>(&motorID, "Motor ID");
        // read in params
        this->readParamsFromPython();
    }
};

#endif