#ifndef __SPRING_HPP__
#define __SPRING_HPP__

#pragma once

#include "math.h"
#include "stdlib.h"

#include "entity.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"


class LinearSpringJoint : public Entity{
protected:
    int body1ID, body2ID; // entity ids
    float stiffness;
    float restingLength;
    float damping;
    float sliderOffset;
    dBodyID body1, body2;
    dJointID joint;

public:
    LinearSpringJoint(){};

    virtual EntityType getEntityType(void){
        return JOINT;
    }

    void readFromPython( void ){
        readValueFromPython<int>( &this->body1ID, "Body 1 ID" );
        readValueFromPython<int>( &this->body2ID, "Body 2 ID" );
        readValueFromPython<float>( &this->restingLength, "Resting Length" );
        readValueFromPython<float>( &this->stiffness, "Linear Stiffness" );
        readValueFromPython<float>( &this->damping, "Damping" );
    }

    void create( Environment *environment ){
        std::cerr << "Creating Linear Spring" << std::endl;
        RigidBody *body1Obj = (RigidBody *) environment->getEntity( this->body1ID );
        RigidBody *body2Obj = (RigidBody *) environment->getEntity( this->body2ID );

        this->body1 = body1Obj->getBody();
        this->body2 = body2Obj->getBody();

        // create slider joint connecting bodies
        const dReal *pos1 = dBodyGetPosition( this->body1 );
        const dReal *pos2 = dBodyGetPosition( this->body2 );   

        dReal direction[3] = { ( pos2[0] - pos1[0] ),
                               ( pos2[1] - pos1[1] ),
                               ( pos2[2] - pos1[2] ) };
        dReal magnitude = sqrt( direction[0] * direction[0] +
                                direction[1] * direction[1] +
                                direction[2] * direction[2] );
        for( int i = 0; i < 3; i++ ){
            direction[i] /= magnitude;
        }

        // offset is later used for displacement calculation
        this->sliderOffset = this->restingLength - magnitude;

        // create and attach joint
        this->joint = dJointCreateSlider( environment->getWorld(), 0 );
        dJointAttach( this->joint, this->body1, this->body2 );
        dJointSetSliderParam( this->joint, dParamLoStop, -magnitude );
        dJointSetSliderParam( this->joint, dParamHiStop, +dInfinity );
        dJointSetSliderAxis( this->joint, direction[0], direction[1], direction[2] );
    }

    void takeStep( int timeStep, dReal dt ){
        float sliderPosition = dJointGetSliderPosition( this->joint );

        float displacement = this->sliderOffset + sliderPosition;


        float force = -this->stiffness * displacement;

        // undamped
        dJointAddSliderForce( this->joint, force );
    }

    void draw( void ){
        dsSetColorAlpha( 0.0, 0.0, 0.0, 0.5 );
        const dReal *p1 = dBodyGetPosition( this->body1 );
        const dReal *p2 = dBodyGetPosition( this->body2 );

        int nSpheres = 4;
        // draw nSperes space between the bodies
        for(int i=0; i<=nSpheres; i++){
            float alpha = float( i ) / float(nSpheres);
            // actual sampled value for center of spheres
            const dReal p[3] = { p2[0] * (1 - alpha) + p1[0] * alpha,
                                 p2[1] * (1 - alpha) + p1[1] * alpha,
                                 p2[2] * (1 - alpha) + p1[2] * alpha };

            dsDrawSphere( p, dBodyGetRotation( this->body1 ), 0.1 );
        }
    }
};

class HingeSpringJoint : public Entity{
protected:
    dBodyID body1, body2;
    int body1ID, body2ID;
    float stiffness;
    float damping;
    float axis1[3];
    float axis2[3];

    dJointID joint;

public:
    HingeSpringJoint(){};

    virtual EntityType getEntityType(void){
        return JOINT;
    }

    void create( Environment *environment ){
        std::cerr << "Creating Hinge Spring" << std::endl;
        RigidBody *body1Obj = (RigidBody *) environment->getEntity( this->body1ID );
        RigidBody *body2Obj = (RigidBody *) environment->getEntity( this->body2ID );

        this->body1 = body1Obj->getBody();
        this->body2 = body2Obj->getBody();

        // create slider joint connecting bodies
        const dReal *pos1 = dBodyGetPosition( this->body1 );
        const dReal *pos2 = dBodyGetPosition( this->body2 );   

        const dReal midpoint[3] = { ( pos1[0] + pos2[0] ) / 2.0,
                                   ( pos1[1] + pos2[1] ) / 2.0,
                                   ( pos1[2] + pos2[2] ) / 2.0,
                                  };

        // create universal joint and attach bodies
        this->joint = dJointCreateUniversal( environment->getWorld(), 0 );
        dJointAttach( this->joint, this->body1, this->body2 );
        dJointSetUniversalAnchor( this->joint,
                                  midpoint[0],
                                  midpoint[1],
                                  midpoint[2] );
        dJointSetUniversalAxis1( this->joint, this->axis1[0], this->axis1[1], this->axis1[2] );
        dJointSetUniversalAxis2( this->joint, this->axis2[0], this->axis2[1], this->axis2[2] );
    }

    void readFromPython( void ){
        readValueFromPython<int>( &this->body1ID, "Body 1 ID" );
        readValueFromPython<int>( &this->body2ID, "Body 2 ID" );
        readValueFromPython<float>( &this->stiffness, "Rotational Stiffness" );
        readValueFromPython<float>( this->axis1, 3, "Axis 1" );
        readValueFromPython<float>( this->axis2, 3, "Axis 2" );
        readValueFromPython<float>( &this->damping, "Damping" );
    }

    void takeStep( int timeStep, dReal dt ){
        // maintain straightness
        dReal angle1, angle2;
        dJointGetUniversalAngles( this->joint, &angle1, &angle2 );
        // angular component of spring
        dReal torque1, torque2;
        torque1 = - this->stiffness * angle1;
        torque2 = - this->stiffness * angle2;

        // std::cerr << "JOINT STEP: " << angle1 << " " << angle2 << std::endl;
        dJointAddUniversalTorques( this->joint, torque1, torque2 );
    }

    void draw( void ){
        dVector3 c2;
        dJointGetUniversalAnchor( this->joint, c2 );
        dsSetColorAlpha( 0.0, 0.0, 0.0, 0.5 );
        const dReal *c1 = dBodyGetPosition( this->body1 );

        const dReal *c3 = dBodyGetPosition( this->body2 );

        int nSpheres = 4;
        // draw bezier curve with 10 spheres
        for(int i=0; i<=nSpheres; i++){
            float alpha = float( i ) / float(nSpheres);
            // weighted points between control lines
            const dReal p1[3] = { c2[0] * (1 - alpha) + c1[0] * alpha,
                                  c2[1] * (1 - alpha) + c1[1] * alpha,
                                  c2[2] * (1 - alpha) + c1[2] * alpha };
            const dReal p2[3] = { c3[0] * (1 - alpha) + c2[0] * alpha,
                                  c3[1] * (1 - alpha) + c2[1] * alpha,
                                  c3[2] * (1 - alpha) + c2[2] * alpha };

            // actual sampled value for center of spheres
            const dReal p[3] = { p2[0] * (1 - alpha) + p1[0] * alpha,
                                 p2[1] * (1 - alpha) + p1[1] * alpha,
                                 p2[2] * (1 - alpha) + p1[2] * alpha };

            dsDrawSphere( p, dBodyGetRotation( this->body1 ), 0.1 );
        }
    }
};

class UniversalSpringJoint : public Entity{
protected:
    int body1ID, body2ID; // entity ids
    float linearStiffness, rotationalStiffness;
    float restingLength;
    dBodyID body1, body2, internalBody1, internalBody2; // ode body ids
    dJointID universalJoint, slider1, slider2; // ode joint ids

public:
    UniversalSpringJoint(){};

    void create( Environment *environment ){
        // universal spring consists of two extra small bodies
        // two sliders connecting main bodies to internal bodies
        // universal joint between internal bodies
        RigidBody *body1Obj = (RigidBody *) environment->getEntity( this->body1ID );
        RigidBody *body2Obj = (RigidBody *) environment->getEntity( this->body2ID );

        this->body1 = body1Obj->getBody();
        this->body2 = body2Obj->getBody();
        // find midpoint to create new bodies
        const dReal *pos1 = dBodyGetPosition( this->body1 );
        const dReal *pos2 = dBodyGetPosition( this->body2 );

        dReal midpoint[3] = { ( pos1[0] + pos2[0] ) / 2.0,
                              ( pos1[1] + pos2[1] ) / 2.0,
                              ( pos1[2] + pos2[2] ) / 2.0
                            };

        dReal direction[3] = { ( pos2[0] - pos1[0] ),
                               ( pos2[1] - pos1[1] ),
                               ( pos2[2] - pos1[2] ) };
        dReal magnitude = sqrt( direction[0] * direction[0] +
                                direction[1] * direction[1] +
                                direction[2] * direction[2] );
        for( int i = 0; i < 3; i++ ){
            direction[i] /= magnitude;
        }
        // create internal bodies, assign small masses
        // and set position as midpoint of positions
        dMass m1, m2;
        this->internalBody1 = dBodyCreate( environment->getWorld() );
        dBodySetPosition( this->internalBody1,
                          midpoint[0], midpoint[1], midpoint[2] );
        dMassSetSphere( &m1, 0.01, 0.0001 );

        this->internalBody2 = dBodyCreate( environment->getWorld() );
        dMassSetSphere( &m2, 0.01, 0.0001 );
        dBodySetPosition( this->internalBody2,
                          midpoint[0], midpoint[1], midpoint[2] );

        // create and attach slider joints
        this->slider1 = dJointCreateSlider( environment->getWorld(), 0 );
        dJointAttach( this->slider1, this->body1, this->internalBody1 );
        dJointSetSliderParam( this->slider1, dParamLoStop, -restingLength * 2.0 );
        dJointSetSliderParam( this->slider1, dParamHiStop,  restingLength * 2.0 );
        dJointSetSliderAxis( this->slider1, direction[0], direction[1], direction[2] );

        this->slider2 = dJointCreateSlider( environment->getWorld(), 0 );
        dJointAttach( this->slider2, this->body2, this->internalBody2 );
        dJointSetSliderParam( this->slider2, dParamLoStop, -restingLength * 2.0 );
        dJointSetSliderParam( this->slider2, dParamHiStop, +restingLength * 2.0 );
        dJointSetSliderAxis( this->slider2, direction[0], direction[1], direction[2] );

        // connect internal bodies with universal joint
        this->universalJoint = dJointCreateUniversal( environment->getWorld(), 0 );
        dJointAttach( this->universalJoint, this->internalBody1, this->internalBody2 );
        dJointSetUniversalAnchor( this->universalJoint,
                                  midpoint[0],
                                  midpoint[1],
                                  midpoint[2] );
    }

    void readFromPython( void ){
        readValueFromPython<int>( &this->body1ID, "Body 1 ID" );
        readValueFromPython<int>( &this->body2ID, "Body 2 ID" );
        readValueFromPython<float>( &this->restingLength, "Resting Length" );
        readValueFromPython<float>( &this->linearStiffness, "Linear Stiffness" );
        readValueFromPython<float>( &this->rotationalStiffness, "rotationalStiffness" );
    }

    virtual EntityType getEntityType(void){
        return JOINT;
    }

    void takeStep( int timeStep, dReal dt ){
        // timestep and dt are unused here
        

        // maintain straightness
        dReal angle1, angle2;
        dJointGetUniversalAngles( this->universalJoint, &angle1, &angle2 );
        // angular component of spring
        dReal torque1, torque2;
        torque1 = - this->rotationalStiffness * angle1;
        torque2 = - this->rotationalStiffness * angle2;

        // std::cerr << "JOINT STEP: " << angle1 << " " << angle2 << std::endl;
        dJointAddUniversalTorques( this->universalJoint, torque1, torque2 );
        
        // maintain length;
        const dReal *pos1 = dBodyGetPosition( this->body1 );
        const dReal *pos3 = dBodyGetPosition( this->body2 );
        const dReal *pos2 = dBodyGetPosition( this->internalBody1 );


        dReal length1 = sqrt( pow( pos1[0] - pos2[0], 2 ) +
                              pow( pos1[1] - pos2[1], 2 ) +
                              pow( pos1[2] - pos2[2], 2 ) );
        dReal length2 = sqrt( pow( pos3[0] - pos2[0], 2 ) +
                              pow( pos3[1] - pos2[1], 2 ) +
                              pow( pos3[2] - pos2[2], 2 ) ); 
        // dReal length2 = dJointGetSliderPosition( this->slider2 );

        dReal magnitude = ( length1 + length2 - this->restingLength ) * this->linearStiffness;

        dJointAddSliderForce( this->slider1, magnitude / 2.0 );
        dJointAddSliderForce( this->slider2, -magnitude / 2.0 );
    }

    void draw( void ){
        dsSetColorAlpha( 1.0, 0.0, 0.0, 0.5 );
        dsDrawSphere( dBodyGetPosition( this->internalBody1 ), 
                      dBodyGetRotation( this->internalBody1 ),
                      0.1 );
        // dsSetColorAlpha( 1.0, 0.0, 0.0, 1.0 );
        // dsDrawSphere( dBodyGetPosition( this->internalBody2 ), 
        //               dBodyGetRotation( this->internalBody2 ),
        //               0.1 );
        dsSetColorAlpha( 0.0, 0.0, 0.0, 0.5 );
        const dReal *c1 = dBodyGetPosition( this->body1 );
        const dReal *c2 = dBodyGetPosition( this->internalBody1 );
        const dReal *c3 = dBodyGetPosition( this->body2 );

        int nSpheres = 4;
        // draw bezier curve with 10 spheres
        for(int i=0; i<=nSpheres; i++){
            float alpha = float( i ) / float(nSpheres);
            // weighted points between control lines
            const dReal p1[3] = { c2[0] * (1 - alpha) + c1[0] * alpha,
                                  c2[1] * (1 - alpha) + c1[1] * alpha,
                                  c2[2] * (1 - alpha) + c1[2] * alpha };
            const dReal p2[3] = { c3[0] * (1 - alpha) + c2[0] * alpha,
                                  c3[1] * (1 - alpha) + c2[1] * alpha,
                                  c3[2] * (1 - alpha) + c2[2] * alpha };

            // actual sampled value for center of spheres
            const dReal p[3] = { p2[0] * (1 - alpha) + p1[0] * alpha,
                                 p2[1] * (1 - alpha) + p1[1] * alpha,
                                 p2[2] * (1 - alpha) + p1[2] * alpha };

            dsDrawSphere( p, dBodyGetRotation( this->internalBody1 ), 0.1 );
        }
    }
};

#endif