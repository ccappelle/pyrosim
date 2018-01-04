#ifndef _OBJECT_H
#define _OBJECT_H

#include <ode/ode.h>
#include "raySensor.h"
#include "lightSensor.h"
#include "positionSensor.h"
#include "touchSensor.h"
#include "vestibularSensor.h"
#include "isSeenSensor.h"
#include <map>
#include <utility>
#include <array>
#include <set>

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

	LIGHT_SENSOR *lightSensor;
	POSITION_SENSOR *positionSensor;
	RAY_SENSOR *raySensor;
	TOUCH_SENSOR *touchSensor;
	VESTIBULAR_SENSOR *vestibularSensor;
	IS_SEEN_SENSOR *isSeenSensor;

	int	containsLightSource;

	std::map< int, std::array<float,3> > forces;

	std::set< int > adhesionTypesSusceptible;
	std::multiset< int > adhesionTypesExhibiting;
	std::map< dJointID, std::multiset<int> > adhesiveJointsToTypes;

public:
	OBJECT(void) :
		ID(0),
		raySensor(NULL),
		lightSensor(NULL),
		positionSensor(NULL),
		touchSensor(NULL),
		vestibularSensor(NULL),
		isSeenSensor(NULL),
		containsLightSource(false) {

		adhesionTypesSusceptible.insert(0);
	};

	~OBJECT(void) {};

	void Add_External_Force(float x, float y, float z, int timeStep);

	void Apply_Stored_Forces(int timeStep);

	int  Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron);

	void Create_IsSeen_Sensor(int myID, int evalPeriod);

	void Create_Ray_Sensor(dSpaceID space, int myID, int evalPeriod);

	void Create_Light_Sensor(int myID, int evalPeriod);

	void Create_Light_Source(void);

	void Create_Position_Sensor(int myID, int evalPeriod);

	void Create_Touch_Sensor(int myID, int evalPeriod);

    void Create_Vestibular_Sensor(int myID, int evalPeriod);

	void Set_Adhesion(int adhesionKind);
	//! Adds an adhesion group to the object
	/*!
	  An adhesion group of 0 means that the body will stick to anything;
	  adhesion groups with number greater than 0 ensure that only the objects
	  with matching groups will adhese to the object.
	  Adhesion groups can be assigned to objects multiple times; in this case
	  Unset_Adhesion() must be called multiple times before the objects are
	  destroyed.
	*/

	void Unset_Adhesion(int adhesionKind);
	//! Removes the object from an adhesion group

	void Process_Adhesive_Touch(dWorldID world, OBJECT* other);
	//! Connects the approapriate objects when they touch in nearCallback()

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

	void Touch_Sensor_Fires(int t);

	void IsSeen_Sensor_Fires(int t);
	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);

private:
	int  Contains_A_Light_Source(void);
	double Distance_To(OBJECT *otherObject);
	void CreateBody(dWorldID world, dSpaceID space);

	OBJECT *Find_Closest_Light_Source(int numObjects, OBJECT **objects);

};

#endif
