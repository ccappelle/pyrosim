#ifndef _ENVIRONMENT_CPP
#define _ENVIRONMENT_CPP

#include "environment.h"
#include "iostream"


extern int HINGE;
extern int SLIDER;
extern int THRUSTER;

extern int BOX;
extern int CYLINDER;
extern int SPHERE;
extern int CAPSULE;

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

void ENVIRONMENT::Draw(int debug) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Draw();

        if (debug){
                for (int j=0; j<numberOfJoints; j++){
                        joints[j]->Draw();
                    }
        }

}

void ENVIRONMENT::Get_Object_Position(float *xyz, int bodyID){
        const dReal *pos = dBodyGetPosition(objects[bodyID]->Get_Body());
        xyz[0] = pos[0];
        xyz[1] = pos[1];
        xyz[2] = pos[2];
}

void ENVIRONMENT::Read_From_Python(dWorldID world, dSpaceID space, Data *data)
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
                else if ( strcmp(incomingString,"TexturePath") == 0)
                        std::cin >> data->texturePathStr;
                else if ( strcmp(incomingString,"Debug") == 0)
                        std::cin >> data->debug;
                else if ( strcmp(incomingString,"ExternalForce") == 0){
                        int bodyID;
                        float x,y,z;
                        int time;
                        std::cin >> bodyID;
                        objects[bodyID]->Read_In_External_Force();
                }
                else if ( strcmp(incomingString,"WindowSize") == 0)
                {
                    std::cin >> data->windowWidth;
                    std::cin >> data->windowHeight;
                }

                //Camera
                else if ( strcmp(incomingString,"Camera") == 0)
                {
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
                {
                		std::cin >> data->capture;
                }

                //Collision data
                else if ( strcmp(incomingString,"CollisionMatrix")==0){
                        std::cin >> data->numCollisionGroups;
                        for(int i=0;i<data->numCollisionGroups;i++){
                          for(int j=i;j<data->numCollisionGroups;j++){
                                data->collisionMatrix[i][j]=0;
                                data->collisionMatrix[j][i]=0;
                                std::cin >> data->collisionMatrix[i][j];
                                data->collisionMatrix[j][i] = data->collisionMatrix[i][j];
                         }
                        }
                }
                //Bodies
                else if ( strcmp(incomingString,"Box") == 0 )

                        Create_Object(world,space,numberOfBodies,BOX);

                else if ( strcmp(incomingString,"Cylinder") == 0 )

                        Create_Object(world,space,numberOfBodies,CYLINDER);
                else if ( strcmp(incomingString,"Capsule") == 0)
                        Create_Object(world,space,numberOfBodies,CAPSULE);
                else if ( strcmp(incomingString,"Sphere") == 0)
                        Create_Object(world,space,numberOfBodies, SPHERE);

                //Joints
                else if ( strcmp(incomingString,"HingeJoint") == 0 )
                        Create_Joint(world,space,numberOfJoints,HINGE);

                else if ( strcmp(incomingString,"SliderJoint") == 0)
                        Create_Joint(world,space,numberOfJoints,SLIDER);
                else if ( strcmp(incomingString,"Thruster") == 0)
                        Create_Joint(world,space,numberOfJoints,THRUSTER);
                //Sensors
                else if ( strcmp(incomingString,"IsSeenSensor") == 0)
                        Create_IsSeen_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"PositionSensor") == 0 )

                        Create_Position_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"TouchSensor") == 0 )

                        Create_Touch_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"RaySensor") == 0 )

                        Create_Ray_Sensor(space,data->evaluationTime);

                else if ( strcmp(incomingString,"ProprioceptiveSensor") == 0 )

                        Create_Proprioceptive_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"LightSensor") == 0 )

                        Create_Light_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"VestibularSensor") == 0 )

                        Create_Vestibular_Sensor(data->evaluationTime);

                else if ( strcmp(incomingString,"LightSource") == 0 )

                        Create_Light_Source();

                //Neurons
                else if ( strcmp(incomingString,"BiasNeuron") == 0 )

                        Create_Bias_Neuron();
                else if ( strcmp(incomingString,"SensorNeuron") == 0 )

                        Create_Sensor_Neuron();

                else if ( strcmp(incomingString,"HiddenNeuron") == 0 )

                        Create_Hidden_Neuron();

                else if ( strcmp(incomingString,"MotorNeuron") == 0 )

                        Create_Motor_Neuron();

                else if ( strcmp(incomingString,"FunctionNeuron") == 0)
                        Create_Function_Neuron(data->evaluationTime);
                //Synapse
                else
                        Create_Synapse();

                std::cin >> incomingString;
        }
}

void ENVIRONMENT::Poll_Sensors(int timeStep) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Poll_Sensors(numberOfBodies,objects,timeStep);

        for (int j=0;j<numberOfJoints;j++)

                joints[j]->Poll_Sensors(timeStep);
}

void ENVIRONMENT::Update_Neural_Network(int timeStep) {

	Update_Sensor_Neurons(timeStep);
	if ( neuralNetwork )

		neuralNetwork->Update(timeStep);
}

void ENVIRONMENT::Update_Forces(int timeStep){
    for(int i=0;i<numberOfBodies;i++){
        objects[i]->Apply_Stored_Forces(timeStep);
    }
}
void ENVIRONMENT::Write_Sensor_Data(int evalPeriod) {

    std::cerr << "finishing" << std::endl;
	for (int i=0;i<numberOfBodies;i++)
		objects[i]->Write_To_Python(evalPeriod);
    for (int j=0;j<numberOfJoints;j++)
    	joints[j]->Write_To_Python(evalPeriod);

	std::cout << "Done\n";
}

// ----------------------- Private methods ---------------------------

void ENVIRONMENT::Add_Motor_Neuron(int ID, int jointID, double tau, double alpha, double start) {

        NEURON *motorNeuron = neuralNetwork->Add_Motor_Neuron(ID,tau, alpha,start);

        Connect_Motor_Neuron_to_Joint( jointID, motorNeuron );
}

void ENVIRONMENT::Add_Sensor_Neuron(int ID, int sensorID, int sensorValueIndex) {

        NEURON *sensorNeuron = neuralNetwork->Add_Sensor_Neuron(ID,sensorValueIndex);

        Connect_Sensor_To_Sensor_Neuron( sensorID, sensorNeuron );
}

void ENVIRONMENT::Create_Bias_Neuron(void) {

        int ID;

        std::cin >> ID;

        if ( neuralNetwork == NULL )

                Create_Neural_Network();

        neuralNetwork->Add_Bias_Neuron(ID);
}

void ENVIRONMENT::Create_Function_Neuron(int evalPeriod){
        int ID;
        std::cin >> ID;

        if( neuralNetwork == NULL)
                Create_Neural_Network();
        double *timeValues = new double[evalPeriod];

        for(int i=0; i<evalPeriod; i++)
        {
                std::cin >> timeValues[i];
        }

        neuralNetwork->Add_Function_Neuron(ID, timeValues);
}
void ENVIRONMENT::Create_Hidden_Neuron(void) 
{

    int ID;
    std::cin >> ID;

	double tau;
	std::cin >> tau;

    double alpha;
    std::cin >> alpha;

    if ( neuralNetwork == NULL )
        Create_Neural_Network();

	neuralNetwork->Add_Hidden_Neuron(ID,tau,alpha);
}

void ENVIRONMENT::Create_Joint( dWorldID world, dSpaceID space, int index, int jointType) {

	joints[index] = new JOINT(jointType);

	joints[index]->Read_From_Python();
	int firstObjectID = joints[index]->Get_First_Object_Index();

	int secondObjectID = joints[index]->Get_Second_Object_Index();

	OBJECT *firstObject = NULL;

	if ( firstObjectID >= 0 )
		firstObject = objects[ firstObjectID ];

    OBJECT *secondObject = NULL;

	if ( secondObjectID >= 0 )
		secondObject = objects[ secondObjectID ];

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

    double tau;
    std::cin >> tau;

    double alpha;
    std::cin >> alpha;

    double start;
    std::cin >> start;

    if ( neuralNetwork == NULL )
        Create_Neural_Network();

    Add_Motor_Neuron(ID,jointID,tau,alpha,start);
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
    // while ( (done == false) && (jointIndex < numberOfJoints))
    //   done = joints[jointIndex++]->Connect_To_Motor_Neuron( jointID , motorNeuron );
    
    joints[jointID]->Connect_To_Motor_Neuron(jointID, motorNeuron);
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

	if ( neuralNetwork == NULL )

		Create_Neural_Network();

	Add_Sensor_Neuron(ID,sensorID,sensorValueIndex);
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

void ENVIRONMENT::Update_Sensor_Neurons(int timeStep) {

        for (int i=0;i<numberOfBodies;i++)

                objects[i]->Update_Sensor_Neurons(timeStep);

        for (int j=0;j<numberOfJoints;j++)

                joints[j]->Update_Sensor_Neurons(timeStep);
}
#endif
