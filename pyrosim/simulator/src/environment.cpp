#include "environment.hpp"
#include "pythonReader.hpp"

#include <map>

template<typename entityClass> Entity * createEntityInstance(){ return static_cast <Entity *> (new entityClass);}
typedef std::map<std::string, Entity * (*) ()> StringToEntity;

#include "body/rigidBody.hpp"
#include "body/heightMap.hpp"

// C.C. we can possibly put this in separate file?
// maybe when it becomes bigger we will know how to best handle it
StringToEntity stringToEntityMap{
    {"Box",        &createEntityInstance<BoxBody>      }, // simple body with one box
    {"Cylinder",   &createEntityInstance<CylinderBody> }, // simple body with one cylinder
    {"Sphere",     &createEntityInstance<SphereBody>   }, // simple body with one shpere
    {"Composite",  &createEntityInstance<RigidBody>    }, // initially empty composite body
    {"HeightMap",  &createEntityInstance<HeightMap>    }, // Landscape
};

Environment::Environment(dWorldID world, dSpaceID space, int numEntities){
    // initialize world and top space
    this->world = world;
    this->space = space;
    this->entities.reserve(numEntities);
};

void Environment::addToEntityFromPython(void){
    // read in ID to add to
    int entityID;
    readValueFromPython<int>(&entityID, "Entity ID");
    // execute entities addition function
    this->entities[entityID]->readAdditionFromPython();
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

void Environment::createEntities(void){
    std::cerr << "Creating Entities In Environment" << std::endl
              << "---------------------------" << std::endl;
    for (auto entity : this->entities){
        // create entity with env data
        entity->create(this);
    }
    std::cerr << "---------------------------" << std::endl << std::endl;
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
}

void Environment::writeToPython(void){
    for (auto entity : this->entities){
        entity->writeToPython();
    }
}