#ifndef _GEOM_DATA_H
#define _GEOM_DATA_H

#include "object.h"

enum GeomType { DEFAULT, GROUND, SENSOR_RAY, SENSOR_PROXIMITY };

struct GeomData {

	GeomType geomType;
	OBJECT* objectPtr;
};

#endif // _GEOM_DATA_H
