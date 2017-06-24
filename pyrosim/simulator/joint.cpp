#ifndef _JOINT_CPP
#define _JOINT_CPP

#include "iostream"

#include "joint.h"

#include <drawstuff/drawstuff.h>
#include "texturepath.h"
//#include "constants.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

extern int HINGE;
extern int SLIDER;

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

    joint = NULL;

    positionControl = true;
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
    if (type==HINGE){
	   if ( positionControl )
     {
          currentTarget = dJointGetHingeAngle(joint); 
          diff = desiredTarget - currentTarget;
          dJointSetHingeParam(joint,dParamVel, speed * diff);
    
       }       	
    	else
      {
            dJointSetHingeParam(joint,dParamVel, 0.5*speed*desiredTarget);
        }
	   dJointSetHingeParam(joint,dParamFMax, torque);
    }
    else{
        if (positionControl){
            currentTarget = dJointGetSliderPosition(joint);

            diff = desiredTarget - currentTarget;
            dJointSetSliderParam(joint,dParamVel, speed*diff);
        }
        else {
            dJointSetSliderParam(joint,dParamVel, speed*desiredTarget);
        }
        dJointSetSliderParam(joint,dParamFMax, torque);   
    }
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
    if(type ==HINGE)
        Create_Hinge_Joint_In_Simulator(world,firstObject,secondObject);
    else 
        Create_Slider_Joint_In_Simulator(world,firstObject,secondObject);
}

void JOINT::Create_Proprioceptive_Sensor(int myID, int evalPeriod) {

        proprioceptiveSensor = new PROPRIOCEPTIVE_SENSOR(myID,evalPeriod);
}

void JOINT::Draw(OBJECT *first, OBJECT *second){
    dVector3 jointPosition;
    dVector3 jointAxis;
    dMatrix3 rotation;
    dReal jointActuation;
    dReal radius = .025;
    dReal length = .3;
    if( type==HINGE){
        float r = 1.;
        float g = .3;
        float b = .3;


        dsSetColorAlpha(r,g,b,1.0);

        dJointGetHingeAnchor(joint,jointPosition);
        dJointGetHingeAxis(joint,jointAxis);

        dRFromZAxis(rotation, jointAxis[0],jointAxis[1],jointAxis[2]);
        dsDrawCylinder(jointPosition,rotation,length,radius);
    }
    else{
        float r = .3;
        float g = 1.0;
        float b = .3;
        radius = .01;

        if (first != NULL && second != NULL){
           const dReal *pos1 = dBodyGetPosition(first->Get_Body());
           const dReal *pos2 = dBodyGetPosition(second->Get_Body());
           x = (pos1[0] + pos2[0]) /2.0;
           y = (pos1[1] + pos2[1])/2.0;
           z = (pos1[2] + pos2[2])/2.0;
        }
        dJointGetSliderAxis(joint, jointAxis);
        jointActuation = dJointGetSliderPosition(joint);
        jointPosition[0] = x;
        jointPosition[1] = y;
        jointPosition[2] = z;

        dRFromZAxis(rotation, jointAxis[0], jointAxis[1], jointAxis[2]);
        dsSetColorAlpha(r,g,b,1.0);
        dsDrawCylinder(jointPosition, rotation, highStop-lowStop, radius);

        jointPosition[0] += jointActuation*jointAxis[0];
        jointPosition[1] += jointActuation*jointAxis[1];
        jointPosition[2] += jointActuation*jointAxis[2];
        dsSetColorAlpha(1.0,1.0,.3,1.0);
        dsDrawSphere(jointPosition, rotation, radius*2.0);  
        
        jointPosition[0] -= 2*jointActuation*jointAxis[0];
        jointPosition[1] -= 2*jointActuation*jointAxis[1];
        jointPosition[2] -= 2*jointActuation*jointAxis[2];
        dsSetColorAlpha(.3,1.0,1.0,1.0);
        dsDrawSphere(jointPosition, rotation, radius*2.0);  
    }
}

int JOINT::Get_First_Object_Index(void) {

	return firstObject;
}

int JOINT::Get_Second_Object_Index(void) {

        return secondObject;
}

void JOINT::Poll_Sensors(int t) {

        if ( proprioceptiveSensor )

                proprioceptiveSensor->Poll(joint, type, t);
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


void JOINT::Create_Hinge_Joint_In_Simulator(dWorldID world, OBJECT *first, OBJECT *second) {

        joint = dJointCreateHinge(world,0);

        if (first == NULL)
            dJointAttach( joint , 0 , second->Get_Body() );
        else if(second == NULL)
            dJointAttach( joint , first->Get_Body() , 0 );
        else
            dJointAttach( joint , first->Get_Body() , second->Get_Body() );

        dJointSetHingeAnchor(joint,x,y,z);

        dJointSetHingeAxis(joint,normalX,normalY,normalZ);

        if ( positionControl == true ) {
                dJointSetHingeParam(joint,dParamLoStop,lowStop);
                dJointSetHingeParam(joint,dParamHiStop,highStop);
        }
}

void JOINT::Create_Slider_Joint_In_Simulator(dWorldID world, OBJECT *first, OBJECT *second){

        joint = dJointCreateSlider(world,0);
        if (first == NULL){
            dJointAttach( joint , 0 , second->Get_Body() );
            const dReal *position = dBodyGetPosition(second->Get_Body());
            x = position[0];
            y = position[1];
            z = position[2];
        }
        else if(second == NULL){
            dJointAttach( joint , first->Get_Body() , 0 );
            const dReal *position = dBodyGetPosition(first->Get_Body());
            x = position[0];
            y = position[1];
            z = position[2];
        }
        else
            dJointAttach( joint , first->Get_Body() , second->Get_Body() );

        //dJointSetHingeAnchor(joint,x,y,z);

        dJointSetSliderAxis(joint,normalX,normalY,normalZ);

        if ( positionControl == true ) {
                dJointSetSliderParam(joint,dParamLoStop,lowStop);
                dJointSetSliderParam(joint,dParamHiStop,highStop);
        }
}

#endif
