#ifndef __RAY_HPP__
#define __RAY_HPP__

#pragma once

#include "body/rigidBody.hpp"
#include "pythonReader.hpp"
#include "geomData.hpp"

class Ray : public Entity
{   
protected:
    int bodyID;
    RigidBody* body;
    dReal orientation[3];
    dReal position[3];
    dReal color[4];
    float maxLength;
    dGeomID ray;
    float distance;
    float cachedDistance;

public:
    Ray(){};

    void readFromPython(void){
        readValueFromPython(&this->bodyID, "Body ID");
        readValueFromPython(this->position, 3, "Ray Position");
        readValueFromPython(this->orientation, 3, "Ray Orientation");
        readValueFromPython(&this->maxLength, "Length");
    }

    void create(Environment *environment){
        // create ray and attach to body
        this->body = (RigidBody *) environment->getEntity(this->bodyID);
        this->ray = dCreateRay(environment->getSpace("default"), this->maxLength);


        // dGeomSetData(this->ray, static_cast<void*>(&this->entityID));
        GeomData* geomData = new GeomData();
        geomData->entityID = this->entityID;
        geomData->color[0] = 0.0;
        geomData->color[1] = 0.0;
        geomData->color[2] = 0.0;
        dGeomSetData(this->ray, static_cast<void*>(geomData));

        dGeomSetBody(this->ray, this->body->getBody());
        dMatrix3 R;
        dRFromZAxis(R,
                    this->orientation[0],
                    this->orientation[1],
                    this->orientation[2]);
        
        dGeomSetOffsetWorldPosition(this->ray,
                                    this->position[0],
                                    this->position[1],
                                    this->position[2]);
        dGeomSetOffsetWorldRotation(this->ray,R);
        dGeomRaySetParams(this->ray, true, true);
        this->distance = this->maxLength;
        this->cachedDistance = this->maxLength;
    }

    void collisionUpdate(float newDistance,
                         float r, float g, float b){
        // update distance value with closest collision
        if (newDistance < this->distance){
            this->distance = newDistance;

            this->color[0] = r;
            this->color[1] = g;
            this->color[2] = b;
            this->color[3] = 1.0;
        }
    }

    void takeStep(int timeStep, dReal dt){
        // reset
        this->distance = this->maxLength;
        this->color[0] = 0.0;
        this->color[1] = 0.0;
        this->color[2] = 0.0;
        this->color[3] = 0.5;
    }

    float getDistance(void){
        return this->distance;
    }

    float getColorComponent(int index){
        return this->color[index];
    }

    void draw(){
        dVector3 start, dir;
        dGeomRayGet(this->ray, start, dir);

        const dReal point1[3] = {start[0], start[1], start[2]};
        const dReal point2[3] = {start[0] + dir[0] * this->distance,
                                 start[1] + dir[1] * this->distance,
                                 start[2] + dir[2] * this->distance};

        dsSetColorAlpha(this->color[0], this->color[1], this->color[2], this->color[3]);
        dsDrawLine(point1, point2);
    };

    virtual EntityType getEntityType(void){
        return BODY;
    }
};

#endif