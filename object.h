#ifndef _OBJECT_H
#define _OBJECT_H

#include <ode/ode.h>
#include "raySensor.h"
#include "lightSensor.h"
#include "positionSensor.h"
#include "touchSensor.h"
#include "vestibularSensor.h"

class NEURON;

class OBJECT {

private:

	int	ID;

	int	myShape;

        dBodyID body;

        dGeomID geom;

	double x,y,z;

	double r1,r2,r3;

	double length,width,height;
	
	double radius;

	double r,g,b;

        LIGHT_SENSOR *lightSensor;

	POSITION_SENSOR *positionSensor;

        RAY_SENSOR *raySensor;

	TOUCH_SENSOR *touchSensor;

	VESTIBULAR_SENSOR *vestibularSensor;

	int	containsLightSource;

public:
	OBJECT();

	~OBJECT(void);

	int  Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron);

	void Create_Ray_Sensor(dSpaceID space, int myID, int evalPeriod);

	void Create_Light_Sensor(int myID, int evalPeriod);

	void Create_Light_Source(void);

	void Create_Position_Sensor(int myID, int evalPeriod);

	void Create_Touch_Sensor(int myID, int evalPeriod);

        void Create_Vestibular_Sensor(int myID, int evalPeriod);

	void Draw(void);

	void Draw_Ray_Sensor(double x, double y, double z, int t);

        double Get_Blue_Component(void);

	dBodyID Get_Body(void);

        double Get_Green_Component(void);

	double Get_Length(void);

	double Get_Radius(void);

	double Get_Red_Component(void);

        void Poll_Sensors(int numObjects, OBJECT **objects, int t);

	void Read_From_Python(dWorldID world, dSpaceID space, int shape);

	void Set_Ray_Sensor(double distance,OBJECT *objectThatWasHit, int t);

	void Touch_Sensor_Fires(int t);

	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);

private:
	int  Contains_A_Light_Source(void);

	double Distance_To(OBJECT *otherObject);

	void CreateBox(dWorldID world, dSpaceID space,
                                double x, double y, double z,
                                double length, double width, double height);

	void CreateCylinder(dWorldID world, dSpaceID space,
                                                double x, double y, double z,
                                                double r1, double r2, double r3,
                                                double length, double radius);
	void DrawBox(void);

	void DrawCylinder(void);

	OBJECT *Find_Closest_Light_Source(int numObjects, OBJECT **objects);

};

#endif
