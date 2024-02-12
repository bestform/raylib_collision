//
// Created by bestf on 2/12/2024.
//

#include <malloc.h>
#include "character.h"

typedef struct Character {
    Vector2 position;
    Poly *hit_box;
    Texture2D *sprite;
    Image *image;
    bool debug;
    float delta_counter;
    float delta_delay;
    int current_anim_frame;
    int anim_frames;
    unsigned int next_frame_data_offset
} Character;

Character *character_create(Poly *hit_box, Vector2 position, char *idle_file_path) {

    int frames = 0;
    Image *char_image_anim = malloc(sizeof(Image));
    *char_image_anim = LoadImageAnim("assets/character/idle.gif", &frames);
    Texture2D *char_texture = malloc(sizeof(Texture2D));
    *char_texture = LoadTextureFromImage(*char_image_anim);


    Character *c = malloc(sizeof(Character));
    c->hit_box = hit_box;
    c->position = position;
    c->sprite = char_texture;
    c->image = char_image_anim;
    c->debug = true;
    c->delta_counter = 0.0f;
    c->delta_delay = 0.1f;
    c->current_anim_frame = 0;
    c->anim_frames = frames;
    c->next_frame_data_offset = 0;


    return c;
}

void character_destroy(Character *c) {
    free(c->image);
    free(c->sprite);
    free(c);
}

void character_update(Character *c, float delta_time) {
    c->delta_counter+=delta_time;
    if (c->delta_counter >= c->delta_delay)
    {
        // Move to next frame
        // NOTE: If final frame is reached we return to first frame
        c->current_anim_frame++;
        if (c->current_anim_frame >= c->anim_frames) c->current_anim_frame = 0;

        // Get memory offset position for next frame data in image.data
        c->next_frame_data_offset = c->sprite->width*c->sprite->height*4*c->current_anim_frame;

        // Update GPU texture data with next frame image data
        // WARNING: Data size (frame size) and pixel format must match already created texture
        UpdateTexture(*c->sprite, ((unsigned char *)c->image->data) + c->next_frame_data_offset);

        c->delta_counter = 0.0f;
    }

}

void character_draw(Character *c) {
    DrawTextureV(*c->sprite, c->position, WHITE);
    if (c->debug) {
        poly_draw(c->hit_box);
    }
}


