#pragma once
#include "EntityTypes.h"

// Struct to store both entity type and pointer to the actual object
struct EntityData {
    EntityType type;
    void* objectPtr;
    
    EntityData(EntityType t, void* ptr) : type(t), objectPtr(ptr) {}
};
