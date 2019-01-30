#ifndef __SPRING_HPP__
#define __SPRING_HPP__

#pragma once

#include "math.h"
#include "stdlib.h"

#include "entity.hpp"
#include "body/rigidBody.hpp"
#include "pythonReader.hpp"

class SpringJoint : public Entity{
// Base class for springs
protected:
    int body1ID, body2ID;
    float stiffness;
    float damping;
    dBodyID body1, body2;

public:
    virtual EntityType getEntityType( void ){
        return JOINT;
    }

    void readBodiesFromPython( void ){
        readValueFromPython<int>( &this->body1ID, "Body 1 ID" );
        readValueFromPython<int>( &this->body2ID, "Body 2 ID" );
    }

    void setBodies( Environment *environment ){
        RigidBody *body1Obj = (RigidBody *) environment->getEntity( this->body1ID );
        RigidBody *body2Obj = (RigidBody *) environment->getEntity( this->body2ID );

        this->body1 = body1Obj->getBody();
        this->body2 = body2Obj->getBody();
    }
};

class LengthSpringJoint : public SpringJoint{
protected:
    float restingLength;
public:
    LengthSpringJoint(){};

    virtual EntityType getEntityType( void ){
        return JOINT;
    }

    void readFromPython( void ){
        readBodiesFromPython();
        readValueFromPython<float>( &this->restingLength, "Resting Length" );
        readValueFromPython<float>( &this->stiffness, "Stiffness" );
        readValueFromPython<float>( &this->damping, "Damping" );
    }

    void create( Environment *environment ){
        // linear spring does not use any joints
        // simply provides calculated force at each time step
        this->setBodies( environment );
    }

    void takeStep( int timestep, dReal dt ){
        // calculate f = -kx and apply forces on each
        // body in the appropriate direction
        const dReal *pos1 = dBodyGetPosition( this->body1 );
        const dReal *pos2 = dBodyGetPosition( this->body2 );  

        dReal direction[3] = { pos2[0] - pos1[0],
                               pos2[1] - pos1[1],
                               pos2[2] - pos1[2] };

        // std::cerr << "Vector " << direction[0] << " " << direction[1] << " " << direction[2] << std::endl;
        dReal magnitude = sqrt( direction[0] * direction[0] +
                                direction[1] * direction[1] +
                                direction[2] * direction[2] );
        if ( magnitude > 0 ){
            direction[0] /= magnitude;
            direction[1] /= magnitude;
            direction[2] /= magnitude;

            dReal displacement = magnitude - this->restingLength;

            dBodyAddForce( this->body1,
                           displacement * this->stiffness * direction[0],
                           displacement * this->stiffness * direction[1],
                           displacement * this->stiffness * direction[2] );
            dBodyAddForce( this->body2,
                           -displacement * this->stiffness * direction[0],
                           -displacement * this->stiffness * direction[1],
                           -displacement * this->stiffness * direction[2] );
        }

    }

    void draw( void ){
        // draw as spheres along a line between bodies
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

class LinearSpringJoint : public SpringJoint{
protected:
    float restingLength;
    float sliderOffset;
    dJointID joint;

public:
    LinearSpringJoint(){};

    virtual EntityType getEntityType(void){
        return JOINT;
    }

    void readFromPython( void ){
        this->readBodiesFromPython( );
        readValueFromPython<float>( &this->restingLength, "Resting Length" );
        readValueFromPython<float>( &this->stiffness, "Linear Stiffness" );
        readValueFromPython<float>( &this->damping, "Damping" );
    }

    void create( Environment *environment ){
        // linear spring joint uses a slider joint to
        // maintain rotational relationship between bodies
        std::cerr << "Creating Linear Spring" << std::endl;
        this->setBodies( environment );

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

        // calculate damping based of velocity of slider joint
        float velocity = dJointGetSliderPositionRate( this->joint );

        dJointAddSliderForce( this->joint, force - velocity * this->damping );
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

class HingeSpringJoint : public SpringJoint{
protected:
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
        this->setBodies( environment );

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
        this->readBodiesFromPython();
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

        // calc damping
        float v1 = dJointGetUniversalAngle1Rate( this->joint );
        float v2 = dJointGetUniversalAngle2Rate( this->joint );

        dJointAddUniversalTorques( this->joint,
                                   torque1 - this->damping * v1,
                                   torque2 - this->damping * v2 );
    }

    void draw( void ){
        dVector3 c2;
        dJointGetUniversalAnchor( this->joint, c2 );
        dsSetColorAlpha( 0.0, 0.0, 0.0, 0.5 );
        const dReal *c1 = dBodyGetPosition( this->body1 );

        const dReal *c3 = dBodyGetPosition( this->body2 );

        int nSpheres = 4;
        // draw bezier curve with sspheres
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

#endif