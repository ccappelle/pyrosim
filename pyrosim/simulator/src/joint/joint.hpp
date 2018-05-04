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
            this->body1 = environment->getBody(this->body1ID);
        }
        else{
            this->body1 = (dBodyID) 0;
        }
        if (this->body2ID >= 0){
            this->body2 = environment->getBody(this->body2ID);
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
    }

    void draw(){
        // temporary draw function
        dReal pos[3];
        dMatrix3 R;
        dRSetIdentity(R);

        dJointGetHingeAnchor(this->joint, pos);

        dsSetColor(0, 0, 0);
        dsDrawSphere(pos, R, 0.1);
    }

};

#endif