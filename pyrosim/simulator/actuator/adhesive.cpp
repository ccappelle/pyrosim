#ifndef _ACTUATOR_ADHESIVE_CPP
#define _ACTUATOR_ADHESIVE_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>

#include "adhesive.h"

void ADHESIVE::Actuate(void) {

	// If no motor neuron is connected, do nothing
	if ( motorNeurons[0] == NULL )
		return;

	// Update the adhesion groups according to the output of the motor neuron
	double neuVal = motorNeurons[0]->Get_Value();

//	std::cerr << neuVal << std::endl;

	if ( enabledNow && neuVal <= 0.0 ) {

		std::cerr << "Unsetting adhesion\n";
		first->Unset_Adhesion(adhesionKind);
		enabledNow = false;
	}

	if ( !enabledNow && neuVal > 0.0 ) {

		std::cerr << "Setting adhesion\n";
		first->Set_Adhesion(adhesionKind);
		enabledNow = true;
	}
}

void ADHESIVE::Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

	if ( firstObject >= 0 )
		first = allObjects[firstObject];
}

void ADHESIVE::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> adhesionKind;
}

bool ADHESIVE::Create_Proprioceptive_Sensor(int sensorID, int evalPeriod) {

	proprioceptiveSensor = new PROPRIOCEPTIVE_ADHESIVE_SENSOR(sensorID, evalPeriod);
	return true;
}

void ADHESIVE::Poll_Sensors(int t) {

	if(proprioceptiveSensor)
		proprioceptiveSensor->Poll(first, t);
}

void ADHESIVE::Update_Sensor_Neurons(int t) {

	if(proprioceptiveSensor)
		proprioceptiveSensor->Update_Sensor_Neurons(t);
}

void ADHESIVE::Write_To_Python(int evalPeriod) const {

	if(proprioceptiveSensor)
		proprioceptiveSensor->Write_To_Python(evalPeriod);
}

bool ADHESIVE::Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sNeuron) {

	if(proprioceptiveSensor)
		if (proprioceptiveSensor->Get_ID() == sensorID) {
			proprioceptiveSensor->Connect_To_Sensor_Neuron(sNeuron, sensorValueIndex);
			return true;
		}
	return false;
}

#endif // _ACTUATOR_ADHESIVE_CPP
