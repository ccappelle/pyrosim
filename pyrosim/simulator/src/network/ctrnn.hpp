#ifndef _ANN_HPP
#define _ANN_HPP

#pragma once

#include <math.h>
#include <vector>

#include "entity.hpp"
#include "actuator/actuator.hpp"

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

    virtual EntityType getEntityType(void){return NEURON;};

    void addSynapse(Synapse *synapse){
        // adds synapse to vector of synapses
        this->outSynapses.push_back(synapse);
    }

    virtual void takeStep(int timeStep, dReal dt){
        // two step approach to ensure all nodes are updated
        // correctly. First step sends out value of neuron
        // along outgoing synapses (fire) to be cached in 
        // target neurons.
        // Second step thresholds cached value to set new 
        // value of neuron
        if (this->lastUpdated != timeStep){
            this->fireStep();
            this->lastUpdated = timeStep;
        }
        else{
            this->thresholdStep();
        }
    }

    virtual void fireStep() =0;
    virtual void thresholdStep() =0;
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


class BiasNeuron : public Neuron
{
public:
    BiasNeuron(){};

    virtual void create(Environment* environment) {};
    virtual void readFromPython(void){
        readValueFromPython(&this->value, "Bias Value");
    }
    virtual void fireStep(){
        this->fire();
    }
    virtual void fire(){
        for (Synapse* synapse : this->outSynapses){
            Neuron* targetNeuron = synapse->getTargetNeuron();
            targetNeuron->addSynapticInput(this->value, synapse->getWeight()); 
        }
    }
    virtual void thresholdStep(){cachedValue = 0.0f;};
};

class UserNeuron : public Neuron
{
protected:
    std::vector<float> inputValues;
    int indexValue;
public:
    UserNeuron(){};

    virtual void create(Environment * environment){
        indexValue = 0;
    };

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
    }

    virtual void fireStep(){
        this->fire();
    }
    virtual void fire(){
        for (Synapse* synapse : this->outSynapses){
            Neuron* targetNeuron = synapse->getTargetNeuron();
            targetNeuron->addSynapticInput(this->value, synapse->getWeight()); 
        }
    }
    virtual void thresholdStep(){
        // set value from inputValues
        this->value = this->inputValues[this->indexValue];
        // set next index value
        this->indexValue = (this->indexValue + 1) % this->inputValues.size();
    };
};


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

    void updateNeuron(){
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

    virtual void thresholdStep(){
        this->updateNeuron();
        this->threshold();
    }
};


class MotorNeuron : public TargetableNeuron
{
protected:
    Actuator *motor;
    int motorID;

public:
    MotorNeuron(){};

    virtual void thresholdStep(){
        this->updateNeuron();
        this->threshold();
        motor->setNextInput(this->value);
    }

    virtual void create(Environment *environment){
        motor = (Actuator *) environment->getEntity(this->motorID);
    }

    virtual void readFromPython(){
        // read in motor's entity ID
        readValueFromPython<int>(&motorID, "Motor ID");
        // read in params
        this->readParamsFromPython();
    }
};

#endif