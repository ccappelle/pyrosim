#include "environment.hpp"
#include "pythonReader.hpp"

#include <map>

// mumbo jumbo to create a map from strings to entity initializer functions
template<typename entityClass> Entity * createEntityInstance(){ return static_cast <Entity *> (new entityClass);}
typedef std::map<std::string, Entity * (*) ()> StringToEntity;
template<typename actuatorClass> Actuator * createActuatorInstance(){ return static_cast <Actuator *> (new actuatorClass);}
typedef std::map<std::string, Actuator * (*) ()> StringToActuator;

#include "body/rigidBody.hpp"
#include "body/heightMap.hpp"
#include "joint/joint.hpp"
#include "actuator/jointMotor.hpp"

// fill up map
// C.C. we can possibly put this in separate file?
// maybe when it becomes bigger we will know how to best handle it
StringToEntity stringToEntityMap{
    {"Box",        &createEntityInstance<BoxBody>      }, // simple body with one box
    {"Cylinder",   &createEntityInstance<CylinderBody> }, // simple body with one cylinder
    {"Sphere",     &createEntityInstance<SphereBody>   }, // simple body with one shpere
    {"Composite",  &createEntityInstance<RigidBody>    }, // initially empty composite body
    {"HeightMap",  &createEntityInstance<HeightMap>    }, // Landscape
    {"Hinge",      &createEntityInstance<HingeJoint>        }, // Hinge joint
    {"Slider",     &createEntityInstance<SliderJoint>       }, // slider joint
};

StringToActuator stringToActuatorMap{
    {"Rotary", &createActuatorInstance<Rotary> }, // Hinge rotary motor
};

Environment::Environment(dWorldID world, dSpaceID topspace, int numEntities){
    // initialize world and top space
    this->world = world;
    this->topspace = topspace;
    this->entities.reserve(numEntities);
};

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

    for (auto actuator : this->actuators){
        actuator->create(this);
    }
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
        // C.C. probably a better way to do this
        std::string drawGroup = entity->getDrawName();
        if (drawGroup == "Joint"){
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

// Sensor* Environment::getSensor(int i){

// }

Actuator* Environment::getActuator(int i){
    return this->actuators[i];
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
              << " From Python: " << this->entities.size()
              << std::endl;

    // create new instance of entity from map index
    // associated with entityName
    Entity *entity = stringToEntityMap[entityName]();

    // read entity info from python
    entity->readFromPython();
    // set entity id (probably unnecessary)
    entity->setID(this->entities.size());

    // add entity to entity list
    this->entities.push_back(entity);
    std::cerr << "---------------------------" << std::endl << std::endl;
}

void Environment::readActuatorFromPython(void){
    // get name of actuator
    std::string actuatorName;
    readStringFromPython(actuatorName);
    if (stringToActuatorMap.count(actuatorName) == 0){
        // entity key doesnt exist, either change python send
        // or add key to stringToEntityMap
        std::cerr << "ERROR: Entity " << actuatorName << " does not exist"
                  << std::endl
                  << "Exiting" << std::endl;
        exit(0);
    }
    // entity key name exists
    std::cerr << "---------------------------" << std::endl
              << "Creating Actuator " << actuatorName  
              << " From Python: " << this->actuators.size()
              << std::endl;

    // create new instance of entity from map index
    // associated with entityName
    Actuator *actuator = stringToActuatorMap[actuatorName]();

    // read entity info from python
    actuator->readFromPython();
    // set entity id (probably unnecessary)
    actuator->setID(this->actuators.size());

    // add entity to entity list
    this->actuators.push_back(actuator);
    std::cerr << "---------------------------" << std::endl << std::endl;
}

void Environment::takeStep(int timeStep, dReal dt){
    // update sensors 
    // for (auto sensor : this->sensors){
    //     sensor->sense(this);
    // }

    // update network
    // this->network->activate(this->sensors);

    // actuate
    for (auto actuator : this->actuators){
        actuator->actuate(1.0);
    }

    // take steps with entities
    // impulses etc.
    for (auto entity : this->entities){
        entity->takeStep(timeStep, dt);
    }
}

void Environment::writeToPython(void){
    for (auto entity : this->entities){
        entity->writeToPython();
    }
}