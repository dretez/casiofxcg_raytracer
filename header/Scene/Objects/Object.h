#ifndef INCLUDE_OBJECTS_OBJECT_H
#define INCLUDE_OBJECTS_OBJECT_H

#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"
#include "Vector/FloatingVector.h"

typedef enum ObjectType {
    SPHERE,
    TRIANGLE,
    OBJECT_TYPE_COUNT,
} ObjectType;

typedef struct Object Object;

typedef int (*Object_intersect)(const Object* self, const Ray*, float);
typedef int (*Object_intersectAt)(const Object* self, const Ray*, float, float*);
typedef Vec3 (*Object_normal)(const Object* self, Vec3 hitPoint);

typedef struct ObjectVTable {
    const Object_intersect intersect;
    const Object_intersectAt intersectAt;
    const Object_normal normal;
} ObjectVTable;

struct Object {
    const ObjectVTable* vtable;
    const Material*     material;
    ObjectType          type;
};

#endif /* ifndef INCLUDE_OBJECTS_OBJECT_H */
