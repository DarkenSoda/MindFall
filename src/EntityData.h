#pragma once
#include "EntityTypes.h"

struct EntityData {
    EntityType type;
    void* objectPtr;
    
    EntityData(EntityType t, void* ptr) : type(t), objectPtr(ptr) {}
};
