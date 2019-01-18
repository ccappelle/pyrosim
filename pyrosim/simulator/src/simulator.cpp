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
#include "body/ray.hpp"
#include "geomData.hpp"

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

std::string COLLIDE_ALWAYS_STR = "Collide";
int COLLIDE_ALWAYS = -1;

void readCollisionFromPython(void);
static void command(void);
void createEnvironment(void);
static void drawLoop(int pause);
void endSimulation();
void handleRayCollision();
static void nearCallback(void *callbackData, dGeomID o1, dGeomID o2);
void readFromPython(void);
void initializeDrawStuff(void);
void initializeEnvironment(void);
void initializeODE(void);
void initializeParameters(void);
static void start(void);
void simulationStep(void);

int main(int argc, char **argv){
    // initialize everything and run
    playBlind = false;

    // read in input arguments searching for
    // headless running
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

    simulationStep();
    if (playBlind){
        while(1){
            simulationStep();
        }
    }
    else{
        initializeDrawStuff();
        // can't set camera here :(
        dsSimulationLoop(argc, argv, 900, 700, &fn);
    }
}

void readCollisionFromPython(void){
    // creates entry into map to specify collision
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
    // dGeomSetData(plane, static_cast<void*>(&COLLIDE_ALWAYS));
    GeomData* planeData = new GeomData();
    planeData->entityID = -1;
    planeData->color[0] = 0.0;
    planeData->color[1] = 0.0;
    planeData->color[2] = 0.0;
    dGeomSetData(plane, static_cast<void*>(planeData));
    // GeomData planeData = (GeomData) {-1, 0.0, 0.0, 0.0};
    // dGeomSetData(plane, static_cast<void*>(&planeData));
    // create bodies, joints, ANN, etc
    environment->createInODE();

    std::cerr << "Completed Creation" << std::endl;
}

static void drawLoop(int pause){
    if (firstStep){
        // float xyz[] = {parameters["CameraX"], parameters["CameraY"], parameters["CameraZ"]};
        // float hpr[] = {parameters["CameraH"], parameters["CameraP"], parameters["CameraR"]};
        // dsSetViewpoint(xyz, hpr);
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


        // draws pause lines - may change this in the future
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
    // write out total time steps to cout for sensory collection
    std::cout << evalStep;
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
    parameters["CameraY"] = -5.0;
    parameters["CameraZ"] = 2.0;

    parameters["CameraH"] = 90.0f;
    parameters["CameraP"] = -10.0f;
    parameters["CameraR"] = 0.0f;

    parameters["GravityX"] = 0.0f;
    parameters["GravityY"] = 0.0f;
    parameters["GravityZ"] = -9.8f;

    parameters["nContacts"] = 10;

    parameters["Friction"] = dInfinity;
}

void simulationStep(void){
    // // place action before collision detection?
    environment->takeStep(evalStep, parameters["DT"]);
    environment->emptyCollisionPairs();
    dSpaceCollide(topspace, 0, &nearCallback); // run collision
    dWorldStep(world, parameters["DT"]); // take time step
    dJointGroupEmpty(contactgroup);
    // // empty collision pairs
    
    // place action before collision detection?
    // dSpaceCollide(topspace, 0, &nearCallback); // run collision
    // environment->takeStep(evalStep, parameters["DT"]);

    // dWorldStep(world, parameters["DT"]); // take time step
    // environment->emptyCollisionPairs();
    // dJointGroupEmpty(contactgroup);
    // empty collision pairs

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
  // set camera here?
    float xyz[] = {parameters["CameraX"], parameters["CameraY"], parameters["CameraZ"]};
    float hpr[] = {parameters["CameraH"], parameters["CameraP"], parameters["CameraR"]};
    dsSetViewpoint(xyz, hpr);
}

void handleRayCollision(dGeomID ray, dGeomID o2){
    // handles ray collisions

    dContact contact;
    int n = dCollide(ray, o2, 1, &contact.geom, sizeof(dContact));

    std::cerr << "N contacts: " << n << std::endl;
    if (n > 0){
        std::cerr << "Handling Ray Collision" << std::endl;
        GeomData* rayData = static_cast<GeomData*>(dGeomGetData(ray));

        Ray *rayObj = (Ray*) environment->getEntity(rayData->entityID);
        dReal color[3];

        GeomData* bodyData = static_cast<GeomData*>(dGeomGetData(o2));

        // if ray 'sees' a body, set the sensor to true
        if ( bodyData->entityID > 0 ){
            RigidBody *body = (RigidBody *) environment->getEntity( bodyData->entityID );
            body->setIsSeen( true );
        }

        rayObj->collisionUpdate(contact.geom.depth,
                                bodyData->color[0],
                                bodyData->color[1],
                                bodyData->color[2]);
    }
}

void nearCallback(void *callbackData, dGeomID o1, dGeomID o2){
    // runs collision response between 'near' pair of geom o1 and o2
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
    if (dGeomGetClass(o1) == dRayClass){
         handleRayCollision(o1, o2);
         return;
    }
    else if (dGeomGetClass(o2) == dRayClass){
         handleRayCollision(o2, o1);
         return;
    }

    GeomData* g1 = static_cast<GeomData*> (dGeomGetData(o1));
    GeomData* g2 = static_cast<GeomData*> (dGeomGetData(o2));

    if (g1->entityID == -1 or g2->entityID == -1){
        // an entity ID of -1 indicates always collide
        // contitne
    }
    else{
        RigidBody *body1 = (RigidBody *) environment->getEntity(g1->entityID);
        RigidBody *body2 = (RigidBody *) environment->getEntity(g2->entityID);

        if (dAreConnected(body1->getBody(), body2->getBody())){ // exit early if connected
            return;
        }

        std::string group1 = body1->getCollisionGroupName();
        std::string group2 = body2->getCollisionGroupName();
        if (group1 != COLLIDE_ALWAYS_STR and group2 != COLLIDE_ALWAYS_STR){
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
            if ( parameters["Friction"] < 0.0 ){
                contact[i].surface.mu = dInfinity;
            } else {
                contact[i].surface.mu = parameters["Friction"];
            }
            // contact[i].surface.mu = dInfinity;
            // contact[i].surface.mu = 0.0;
            contact[i].surface.slip1 = 0.01;
            contact[i].surface.slip2 = 0.01;

            dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
            dJointAttach(c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
            // create collision pair and append to environment.collisions
            environment->addCollisionPair(g1->entityID, g2->entityID);
        }
    }
}

void readParameterFromPython(void){
    // reads in parameter from python string
    // and sets it as in the global map parameters
    std::string paramName;
    readStringFromPython(paramName);
    readValueFromPython<float>(&parameters[paramName]);
    std::cerr << paramName << " set to "
              << parameters[paramName]
              << std::endl << std::endl;
              // double space for clarity
}

void readFromPython(void){
    // main reading loop
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
        // else if(incomingString == "Actuator"){
        //     environment->readActuatorFromPython();
        // }
        // else if(incomingString == "Sensor"){
        //     enviornment->readSensorFromPython();
        // }
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