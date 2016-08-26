#include "iostream"
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include "environment.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

#include <drawstuff/drawstuff.h>
#include "texturepath.h"
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
int evaluationTime = 100;
int runBlind;
ENVIRONMENT *environment;
int numberOfBodies = 0;
static dGeomID ground;

void Draw_Distance_Sensor(dGeomID myGeom, dGeomID hisGeom);

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

                	if ( runBlind == false )

				obj->Draw_Ray_Sensor(contact.geom.pos[0],contact.geom.pos[1],contact.geom.pos[2],timer);
		}
        }
}

void Handle_Ray_Sensors(dGeomID o1, dGeomID o2) {

	Handle_Ray_Sensor(o1,o2);

	Handle_Ray_Sensor(o2,o1);
}

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
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

  static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
  static float hpr[3] = {121.0000f,-27.5000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
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

        dWorldStep (world,0.05);

        dJointGroupEmpty(contactgroup);

        timer++;

        if ( timer==evaluationTime )

        	Terminate();
}

static void simLoop (int pause)
{
	if ( !pause )

		Simulate_For_One_Time_Step();

	environment->Draw();
}

void Initialize_ODE(void) {

	// setup pointers to drawstuff callback functions
  	fn.version = DS_VERSION;
  	fn.start = &start;
  	fn.step = &simLoop;
  	fn.command = &command;
  	fn.stop = 0;
  	fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

 	dInitODE2(0);
  	world = dWorldCreate();
  	space = dHashSpaceCreate (0);
  	contactgroup = dJointGroupCreate (0);
  	dWorldSetGravity (world,0,0,-0.5);
  	ground = dCreatePlane (space,0,0,1,0);
 
	dGeomSetData(ground,NULL); 

	timer = 0;
}

void Initialize_Environment(void) {

        environment = new ENVIRONMENT();
}

void Read_From_Python(void) {

	environment->Read_From_Python(world,space,&evaluationTime);
}

void Terminate(void) {

	environment->Write_Sensor_Data(evaluationTime);

	exit(0);
}

void Run_Blind(void) {

	while ( 1 )

		Simulate_For_One_Time_Step();
}

int main (int argc, char **argv)
{

	runBlind = false; 

	if ( (argc > 1) && (strcmp(argv[1],"-blind")==0) )

		runBlind = true;

        Initialize_ODE();

        Initialize_Environment();

        Read_From_Python();

	if ( runBlind )

		Run_Blind();
	else
  		dsSimulationLoop (argc,argv,352*3,288*3,&fn);

  return 0;
}
