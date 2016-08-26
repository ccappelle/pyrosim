#ifndef _ENVIRONMENT_CPP
#define _ENVIRONMENT_CPP

#include "environment.h"
#include "iostream"

extern int BOX;
extern int CYLINDER;

extern int MAX_OBJECTS;
extern int MAX_JOINTS;

ENVIRONMENT::ENVIRONMENT(void) {

	objects = new OBJECT * [MAX_OBJECTS];

	joints  = new JOINT * [MAX_JOINTS];

	numberOfBodies = 0;

	numberOfJoints = 0;

	neuralNetwork = NULL;
}

ENVIRONMENT::~ENVIRONMENT(void) {

}

void ENVIRONMENT::Actuate_Joints(void) {

        for (int j=0;j<numberOfJoints;j++)

                joints[j]->Actuate();
}

void ENVIRONMENT::Draw(void) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Draw();
}

void ENVIRONMENT::Read_From_Python(dWorldID world,dSpaceID space, int *evaluationTime) {

        char incomingString[100];

        std::cin >> incomingString;

        while ( strcmp(incomingString,"Done") != 0 ) {

                if ( strcmp(incomingString,"EvaluationTime") == 0 )

			std::cin >> (*evaluationTime);

                else if ( strcmp(incomingString,"Box") == 0 )

			Create_Object(world,space,numberOfBodies,BOX);

		else if ( strcmp(incomingString,"Cylinder") == 0 )

			Create_Object(world,space,numberOfBodies,CYLINDER);

		else if ( strcmp(incomingString,"Joint") == 0 )

			Create_Joint(world,space,numberOfJoints);

		else if ( strcmp(incomingString,"PositionSensor") == 0 )

			Create_Position_Sensor(*evaluationTime);

                else if ( strcmp(incomingString,"TouchSensor") == 0 )

                        Create_Touch_Sensor(*evaluationTime);

                else if ( strcmp(incomingString,"RaySensor") == 0 )

                        Create_Ray_Sensor(space,*evaluationTime);

                else if ( strcmp(incomingString,"ProprioceptiveSensor") == 0 )

			Create_Proprioceptive_Sensor(*evaluationTime);

                else if ( strcmp(incomingString,"LightSensor") == 0 )

                        Create_Light_Sensor(*evaluationTime);

                else if ( strcmp(incomingString,"VestibularSensor") == 0 )

                        Create_Vestibular_Sensor(*evaluationTime);

		else if ( strcmp(incomingString,"LightSource") == 0 )

			Create_Light_Source();

		else if ( strcmp(incomingString,"SensorNeuron") == 0 )

			Create_Sensor_Neuron();

                else if ( strcmp(incomingString,"BiasNeuron") == 0 )

                        Create_Bias_Neuron();

		else if ( strcmp(incomingString,"HiddenNeuron") == 0 )

			Create_Hidden_Neuron();

                else if ( strcmp(incomingString,"MotorNeuron") == 0 )

                        Create_Motor_Neuron();
		else
			Create_Synapse();

                std::cin >> incomingString;
        }

	Finished_Reading_From_Python();
}

void ENVIRONMENT::Poll_Sensors(int t) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Poll_Sensors(numberOfBodies,objects,t);

        for (int j=0;j<numberOfJoints;j++)

                joints[j]->Poll_Sensors(t);
}

void ENVIRONMENT::Update_Neural_Network(int t) {

	Update_Sensor_Neurons(t);

	if ( neuralNetwork )

		neuralNetwork->Update();
}

void ENVIRONMENT::Write_Sensor_Data(int evalPeriod) {

	for (int i=0;i<numberOfBodies;i++)

		objects[i]->Write_To_Python(evalPeriod);

        for (int j=0;j<numberOfJoints;j++)

        	joints[j]->Write_To_Python(evalPeriod);

	std::cout << "Done\n";
}

// ----------------------- Private methods ---------------------------

void ENVIRONMENT::Add_Motor_Neuron(int ID, int jointID, int layer, double tau) {

        NEURON *motorNeuron = neuralNetwork->Add_Motor_Neuron(ID,layer,tau);

        Connect_Motor_Neuron_to_Joint( jointID, motorNeuron );
}

void ENVIRONMENT::Add_Sensor_Neuron(int ID, int sensorID, int sensorValueIndex, int layer, double tau) {

        NEURON *sensorNeuron = neuralNetwork->Add_Sensor_Neuron(ID,sensorValueIndex,layer,tau);

        Connect_Sensor_To_Sensor_Neuron( sensorID, sensorNeuron );
}

void ENVIRONMENT::Create_Bias_Neuron(void) {

        int ID;

        std::cin >> ID;

        int layer;

        std::cin >> layer;

        if ( neuralNetwork == NULL )

                Create_Neural_Network();

        neuralNetwork->Add_Bias_Neuron(ID,layer);
}

void ENVIRONMENT::Create_Hidden_Neuron(void) {

        int ID;

        std::cin >> ID;

        int layer;

        std::cin >> layer;

	double tau;

	std::cin >> tau;

        if ( neuralNetwork == NULL )

                Create_Neural_Network();

	neuralNetwork->Add_Hidden_Neuron(ID,layer,tau);
}

void ENVIRONMENT::Create_Joint( dWorldID world, dSpaceID space, int index) {

	joints[index] = new JOINT();

	joints[index]->Read_From_Python();

	OBJECT *firstObject = objects[ joints[index]->Get_First_Object_Index() ];

        OBJECT *secondObject = objects[ joints[index]->Get_Second_Object_Index() ];

	joints[index]->Create_In_Simulator(	world, firstObject , secondObject );

	numberOfJoints++;
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

void ENVIRONMENT::Create_Motor_Neuron(void) {

        int ID;

        std::cin >> ID;

        int jointID;

        std::cin >> jointID;

        int layer;

        std::cin >> layer;

	double tau;

	std::cin >> tau;

        if ( neuralNetwork == NULL )

                Create_Neural_Network();

        Add_Motor_Neuron(ID,jointID,layer,tau);
}

void ENVIRONMENT::Create_Neural_Network(void) {

	neuralNetwork = new NEURAL_NETWORK();
}

void ENVIRONMENT::Create_Object(dWorldID world, dSpaceID space, int index, int shape) {

        objects[index] = new OBJECT();

	objects[index]->Read_From_Python(world,space,shape);

	numberOfBodies++;
}

void ENVIRONMENT::Create_Ray_Sensor(dSpaceID space, int evalPeriod) {

        int objectIndex;

	int ID;

	std::cin >> ID;

        std::cin >> objectIndex;

        objects[objectIndex]->Create_Ray_Sensor(space,ID,evalPeriod);
}

void ENVIRONMENT::Create_Position_Sensor(int evalPeriod) {

	int objectIndex;

	int ID;

	std::cin >> ID;

        std::cin >> objectIndex; 

	objects[objectIndex]->Create_Position_Sensor(ID,evalPeriod);	
}

void ENVIRONMENT::Create_Proprioceptive_Sensor(int evalPeriod) {

        int jointIndex;

	int ID;

	std::cin >> ID;

        std::cin >> jointIndex;

        joints[jointIndex]->Create_Proprioceptive_Sensor(ID,evalPeriod);
}

void ENVIRONMENT::Connect_Motor_Neuron_to_Joint( int jointID, NEURON *motorNeuron ) {

        int done = false;

        int jointIndex = 0;

        while ( (done == false) && (jointIndex < numberOfJoints) )

                done = joints[jointIndex++]->Connect_To_Motor_Neuron( jointID , motorNeuron );
}

void ENVIRONMENT::Connect_Sensor_To_Sensor_Neuron( int sensorID , NEURON *sensorNeuron ) {

        int done = false;

        int objectIndex = 0;

        while ( (done == false) && (objectIndex < numberOfBodies) )

                done = objects[objectIndex++]->Connect_Sensor_To_Sensor_Neuron(sensorID,sensorNeuron);

        int jointIndex = 0;

        while ( (done == false) && (jointIndex < numberOfJoints) )

                done = joints[jointIndex++]->Connect_Sensor_To_Sensor_Neuron(sensorID,sensorNeuron);
}

void ENVIRONMENT::Create_Sensor_Neuron(void) {

	int ID;

	std::cin >> ID;

	int sensorID;

        std::cin >> sensorID;

	int sensorValueIndex;

        std::cin >> sensorValueIndex;

        int layer; 

        std::cin >> layer; 

	double tau;

	std::cin >> tau;

	if ( neuralNetwork == NULL )

		Create_Neural_Network();

	Add_Sensor_Neuron(ID,sensorID,sensorValueIndex,layer,tau);
}

void ENVIRONMENT::Create_Synapse(void) {

        if ( neuralNetwork == NULL )

                Create_Neural_Network();

        neuralNetwork->Add_Synapse();
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

void ENVIRONMENT::Finished_Reading_From_Python(void) {

	if ( neuralNetwork )

		neuralNetwork->Finished_Reading_From_Python();
}

void ENVIRONMENT::Update_Sensor_Neurons(int t) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Update_Sensor_Neurons(t);

        for (int j=0;j<numberOfJoints;j++)

                joints[j]->Update_Sensor_Neurons(t);
}
#endif
