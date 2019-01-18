#ifndef __JOINT_HPP
#define __JOINT_HPP

#pragma once

#include "entity.hpp"
#include "pythonReader.hpp"

class Joint : public Entity{
protected:
    int body1ID, body2ID; // entity IDs
    dReal lowStop, highStop;
    dBodyID body1, body2; // ode bodies
    dJointID joint;
    int oneBody;

public:
    virtual void create(Environment *environment) =0;
    virtual void readJointParamsFromPython() =0;
    void readFromPython(void){
        // every joint may contain bodies
        this->readBodiesFromPython();

        // read the individual params for each joint
        this->readJointParamsFromPython();
    }

    void readBodiesFromPython(void){
        readValueFromPython<int>(&this->body1ID, "Body 1");
        readValueFromPython<int>(&this->body2ID, "Body 2");
    }

    void readStopsFromPython(void){
        // function to read low and high stops
        readValueFromPython<dReal>(&this->lowStop, "Low Stop");
        readValueFromPython<dReal>(&this->highStop, "High Stop");
    }

    void setBodies(Environment *environment){
        // set bodies from received input
        if (this->body1ID < 0){
            this->body1 = (dBodyID) 0;
        }
        else{
            RigidBody *bodyObj = (RigidBody *) environment->getEntity(this->body1ID);
            this->body1 = bodyObj->getBody();
        }
        if (this->body2ID < 0){
            this->body2 = (dBodyID) 0;
        }
        else{
            RigidBody *bodyObj = (RigidBody *) environment->getEntity(this->body2ID);
            this->body2 = bodyObj->getBody();
        }
    }

    dJointID getJoint(void){
        return this->joint;
    }

    virtual EntityType getEntityType(void){
        return JOINT;
    }

    virtual dReal getProprioception(void){
        return 0;
    }
};


class HingeJoint : public Joint{
protected:
    dReal anchor[3];
    dReal axis[3];
    
public:
    HingeJoint(){}

    void readJointParamsFromPython(void){
        readValueFromPython<dReal>(this->anchor, 3,"Hinge Anchor");
        readValueFromPython<dReal>(this->axis, 3,"Hinge Axis");
        this->readStopsFromPython();
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

    void draw(float r, float g, float b, float a){
        // temporary draw function
        dVector3 pos;
        dVector3 rot;

        dJointGetHingeAnchor(this->joint, pos);
        dJointGetHingeAxis(this->joint, rot);

        dMatrix3 R;
        dRFromZAxis(R, rot[0], rot[1], rot[2]);

        dsSetColorAlpha(r, g, b, a); 

        // pos[0] += rot[0];
        // pos[1] += rot[1];
        // pos[2] += rot[2];        
        dsDrawCylinder(pos, R, 0.3, 0.03);
    }

    void draw(){
        if (this->highStop == this->lowStop){
            this->draw(0.1f, 0.1f, 0.1f, 0.5f);
        }
        else{
            this->draw(0.8f, 0.2f, 0.2f, 0.5f);
        }
    }

    virtual dReal getProprioception(void){
        return dJointGetHingeAngle(this->joint);
    }
};


class SliderJoint : public Joint{
protected:
    dReal anchor[3];
    dReal axis[3];
    
public:
    SliderJoint(){}

    void readJointParamsFromPython(void){
        readValueFromPython<dReal>(this->axis, 3,"Slider Axis");
        this->readStopsFromPython();
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
        // draw a slider joint as a green line between the two bodies
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

        dReal distance = this->highStop - this->lowStop;

        dReal point1[3];
        dReal point2[3];

        if (one){
            // com of body if only one body
            point1[0] = center[0] + direction[0] * (distance / 2.0 + offset);
            point1[1] = center[1] + direction[1] * (distance / 2.0 + offset);
            point1[2] = center[2] + direction[2] * (distance / 2.0 + offset);

            point2[0] = center[0] + direction[0] * - (distance / 2.0 - offset);
            point2[1] = center[1] + direction[1] * - (distance / 2.0 - offset);
            point2[2] = center[2] + direction[2] * - (distance / 2.0 - offset);        
        }
        else{
            // midpoint between two bodies if two bodies present
            point1[0] = center[0] / 2.0 + direction[0] * distance / 2.0;
            point1[1] = center[1] / 2.0 + direction[1] * distance / 2.0;
            point1[2] = center[2] / 2.0 + direction[2] * distance / 2.0;

            point2[0] = center[0] / 2.0 + direction[0] * -distance / 2.0;
            point2[1] = center[1] / 2.0 + direction[1] * -distance / 2.0;
            point2[2] = center[2] / 2.0 + direction[2] * -distance / 2.0;
        }

        dsDrawLine(point1, point2);
    }

    
    virtual dReal getProprioception(void){
        return dJointGetSliderPosition(this->joint);
    }
};

class BallAndSocketJoint : public Joint{
protected:
    dReal anchor[3];

public:
    BallAndSocketJoint(){}

    void readJointParamsFromPython(void){
        // read in anchor position for ball joint
        readValueFromPython<dReal>(this->anchor, 3, "Ball and Socket Anchor");
    }

    void create(Environment *environment){
        this->setBodies(environment);
        this->joint = dJointCreateBall(environment->getWorld(), 0);
        
        dJointAttach(this->joint, this->body1, this->body2);
        dJointSetBallAnchor(this->joint, this->anchor[0], this->anchor[1], this->anchor[2]);
    }

    void draw(){
        // draw a blue ball to represent ball joint
        dVector3 jointPosition;
        dJointGetBallAnchor(this->joint, jointPosition);
        dMatrix3 R;
        dRSetIdentity(R);

        dsSetColorAlpha(0.2, 0.5, 0.7, 0.5);
        dsDrawSphere(jointPosition, R, 0.05);

        if ( this->body1ID >= 0 ){
            const dReal * body1Position = dBodyGetPosition( this->body1 );
            dsDrawLine( body1Position, jointPosition );
        }
        
        if ( this->body2ID >= 0 ){
            const dReal * body2Position = dBodyGetPosition( this->body2 );
            dsDrawLine( body2Position, jointPosition );
        }
    }
};

class UniversalJoint : public Joint {
protected:
    dReal anchor[3];
    dReal axis1[3];
    dReal axis2[3];

public:
    UniversalJoint(){}

    void readJointParamsFromPython(void){
        readValueFromPython<dReal>(this->anchor, 3, "Universal Anchor Added");
        readValueFromPython<dReal>(this->axis1, 3, "Universal Axis 1 Added");
        readValueFromPython<dReal>(this->axis2, 3, "Universal Axis 2 Added");
    }

    void create(Environment *environment){
        this->setBodies(environment);
        this->joint = dJointCreateUniversal(environment->getWorld(), 0);

        dJointAttach(this->joint, this->body1, this->body2);

        dJointSetUniversalAnchor(this->joint, 
                                 this->anchor[0],
                                 this->anchor[1],
                                 this->anchor[2]);
        dJointSetUniversalAxis1(this->joint,
                               this->axis1[0],
                               this->axis1[1],
                               this->axis1[2]);
        dJointSetUniversalAxis2(this->joint,
                               this->axis2[0],
                               this->axis2[1],
                               this->axis2[2]);        
    }

    void draw(){
        // temporary draw function
        dVector3 pos;
        dVector3 rot1;
        dVector3 rot2;

        dJointGetUniversalAnchor(this->joint, pos);
        dJointGetUniversalAxis1(this->joint, rot1);
        dJointGetUniversalAxis2(this->joint, rot2);

        dMatrix3 R1;
        dMatrix3 R2;

        dRFromZAxis(R1, rot1[0], rot1[1], rot1[2]);
        dRFromZAxis(R2, rot2[0], rot2[1], rot2[2]);
        
        dsSetColorAlpha(0.7, 0.7, 0.2, 0.5); 

        // pos[0] += rot[0];
        // pos[1] += rot[1];
        // pos[2] += rot[2];        
        dsDrawCylinder(pos, R1, 0.3, 0.03);
        dsDrawCylinder(pos, R2, 0.3, 0.03);
    }
};

#endif