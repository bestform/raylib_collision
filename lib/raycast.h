//
// Created by bestf on 2/2/2024.
//

#ifndef RAYCAST_H
#define RAYCAST_H
#include <raylib.h>

#include "poly.h"

typedef struct Raycast Raycast;
typedef struct {
    bool has_hit;
    Vector2 hit;
} Rayhit;

Raycast *raycast_create(Vector2 pos, float angle, float length);

void raycast_set_angle(Raycast *r, float angle);

void raycast_set_length(Raycast *r, float length);

void raycast_draw(Raycast *r);

Rayhit raycast_cast(Raycast *r, Poly *p, int steps);

void raycast_destroy(Raycast *r);
#endif //RAYCAST_H
