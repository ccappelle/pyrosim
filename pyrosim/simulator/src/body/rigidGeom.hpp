#ifndef _RIGID_GEOM_HPP
#define _RIGID_GEOM_HPP

#pragma once

#include "entity.hpp"
#include "geomData.hpp"

class RigidGeom : public Entity{
public:
    dReal position[3]; // global position
    dReal orientation[3]; // global orientation
    dGeomID geom;
    dReal density;
    dReal color[3];
    std::string spaceName;
    std::string geomType;

public:
    virtual void create(Environment *environment) =0;
    virtual dMass calculateMass(void) =0;
    virtual void draw(void) =0;
    virtual void writeToPython(void) =0;
    virtual void readDimensionsFromPython(void) = 0; // unique to body
    virtual void takeStep(int timeStep, dReal dt) {};

    dGeomID getGeom(){return this->geom;};

    void readColorFromPython(void){ readValueFromPython<dReal>(this->color, 3, "Color");}
    void readDensityFromPython(void){readValueFromPython<dReal>(&this->density, "Density");}
    void readOrientationFromPython(void){readValueFromPython<dReal>(this->orientation, 3, "Orientation");}
    void readPositionFromPython(void){readValueFromPython<dReal>(this->position, 3, "Position");}
    
    virtual void readFromPython(void){
        std::cerr << "Reading In " << geomType << " From Python " << std::endl;
        this->readPositionFromPython();
        this->readOrientationFromPython();
        this->readDimensionsFromPython();
        this->readDensityFromPython();
        this->readColorFromPython();
        std::cerr << "Completed Reading In " << geomType << std::endl;
    }

    void setBody(dBodyID body){ dGeomSetBody(this->geom, body); }
    void setSpaceName(std::string name){this->spaceName = name;};

    void setData(int entityID){
        GeomData* geomData = new GeomData();
        geomData->entityID = entityID;
        geomData->color[0] = this->color[0];
        geomData->color[1] = this->color[1];
        geomData->color[2] = this->color[2];
        dGeomSetData(this->geom, static_cast<void*>(geomData));
    }

    void resetGeom(void){
        dGeomSetPosition(this->geom,
                         this->position[0],
                         this->position[1],
                         this->position[2]);
        dMatrix3 R;
        dRFromZAxis(R, this->orientation[0], this->orientation[1], this->orientation[2]);
        dGeomSetRotation(this->geom, R);
    }

    void resetGeomUsingOffset(void){
        dGeomSetOffsetWorldPosition(this->geom,
                                    this->position[0],
                                    this->position[1],
                                    this->position[2]);
        dMatrix3 R;
        dRFromZAxis(R, this->orientation[0], this->orientation[1], this->orientation[2]);
        dGeomSetOffsetWorldRotation(this->geom, R);
    }
};

// should probably move these to a different file
class BoxGeom : public RigidGeom{
protected:
    dReal sides[3];
public:
    BoxGeom(){this->geomType = "Box";};

    void create(Environment *environment){
        std::cerr << "  Creating Box Geom" << std::endl;
        dSpaceID space = environment->getSpace(this->spaceName);
        this->geom = dCreateBox(space, this->sides[0], this->sides[1], this->sides[2]);
    }

    dMass calculateMass(void){
        dMass mass;
        dMassSetBox(&mass, this->density, this->sides[0], this->sides[1], this->sides[2]);
        return mass;
    }

    void draw(void){
        dsSetColor(this->color[0], this->color[1], this->color[2]);
        const dReal *pos = dGeomGetPosition(this->geom);
        const dReal *rot = dGeomGetRotation(this->geom);

        dsDrawBox(pos, rot, this->sides);
    }

    void readDimensionsFromPython(void){ readValueFromPython<dReal>(sides, 3);}

    void writeToPython(void){
        std::cerr << "  Geom Box   :" << std::endl
                  << "      Position  : " << this->position[0] << ", " << this->position[1] << ", " << this->position[2] << std::endl
                  << "      Sides     : " << this->sides[0] << ", " << this->sides[1] << ", " << this->sides[2] << std::endl
                  << "      Density   : " << this->density << std::endl
                  << "      Color     : " << this->color[0] << ", " << this->color[1] << ", " << this->color[2] << std::endl;
    }
};

class CylinderGeom : public RigidGeom{
protected:
    dReal radius, length;
    int capped;
public:
    CylinderGeom(){this->geomType = "Cylinder";}

    void create(Environment *environment){
        std::cerr << "  Creating Cylinder Geom" << std::endl;
        dSpaceID space = environment->getSpace(this->spaceName);
        if (this->capped)
            this->geom = dCreateCapsule(space, this->radius, this->length);
        else
            this->geom = dCreateCylinder(space, this->radius, this->length);
    }

    dMass calculateMass(void){
        dMass mass;
        if (this->capped)
            dMassSetCapsule(&mass, this->density, 3, this->radius, this->length);
        else
            dMassSetCylinder(&mass, this->density, 3, this->radius, this->length);
        return mass;
    }

    void draw(void){
        dsSetColor(this->color[0], this->color[1], this->color[2]);
        const dReal *pos = dGeomGetPosition(this->geom);
        const dReal *rot = dGeomGetRotation(this->geom);

        if (this->capped)
            dsDrawCapsule(pos, rot, this->length, this->radius);
        else
            dsDrawCylinder(pos, rot, this->length, this->radius);
    }

    void readDimensionsFromPython(void){
        readValueFromPython<dReal>(&this->length, "Length");
        readValueFromPython<dReal>(&this->radius, "Radius");
        readValueFromPython<int>(&this->capped,   "Capped");
    }


    void writeToPython(void){
        std::cerr << "  Geom Cylinder:" << std::endl
                  << "      Position  : " << this->position[0] << ", " << this->position[1] << ", " << this->position[2] << std::endl
                  << "      Length    : " << this->length << std::endl
                  << "      Radius    : " << this->radius << std::endl
                  << "      Capped    : " << this->capped << std::endl
                  << "      Density   : " << this->density << std::endl
                  << "      Color     : " << this->color[0] << ", " << this->color[1] << ", " << this->color[2] << std::endl;
    }
};

class SphereGeom : public RigidGeom{
    dReal radius;
public:
    SphereGeom(){this->geomType="Sphere";}

    void create(Environment *environment){
        std::cerr << "Creating Sphere Geom" << std::endl;

        dSpaceID space = environment->getSpace(this->spaceName);
        this->geom = dCreateSphere(space, this->radius);
    }

    dMass calculateMass(void){
        dMass mass;
        dMassSetSphere(&mass, this->density, this->radius);
        return mass;
    }

    void draw(void){
        dsSetColor(this->color[0], this->color[1], this->color[2]);
        const dReal *pos = dGeomGetPosition(this->geom);
        const dReal *rot = dGeomGetRotation(this->geom);

        dsDrawSphere(pos, rot, this->radius);
    }

    void readDimensionsFromPython(){
        readValueFromPython<dReal>(&this->radius);
    }

    void writeToPython(void){
        std::cerr << "  Geom Sphere:" << std::endl
                  << "      Position  : " << this->position[0] << ", " << this->position[1] << ", " << this->position[2] << std::endl
                  << "      Radius    : " << this->radius << std::endl
                  << "      Density   : " << this->density << std::endl
                  << "      Color     : " << this->color[0] << ", " << this->color[1] << ", " << this->color[2] << std::endl;
    }
};
#endif
