//
// Created by bestf on 2/2/2024.
//

#include "raycast.h"


#ifndef RAYLIB_INCLUDE
#define RAYLIB_INCLUDE
#include <raylib.h>
#include <raymath.h>
#endif
#include <stdlib.h>


typedef struct Raycast {
    Vector2 start;
    Vector2 end;
    float angle;
    float length;
} Raycast;

void calc_end(Raycast * r) {
    float rad = r->angle * PI / 180;
    Vector2 target = Vector2Multiply((Vector2){.x = cosf(rad), .y = sinf(rad)}, (Vector2){.x = r->length, .y = r->length});
    r->end = (Vector2){
        .x = target.x + r->start.x,
        .y = target.y + r->start.y
    };
}

Raycast *raycast_create(Vector2 pos, float angle, float length) {
    Raycast *r = malloc(sizeof(Raycast));

    r->start = pos;
    r->angle = angle;
    r->length = length;

    calc_end(r);

    return r;
}

void raycast_set_angle(Raycast *r, float angle) {
    r->angle = angle;
    calc_end(r);
}

void raycast_set_length(Raycast *r, float length) {
    r->length = length;

    calc_end(r);
}

void raycast_draw(Raycast *r) {
    DrawCircleV(r->start, 10, BLUE);
    DrawLineV(r->start, r->end, BLUE);
}

Rayhit raycast_cast(Raycast *r, Poly *p, int steps) {
    float original_length = r->length;
    float step_size = original_length / (float) steps;

    Rayhit hit = {
            .has_hit = false,
            .hit = Vector2Zero()
    };

    for (int i = 0; i < steps; i++) {
        raycast_set_length(r, (float) i * step_size);
        if (poly_detect_collision_with_point(p, r->end)) {
            hit.has_hit = true;
            hit.hit = r->end;
            break;
        }
    }

    raycast_set_length(r, original_length);

    return hit;
}

void raycast_destroy(Raycast *r) {
    free(r);
}



