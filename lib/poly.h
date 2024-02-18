//
// Created by bestf on 1/30/2024.
//

#ifndef COLLISION_POLY_H
#define COLLISION_POLY_H

#ifndef RAYLIB_INCLUDE
#define RAYLIB_INCLUDE
#include <raylib.h>
#include <raymath.h>
#endif
#include <corecrt.h>

typedef struct Poly Poly;

Poly *poly_create_rect(Vector2 pos, float width, float height);

void poly_rotate(Poly *p, float degrees);

bool poly_detect_collision_with_point(Poly *p, Vector2 point);

bool poly_detect_collision_with_poly(Poly *p1, Poly *p2);

void poly_set_colliding(Poly *p, bool colliding);

void poly_move_by(Poly *p, float x, float y);

void poly_set_marked(Poly *p, bool marked);

void poly_draw(Poly *p);

void poly_destroy(Poly *p);

Vector2 poly_get_bottom_center(Poly *p);

#endif //COLLISION_POLY_H
