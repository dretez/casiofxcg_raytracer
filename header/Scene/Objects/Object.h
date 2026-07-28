#ifndef INCLUDE_OBJECTS_OBJECT_H
#define INCLUDE_OBJECTS_OBJECT_H

#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"

typedef struct Object Object;

typedef int (*Object_intersect)(const Object* self, const Ray*, geo_t);
typedef int (*Object_intersectAt)(const Object* self, const Ray*, geo_t, geo_t*);
typedef Vec3 (*Object_normal)(const Object* self, Vec3 hitPoint);

typedef struct ObjectVTable {
    const Object_intersect   intersect;
    const Object_intersectAt intersectAt;
    const Object_normal      normal;
} ObjectVTable;

struct Object {
    const ObjectVTable* vtable;
    const Material*     material;
};

#endif /* ifndef INCLUDE_OBJECTS_OBJECT_H */
