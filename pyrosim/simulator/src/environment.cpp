#include "environment.hpp"
#include "pythonReader.hpp"

// mumbo jumbo to create a map from strings to entity initializer functions
template<typename entityClass> Entity * createEntityInstance(){ return static_cast <Entity *> (new entityClass);}
typedef std::map<std::string, Entity * (*) ()> StringToEntity;

#include "body/rigidBody.hpp"
#include "body/heightMap.hpp"
#include "body/ray.hpp"
#include "joint/joint.hpp"
#include "actuator/jointMotor.hpp"
#include "actuator/thruster.hpp"
#include "joint/spring.hpp"
#include "network/ctrnn.hpp"
#include "sensor/isSeenSensor.hpp"
#include "sensor/positionSensor.hpp"
#include "sensor/raySensor.hpp"
#include "sensor/touchSensor.hpp"
#include "sensor/proprioceptiveSensor.hpp"
#include "sensor/vestibularSensor.hpp"

// fill up map
// C.C. we can possibly put this in separate file?
// maybe when it becomes bigger we will know how to best handle it
StringToEntity stringToEntityMap{
    {"Box",                  &createEntityInstance<BoxBody>              }, // simple body with one box
    {"Cylinder",             &createEntityInstance<CylinderBody>         }, // simple body with one cylinder
    {"Sphere",               &createEntityInstance<SphereBody>           }, // simple body with one shpere
    {"Composite",            &createEntityInstance<RigidBody>            }, // initially empty composite body
    {"Ray",                  &createEntityInstance<Ray>                  }, // ray geom object
    {"HeightMap",            &createEntityInstance<HeightMap>            }, // Landscape
    {"HingeJoint",           &createEntityInstance<HingeJoint>           }, // Hinge joint
    {"SliderJoint",          &createEntityInstance<SliderJoint>          }, // slider joint
    {"BallAndSocketJoint",   &createEntityInstance<BallAndSocketJoint>   }, // Ball and socket Joint
    {"UniversalJoint",       &createEntityInstance<UniversalJoint>       }, // Universal joint
    {"LengthSpringJoint",    &createEntityInstance<LengthSpringJoint>    }, // Length Spring joint
    {"LinearSpringJoint",    &createEntityInstance<LinearSpringJoint>    }, // Slider Spring
    {"HingeSpringJoint",     &createEntityInstance<HingeSpringJoint>     }, // Hinge Spring
    // {"UniversalSpringJoint", &createEntityInstance<UniversalSpringJoint> }, // spring
    {"RotaryActuator",       &createEntityInstance<RotaryActuator>       }, // Rotary actuator - attaches to hinge joint
    {"LinearActuator",       &createEntityInstance<LinearActuator>       }, // Linear actuator - attaches to slider joint
    {"ThrusterActuator",     &createEntityInstance<ThrusterActuator>     }, // thruster - like a rocket
    {"Synapse",              &createEntityInstance<Synapse>              }, // Synapse
    {"BiasNeuron",           &createEntityInstance<BiasNeuron>           }, // Bias neuron
    {"MotorNeuron",          &createEntityInstance<MotorNeuron>          }, // Motor Neuron
    {"UserNeuron",           &createEntityInstance<UserNeuron>           }, // User Neuron
    {"SensorNeuron",         &createEntityInstance<SensorNeuron>         }, // Sensor Neuron
    {"IsSeenSensor",         &createEntityInstance<IsSeenSensor>         }, // Is seen sensor
    {"PositionSensor",       &createEntityInstance<PositionSensor>       }, // Position Sensor
    {"RaySensor",            &createEntityInstance<RaySensor>            }, // Ray sensor
    {"TouchSensor",          &createEntityInstance<TouchSensor>          }, // Touch sensor
    {"ProprioceptiveSensor", &createEntityInstance<ProprioceptiveSensor> }, // Proprioceptive sensor
    {"quaternionSensor",     &createEntityInstance<QuaternionSensor>     }, // vestibular sensor returning quaternion
};

Environment::Environment(dWorldID world, dSpaceID topspace, int numEntities){
    // initialize world and top space
    this->world = world;
    this->topspace = topspace;
    this->entities.reserve(numEntities);

    // initialize empty vectors containing the indexes for 
    // each entity type
    // ENTITY, ACTUATOR, JOINT, NEURON, SYNAPSE, BODY, SENSOR
    int numEnums = 7;
    for (int i=0; i<numEnums; i++){
        std::vector<int> vec;
        this->entityVectors.push_back(vec);
    }
};

void Environment::addCollisionPair(int firstID, int secondID){
    std::pair<int, int> collisionPair (firstID, secondID);
    this->collisions.push_back(collisionPair);
}

void Environment::emptyCollisionPairs(void){
    this->collisions.clear();
}
void Environment::addToEntityFromPython(void){
    // read in ID to add to
    int entityID;
    readValueFromPython<int>(&entityID, "Entity ID");
    // execute entities addition function
    this->entities[entityID]->readAdditionFromPython();
}

void Environment::createInODE(void){
    std::cerr << "Creating Entities In Environment" << std::endl
              << "---------------------------" << std::endl;
    for (auto entity : this->entities){
        // create entity with env data
        entity->create(this);
    }

    // for (auto actuator : this->actuators){
    //     actuator->create(this);
    // }
    std::cerr << "---------------------------" << std::endl << std::endl;
}

void Environment::createSpace(std::string name){
    // check if name exists
    if (subspaces.count(name) == 0){
        this->subspaces[name] = dHashSpaceCreate (0);
        // add to top space
        dSpaceSetSublevel(this->subspaces[name], 1);
        dSpaceAdd(this->topspace, (dGeomID) this->subspaces[name]);
        std::cerr << "**Created space: " << name << " **" << std::endl;
    }
}

void Environment::draw(int drawJoints, int drawSpaces){
    for (auto entity : this->entities){
        EntityType drawGroup = entity->getEntityType();
        if (drawGroup == JOINT){
           if (drawJoints == true){
                entity->draw();
           }
        }
        else{
            entity->draw();
        }
    }

    // draw subspaces
    if (drawSpaces == true){
        int i;
        dReal aabb[6]; // bounding box
        for (auto keyAndSpace : this->subspaces){
            dSpaceID space = keyAndSpace.second;
            dGeomGetAABB((dGeomID) space, aabb);

            dVector3 bbpos;
            for (i=0; i<3; i++) bbpos[i] = 0.5*(aabb[i*2] + aabb[i*2+1]);
            dVector3 bbsides;
            for (i=0; i<3; i++) bbsides[i] = aabb[i*2+1] - aabb[i*2];
            dMatrix3 R;
            dRSetIdentity (R);
            dsSetColorAlpha (1,0,0,0.25);
            dsDrawBox (bbpos,R,bbsides);
        }
    }
}

Entity* Environment::getEntity(int i){
    return this->entities[i];
}

dSpaceID Environment::getSpace(std::string name){
    if (name == "None" or name == "default"){
        return this->topspace;
    }
    
    if (subspaces.count(name) == 0){
        this->createSpace(name);
    }

    return this->subspaces[name];
}

void Environment::readEntityFromPython(void){
    // get name of entity
    std::string entityName;
    readStringFromPython(entityName);
    if (stringToEntityMap.count(entityName) == 0){
        // entity key doesnt exist, either change python send
        // or add key to stringToEntityMap
        std::cerr << "ERROR: Entity " << entityName << " does not exist"
                  << std::endl
                  << "Exiting" << std::endl;
        exit(0);
    }
    // entity key name exists
    std::cerr << "---------------------------" << std::endl
              << "Creating Entity " << entityName  
              << " From Python: " << std::endl
              << "Entity ID: " << this->entities.size()
              << std::endl;

    // create new instance of entity from map index
    // associated with entityName
    Entity *entity = stringToEntityMap[entityName]();

    // read entity info from python
    entity->readFromPython();

    // set entity id
    entity->setID(this->entities.size());
    EntityType entityType = entity->getEntityType();
    this->entityVectors[entityType].push_back(this->entities.size());

    // add entity to entity list
    this->entities.push_back(entity);
    std::cerr << "---------------------------" << std::endl << std::endl;
}

void Environment::takeStep(std::vector<int> entityIDs, int timeStep, dReal dt){
    // take a step with the specified entity ids
    for (int entityID : entityIDs){
        Entity *entity = this->getEntity(entityID);
        entity->takeStep(timeStep, dt);
    }
}

void Environment::takeStep(int timeStep, dReal dt){
    // take step with relevant sensory motor entities
    std::vector<int> sensorIDs = this->entityVectors[SENSOR];
    std::vector<int> neuronIDs = this->entityVectors[NEURON];
    std::vector<int> actuatorIDs = this->entityVectors[ACTUATOR];
    std::vector<int> jointIDs = this->entityVectors[JOINT];
    std::vector<int> bodyIDs = this->entityVectors[BODY];
    // order is important
    // sense -> think -> act -> simulate

    // update sensors (sense)
    takeStep(sensorIDs, timeStep, dt);

    // update network (think)
    // note, neurons currently need to take step twice,
    // once to reset and once to update
    takeStep(neuronIDs, timeStep, dt);
    takeStep(neuronIDs, timeStep, dt);
    // update motors  (act)
    takeStep(actuatorIDs, timeStep, dt);
    // update other physics (simulate)
    takeStep( jointIDs, timeStep, dt );
    takeStep(bodyIDs, timeStep, dt);
}

void Environment::writeToPython(void){
    for (auto entity : this->entities){
        entity->writeToPython();
    }
}