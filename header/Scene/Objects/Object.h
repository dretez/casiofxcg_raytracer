#ifndef INCLUDE_OBJECTS_OBJECT_H
#define INCLUDE_OBJECTS_OBJECT_H

#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"
#include "Vector/FloatingVector.h"

typedef enum ObjectType {
    SPHERE,
    OBJECT_TYPE_COUNT,
} ObjectType;

typedef struct Object Object;

typedef struct ObjectVTable {
    int (*intersect)(const Object* self, const Ray*, float);
    int (*intersectAt)(const Object* self, const Ray*, float, float*);
    Vec3 (*normal)(const Object* self, Vec3 hitPoint);
} ObjectVTable;

struct Object {
    const ObjectVTable* vtable;
    const Material*     material;
    ObjectType          type;
};

#endif /* ifndef INCLUDE_OBJECTS_OBJECT_H */
