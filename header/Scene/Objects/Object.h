#ifndef INCLUDE_OBJECTS_OBJECT_H
#define INCLUDE_OBJECTS_OBJECT_H

#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"

typedef void ObjectData;

typedef int (*Object_intersect)(const ObjectData* data, const Ray*, geo_t);
typedef int (*Object_intersectAt)(const ObjectData* data, const Ray*, geo_t, geo_t*);
typedef Vec3 (*Object_normal)(const ObjectData* data, Vec3 hitPoint);

typedef struct ObjectVTable {
    const Object_intersect   intersect;
    const Object_intersectAt intersectAt;
    const Object_normal      normal;
} ObjectVTable;

typedef struct Object {
    const ObjectVTable* vtable;
    const Material*     material;
    ObjectData*         data;
} Object;

#endif /* ifndef INCLUDE_OBJECTS_OBJECT_H */
