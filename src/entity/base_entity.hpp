#pragma once

#include <vector>

struct BaseEntity {
    virtual void update(double delta) = 0;

    virtual void draw() = 0;

    virtual void begin() = 0;

    virtual ~BaseEntity() = default;

    void addChild(BaseEntity* child);

    int id;

    std::vector<BaseEntity*> children;
};
