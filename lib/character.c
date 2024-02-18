//
// Created by bestf on 2/12/2024.
//

#include <malloc.h>
#ifndef RAYLIB_INCLUDE
#define RAYLIB_INCLUDE
#include <raylib.h>
#include <raymath.h>
#endif
#include "character.h"

typedef enum {
    STANDING,
    IN_AIR,
} CharacterState;

typedef struct Character {
    Vector2 position;
    Poly *hit_box;
    Texture2D *sprite;
    Image *image;
    Environment *env;
    CharacterState state;
    bool debug;
    Vector2 force;

    bool brk;

    float gravity;

    float delta_counter;
    float delta_delay;
    int current_anim_frame;
    int anim_frames;
    unsigned int next_frame_data_offset
} Character;

void update_animation(Character *c, float delta_time);

Character *character_create(Poly *hit_box, Vector2 position, char *idle_file_path) {

    int frames = 0;
    Image *char_image_anim = malloc(sizeof(Image));
    *char_image_anim = LoadImageAnim("assets/character/idle.gif", &frames);
    Texture2D *char_texture = malloc(sizeof(Texture2D));
    *char_texture = LoadTextureFromImage(*char_image_anim);


    Character *c = malloc(sizeof(Character));
    c->hit_box = hit_box;
    c->position = position;
    c->debug = true;

    // animation
    c->image = char_image_anim;
    c->sprite = char_texture;
    c->delta_counter = 0.0f;
    c->delta_delay = 0.1f;
    c->current_anim_frame = 0;
    c->anim_frames = frames;
    c->next_frame_data_offset = 0;

    // movement
    c->state = IN_AIR;
    c->gravity = 8.90f;
    c->force = (Vector2) {.x = 0.0f, .y = 0.0f};

    // debug
    c->brk = false;

    return c;
}

void character_destroy(Character *c) {
    free(c->image);
    free(c->sprite);
    free(c);
}

void character_update(Character *c, float delta_time) {
    //update_animation(c, delta_time);
    if (c->state == IN_AIR) {
        Vector2 start_ray_pos = poly_get_bottom_center(c->hit_box);

        c->force.y += c->gravity * delta_time;

        Vector2 end_ray_pos = (Vector2) {.x = start_ray_pos.x, .y = start_ray_pos.y + c->force.y};
        float angle = 90;
        if (c->force.y < 0) angle = 270;
        Raycast *raycast = raycast_create(start_ray_pos, angle, Vector2Distance(start_ray_pos, end_ray_pos));

        raycast_draw(raycast);

        Vector2 final_ray_pos = end_ray_pos;

        for (int i = 0; i < c->env->num_colliders; i++) {
            Rayhit hit = raycast_cast(raycast, c->env->colliders[i], 400);
            if (hit.has_hit) {
                final_ray_pos = hit.hit;
                poly_set_marked(c->hit_box, true);
                c->force.y = 0;
                c->state = STANDING;
            }
        }

        poly_move_by(c->hit_box, 0, final_ray_pos.y - start_ray_pos.y);
        c->position.y += final_ray_pos.y - start_ray_pos.y;
        c->position.x += final_ray_pos.x - start_ray_pos.x;

        if (c->brk) {
            TraceLog(LOG_INFO, "foo");
        }

        raycast_destroy(raycast);
    }
}

void update_animation(Character *c, float delta_time) {
    c->delta_counter += delta_time;
    if (c->delta_counter >= c->delta_delay) {
        // Move to next frame
        // NOTE: If final frame is reached we return to first frame
        c->current_anim_frame++;
        if (c->current_anim_frame >= c->anim_frames) c->current_anim_frame = 0;

        // Get memory offset position for next frame data in image.data
        c->next_frame_data_offset = c->sprite->width * c->sprite->height * 4 * c->current_anim_frame;

        // Update GPU texture data with next frame image data
        // WARNING: Data size (frame size) and pixel format must match already created texture
        UpdateTexture(*c->sprite, ((unsigned char *) c->image->data) + c->next_frame_data_offset);

        c->delta_counter = 0.0f;
    }
}

void character_draw(Character *c) {
    DrawTextureV(*c->sprite, c->position, WHITE);
    if (c->brk) {
        TraceLog(LOG_INFO, "foo");
    }
    if (c->debug) {
        poly_draw(c->hit_box);
    }
}

void character_set_environment(Character *c, Environment *env) {
    c->env = env;
}

void character_control(Character *c) {
    c->brk = IsKeyPressed(KEY_D);
    if (c->state == STANDING && IsKeyPressed(KEY_SPACE)) {
        c->force.y = -5.0f;
        c->position.y -= 1;
        poly_move_by(c->hit_box, 0, -1);
        c->state = IN_AIR;
    }
}


