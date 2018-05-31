#ifndef _NEURON_HPP
#define _NEURON_HPP

#pragma once

#include "entity.hpp"

class Neuron : public Entity
{
private:
    float lastValue;
    float currentValue;
    float alpha, tau;
public:
    Neuron();
};

#endif