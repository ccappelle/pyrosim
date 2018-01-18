#ifndef _ACTUATOR_ROPE_H
#define _ACTUATOR_ROPE_H

#include "../object.h"
#include "actuator.h"

class ROPE : public ACTUATOR {

private:
	int	firstObject;
    int secondObject;

	double pos1[3], pos2[3];
	double relaxedLength, currentLength, previousLength;
	double springConstant;
	double dampeningCoefficient;

	OBJECT* first;
    OBJECT* second;

public:
	ROPE(void) : relaxedLength(1.0),
	             springConstant(1.0),
	             dampeningCoefficient(10.),
	             first(NULL),
	             second(NULL) {};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);

	void Draw(void) const;

	void Read_From_Python(void);

private:
	inline double Get_Current_Length(void) {
		return sqrt( (pos1[0]-pos2[0])*(pos1[0]-pos2[0]) + (pos1[1]-pos2[1])*(pos1[1]-pos2[1]) + (pos1[2]-pos2[2])*(pos1[2]-pos2[2]) );
	};

	void Update_Geometry(void);
};

#endif // _ACTUATOR_ROPE_H
