/**
 * Simple Pong example using Raylib and Box2D
 * Demonstrates:
 * - Box2D api
 * - Decoupling physics and rendering
 * - Tweening with anvil2
 * Possible Issues: box2d coordinate system
 */
#include "anvil2/tween/tween.hpp"

// #include <frameflow-rs.h>

#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <box2d/math_types.h>
#include <raylib.h>
#include <fmt/core.h>

namespace {

constexpr float PIXELS_PER_METER = 10.0f;

struct RectangleDrawParameters {
    Rectangle rect;
    Vector2 origin;
    float rotation;
};

float deg2rad(float deg) { return deg * B2_PI / 180.0f; }

float rad2deg(float rad) { return rad * 180.0f / B2_PI; }

Vector2 vec2_from_box2d(float x, float y) {
    return Vector2{x * PIXELS_PER_METER, y * PIXELS_PER_METER};
}

float deg_from_box2d(b2Rot rotation) {
    return rad2deg(b2Atan2(rotation.s, rotation.c));
}

RectangleDrawParameters rect_params_from_box2d(b2Pos pos, float half_width,
                                               float half_height, b2Rot rot) {
    // Assuming the polygon is a box
    Vector2 v = vec2_from_box2d(pos.x, pos.y);
    auto rect = Rectangle{v.x, v.y, half_width * 2, half_height * 2};
    return RectangleDrawParameters{
        rect, {half_width, half_height}, deg_from_box2d(rot)};
}
} // namespace

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pong");
    // SetTargetFPS(60);

    // rs_greet("rust");

    // auto *system = frameflow_init_system();
    // frameflow_destroy_system(system);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position =
        (b2Pos){100.0f / PIXELS_PER_METER, 50.0f / PIXELS_PER_METER};
    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox =
        b2MakeBox(50.0f / PIXELS_PER_METER, 10.0f / PIXELS_PER_METER);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    auto rect_body_id = [&]() {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position =
            (b2Pos){150.0f / PIXELS_PER_METER, 4.0f / PIXELS_PER_METER};
        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
        b2Polygon dynamicBox =
            b2MakeBox(10.0f / PIXELS_PER_METER, 10.0f / PIXELS_PER_METER);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.3f;
        b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
        return bodyId;
    }();

    // 1. Define and create the rigid body
    auto circle_body_id = [&]() {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody; // Or b2_staticBody / b2_kinematicBody
        bodyDef.position = (b2Vec2){10.0f, 0.0f};
        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        // 2. Define the circle shape geometry
        b2Circle circle;
        circle.center =
            (b2Vec2){0.0f, 0.0f}; // Position relative to body center
        circle.radius = 0.5f;     // Radius in meters

        // 3. Define shape properties and attach it to the body
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.3f;
        shapeDef.material.restitution = 1.0f; // Bounciness

        b2CreateCircleShape(bodyId, &shapeDef, &circle);
        return bodyId;
    }();

    auto &tween_manager = TweenManager::get_instance();
    float x = 0;
    tween_manager.create_tween(&x, 400.0f, 2.0f, Easing::EaseInOutSine);

    const int subStepCount = 4;
    const float timeStep = 1.0f / 60.0f;

    float physics_accumulator = 0.0f;

    while (!WindowShouldClose()) {
        tween_manager.update(GetFrameTime());

        physics_accumulator += GetFrameTime();
        while (physics_accumulator >= timeStep) {
            b2World_Step(worldId, timeStep, subStepCount);
            physics_accumulator -= timeStep;
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20,
                     LIGHTGRAY);

            DrawRectangleV({x, 50.0f}, {32.0f, 32.0f}, BLUE);

            {
                b2Pos position = b2Body_GetPosition(rect_body_id);
                b2Rot rotation = b2Body_GetRotation(rect_body_id);
                auto rect = rect_params_from_box2d(position, 10, 10, rotation);
                DrawRectanglePro(rect.rect, rect.origin, rect.rotation, RED);
            }

            {
                b2Pos position = b2Body_GetPosition(groundId);
                b2Rot rotation = b2Body_GetRotation(groundId);
                auto rect = rect_params_from_box2d(position, 50, 10, rotation);
                DrawRectanglePro(rect.rect, rect.origin, rect.rotation, GRAY);
            }

            {
                b2Pos position = b2Body_GetPosition(circle_body_id);
                b2Rot rotation = b2Body_GetRotation(circle_body_id);
                DrawCircleV({position.x * PIXELS_PER_METER, position.y * PIXELS_PER_METER}, 0.5f * PIXELS_PER_METER, GREEN);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
