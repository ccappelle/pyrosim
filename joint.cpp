#ifndef _JOINT_CPP
#define _JOINT_CPP

#include "iostream"

#include "joint.h"

JOINT::JOINT(void) {

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

	double desiredAngle = zeroToOne * ( highStop - lowStop ) + lowStop;

	double currentAngle = dJointGetHingeAngle(joint);

	double diff = desiredAngle - currentAngle;

	dJointSetHingeParam(joint,dParamVel, speed * diff);

	dJointSetHingeParam(joint,dParamFMax,1000000);
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

	joint = dJointCreateHinge(world,0);

	dJointAttach( joint , firstObject->Get_Body() , secondObject->Get_Body() );

	dJointSetHingeAnchor(joint,x,y,z);

	dJointSetHingeAxis(joint,normalX,normalY,normalZ);

	dJointSetHingeParam(joint,dParamLoStop,lowStop);

        dJointSetHingeParam(joint,dParamHiStop,highStop);
}

void JOINT::Create_Proprioceptive_Sensor(int myID, int evalPeriod) {

        proprioceptiveSensor = new PROPRIOCEPTIVE_SENSOR(myID,evalPeriod);
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
}

void JOINT::Update_Sensor_Neurons(int t) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Update_Sensor_Neurons(t);
}

void JOINT::Write_To_Python(int evalPeriod) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Write_To_Python(evalPeriod);
}

#endif
