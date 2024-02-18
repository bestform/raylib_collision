//
// Created by bestf on 2/12/2024.
//

#ifndef COLLISION_CHARACTER_H
#define COLLISION_CHARACTER_H
#ifndef RAYLIB_INCLUDE
#define RAYLIB_INCLUDE
#include <raylib.h>
#endif

#include "poly.h"
#include "raycast.h"

typedef struct Character Character;

typedef struct Environment {
    Poly **colliders;
    size_t num_colliders;
} Environment;

Character *character_create(Poly *hit_box, Vector2 position, char *idle_file_path);

void character_set_environment(Character *c, Environment *env);

void character_destroy(Character *c);

void character_update(Character *c, float delta_time);

void character_control(Character *c);

void character_draw(Character *c);

#endif //COLLISION_CHARACTER_H
