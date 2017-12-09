#include "iostream"

//ode headers
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

// glut 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
bool initialized = false;
static dGeomID ground;


Data *data = new Data;//struct which keeps all user input values for various parameterss. see datastruct.h
static float updated_xyz[3];
int LAGSIZE = 20;
static float average_z[20];

const float FPS = 300.0;
const float baseDT = 1.0/FPS;
float framestart;
float accumulator = 0.0;

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
            OBJECT *obj2 = (OBJECT *)dGeomGetData(o2);

            obj->Set_Ray_Sensor(contact.geom.depth,obj2,timer);

            if ( data->runBlind == false )
                obj->Draw_Ray_Sensor(contact.geom.pos[0],contact.geom.pos[1],contact.geom.pos[2],timer);

        }
    }
}

void Handle_Ray_Sensors(dGeomID o1, dGeomID o2) {

    Handle_Ray_Sensor(o1,o2);

    Handle_Ray_Sensor(o2,o1);
}

static void nearCallback (void *callbackData, dGeomID o1, dGeomID o2)
{
  int i,n;

  Handle_Ray_Sensors(o1,o2);
    // Cancel collisions between distance sensors and other objects.
  if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass) ) return;


  OBJECT *d1 = (OBJECT *)dGeomGetData(o1);

  OBJECT *d2 = (OBJECT *)dGeomGetData(o2);
  // if (d1 && d2)
  //   return;
  if ( d1 && d2 ){
        if (dAreConnected (d1->Get_Body(),d2->Get_Body())) return; //no collision between joint connected bodies
        int d1Group = d1->Get_Group();
        int d2Group = d2->Get_Group();
        if(!data->collisionMatrix[d1Group][d2Group]) return; //no collision between groups where matrix[i][j]=0
    }

    // std::cerr << "Collision Occurs" << std::endl;
    if ( d1 ){
        d1->Touch_Sensor_Fires(timer);
        // std::cerr << "one :" << d1->Get_ID() << std::endl;
    }
    if ( d2 ){
            d2->Touch_Sensor_Fires(timer);
        // std::cerr << "two :" << d2->Get_ID() << std::endl;
    }


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


static void captureFrame(int num) {


	char s[200];

	//printf("capturing frame %04d\n",num);
	sprintf (s,"frame/%04d.ppm",num);

	FILE *f = fopen (s,"wb");
	fprintf (f,"P6\n%d %d\n255\n",data->windowWidth,data->windowHeight);

	void *buf = malloc( data->windowWidth * data->windowHeight * 3 );
	glReadPixels( 0, 0, data->windowWidth, data->windowHeight, GL_RGB, GL_UNSIGNED_BYTE, buf );

	for (int y=(data->windowHeight - 1); y>=0; y--) {
		for (int x=0; x<data->windowWidth; x++) {
			unsigned char *pixel = ((unsigned char *)buf)+((y*data->windowWidth+ x)*3);
			unsigned char b[3];
			b[0] = *pixel;
			b[1] = *(pixel+1);
			b[2] = *(pixel+2);
			fwrite(b,3,1,f);
		}
	}
	free(buf);
	fclose(f);

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
  environment->Update_Forces(timer);

  dWorldStep (world, data->dt);

  dJointGroupEmpty(contactgroup);

  timer++;

  if ( timer==data->evaluationTime )
    Terminate();
}

static void simLoop (int pause)
{
    if (!initialized){
        dsSetViewpoint (data->xyz,data->hpr);
        if(data->followBody>=0){
          environment->Get_Object_Position(updated_xyz, data->followBody);
          for(int i=0;i<LAGSIZE;i++) average_z[i] = updated_xyz[2];
      }
    initialized = true;
    }

    // accumulator += dsElapsedTime();
    // if (accumulator>10.0*baseDT)
    // {
    //   accumulator = 10.0*baseDT;
    // }
    // accumulator to maintain frame rate
    // while(accumulator>baseDT)
    //{   
      // accumulator -= data->dt;
      if ( !pause ){
          Simulate_For_One_Time_Step();
 
          if (data->followBody>=0)
          {
            environment->Get_Object_Position(updated_xyz, data->followBody);

            average_z[timer%LAGSIZE] = updated_xyz[2];

            updated_xyz[0] += data->xyz[0];
            updated_xyz[1] += data->xyz[1];
            updated_xyz[2] += data->xyz[2];

            //for(int i=0;i<LAGSIZE;i++) updated_xyz[2] +=  average_z[i]/float(LAGSIZE); //lag movement

                dsSetViewpoint(updated_xyz,data->hpr);
          }

        if (data->trackBody>=0)
         {
            float dirVector[3];
            environment->Get_Object_Position(dirVector, data->trackBody);
            
            for(int i=0;i<3;i++)
              dirVector[i] -= data->xyz[i];

          if (!(dirVector[0]==0 and dirVector[1]==0 and dirVector[2]==0)){
            float zDrop = dirVector[2];
            float magnitude = sqrt(pow(dirVector[0],2)+ pow(dirVector[1],2)+pow(dirVector[2],2));
            for(int i=0;i<3;i++) dirVector[i] = dirVector[i]/magnitude;

              float heading = -atan2(dirVector[0],dirVector[1]) * 180.0 / 3.14159+90.;
              float pitch = asin(zDrop/magnitude) * 180.0 / 3.14159;
              float update_hpr[3];

              update_hpr[0] = heading;
              update_hpr[1] = pitch;
              update_hpr[2] = data->hpr[2];

              dsSetViewpoint(data->xyz, update_hpr);
              }
          }
      }

    // }
    environment->Draw(data->debug);
    
	if((!pause) && data->capture && (timer % data->capture == 0))
		captureFrame(timer / data->capture);
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
    fn.path_to_textures = data->texturePathStr;
    
}
void Initialize_Environment(void) {
    environment = new ENVIRONMENT();
    data->followBody = -1;
    data->trackBody = -1;
}


void Read_From_Python(void) {
    //environment->Read_From_Python(world,space, texturePathStr, &evaluationTime,&dt,&gravity,xyz,hpr,&debug,&followBody,&trackBody);
  environment->Read_From_Python(world,space,data);
}

void Terminate(void) {
    environment->Write_Sensor_Data(data->evaluationTime);
    delete data;
    exit(0);
}

void Run_Blind(void) {

    while ( 1 )

        Simulate_For_One_Time_Step();
}

int main (int argc, char **argv)
{
    data->runBlind = false; 

    if ( (argc > 1) && (strcmp(argv[1],"-blind")==0) )
        data->runBlind = true;


    Initialize_ODE();
    Initialize_Environment();
    Read_From_Python();
    dWorldSetGravity(world,0,0,data->gravity);

    if ( data->runBlind )
        Run_Blind();
    else{
      Initialize_Draw_Stuff();

      dsSimulationLoop (argc,argv,data->windowWidth,data->windowHeight,&fn);
  }
  return 0;
}
