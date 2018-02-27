#ifndef _ENVIRONMENT_CPP
#define _ENVIRONMENT_CPP

#include <iostream>
#include <map>
#include <cstdlib>

#include "environment.h"
#include "constants.h"

extern const int BOX;
extern const int CYLINDER;
extern const int SPHERE;
extern const int CAPSULE;

ENVIRONMENT::ENVIRONMENT(void) : numberOfBodies(0),
                                 numberOfActuators(0),
                                 neuralNetwork(new NEURAL_NETWORK) {};

ENVIRONMENT::~ENVIRONMENT(void) {}; // FIXME: destroy the neural network when its destructor is sane

void ENVIRONMENT::Actuate_Actuators(void) {

	for (int j=0;j<numberOfActuators;j++)

		actuators[j]->Actuate();
}

void ENVIRONMENT::Draw(int debug) {

	for (int i=0;i<numberOfBodies;i++)

		objects[i]->Draw();

	if (debug){

		for (int j=0; j<numberOfActuators; j++)

			actuators[j]->Draw();
	}
}

void ENVIRONMENT::Get_Object_Position(float *xyz, int bodyID){
        const dReal *pos = dBodyGetPosition(objects[bodyID]->Get_Body());
        xyz[0] = pos[0];
        xyz[1] = pos[1];
        xyz[2] = pos[2];
}

void ENVIRONMENT::Read_From_Python(dWorldID world, dSpaceID space, Data* data)
{
	char incomingString[10000];
	std::cin >> incomingString;

	while ( strcmp(incomingString,"Done") != 0 ) {
		std::cerr << incomingString << "\n";

		//Simulator options
		if ( strcmp(incomingString,"EvaluationTime") == 0 )
			std::cin >> data->evaluationTime;
		else if ( strcmp(incomingString,"TimeInterval") == 0)
			std::cin >> data->dt;
		else if ( strcmp(incomingString,"Gravity") == 0)
			std::cin >> data->gravity;
		else if ( strcmp(incomingString,"DisableFloor") == 0)
			data->disableFloor = true;
		else if ( strcmp(incomingString,"TexturePath") == 0)
			std::cin >> data->texturePathStr;
		else if ( strcmp(incomingString,"Debug") == 0)
			std::cin >> data->debug;
		else if ( strcmp(incomingString,"ExternalForce") == 0) {
			int bodyID;
			std::cin >> bodyID;
			objects[bodyID]->Read_In_External_Force();
		}
		else if ( strcmp(incomingString,"WindowSize") == 0) {
			std::cin >> data->windowWidth;
			std::cin >> data->windowHeight;
		}

		//Camera
		else if ( strcmp(incomingString,"Camera") == 0) {
			std::cin >> data->xyz[0];
			std::cin >> data->xyz[1];
			std::cin >> data->xyz[2];

			std::cin >> data->hpr[0];
			std::cin >> data->hpr[1];
			std::cin >> data->hpr[2];
		}

		else if ( strcmp(incomingString,"FollowBody")==0)
			std::cin >> data->followBody;
		else if ( strcmp(incomingString,"TrackBody")==0)
			std::cin >> data->trackBody;
		else if ( strcmp(incomingString,"Capture")==0)
			std::cin >> data->capture;

		//Collision data
		else if ( strcmp(incomingString,"CollisionMatrix")==0) {
			std::cin >> data->numCollisionGroups;
			for(int i=0;i<data->numCollisionGroups;i++) {
				for(int j=i;j<data->numCollisionGroups;j++) {
					data->collisionMatrix[i][j]=0;
					data->collisionMatrix[j][i]=0;
					std::cin >> data->collisionMatrix[i][j];
					data->collisionMatrix[j][i] = data->collisionMatrix[i][j];
				}
			}
		}

		//Bodies
		else if ( strcmp(incomingString, "Box") == 0 )
			Create_Object(world, space, numberOfBodies, BOX);
		else if ( strcmp(incomingString, "Cylinder") == 0 )
			Create_Object(world, space, numberOfBodies, CYLINDER);
		else if ( strcmp(incomingString, "Capsule") == 0)
			Create_Object(world, space, numberOfBodies, CAPSULE);
		else if ( strcmp(incomingString, "Sphere") == 0)
			Create_Object(world, space, numberOfBodies, SPHERE);

		//Body properties
		else if ( strcmp(incomingString,"MakeObjectSusceptibleToAdhesionKind") == 0 )
			Make_Object_Susceptible_To_Adhesion_Kind();

		//Actuators
		else if ( stringToActuatorMap.find(incomingString) != stringToActuatorMap.end() )
			Create_Actuator(world, space, incomingString);

		//Sensors
		else if ( strcmp(incomingString,"IsSeenSensor") == 0)
			Create_IsSeen_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"PositionSensor") == 0 )
			Create_Position_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"TouchSensor") == 0 )
			Create_Touch_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"RaySensor") == 0 )
			Create_Ray_Sensor(space,data->evaluationTime);
		else if ( strcmp(incomingString,"ProximitySensor") == 0 )
			Create_Proximity_Sensor(space,data->evaluationTime);
		else if ( strcmp(incomingString,"ProprioceptiveSensor") == 0 )
			Create_Proprioceptive_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"LightSensor") == 0 )
			Create_Light_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"VestibularSensor") == 0 )
			Create_Vestibular_Sensor(data->evaluationTime);
		else if ( strcmp(incomingString,"LightSource") == 0 )
			Create_Light_Source();

		//Neural network components: currently neurons and synapses
		else if ( neuralNetwork->Is_A_Neuron_Type(incomingString) )
			neuralNetwork->Read_Neuron_From_Python(incomingString, this, data);
		else if ( neuralNetwork->Is_A_Synapse_Type(incomingString) )
			neuralNetwork->Read_Synapse_From_Python(incomingString, this, data);

		//If the string was not recognized, exit with error
		else {
			std::cerr << "Unrecognized command " << incomingString << " received, exiting\n";
			exit(1);
		}

		std::cin >> incomingString;
	}
}

void ENVIRONMENT::Poll_Sensors(int timeStep) {

	for (int i=0;i<numberOfBodies;i++)
		objects[i]->Poll_Sensors(numberOfBodies,objects,timeStep);

	for (int j=0;j<numberOfActuators;j++)
		actuators[j]->Poll_Sensors(timeStep);
}

void ENVIRONMENT::Update_Neural_Network(int timeStep) {

	Update_Sensor_Neurons(timeStep);
	neuralNetwork->Update(timeStep);
}

void ENVIRONMENT::Update_Forces(int timeStep){
    for(int i=0;i<numberOfBodies;i++){
        objects[i]->Apply_Stored_Forces(timeStep);
    }
}
void ENVIRONMENT::Write_Sensor_Data(int evalPeriod) {

    std::cerr << "finishing" << std::endl;

	for (int i=0; i<numberOfBodies; i++)

		objects[i]->Write_To_Python(evalPeriod);

    for (int j=0; j<numberOfActuators; j++)

		actuators[j]->Write_To_Python(evalPeriod);

	std::cout << "Done\n";
}

// ----------------------- Private methods ---------------------------

void ENVIRONMENT::Create_Actuator( dWorldID world, dSpaceID space, std::string actuatorTypeString ) {

	actuators[numberOfActuators] = stringToActuatorMap.at(actuatorTypeString)(); // WARNING: const at() is C++11-specific

	actuators[numberOfActuators]->Read_From_Python();

	actuators[numberOfActuators]->Create_In_Simulator( world, objects, numberOfBodies, actuators, numberOfActuators );

	numberOfActuators++;
}

void ENVIRONMENT::Create_Light_Sensor(int evalPeriod) {

	int objectIndex;
	int ID;
	std::cin >> ID;
	std::cin >> objectIndex;

	objects[objectIndex]->Create_Light_Sensor(ID,evalPeriod);
}

void ENVIRONMENT::Create_Light_Source(void) {

	int objectIndex;
	std::cin >> objectIndex;

	objects[objectIndex]->Create_Light_Source();
}

void ENVIRONMENT::Create_Object(dWorldID world, dSpaceID space, int index, int shape) {

    objects[index] = new OBJECT();
	objects[index]->Read_From_Python(world, space, shape);
	numberOfBodies++;
}

void ENVIRONMENT::Make_Object_Susceptible_To_Adhesion_Kind(void) {

	int objectIndex;
	int adhesionKind;

	std::cin >> objectIndex;
	std::cin >> adhesionKind;

	objects[objectIndex]->Set_Adhesion_Susceptibility(adhesionKind);
}

void ENVIRONMENT::Create_Ray_Sensor(dSpaceID space, int evalPeriod) {

    int objectIndex;
    int ID;

    std::cin >> ID;
    std::cin >> objectIndex;

    objects[objectIndex]->Create_Ray_Sensor(space,ID,evalPeriod);
}

void ENVIRONMENT::Create_Proximity_Sensor(dSpaceID space, int evalPeriod) {

    int objectIndex;
    int ID;

    std::cin >> ID;
    std::cin >> objectIndex;

    objects[objectIndex]->Create_Proximity_Sensor(space,ID,evalPeriod);
}

void ENVIRONMENT::Create_IsSeen_Sensor(int evalPeriod){

	int objectIndex;
	int ID;

	std::cin >> ID;
	std::cin >> objectIndex;
	std::cerr << "Creating is seen " << ID << std::endl;

	objects[objectIndex]->Create_IsSeen_Sensor(ID, evalPeriod);
}

void ENVIRONMENT::Create_Position_Sensor(int evalPeriod) {

    int objectIndex;
    int ID;

    std::cin >> ID;
    std::cin >> objectIndex;

    objects[objectIndex]->Create_Position_Sensor(ID, evalPeriod);
}

void ENVIRONMENT::Create_Proprioceptive_Sensor(int evalPeriod) {

	int actuatorIndex;
	int ID;

	std::cin >> ID;
	std::cin >> actuatorIndex;

	actuators[actuatorIndex]->Create_Proprioceptive_Sensor(ID, evalPeriod);
}

void ENVIRONMENT::Create_Touch_Sensor(int evalPeriod) {

    int objectIndex;
    int ID;
	std::cin >> ID;
    std::cin >> objectIndex;

    objects[objectIndex]->Create_Touch_Sensor(ID,evalPeriod);
}

void ENVIRONMENT::Create_Vestibular_Sensor(int evalPeriod) {

	int objectIndex;
	int ID;
	std::cin >> ID;
	std::cin >> objectIndex;

	objects[objectIndex]->Create_Vestibular_Sensor(ID,evalPeriod);
}

void ENVIRONMENT::Update_Sensor_Neurons(int timeStep) {

	for (int i=0; i<numberOfBodies; i++)
		objects[i]->Update_Sensor_Neurons(timeStep);

	for (int j=0; j<numberOfActuators; j++)
		actuators[j]->Update_Sensor_Neurons(timeStep);
}

#endif // _ENVIRONMENT_CPP
