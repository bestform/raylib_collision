//
// Created by bestf on 1/30/2024.
//

#include <malloc.h>
#include <math.h>
#include <raylib.h>
#include <assert.h>
#include "poly.h"

typedef struct Poly {
    Vector2 *points;
    size_t point_count;
    bool colliding;
    bool marked;
} Poly;

Poly *poly_create_rect(Vector2 pos, float width, float height) {
    Poly *p = malloc(sizeof(Poly));
    p->point_count = 4;
    p->colliding = false;
    p->marked = false;
    p->points = malloc(sizeof(Vector2*) * 4);

    p->points[0] = pos;
    p->points[1] = (Vector2){.x = pos.x + width, .y = pos.y};
    p->points[2] = (Vector2){.x = pos.x + width, .y = pos.y + height};
    p->points[3] = (Vector2){.x = pos.x, .y = pos.y + height};

    return p;
}

void poly_destroy(Poly *p) {
    free(p->points);
    free(p);
}

void poly_draw(Poly *p) {
    Color c = BLACK;
    if (p->colliding) {
        c = RED;
        if (p->marked) {
            c = YELLOW;
        }
    } else {
        if (p->marked) {
            c = RAYWHITE;
        }
    }
    for (size_t i = 0; i < p->point_count; i++) {
        Vector2 from = p->points[i];
        Vector2 to = p->points[(i+1) % p->point_count];

        DrawLineV(from, to, c);
    }
}

void poly_rotate(Poly *p, float degrees) {
    if (p->point_count == 1) return;

    float angle = degrees * PI / 180;

    Vector2 origin = p->points[0];

    for (size_t i = 1; i < p->point_count; i++) {
        Vector2 point = p->points[i];
        point.x -= origin.x;
        point.y -= origin.y;
        float x = point.x;
        float y = point.y;
        point.x = x * cosf(angle) - y * sinf(angle);
        point.y = y * cosf(angle) + x * sinf(angle);
        point.x += origin.x;
        point.y += origin.y;
        p->points[i] = point;
    }
}

bool poly_detect_collision_with_point(Poly *p, Vector2 point) {
    int hit = 0;
    for (size_t i = 0; i < p->point_count; i++) {
        Vector2 from = p->points[i];
        Vector2 to = p->points[(i+1) % p->point_count];

        Vector2 edge_v = Vector2Subtract(to, from);
        Vector2 edge_normal = (Vector2){.x = -edge_v.y, .y = edge_v.x};
        Vector2 axis = Vector2Normalize(edge_normal);

        float point_projected = Vector2DotProduct(axis, point);

        float min = -1;
        float max = -1;
        for (size_t j = 0; j < p->point_count; j++) {
            float projected = Vector2DotProduct(axis, p->points[j]);
            if (min == -1 || min > projected) min = projected;
            if (max == -1 || max < projected) max = projected;
        }

        if (point_projected > min && point_projected < max) {
            hit++;
        }

    }

    return hit >= p->point_count;
}

void normals_of_poly(Poly *p, Vector2 *into) {
    for (size_t i = 0; i < p->point_count; i++) {
        Vector2 from = p->points[i];
        Vector2 to = p->points[(i+1) % p->point_count];

        Vector2 edge_v = Vector2Subtract(to, from);
        Vector2 edge_normal = (Vector2){.x = -edge_v.y, .y = edge_v.x};
        Vector2 axis = Vector2Normalize(edge_normal);

        into[i] = axis;
    }
}


bool poly_detect_collision_with_poly(Poly *p1, Poly *p2) {

    size_t normals_count = p1->point_count + p2->point_count;
    Vector2 normals[normals_count];

    normals_of_poly(p1, &normals[0]);
    normals_of_poly(p2, &normals[p1->point_count]);

    size_t hits = 0;

    for (size_t i = 0; i < normals_count; i++) {
        Vector2 axis = normals[i];
        float p1_min = -1;
        float p1_max = -1;
        for (size_t j = 0; j < p1->point_count; j++) {
            float projected = Vector2DotProduct(axis, p1->points[j]);
            if (p1_min == -1 || p1_min > projected) p1_min = projected;
            if (p1_max == -1 || p1_max < projected) p1_max = projected;
        }
        float p2_min = -1;
        float p2_max = -1;
        for (size_t j = 0; j < p2->point_count; j++) {
            float projected = Vector2DotProduct(axis, p2->points[j]);
            if (p2_min == -1 || p2_min > projected) p2_min = projected;
            if (p2_max == -1 || p2_max < projected) p2_max = projected;
        }

        if (p1_max >= p2_min && p2_max >= p1_min) {
            hits++;
        }
    }

    return hits == normals_count;
}

void poly_set_colliding(Poly *p, bool colliding) {
    p->colliding = colliding;
}

void poly_move_by(Poly *p, float x, float y) {
    for (size_t i = 0; i < p->point_count; i++) {
        p->points[i].x += x;
        p->points[i].y += y;
    }
}

void poly_set_marked(Poly *p, bool marked) {
    p->marked = marked;
}

Vector2 poly_get_bottom_center(Poly *p) {
    float bottom = -1.0f;
    float most_left = 999999999999.0f;
    float most_right = -1.0f;
    for (int i = 0; i < p->point_count; ++i) {
        Vector2 point = p->points[i];
        if (point.y > bottom) bottom = point.y;
        if (point.x < most_left) most_left = point.x;
        if (point.x > most_right) most_right = point.x;
    }

    return (Vector2) {.y = bottom, .x = ((most_right - most_left) / 2) + most_left};
}



