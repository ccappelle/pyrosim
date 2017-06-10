#include "iostream"

//ode headers
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

//custom headers
#include "texturepath.h"
#include "environment.h"
#include "datastruct.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
dsFunctions fn;

int timer;

ENVIRONMENT *environment;
int numberOfBodies = 0;

static dGeomID ground;

Data data;//struct which keeps all user input values for various parameterss. see datastruct.h

void Draw_Distance_Sensor(dGeomID myGeom, dGeomID thisGeom);

void Read_From_Python(void);

void Set_Distance_Sensor(dGeomID o);

void Terminate(void);

void Handle_Ray_Sensor(dGeomID o1, dGeomID o2) {

	if ( dGeomGetClass(o1) == dRayClass ) {

        	dContact contact;

        	int n = dCollide(o1,o2,1,&contact.geom,sizeof(dContact));

        	if ( n>0 ) {

        		OBJECT *obj = (OBJECT *)dGeomGetData(o1);

                	obj->Set_Ray_Sensor(contact.geom.depth,(OBJECT*)dGeomGetData(o2),timer);

                	if ( data.runBlind == false )

				obj->Draw_Ray_Sensor(contact.geom.pos[0],contact.geom.pos[1],contact.geom.pos[2],timer);
		}
        }
}

void Handle_Ray_Sensors(dGeomID o1, dGeomID o2) {

	Handle_Ray_Sensor(o1,o2);

	Handle_Ray_Sensor(o2,o1);
}

static void nearCallback (void *callbakData, dGeomID o1, dGeomID o2)
{
  int i,n;

	Handle_Ray_Sensors(o1,o2);

	if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass) )
	
	// Cancel collisions between distance sensors and other objects.

		return;

        OBJECT *d1 = (OBJECT *)dGeomGetData(o1);

        OBJECT *d2 = (OBJECT *)dGeomGetData(o2);

	if ( d1 && d2 ) // Cancel collisions between objects. 

		return;

	if ( d1 )
		d1->Touch_Sensor_Fires(timer);

	if ( d2 )
		d2->Touch_Sensor_Fires(timer);
	
  // only collide things with the ground
  // int g1 = (o1 == ground );
  // int g2 = (o2 == ground );
  // if (!(g1 ^ g2)) return;

  const int N = 10;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {

	contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactApprox1;

	contact[i].surface.mu = dInfinity;
	contact[i].surface.slip1 = 0.01;
	contact[i].surface.slip2 = 0.01;

	dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
	dJointAttach (c,
		    dGeomGetBody(contact[i].geom.g1),
		    dGeomGetBody(contact[i].geom.g2));
    }
  }
}


// start simulation - set viewpoint

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);
}


// called when a key pressed

static void command (int cmd)
{
  switch (cmd) {
  case '1': {
      FILE *f = fopen ("state.dif","wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
    }
  }
}

// simulation loop

void Simulate_For_One_Time_Step(void) {

  dSpaceCollide (space,0,&nearCallback);

  environment->Poll_Sensors(timer);
  environment->Update_Neural_Network(timer);

  environment->Actuate_Joints();

  dWorldStep (world, data.dt);

  dJointGroupEmpty(contactgroup);

  timer++;

  if ( timer==data.evaluationTime )
        	Terminate();
}

static void simLoop (int pause)
{
  if (timer==0){
    dWorldSetGravity(world,0,0,data.gravity);
    dsSetViewpoint (data.xyz,data.hpr);
  }
  if (data.followBody>=0){
    float updated_xyz[3];
    environment->Get_Object_Position(updated_xyz, data.followBody);

    updated_xyz[0] += data.xyz[0];
    updated_xyz[1] += data.xyz[1];
    updated_xyz[2] = data.xyz[2]; //no movement in x direction

    dsSetViewpoint(updated_xyz,data.hpr);
  }
  if (data.trackBody>=0){
    float dirVector[3];
    environment->Get_Object_Position(dirVector, data.trackBody);

    dirVector[0] -= data.xyz[0];
    dirVector[1] -= data.xyz[1];
    dirVector[2] -= data.xyz[2];

    if (!(dirVector[0]==0 and dirVector[1]==0 and dirVector[2]==0)){
        float normalizer = sqrt(pow(dirVector[0],2)+ pow(dirVector[1],2)+pow(dirVector[2],2));
        dirVector[0] = dirVector[0]/normalizer;
        dirVector[1] = dirVector[1]/normalizer;
        dirVector[2] = dirVector[2]/normalizer;

        float heading = -atan2(dirVector[0],dirVector[1]) * 180.0 / 3.14159+90.;

        float update_hpr[3];

        update_hpr[0] = heading;
        update_hpr[1] = data.hpr[1];
        update_hpr[2] = data.hpr[2];

        dsSetViewpoint(data.xyz, update_hpr);
    }
  }
  


	if ( !pause )

		Simulate_For_One_Time_Step();

	environment->Draw(data.debug);

}

void Initialize_ODE(void) {

 	dInitODE2(0);
  	world = dWorldCreate();
  	space = dHashSpaceCreate (0);
  	contactgroup = dJointGroupCreate (0);
  	ground = dCreatePlane (space,0,0,1,0);
 
	dGeomSetData(ground,NULL); 

	timer = 0;
}

void Initialize_Draw_Stuff(void){
    // setup pointers to drawstuff callback functions
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.command = &command;
    fn.stop = 0;
    fn.path_to_textures = data.texturePathStr;
    
}
void Initialize_Environment(void) {
        environment = new ENVIRONMENT();
        data.followBody = -1;
        data.trackBody = -1;
}


void Read_From_Python(void) {
	//environment->Read_From_Python(world,space, texturePathStr, &evaluationTime,&dt,&gravity,xyz,hpr,&debug,&followBody,&trackBody);
  environment->Read_From_Python(world,space,&data);
}

void Terminate(void) {

	environment->Write_Sensor_Data(data.evaluationTime);
	exit(0);
}

void Run_Blind(void) {

	while ( 1 )

		Simulate_For_One_Time_Step();
}

int main (int argc, char **argv)
{
	data.runBlind = false; 

	if ( (argc > 1) && (strcmp(argv[1],"-blind")==0) )
		data.runBlind = true;

  Initialize_ODE();
  Initialize_Environment();
  Read_From_Python();

	if ( data.runBlind )
		Run_Blind();
	else{
      Initialize_Draw_Stuff();
  		dsSimulationLoop (argc,argv,352*2,288*2,&fn);
    }
  return 0;
}
