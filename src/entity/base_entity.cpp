#include "base_entity.hpp"

void BaseEntity::addChild(BaseEntity* child) {
    children.push_back(child);
}
