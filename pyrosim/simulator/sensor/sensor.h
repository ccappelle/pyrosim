#ifndef _SENSOR_SENSOR_H
#define _SENSOR_SENSOR_H

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "../neuron.h"

class NEURON;

class SENSOR {

protected:
	const int numChannels;
	int ID;
	double** values;
	NEURON** mySensorNeurons;

public:
	SENSOR(int myID, int evalPeriod, int channels = 1) : numChannels(channels),
	                                                     ID(myID),
	                                                     values(new double*[channels]),
	                                                     mySensorNeurons(new NEURON*[channels]) {

		for(int i=0; i<numChannels; i++) {
			values[i] = new double[evalPeriod];
			mySensorNeurons[i] = NULL;
		}
	};

	virtual ~SENSOR(void) {

		for(int i=0; i<numChannels; i++)
			delete [] values[i];
		delete [] values;
		delete [] mySensorNeurons;
	};

	virtual void Connect_To_Sensor_Neuron(NEURON* sensorNeuron, int sensorValueIndex = 0) {

		if(sensorValueIndex < numChannels)
			mySensorNeurons[sensorValueIndex] = sensorNeuron;
		else {
			std::cerr << "Cannot connect sensor neuron to sensor at value index " << sensorValueIndex << ": there are only " << numChannels << " channels\n";
			exit(EXIT_FAILURE);
		}
	};

	virtual void Update_Sensor_Neurons(int t) {

		for(int i=0; i<numChannels; i++) {
			if(mySensorNeurons[i])
				mySensorNeurons[i]->Set(values[i][t]);
		}
	};

	virtual int Get_ID(void) const {return ID;};

	virtual void Write_To_Python(int evalPeriod) const {

		std::ostringstream oss;
		oss << ID << ' ' << numChannels << ' ';
		for(int t=0; t<evalPeriod; t++)
			for(int i=0; i<numChannels; i++)
				oss << values[i][t] << ' ';
		oss << '\n';
		std::cout << oss.str();
	};
};

#endif // _SENSOR_SENSOR_H
