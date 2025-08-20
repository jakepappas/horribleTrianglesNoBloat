#include "engine.h"

Engine::Engine(uint32_t *pixels) {
    this->pixels = pixels;
    this->InitShapes();
}

Engine::~Engine() = default;

void Engine::InitShapes() {
    center = vec3(0, 0, 0);
    std::vector<vec4> points = {p1, p2, p3, p4, p5, p6, p7, p8};
    for (const auto &p: points) {
        center += vec3(p.x, p.y, p.z);
    }
    center /= (float) points.size();

    // front
    Triangles.push_back(
            std::make_unique<Triangle>(p1, p2, p3,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p1, p2, p4,
                                       perspective, modelMatrix, 800, 800, pixels));
    //back
    Triangles.push_back(
            std::make_unique<Triangle>(p7, p8, p5,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p7, p8, p6,
                                       perspective, modelMatrix, 800, 800, pixels));

    // left
    Triangles.push_back(
            std::make_unique<Triangle>(p5, p4, p1,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p5, p4, p8,
                                       perspective, modelMatrix, 800, 800, pixels));

    // right
    Triangles.push_back(
            std::make_unique<Triangle>(p3, p6, p7,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p3, p6, p2,
                                       perspective, modelMatrix, 800, 800, pixels));

    // top
    Triangles.push_back(
            std::make_unique<Triangle>(p4, p6, p2,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p4, p6, p8,
                                       perspective, modelMatrix, 800, 800, pixels));

    // bottom
    Triangles.push_back(
            std::make_unique<Triangle>(p1, p7, p5,
                                       perspective, modelMatrix, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p1, p7, p3,
                                       perspective, modelMatrix, 800, 800, pixels));

}


void Engine::ProcessInput() {

}

mat4 Translation(float x, float y, float z) {
    return mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1
    );
}

mat4 RotationY(float angle) {
    return mat4(
            cos(angle), 0, sin(angle), 0,
            0, 1, 0, 0,
            -sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1
    );
}

mat4 RotationX(float angle) {
    return mat4(
            1, 0, 0, 0,
            0, cos(angle), -sin(angle), 0,
            0, sin(angle), cos(angle), 0,
            0, 0, 0, 1
    );
}


void Engine::Update() {
    mat4 translateToOrigin = Translation(-center.x, -center.y, -center.z);
    mat4 translateBack = Translation(center.x, center.y, center.z);
    mat4 rotation = RotationY(degreeRotation++ * M_PI / 180.0f);
    rotation *= RotationX(degreeRotation++ * M_PI / 180.0f);

    modelMatrix = translateBack * rotation * translateToOrigin;


}

void Engine::Render() {
    std::fill_n(pixels, width * height, 0x000000);

    for (auto &triangle: Triangles) {
        triangle->ApplyAllTransformations(modelMatrix);
        triangle->Draw();
    }
}
