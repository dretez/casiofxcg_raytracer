#ifndef INCLUDE_HITRECORD_H
#define INCLUDE_HITRECORD_H

#include "Scene/Objects/Object.h"
#include "Vector/Geometry.h"

typedef struct {
    Vec3 point;
    Vec3 normal;

    Vec3 outerOffset;
    Vec3 innerOffset;

    geo_t rdotn;

    const Object* object;
} HitRecord;

#endif /* ifndef INCLUDE_HITRECORD_H */
