#ifndef _PHYSICS_AABB_H
#define _PHYSICS_AABB_H

#include <stdlib.h>
#include <math.h>

// Ported from: https://noonat.github.io/intersect/

// Represents a point in 2D space
typedef struct point {
    int32_t x;
    int32_t y;
} point_t;

typedef struct hit {
    // The point of contact between the two objects (or an estimation of it, in some sweep tests). 
    point_t pos;
    // The overlap between the two objects, and is a vector that can be added to the colliding object’s position to move it back to a non-colliding state. 
    point_t delta;
    // The surface normal at the point of contact
    point_t normal;
} hit_t;

typedef struct AABB {
    point_t pos;
    point_t half;
} AABB_t;

#define SIGN(X) copysign(1, X)
#define CLAMP(A, MIN, MAX) (((A)<(MIN))?(MIN):(((A)>(MAX))?(MAX):(A)))

// Intersection test between an AABB and a point
bool intersect_AABB_point(hit_t *result, AABB_t *box, point_t *point) {
    const float dx = point->x - box->pos.x;
    const float px = box->half.x - abs(dx);
    if (px <= 0) {
        return false;
    }

    const float dy = point->y - box->pos.y;
    const float py = box->half.y - abs(dy);
    if (py <= 0) {
        return false;
    }

    hit_t hit;
    if (px < py) {
        const int32_t sx = SIGN(dx);
        hit.delta.x = px * sx;
        hit.normal.x = sx;
        hit.pos.x = box->pos.x + (box->half.x * sx);
        hit.pos.y = point->y;
    } else {
        const int32_t sy = SIGN(dy);
        hit.delta.y = py * sy;
        hit.normal.y = sy;
        hit.pos.x = point->x;
        hit.pos.y = box->pos.y + (box->half.y * sy);
    }
    *result = hit;

    return true;
}

// Intersection test between 2 AABBs
bool intersect_AABB_AABB(hit_t *result, AABB_t *this, AABB_t *box) {
    const int32_t dx = box->pos.x - this->pos.x;
    const int32_t px = (box->half.x + this->half.x) - abs(dx);
    if (px < 0) {
        return false;
    }

    const int32_t dy = box->pos.y - this->pos.y;
    const int32_t py = (box->half.y + this->half.y) - abs(dy);
    if (py < 0) {
        return false;
    }

    hit_t hit = { 0 };
    if (px < py) {
        const int32_t sx = SIGN(dx);
        hit.delta.x = px * sx;
        hit.normal.x = sx;
        hit.pos.x = this->pos.x + (this->half.x * sx);
        hit.pos.y = box->pos.y;
    } else {
        const int32_t sy = SIGN(dy);
        hit.delta.y = py * sy;
        hit.normal.y = sy;
        hit.pos.x = box->pos.x;
        hit.pos.y = this->pos.y + (this->half.y * sy);
    }
    *result = hit;

    return true;
}

#endif