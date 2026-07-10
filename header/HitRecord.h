#ifndef INCLUDE_HITRECORD_H
#define INCLUDE_HITRECORD_H

#include "Scene/Objects/Sphere.h"

typedef struct {
    Ray     normal;
    Sphere* object;
} HitRecord;

#endif /* ifndef INCLUDE_HITRECORD_H */
