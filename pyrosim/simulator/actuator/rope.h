#ifndef _ACTUATOR_ROPE_H
#define _ACTUATOR_ROPE_H

#include "../object.h"
#include "actuator.h"

class ROPE : public ACTUATOR {

private:
	int	firstObject;
    int secondObject;

	double pos1[3], pos2[3];
	double relaxedLength, currentLength;
	double springConstant;

	OBJECT* first;
    OBJECT* second;

public:
	ROPE(void) : relaxedLength(0.0),
	             springConstant(1.0),
	             first(NULL),
	             second(NULL) {};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects);

	void Draw(void) const;

	void Read_From_Python(void);

private:
	void Update_Geometry(void);
};

#endif // _ACTUATOR_ROPE_H
