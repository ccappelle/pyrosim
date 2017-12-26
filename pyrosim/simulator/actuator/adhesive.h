#ifndef _ACTUATOR_ADHESIVE_H
#define _ACTUATOR_ADHESIVE_H

#include "../object.h"
#include "actuator.h"

class ADHESIVE : public ACTUATOR {

private:
	int	firstObject;

//	double	x;
//	double	y;
//	double	z;
//	double  lowStop;
//	double  highStop;

//	double lastDesired;

	OBJECT *first;

	int adhesionKind; // ignored for now

public:
	ADHESIVE(void) : first(NULL) {};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects);

	void Draw(void) const;

	void Read_From_Python(void);
};

#endif // _ACTUATOR_ADHESIVE_H
