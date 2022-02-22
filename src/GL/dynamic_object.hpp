#pragma once

#include <unordered_set>

namespace GL {

class DynamicObject
{
public:
    virtual ~DynamicObject() {}

    virtual void move() = 0;
    virtual bool can_be_del() const { return false; }
};

inline std::unordered_set<DynamicObject*> move_queue;

} // namespace GL
