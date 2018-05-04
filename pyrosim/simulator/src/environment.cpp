#include "environment.hpp"
#include "pythonReader.hpp"

#include <map>

template<typename entityClass> Entity * createEntityInstance(){ return static_cast <Entity *> (new entityClass);}
typedef std::map<std::string, Entity * (*) ()> StringToEntity;

#include "body/rigidBody.hpp"
#include "body/heightMap.hpp"
#include "joint/joint.hpp"

// C.C. we can possibly put this in separate file?
// maybe when it becomes bigger we will know how to best handle it
StringToEntity stringToEntityMap{
    {"Box",        &createEntityInstance<BoxBody>      }, // simple body with one box
    {"Cylinder",   &createEntityInstance<CylinderBody> }, // simple body with one cylinder
    {"Sphere",     &createEntityInstance<SphereBody>   }, // simple body with one shpere
    {"Composite",  &createEntityInstance<RigidBody>    }, // initially empty composite body
    {"HeightMap",  &createEntityInstance<HeightMap>    }, // Landscape
    {"Hinge",      &createEntityInstance<Hinge>        }, // Hinge joint
    {"Slider",     &createEntityInstance<Slider>       }, // slider joint
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

void Environment::createEntities(void){
    std::cerr << "Creating Entities In Environment" << std::endl
              << "---------------------------" << std::endl;
    for (auto entity : this->entities){
        // create entity with env data
        entity->create(this);
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

dBodyID Environment::getBody(int i){
    RigidBody *bodyContainer = (RigidBody *) this->entities[i];
    return bodyContainer->getBody();
}

dSpaceID Environment::getSpace(std::string name){
    if (name == "None"){
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

void Environment::writeToPython(void){
    for (auto entity : this->entities){
        entity->writeToPython();
    }
}