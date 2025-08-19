#include "engine.h"

Engine::Engine(uint32_t *pixels) {
    this->pixels = pixels;
    this->InitShapes();
}

Engine::~Engine() = default;

void Engine::InitShapes() {
    vec4 p1 = vec4(0.5, 0.5, 5, 1);
    vec4 p2 = vec4(-0.5, -0.5, 5, 1);
    vec4 p3 = vec4(-0.5, 0.5, 5, 1);
    vec4 p4 = vec4(0.5, -0.5, 5, 1);

    vec4 p5 = vec4(0.5, 0.5, 10, 1);
    vec4 p6 = vec4(-0.5, -0.5, 10, 1);
    vec4 p7 = vec4(-0.5, 0.5, 10, 1);
    vec4 p8 = vec4(0.5, -0.5, 10, 1);



    Triangles.push_back(
            std::make_unique<Triangle>(p1, p2, p3,
                                       perspective, rotation, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p1, p2, p4,
                                       perspective, rotation, 800, 800, pixels));

    Triangles.push_back(
            std::make_unique<Triangle>(p5, p6, p7,
                                       perspective, rotation, 800, 800, pixels));
    Triangles.push_back(
            std::make_unique<Triangle>(p5, p6, p8,
                                       perspective, rotation, 800, 800, pixels));


}

void Engine::ProcessInput() {

}

void Engine::Update() {

}

void Engine::Render() {
    for (auto &triangle: Triangles) {
//        triangle->SetRotation(degreeRotation);
        triangle->Draw();
    }
}
