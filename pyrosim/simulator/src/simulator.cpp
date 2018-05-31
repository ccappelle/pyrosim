// std headers
#include <iostream>
#include <map>
#include <cmath>
#include <utility>
#include <vector>


// ode headers
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

// local headers
#include "pythonReader.hpp"
#include "environment.hpp"
#include "body/rigidBody.hpp"

// glut stupidity
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// drawing necessity
#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

#define PI 3.14159265

// global variables
std::map<std::string, float> parameters; // maps of parameters useful to the simulator
int evalStep; // current evaluation step
float evalTime; // current eval time in simulated seconds
Environment *environment;

std::string texturePathStr = "../external/ode-0.12/drawstuff/textures";
dsFunctions fn; // drawstuff pointers

dWorldID world; // the entire world
dSpaceID topspace; // top space
dJointGroupID contactgroup; // contact joints group

// collision map
typedef std::pair<std::string, std::string> collisionPair;
std::map<collisionPair, int> collisionMap;

// various flags
int firstStep = true;
int drawJoints = false;
int drawSpaces = false;
int playBlind = false;

std::string COLLIDE_ALWAYS = "Collide";

void readCollisionFromPython(void);
static void command(void);
void createEnvironment(void);
static void drawLoop(int pause);
void endSimulation();
static void nearCallback(void *callbackData, dGeomID o1, dGeomID o2);
void readFromPython(void);
void initializeDrawStuff(void);
void initializeEnvironment(void);
void initializeODE(void);
void initializeParameters(void);
static void start(void);
void simulationStep(void);

int main(int argc, char **argv){

    playBlind = false;
    for (int i=0; i<argc; i++){
        if (strcmp(argv[i], "-blind") == 0){
            playBlind = true;
        }
    }

    // these functions cannot use input parameters
    initializeODE();
    initializeParameters();
    initializeEnvironment();

    readFromPython();
    // below here can use global input parameters
    createEnvironment();
    dWorldSetAutoDisableFlag(world, 1);

    std::cerr << "Simulation Starting" << std::endl;
    if (playBlind){
        while(1){
            simulationStep();
        }
    }
    else{
        initializeDrawStuff();
        dsSimulationLoop(argc, argv, 900, 700, &fn);
    }
}

void readCollisionFromPython(void){
    std::string group1, group2;

    std::cerr << "Reading Collision Assignment" << std::endl;
    readStringFromPython(group1, "Collision Group 1");
    readStringFromPython(group2, "Collision Group 2");

    collisionPair pair1 = std::make_pair(group1, group2);
    collisionMap[pair1] = true;
    collisionPair pair2 = std::make_pair(group2, group1);
    collisionMap[pair2] = true;
}

static void command(int cmd){

    // 'x' for exit
    if (cmd == 'x'){
        endSimulation();
    }
    // 'd' for toggle drawing of debug info (joints for now)
    else if (cmd == 'd'){
        drawJoints = !drawJoints;
    }
    // 's' for toggle drawing of spaces
    else if (cmd == 's'){
        drawSpaces = !drawSpaces;
    }
}

void createEnvironment(void){
    // set gravity
    dWorldSetGravity(world,
                     parameters["GravityX"],
                     parameters["GravityY"],
                     parameters["GravityZ"]);
    // send ground plane
    dGeomID plane = dCreatePlane(topspace, 0, 0, 1, 0);
    int planeID = -1;
    dGeomSetData(plane, static_cast<void*>(&COLLIDE_ALWAYS));
    // create bodies, joints, ANN, etc
    environment->createEntities();

    std::cerr << "Completed Creation" << std::endl;
}

static void drawLoop(int pause){
    if (firstStep){
        float xyz[] = {0.0, -3.0, 2.0};
        float hpr[] = {90.0, -25.0, 0.0};
        dsSetViewpoint(xyz, hpr);
        firstStep=false;
    }

    double elapsed = dsElapsedTime();

    // variable frame rate
    int nsteps = (int) ceilf(elapsed / parameters["DT"]);
    for(int i=0; i < nsteps && !pause; i++){
        simulationStep();
    }

    if (pause == true){
        // draw pause square to signify paused
        float xyz[3];
        float hpr[3];

        dsGetViewpoint(xyz, hpr);
        dVector3 forward, right, up; // direction vector of camera

        forward[0] = cos(hpr[0] * PI / 180.0) * cos(hpr[1] * PI / 180.0); //* (1 - cos(hpr[1] * PI / 180.0));
        forward[1] = sin(hpr[0] * PI / 180.0) * cos(hpr[1] * PI / 180.0); //* (1 - cos(hpr[1] * PI / 180.0));
        forward[2] = sin(hpr[1] * PI / 180.0); 
        dNormalize3(forward);

        // right is orthogonal to forward with no z-component
        right[0] = sin(hpr[0] * PI / 180.0);
        right[1] = -cos(hpr[0] * PI / 180.0);
        right[2] = 0; // dont use roll because its stupid
        dNormalize3(right);

        dCalcVectorCross3(up, right, forward);

        dReal fdist = 0.2;
        dReal rdist = 0.05;
        dReal udist = -0.1;
        dReal rOff = 0.1;
        dReal uOff = 1.1;

        dsSetColor(0.6, 0.1, 0.1);
        for(int i=0; i<2; i++){


            const dReal topPoint[3] = {
                             xyz[0] + forward[0]*fdist + (1+rOff*i)*right[0]*rdist + up[0]*udist,
                             xyz[1] + forward[1]*fdist + (1+rOff*i)*right[1]*rdist + up[1]*udist,
                             xyz[2] + forward[2]*fdist + (1+rOff*i)*right[2]*rdist + up[2]*udist};
            const dReal bottomPoint[3] = {
                           xyz[0] + forward[0]*fdist + (1+rOff*i)*right[0]*rdist + up[0]*udist*uOff,
                           xyz[1] + forward[1]*fdist + (1+rOff*i)*right[1]*rdist + up[1]*udist*uOff,
                           xyz[2] + forward[2]*fdist + (1+rOff*i)*right[2]*rdist + up[2]*udist*uOff};
            dsDrawLine(topPoint, bottomPoint);
        }
    }

    environment->draw(drawJoints, drawSpaces);
}

void endSimulation(void){
    // std::cerr << "Successful Exit" << std::endl;
    std::cerr << "Simulation Completed" << std::endl << std::endl;
    environment->writeToPython();
    std::cerr << "Success" << std::endl;
    exit(0);
}

void initializeDrawStuff(void){
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &drawLoop;
    fn.command = &command;
    fn.stop = 0;
    fn.path_to_textures = texturePathStr.c_str();
}

void initializeEnvironment(void){
    environment = new Environment(world, topspace);
}

void initializeODE(void){
    dInitODE2(0);
    world = dWorldCreate();
    topspace = dHashSpaceCreate(0);
    contactgroup = dJointGroupCreate(0);
}

void initializeParameters(void){
    // C.C while this function is not strictly 
    // necessary, I think it helps to have correspondence
    // between here and python
    parameters["DT"] = 0.01;
    parameters["EvalSteps"] = 200;

    // parameters["ERP"];
    
    // C.C. heavy here to split arrays into different
    // entries in map but maybe necessary
    // to reduce bloat in readFromPython
    // I don't know of other ways other then
    // different parameter maps. aka 
    // v3Parameters, iParameters, fParameters, etc.
    parameters["CameraX"] = 0.0;
    parameters["CameraY"] = 0.0;
    parameters["CameraZ"] = 0.0;

    parameters["CameraH"] = 90.0f;
    parameters["CameraP"] = 0.0f;
    parameters["CameraR"] = 0.0f;

    parameters["GravityX"] = 0.0f;
    parameters["GravityY"] = 0.0f;
    parameters["GravityZ"] = -9.8f;

    parameters["nContacts"] = 10;
}

void simulationStep(void){
    // place action befor collision detection?
    environment->takeStep(evalStep, parameters["DT"]);

    dSpaceCollide(topspace, 0, &nearCallback); // run collision
    dWorldStep(world, parameters["DT"]); // take time step
    dJointGroupEmpty(contactgroup);

    evalTime += parameters["DT"];
    evalStep ++;
    if (evalStep == parameters["EvalSteps"]){
        endSimulation();
    }
    else{
        // std::cerr << evalStep << std::endl;
    }
}

static void start(void)
{
  dAllocateODEDataForThread(dAllocateMaskAll);
}

void nearCallback(void *callbackData, dGeomID o1, dGeomID o2){
    if ( dGeomIsSpace(o1) || dGeomIsSpace(o2) ){
        // collide space with other objects
        dSpaceCollide2(o1, o2, callbackData, &nearCallback);

        // collide with spaces
        if (dGeomIsSpace(o1)){
            dSpaceCollide((dSpaceID) o1, callbackData, &nearCallback);
        }
        if (dGeomIsSpace(o2)){
            dSpaceCollide((dSpaceID) o2, callbackData, &nearCallback);
        }

        return;
    }
    // TODO create exits for connected joints and
    // user defined collision pattern

    // C.C. currently geom data is just a string
    // we should create a data struct instead

    if ( dGeomGetClass (o1) == dPlaneClass       or dGeomGetClass(o2) == dPlaneClass or
         dGeomGetClass (o1) == dHeightfieldClass or dGeomGetClass(o2) == dHeightfieldClass){
        // one or more geoms is a heightfield or plane so collision will occur
    }
    else{
        std::string &group1 = *(static_cast<std::string*> (dGeomGetData(o1)));
        std::string &group2 = *(static_cast<std::string*> (dGeomGetData(o2)));

        if (group1 != COLLIDE_ALWAYS and group2 != COLLIDE_ALWAYS){
            collisionPair pair = std::make_pair(group1, group2);
            if (collisionMap.count(pair) == 0){ // no collision entry, exit early
                return;
            }
            else{
                if (collisionMap[pair] == false){ // collision entry specifies no collision should occur
                    return;
                }
            }
        }
    }


    // generate at most n contacts per collision
    const int N = parameters["nContacts"];
    dContact contact[N];
    int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
    if (n > 0){
        for(int i=0; i<n; i++){
            // set friction parameters for contact
            contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactApprox1;
            contact[i].surface.mu = dInfinity;
            contact[i].surface.slip1 = 0.01;
            contact[i].surface.slip2 = 0.01;

            dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
            dJointAttach(c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
        }
    }
}

void readParameterFromPython(void){
    std::string paramName;
    readStringFromPython(paramName);
    readValueFromPython<float>(&parameters[paramName]);
    std::cerr << paramName << " set to "
              << parameters[paramName]
              << std::endl << std::endl; 
              // double space for clarity
}

void readFromPython(void){
    std::string incomingString;
    readStringFromPython(incomingString);

    while (incomingString != "Done"){
        // read single value parameter
        if (incomingString == "Parameter"){
            readParameterFromPython();
        }
        // read entity
        else if(incomingString == "Entity"){
            environment->readEntityFromPython();
        }
        // add to entity
        else if(incomingString == "Add"){
            environment->addToEntityFromPython();
        }
        // special assignment flag
        // may change layout in the future
        else if(incomingString == "AssignCollision"){
            readCollisionFromPython();
        }
        else{
            std::cerr << "INVALID READ IN " << incomingString << std::endl;
            exit(0);
        }
        // read in next string
        readStringFromPython(incomingString);
    }
    std::cerr << "Finished Reading In From Python" << std::endl << std::endl;
}


// //custom headers
// #include "texturepath.h"
// #include "environment.h"
// #include "datastruct.h"

// #ifdef _MSC_VER
// #pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
// #endif

// #ifdef dDOUBLE
// #define dsDrawLine dsDrawLineD
// #define dsDrawBox dsDrawBoxD
// #define dsDrawSphere dsDrawSphereD
// #define dsDrawCylinder dsDrawCylinderD
// #define dsDrawCapsule dsDrawCapsuleD
// #endif

// static dWorldID world;
// static dSpaceID space;
// static dJointGroupID contactgroup;
// dsFunctions fn;

// int timer;

// ENVIRONMENT *environment;
// int numberOfBodies = 0;
// bool initialized = false;
// static dGeomID ground;


// Data *data = new Data;//struct which keeps all user input values for various parameterss. see datastruct.h
// static float updated_xyz[3];
// int LAGSIZE = 20;
// static float average_z[20];

// const float FPS = 300.0;
// const float baseDT = 1.0/FPS;
// float framestart;
// float accumulator = 0.0;

// void Draw_Distance_Sensor(dGeomID myGeom, dGeomID thisGeom);

// void Read_From_Python(void);

// void Set_Distance_Sensor(dGeomID o);

// void Terminate(void);

// void Handle_Ray_Sensor(dGeomID o1, dGeomID o2) {

//     if ( dGeomGetClass(o1) == dRayClass ) {

//         dContact contact;

//         int n = dCollide(o1,o2,1,&contact.geom,sizeof(dContact));

//         if ( n>0 ) {

//             OBJECT *obj = (OBJECT *)dGeomGetData(o1);
//             OBJECT *obj2 = (OBJECT *)dGeomGetData(o2);

//             obj->Set_Ray_Sensor(contact.geom.depth,obj2,timer);

//             if ( data->runBlind == false )
//                 obj->Draw_Ray_Sensor(contact.geom.pos[0],contact.geom.pos[1],contact.geom.pos[2],timer);

//         }
//     }
// }

// void Handle_Ray_Sensors(dGeomID o1, dGeomID o2) {

//     Handle_Ray_Sensor(o1,o2);

//     Handle_Ray_Sensor(o2,o1);
// }

// static void nearCallback (void *callbackData, dGeomID o1, dGeomID o2)
// {
// 	int i, n;

// 	Handle_Ray_Sensors(o1, o2);
// 	// Cancel collisions between distance sensors and other objects.
// 	if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass) ) return;

// 	OBJECT *d1 = (OBJECT *)dGeomGetData(o1);
// 	OBJECT *d2 = (OBJECT *)dGeomGetData(o2);

// 	if ( d1 && d2 ){
// 		if (dAreConnected (d1->Get_Body(),d2->Get_Body())) return; //no collision between joint connected bodies
// 		int d1Group = d1->Get_Group();
// 		int d2Group = d2->Get_Group();
// 		if(!data->collisionMatrix[d1Group][d2Group]) return; //no collision between groups where matrix[i][j]=0

// 		d1->Process_Adhesive_Touch(world, d2);
// 		d2->Process_Adhesive_Touch(world, d1);
// 	}

// 	// std::cerr << "Collision Occurs" << std::endl;
// 	if ( d1 ){
// 		d1->Touch_Sensor_Fires(timer);
// 		// std::cerr << "one :" << d1->Get_ID() << std::endl;
// 	}
// 	if ( d2 ){
// 		d2->Touch_Sensor_Fires(timer);
// 		// std::cerr << "two :" << d2->Get_ID() << std::endl;
// 	}

// 	const int N = 10;
// 	dContact contact[N];
// 	n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
// 	if (n > 0) {
// 		for (i=0; i<n; i++) {

// 			contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactApprox1;

// 			contact[i].surface.mu = dInfinity;
// 			contact[i].surface.slip1 = 0.01;
// 			contact[i].surface.slip2 = 0.01;

// 			dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
// 			dJointAttach (c,
// 			              dGeomGetBody(contact[i].geom.g1),
// 			              dGeomGetBody(contact[i].geom.g2));
//         }
//     }
// }


// static void captureFrame(int num) {

// 	char s[200];

// 	//printf("capturing frame %04d\n",num);
// 	sprintf (s,"frame/%04d.ppm",num);

// 	FILE *f = fopen (s,"wb");
// 	fprintf (f,"P6\n%d %d\n255\n",data->windowWidth,data->windowHeight);

// 	void *buf = malloc( data->windowWidth * data->windowHeight * 3 );
// 	glReadPixels( 0, 0, data->windowWidth, data->windowHeight, GL_RGB, GL_UNSIGNED_BYTE, buf );

// 	for (int y=(data->windowHeight - 1); y>=0; y--) {
// 		for (int x=0; x<data->windowWidth; x++) {
// 			unsigned char *pixel = ((unsigned char *)buf)+((y*data->windowWidth+ x)*3);
// 			unsigned char b[3];
// 			b[0] = *pixel;
// 			b[1] = *(pixel+1);
// 			b[2] = *(pixel+2);
// 			fwrite(b,3,1,f);
// 		}
// 	}
// 	free(buf);
// 	fclose(f);

// }


// // start simulation - set viewpoint

// static void start()
// {
//   dAllocateODEDataForThread(dAllocateMaskAll);
// }


// // called when a key pressed

// static void command (int cmd)
// {
//   switch (cmd) {
//       case '1': {
//           FILE *f = fopen ("state.dif","wt");
//           if (f) {
//             dWorldExportDIF (world,f,"");
//             fclose (f);
//         }
//     }
// }
// }

// // simulation loop

// void Simulate_For_One_Time_Step(void) {

// 	dSpaceCollide (space,0,&nearCallback);

// 	environment->Poll_Sensors(timer);
// 	environment->Update_Neural_Network(timer);
// 	environment->Actuate_Actuators();
// 	environment->Update_Forces(timer);

// 	dWorldStep (world, data->dt);

// 	dJointGroupEmpty(contactgroup);

// 	timer++;

// 	if ( timer==data->evaluationTime )
// 		Terminate();
// }

// static void simLoop (int pause)
// {
//     if (!initialized){
//         dsSetViewpoint (data->xyz,data->hpr);
//         if(data->followBody>=0){
//           environment->Get_Object_Position(updated_xyz, data->followBody);
//           for(int i=0;i<LAGSIZE;i++) average_z[i] = updated_xyz[2];
//       }
//     initialized = true;
//     }

//     // accumulator += dsElapsedTime();
//     // if (accumulator>10.0*baseDT)
//     // {
//     //   accumulator = 10.0*baseDT;
//     // }
//     // accumulator to maintain frame rate
//     // while(accumulator>baseDT)
//     //{
//       // accumulator -= data->dt;
//       if ( !pause ){
//           Simulate_For_One_Time_Step();

//           if (data->followBody>=0)
//           {
//             environment->Get_Object_Position(updated_xyz, data->followBody);

//             average_z[timer%LAGSIZE] = updated_xyz[2];

//             updated_xyz[0] += data->xyz[0];
//             updated_xyz[1] += data->xyz[1];
//             updated_xyz[2] += data->xyz[2];

//             //for(int i=0;i<LAGSIZE;i++) updated_xyz[2] +=  average_z[i]/float(LAGSIZE); //lag movement

//                 dsSetViewpoint(updated_xyz,data->hpr);
//           }

//         if (data->trackBody>=0)
//          {
//             float dirVector[3];
//             environment->Get_Object_Position(dirVector, data->trackBody);

//             for(int i=0;i<3;i++)
//               dirVector[i] -= data->xyz[i];

//           if (!(dirVector[0]==0 and dirVector[1]==0 and dirVector[2]==0)){
//             float zDrop = dirVector[2];
//             float magnitude = sqrt(pow(dirVector[0],2)+ pow(dirVector[1],2)+pow(dirVector[2],2));
//             for(int i=0;i<3;i++) dirVector[i] = dirVector[i]/magnitude;

//               float heading = -atan2(dirVector[0],dirVector[1]) * 180.0 / 3.14159+90.;
//               float pitch = asin(zDrop/magnitude) * 180.0 / 3.14159;
//               float update_hpr[3];

//               update_hpr[0] = heading;
//               update_hpr[1] = pitch;
//               update_hpr[2] = data->hpr[2];

//               dsSetViewpoint(data->xyz, update_hpr);
//               }
//           }
//       }

//     // }
//     environment->Draw(data->debug);

// 	if((!pause) && data->capture && (timer % data->capture == 0))
// 		captureFrame(timer / data->capture);
// }

// void Initialize_ODE(void) {

//     dInitODE2(0);
//     world = dWorldCreate();
//     space = dHashSpaceCreate (0);
//     contactgroup = dJointGroupCreate (0);
//     ground = dCreatePlane (space,0,0,1,0);

//     dGeomSetData(ground,NULL);

//     timer = 0;
// }

// void Initialize_Draw_Stuff(void){
//     // setup pointers to drawstuff callback functions
//     fn.version = DS_VERSION;
//     fn.start = &start;
//     fn.step = &simLoop;
//     fn.command = &command;
//     fn.stop = 0;
//     fn.path_to_textures = data->texturePathStr;
// }

// void Initialize_Environment(void) {
//     environment = new ENVIRONMENT();
//     data->followBody = -1;
//     data->trackBody = -1;
// }


// void Read_From_Python(void) {
// 	//environment->Read_From_Python(world,space, texturePathStr, &evaluationTime,&dt,&gravity,xyz,hpr,&debug,&followBody,&trackBody);
// 	environment->Read_From_Python(world, space, data);
// }

// void Terminate(void) {
//     environment->Write_Sensor_Data(data->evaluationTime);
//     delete data;
//     exit(0);
// }

// void Run_Blind(void) {

//     while ( 1 )

//         Simulate_For_One_Time_Step();
// }

// int main (int argc, char **argv)
// {
//     data->runBlind = false;

//     if ( (argc > 1) && (strcmp(argv[1],"-blind")==0) )
//         data->runBlind = true;


//     Initialize_ODE();
//     Initialize_Environment();
//     Read_From_Python();
//     dWorldSetGravity(world,0,0,data->gravity);

//     if ( data->runBlind )
//         Run_Blind();
//     else{
//       Initialize_Draw_Stuff();

//       dsSimulationLoop (argc,argv,data->windowWidth,data->windowHeight,&fn);
//   }
//   return 0;
// }
