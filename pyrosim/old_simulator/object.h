#ifndef _OBJECT_H
#define _OBJECT_H

#include <ode/ode.h>
#include "sensor/ray.h"
#include "sensor/light.h"
#include "sensor/position.h"
#include "sensor/touch.h"
#include "sensor/vestibular.h"
#include "sensor/isSeen.h"
#include "sensor/proximity.h"
#include "lightSource.h"
#include <map>
#include <utility>
#include <array>
#include <set>
#include <vector>

class NEURON;

class OBJECT {

private:

	int	ID;
	int	myShape;
	dBodyID body;
	dGeomID geom;
	double x, y, z;
	double mass;
	double r1, r2, r3;
	double length, width, height;
	double radius;
	double dim1, dim2, dim3;
	int collisionGroup;
	double r, g, b;
	double tr, tg, tb; // true values stored for situations when an object must change color temporarily

	LIGHT_SENSOR* lightSensor;
	POSITION_SENSOR* positionSensor;
	RAY_SENSOR* raySensor;
	TOUCH_SENSOR* touchSensor;
	VESTIBULAR_SENSOR* vestibularSensor;
	IS_SEEN_SENSOR* isSeenSensor;
	PROXIMITY_SENSOR* proximitySensor;

	std::vector< LIGHT_SOURCE > lightSources;

	std::map< int, std::array<float,3> > forces;

	std::set< int > adhesionTypesSusceptible;
	std::multiset< int > adhesionTypesExhibiting;
	std::map< dJointID, std::multiset<int> > adhesiveJointsToTypes;

	bool bodyCreated;

public:
	OBJECT(void) :
		ID(0),
		raySensor(NULL),
		lightSensor(NULL),
		positionSensor(NULL),
		touchSensor(NULL),
		vestibularSensor(NULL),
		isSeenSensor(NULL),
		proximitySensor(NULL),
		bodyCreated(false) {
		Set_Adhesion_Susceptibility(0);
	};

	~OBJECT(void);

	void Add_External_Force(float x, float y, float z, int timeStep);
	void Apply_Stored_Forces(int timeStep);
	int Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sensorNeuron);
	void Create_IsSeen_Sensor(int myID, int evalPeriod);
	void Create_Ray_Sensor(dSpaceID space, int myID, int evalPeriod);
	void Create_Proximity_Sensor(dSpaceID space, int myID, int evalPeriod);
	void Create_Light_Sensor(int myID, int evalPeriod);
	void Create_Light_Source(int myID);
	void Create_Position_Sensor(int myID, int evalPeriod);
	void Create_Touch_Sensor(int myID, int evalPeriod);
	void Create_Vestibular_Sensor(int myID, int evalPeriod);

	void Set_Adhesion_Susceptibility(int adhesionKind) {adhesionTypesSusceptible.insert(adhesionKind);};
	//! Makes object susceptible to adhesion of adhesionKind

	void Set_Adhesion(int adhesionKind);
	//! Makes object exhibit the adhesion of adhesionKind
	/*!
	  Makes the body actively stick to every OBJECT with adhesionKind adhesion
	  group. All OBJECTs are in the adhesion group 0;
	  adhesion groups with number greater than 0 ensure that only the objects
	  with matching groups will adhese to the object.
	  Adhesion groups can be assigned to objects multiple times.
	*/

	void Unset_Adhesion(int adhesionKind);
	//! The object will cease to exhibit adhesion of adhesionKind
	/*!
	  If Set_Adhesion was called multiple times on some adhesionKind,
	  Unset_Adhesion must be called the same amount of times before it takes
	  effect.
	*/

	void Process_Adhesive_Touch(dWorldID world, OBJECT* other);
	//! Connects the approapriate objects when they touch in nearCallback()

	int Get_Num_Adhesion_Points(void) {return adhesiveJointsToTypes.size();};
	//! Returns the number of adhesion points/glued items

	void Draw(void);
	void Draw_Ray_Sensor(double x, double y, double z, int t);

	dBodyID Get_Body(void) {return body;};

	int Get_Group(void) {return collisionGroup;};
	int Get_ID(void) {return ID;};

	double Get_Length(void) {return length;};
	double Get_Radius(void) {return radius;};

	double Get_Red_Component(void) {return r;};
	double Get_Green_Component(void) {return g;};
	double Get_Blue_Component(void) {return b;};

	void Poll_Sensors(int numObjects, OBJECT **objects, int t);

	void Read_In_External_Force(void);
	void Read_From_Python(dWorldID world, dSpaceID space, int shape);
	void Set_Ray_Sensor(double distance, OBJECT *objectThatWasHit, int t);
	void Set_Proximity_Sensor(double distance, dVector3 contactPoint, OBJECT *objectThatWasHit, int t);

	void Touch_Sensor_Fires(int t);

	void IsSeen_Sensor_Fires(int t);
	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);

private:
	void CreateBody(dWorldID world, dSpaceID space);
	dReal Get_Luminousity_Produced_At(const dReal* pos, int kindOfLight);
	//! Returns cumulative luminousity that all light sources from this object produce at pos
};

#endif
