#include <stdio.h>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#include "lib/poly.h"
#include "lib/raycast.h"
#include "lib/character.h"

int main() {
    InitWindow(1024, 1024, "Collision");
    SetTargetFPS(120);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    Vector2 player_pos = (Vector2) { .x = 200.0f, .y = 260.0f};
    Poly *hit_box = poly_create_rect(player_pos, 20, 34);

    Poly *floor = poly_create_rect((Vector2){.x = 10, .y = 400}, 400, 20);

    Character *c = character_create(hit_box, player_pos,"assets/character/idle.gif");

    Environment env = {.colliders = &floor, .num_colliders = 1};

    character_set_environment(c, &env);

    while (!WindowShouldClose()) {

        character_control(c);


        BeginDrawing();
        character_update(c, GetFrameTime());
        DrawFPS(5, 5);
        ClearBackground(DARKGREEN);
        character_draw(c);
        poly_draw(floor);

        EndDrawing();
    }

    poly_destroy(floor);
    character_destroy(c);
    CloseWindow();

    return 0;
}

int main_() {

    InitWindow(1024, 1024, "Collision");
    SetTargetFPS(120);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    Poly *p1 = poly_create_rect((Vector2) {.x = 300, .y = 100}, 200, 300);
    poly_rotate(p1, 45);

    Poly *p2 = poly_create_rect((Vector2) {.x = 600, .y = 300}, 100, 300);
    poly_rotate(p2, 70);

    bool dragging = false;
    Poly *dragged_poly = NULL;
    Vector2 last_drag_pos = {0};

    float ray_angle = 0.5f;
    float ray_length = 500.0f;
    Rayhit ray_hit_1;
    Rayhit ray_hit_2;

    Raycast *raycast = raycast_create((Vector2){.x = 1024/2, .y = 1024 / 2}, ray_angle, ray_length);

    while (!WindowShouldClose()) {

        if (!dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            last_drag_pos = GetMousePosition();
            if (poly_detect_collision_with_point(p1, GetMousePosition())) {
                dragged_poly = p1;
                dragging = true;
            } else if (poly_detect_collision_with_point(p2, GetMousePosition())) {
                dragged_poly = p2;
                dragging = true;
            }
        } else if (dragging) {
            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                dragging = false;
            } else {
                Vector2 m = GetMousePosition();
                poly_move_by(dragged_poly, m.x - last_drag_pos.x, m.y - last_drag_pos.y);
                last_drag_pos = m;
            }
        }

        poly_set_marked(p1, poly_detect_collision_with_point(p1, GetMousePosition()));
        poly_set_marked(p2, poly_detect_collision_with_point(p2, GetMousePosition()));
        bool colliding = poly_detect_collision_with_poly(p1, p2);
        poly_set_colliding(p1, colliding);
        poly_set_colliding(p2, colliding);

        raycast_set_angle(raycast, ray_angle);
        raycast_set_length(raycast, ray_length);

        ray_hit_1 = raycast_cast(raycast, p1, 500);
        ray_hit_2 = raycast_cast(raycast, p2, 500);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // GUI

        GuiGroupBox((Rectangle){ 166, 24, 285, 112 }, "RAYCAST");
        GuiSlider((Rectangle){ 210, 48, 202, 16 }, "Angle", TextFormat("%0.2f", ray_angle), &ray_angle, 0.0f, 360.0f);
        GuiSlider((Rectangle){ 210, 80, 202, 16 }, "Length", TextFormat("%0.2f", ray_length), &ray_length, 10.0f, 500.0f);

        // Recs
        DrawFPS(10, 10);
        poly_draw(p1);
        poly_draw(p2);

        // raycast
        raycast_draw(raycast);

        // draw hit
        if (ray_hit_1.has_hit) {
            DrawCircleV(ray_hit_1.hit, 10, RAYWHITE);
        }
        if (ray_hit_2.has_hit) {
            DrawCircleV(ray_hit_2.hit, 10, RAYWHITE);
        }

        EndDrawing();

    }

    poly_destroy(p1);
    poly_destroy(p2);
    raycast_destroy(raycast);

    CloseWindow();

    return 0;
}
