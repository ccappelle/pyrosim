#ifndef _JOINT_CPP
#define _JOINT_CPP

#include "iostream"

#include "joint.h"

#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

JOINT::JOINT(int jointType) {
        type = jointType;
        firstObject = 0;
        secondObject = 0;

        x = 0;
        y = 0;
        z = 0;

        normalX = 0;
        normalY = 0;
        normalZ = 0;

        lowStop = 0;
        highStop = 0;

    	speed = 0.0;
        torque = 0.0;


	positionControl = true;

        joint = NULL;

        proprioceptiveSensor = NULL;

        motorNeuron = NULL;
}

JOINT::~JOINT(void) {

}

void JOINT::Actuate(void) {

	if ( motorNeuron == NULL )

		return;

	double motorNeuronValue = motorNeuron->Get_Value();

	double zeroToOne = motorNeuronValue/2.0 + 0.5;

	double diff;

        double desiredTarget = zeroToOne * ( highStop - lowStop ) + lowStop;

	double currentTarget;

	if ( positionControl )
    {
      currentTarget = dJointGetHingeAngle(joint); 
      diff = desiredTarget - currentTarget;
      dJointSetHingeParam(joint,dParamVel, speed * diff);
    }
        	
	else
    {
		currentTarget = dJointGetHingeAngleRate(joint);
        dJointSetHingeParam(joint,dParamVel, speed*desiredTarget);
    }

	dJointSetHingeParam(joint,dParamFMax, torque);
}

int JOINT::Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron) {

        if ( proprioceptiveSensor )

                if ( proprioceptiveSensor->Get_ID() == sensorID ) {

                        proprioceptiveSensor->Connect_To_Sensor_Neuron(sensorNeuron);

                        return true;
                }

        return false;
}

int  JOINT::Connect_To_Motor_Neuron(int jointID, NEURON *mNeuron) {

	if ( ID == jointID ) {

		motorNeuron = mNeuron;

		return true;
	}
	else
		return false;
}

void JOINT::Create_In_Simulator(dWorldID world, OBJECT *firstObject, OBJECT *secondObject) {

    Create_Hinge_Joint_In_Simulator(world,firstObject,secondObject);
}

void JOINT::Create_Proprioceptive_Sensor(int myID, int evalPeriod) {

        proprioceptiveSensor = new PROPRIOCEPTIVE_SENSOR(myID,evalPeriod);
}

void JOINT::Draw(){
    dVector3 jointPosition;
    dVector3 jointAxis;
    dVector3 jointRotation;
    dMatrix3 rotation;
    dReal jointAngle;

    float r = 1.;
    float g = .3;
    float b = .3;
    dReal radius = .025;
    dReal length = .3;

    dsSetColorAlpha(r,g,b,1.0);

    dJointGetHingeAnchor(joint,jointPosition);
    dJointGetHingeAxis(joint,jointAxis);
    jointAngle = dJointGetHingeAngle(joint);
    

    //Make a rotation matrix from a z-aligned cylinder to target
    //i.e. take cross product between the two vectors (0,0,1) and
    //jointAxis. Then rotate by angle between z-aligned and jointAxis
    float angle = acos( jointAxis[2]);
    dRFromAxisAndAngle(rotation,
                        jointAxis[1],
                        -jointAxis[0],                   
                        0.,
                        -angle);
    dsDrawCylinder(jointPosition,rotation,length,radius);
}

int JOINT::Get_First_Object_Index(void) {

	return firstObject;
}

int JOINT::Get_Second_Object_Index(void) {

        return secondObject;
}

void JOINT::Poll_Sensors(int t) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Poll(joint,t);
}

void JOINT::Read_From_Python(void) {

        std::cin >> ID;

        std::cin >> firstObject;

        std::cin >> secondObject;

        std::cin >> x;

        std::cin >> y;

        std::cin >> z;

        std::cin >> normalX;

        std::cin >> normalY;

        std::cin >> normalZ;

        std::cin >> lowStop;

        std::cin >> highStop;

        std::cin >> speed;

        std::cin >> torque;

	char temp[100];

	std::cin >> temp;

	if ( strcmp(temp,"True")==0 )

		positionControl = true;
	else
		positionControl = false;
}

void JOINT::Update_Sensor_Neurons(int t) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Update_Sensor_Neurons(t);
}

void JOINT::Write_To_Python(int evalPeriod) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Write_To_Python(evalPeriod);
}

// ------------------- Private methods --------------------------


void JOINT::Create_Hinge_Joint_In_Simulator(dWorldID world, OBJECT *firstObject, OBJECT *secondObject) {

        joint = dJointCreateHinge(world,0);

        if (firstObject == NULL)
            dJointAttach( joint , 0 , secondObject->Get_Body() );
        else if(secondObject == NULL)
            dJointAttach( joint , firstObject->Get_Body() , 0 );
        else
            dJointAttach( joint , firstObject->Get_Body() , secondObject->Get_Body() );

        dJointSetHingeAnchor(joint,x,y,z);

        dJointSetHingeAxis(joint,normalX,normalY,normalZ);

        if ( positionControl == true ) {

                dJointSetHingeParam(joint,dParamLoStop,lowStop);

                dJointSetHingeParam(joint,dParamHiStop,highStop);
        }
}


#endif
