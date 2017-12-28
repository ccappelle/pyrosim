#ifndef _ACTUATOR_ADHESIVE_H
#define _ACTUATOR_ADHESIVE_H

#include "../object.h"
#include "actuator.h"

class ADHESIVE : public ACTUATOR {

private:
	int	firstObject;

	OBJECT *first;

	int adhesionKind;

public:
	ADHESIVE(void) : first(NULL), adhesionKind(0) {};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects);

	void Draw(void) const;

	void Read_From_Python(void);
};

#endif // _ACTUATOR_ADHESIVE_H
