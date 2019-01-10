#ifndef _RIGID_BODY_HPP
#define _RIGID_BODY_HPP

#include <string>

#include <ode/ode.h>
#include <map>
#include <vector>

#include "entity.hpp"
#include "rigidGeom.hpp"
#include "geomData.hpp"

class RigidBody : public Entity {
public:
    dBodyID body; // literal body object
    int isSeen;
    std::vector<RigidGeom*> geoms; // our class of geoms

    std::string spaceName;
    std::string collisionGroupName;
    std::map<int, std::vector<float>> impulses;
    std::string componentName = "Body";
    RigidBody(){};

    virtual void draw(void){
        // draw center of mass as black sphere
        // const dReal *pos = dBodyGetPosition(this->body);
        // dMatrix3 R;
        // dRSetIdentity(R);

        // // dont draw if only one geom attached
        // if (this->geoms.size() != 1){
        //     dsSetColorAlpha(0.0, 0.0, 0.0, 0.15);
        //     dsDrawSphere(pos, R, 0.1);
        // }

        for (auto geom : this->geoms){
            geom->draw();
        }
    }

    int getIsSeen( void ){
        return isSeen;
    }

    void setIsSeen( int s ){
        this->isSeen = s;
    }

    const dReal* getPosition(void){
        return dBodyGetPosition(this->body);
    }

    const dReal* getQuaternion(void){
        return dBodyGetQuaternion(this->body);
    }

    void writeToPython(void){
        std::cerr << "Rigid Body:" << std::endl
                  << "  Space     : " << this->spaceName << std::endl
                  << "  Collision : " << this->collisionGroupName << std::endl;

        for (auto geom : this->geoms){
            geom->writeToPython();
        }
    }

    virtual void create(Environment *environment){
        std::cerr << "Creating Rigid Body" << std::endl;
        this->body = dBodyCreate(environment->getWorld());

        // if only one geom attached, create using non-offset
        // otherwise offset is necessary
        if (this->geoms.size() == 1){
            dMass m;
            this->geoms[0]->setSpaceName(this->spaceName);
            this->geoms[0]->create(environment);

            // set paramaters after create
            // dGeomSetData(this->geoms[0]->getGeom(), static_cast<void*>(&this->collisionGroupName));
            // dGeomSetData(this->geoms[0]->getGeom(), static_cast<void*>(&this->entityID));
            // create geom data structure
            this->geoms[0]->setData(this->entityID);

            this->geoms[0]->setBody(this->body);
            this->geoms[0]->resetGeom();
            m = this->geoms[0]->calculateMass();
            dBodySetMass(this->body, &m);
        }
        else if (this->geoms.size() > 1){
            dMass globalMass;
            for (auto geom : this->geoms){
                // dMass localMass;
                geom->setSpaceName(this->spaceName);
                geom->create(environment);

                // set parameters after create
                // dGeomSetData(geom->getGeom(), static_cast<void*>(&this->collisionGroupName));
                // dGeomSetData(this->geoms[0]->getGeom(), static_cast<void*>(&this->entityID));
                geom->setData(this->entityID);

                geom->setBody(this->body);
                geom->resetGeomUsingOffset();

                dMass localMass = geom->calculateMass();
                dMassTranslate(&localMass, geom->position[0], geom->position[1], geom->position[2]);
                dMassAdd(&globalMass, &localMass);
            }

            // readjust masses / body position
            // set body position to center of mass
            const dReal *pos1 = dBodyGetPosition(this->body);
            dBodySetPosition(this->body, 
                             globalMass.c[0],
                             globalMass.c[1],
                             globalMass.c[2]);
            const dReal *pos2 = dBodyGetPosition(this->body);
            // translate mass so it is no long off center
            dMassTranslate(&globalMass, -globalMass.c[0], -globalMass.c[1], -globalMass.c[2]);
            dBodySetMass(this->body, &globalMass);

            // reset geoms to new center of mass
            for (auto geom : this->geoms){
                geom->resetGeomUsingOffset();
            }
        }
    };

    virtual void readFromPython(void){
        readCollisionInfoFromPython();
    }

    void readAdditionFromPython(void){
        std::string addition;
        readStringFromPython(addition, "Adding");
        if (addition == "Geom")
            readGeomFromPython();
        else if (addition == "Impulse")
            readImpulseFromPython();
    }

    void readImpulseFromPython(void){
        // read in time
        int time;
        readValueFromPython<int>(&time, "Time");
        // read direction
        float force[3];
        readValueFromPython<float>(force, 3, "Direction");
        if (this->impulses.count(time) == 0){
            this->impulses[time].push_back(force[0]);
            this->impulses[time].push_back(force[1]);
            this->impulses[time].push_back(force[2]);
        }
        else{
            this->impulses[time][0] += force[0];
            this->impulses[time][1] += force[1];
            this->impulses[time][2] += force[2];
        }
    }

    void readGeomFromPython(void){
        std::string geomName;
        readStringFromPython(geomName, "Geom");
        readNamedGeomFromPython(geomName);
    }


    void readNamedGeomFromPython(std::string geomName){
        // C.C. could probably also put this in a map but
        // geoms are finite so I don't see the need
        RigidGeom *geom;
        if (geomName == "Box"){
            geom = new BoxGeom();
        }
        else if (geomName == "Cylinder"){
            geom = new CylinderGeom();
        }
        else if (geomName == "Sphere"){
            geom = new SphereGeom();
        }
        else{
            std::cerr << "Geom Name " << geomName << " not implemented" << std::endl;
            exit(0);
        }
        geom->readFromPython();
        this->geoms.push_back(geom);
    }

    dBodyID getBody(void){
        return this->body;
    }

    std::string getCollisionGroupName(void){
        return this->collisionGroupName;
    }

    // dReal* getColor(void){
    //     return this->color
    // }
    void takeStep(int timeStep, dReal dt){
        // apply impulse
        if (impulses.count(timeStep) > 0){
            dBodyAddForce(this->body,
                          impulses[timeStep][0] / dt,
                          impulses[timeStep][1] / dt,
                          impulses[timeStep][2] / dt);
        }
    }

    virtual EntityType getEntityType(void){
        return BODY;
    }
protected:
    void readCollisionInfoFromPython(void){
        readStringFromPython(this->spaceName, "Space");
        readStringFromPython(this->collisionGroupName, "Collision");
    }
};

class BoxBody: public RigidBody{
public:
    BoxBody(){};

    void readFromPython(){
        // create box geom and read it in
        this->readNamedGeomFromPython("Box");
        // read space and collision info
        this->readCollisionInfoFromPython();
    }
};

class CylinderBody: public RigidBody{
public:
    CylinderBody(){}

    void readFromPython(){
        this->readNamedGeomFromPython("Cylinder");
        this->readCollisionInfoFromPython();
    }
};

class SphereBody: public RigidBody{
public:
    SphereBody(){}

    void readFromPython(){
        this->readNamedGeomFromPython("Sphere");
        this->readCollisionInfoFromPython();
    }
};
#endif