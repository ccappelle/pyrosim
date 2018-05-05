#ifndef _HINGE_JOINT_HPP
#define _HINGE_JOINT_HPP

#pragma once

#include "entity.hpp"
#include "pythonReader.hpp"

class Joint : public Entity{
protected:
    int body1ID, body2ID; // entity IDs
    dReal lowStop, highStop;
    dBodyID body1, body2; // ode bodies
    dJointID joint;

    virtual void create(Environment *environment) =0;
    virtual void readJointParamsFromPython() =0;

    void readFromPython(void){
        this->readBodiesFromPython();
        this->readJointParamsFromPython();
        this->readStopsFromPython();
    }

    void readBodiesFromPython(void){
        readValueFromPython<int>(&this->body1ID, "Body 1");
        readValueFromPython<int>(&this->body2ID, "Body 2");
    }

    void readStopsFromPython(void){
        readValueFromPython<dReal>(&this->lowStop, "Low Stop");
        readValueFromPython<dReal>(&this->highStop, "High Stop");
    }

    void setBodies(Environment *environment){
        // set bodies
        if (this->body1ID >= 0){
            RigidBody *rigidBody = (RigidBody *) environment->getEntity(this->body1ID);
            this->body1 = rigidBody->getBody();
        }
        else{
            this->body1 = (dBodyID) 0;
        }

        if (this->body2ID >= 0){
            RigidBody *rigidBody = (RigidBody *) environment->getEntity(this->body2ID);
            this->body2 = rigidBody->getBody();
        }
        else{
            this->body2 = (dBodyID) 0;
        }

    }
};


class Hinge : public Joint{
protected:
    dReal anchor[3];
    dReal axis[3];
    
public:
    Hinge(){this->drawName = "Joint";}

    void readJointParamsFromPython(void){
        readValueFromPython<dReal>(this->anchor, 3,"Hinge Anchor");
        readValueFromPython<dReal>(this->axis, 3,"Hinge Axis");
    }

    void create(Environment *environment){
        this->setBodies(environment);
        this->joint = dJointCreateHinge(environment->getWorld(), 0);
        
        dJointAttach(this->joint, this->body1, this->body2);
        // sets anchor
        dJointSetHingeAnchor(this->joint,
                             this->anchor[0],
                             this->anchor[1],
                             this->anchor[2]);

        // sets axis
        dJointSetHingeAxis(this->joint,
                           this->axis[0],
                           this->axis[1],
                           this->axis[2]);

        dJointSetHingeParam(this->joint, dParamLoStop, this->lowStop);
        dJointSetHingeParam(this->joint, dParamHiStop, this->highStop);
    }

    void draw(){

        // temporary draw function
        dVector3 pos;
        dVector3 rot;

        dJointGetHingeAnchor(this->joint, pos);
        dJointGetHingeAxis(this->joint, rot);

        dMatrix3 R;
        dRFromZAxis(R, rot[0], rot[1], rot[2]);


        if(this->highStop == this->lowStop){
            dsSetColorAlpha(0.1, 0.1, 0.1, 0.5);          
        }
        else{
            dsSetColorAlpha(1, 0.2, 0.2, 0.5);
        }
        dsDrawCylinder(pos, R, 0.3, 0.03);
    }

};


class Slider : public Joint{
protected:
    dReal anchor[3];
    dReal axis[3];
    
public:
    Slider(){this->drawName = "Joint";}

    void readJointParamsFromPython(void){
        readValueFromPython<dReal>(this->axis, 3,"Hinge Axis");
    }

    void create(Environment *environment){
        this->setBodies(environment);
        this->joint = dJointCreateSlider(environment->getWorld(), 0);
        
        dJointAttach(this->joint, this->body1, this->body2);

        // sets axis
        dJointSetSliderAxis(this->joint,
                            this->axis[0],
                            this->axis[1],
                            this->axis[2]);

        dJointSetSliderParam(this->joint, dParamLoStop, this->lowStop);
        dJointSetSliderParam(this->joint, dParamHiStop, this->highStop);
    }

    void draw(){
        dVector3 center = {0, 0, 0};

        dVector3 direction;
        dJointGetSliderAxis(this->joint, direction);

        dReal offset = dJointGetSliderPosition(this->joint);

        dsSetColorAlpha(0.3, 0.8, 0.3, 0.75);

        int one = false;
        if (this->body1ID >= 0){ // not the world
            const dReal *bodyCenter = dBodyGetPosition(this->body1);
            dAddVectors3(center, bodyCenter, center);
            one = !one;
        }
        if (this->body2ID >= 0){
            const dReal *bodyCenter = dBodyGetPosition(this->body2);
            dAddVectors3(center, bodyCenter, center);
            one = !one;
        }

        dReal point1[3];
        dReal point2[3];

        if (one){
            // com of body if only one body
            point1[0] = center[0] + direction[0] * (this->highStop - offset);
            point1[1] = center[1] + direction[1] * (this->highStop - offset);
            point1[2] = center[2] + direction[2] * (this->highStop - offset);

            point2[0] = center[0] + direction[0] * (this->lowStop + offset);
            point2[1] = center[1] + direction[1] * (this->lowStop + offset);
            point2[2] = center[2] + direction[2] * (this->lowStop + offset);        
        }
        else{
            // midpoint between two bodies if two bodies present
            point1[0] = center[0] / 2.0 + direction[0] * this->highStop;
            point1[1] = center[1] / 2.0 + direction[1] * this->highStop;
            point1[2] = center[2] / 2.0 + direction[2] * this->highStop;

            point2[0] = center[0] / 2.0 + direction[0] * this->lowStop;
            point2[1] = center[1] / 2.0 + direction[1] * this->lowStop;
            point2[2] = center[2] / 2.0 + direction[2] * this->lowStop;
        }

        dsDrawLine(point1, point2);
    }

};

#endif