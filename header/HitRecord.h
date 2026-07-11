#ifndef INCLUDE_HITRECORD_H
#define INCLUDE_HITRECORD_H

#include "Scene/Objects/Object.h"

typedef struct {
    Ray           normal;
    const Object* object;
} HitRecord;

#endif /* ifndef INCLUDE_HITRECORD_H */
