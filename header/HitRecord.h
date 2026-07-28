#ifndef INCLUDE_HITRECORD_H
#define INCLUDE_HITRECORD_H

#include "Scene/Objects/Object.h"
#include "Vector/Geometry.h"

typedef struct {
    Vec3 point;
    Vec3 normal;

    Vec3          offset;
    Vec3          innerOffset;
    Vec3          outerOffset;
    const Object* object;
    geo_t         rdotn;
} HitRecord;

#endif /* ifndef INCLUDE_HITRECORD_H */
