#ifndef INCLUDE_RAY_H
#define INCLUDE_RAY_H

#include "Vector.h"

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

#define RAY_EPSILON 1e-3

#endif /* ifndef INCLUDE_RAY_H */
