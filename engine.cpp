#include "engine.h"

Engine::Engine(uint32_t *pixels) {
    this->pixels = pixels;
    this->InitShapes();
}

Engine::~Engine() = default;

void Engine::InitShapes() {



}

void Engine::ProcessInput() {

}

void Engine::Update() {
    Triangles.clear();

    float degree = degreeRotation++ * M_PI / 180.0f;
    mat4 rotation = mat4(
            cos(degree), 0, sin(degree), 0,
            0, 1, 0, 0,
            -sin(degree), 0, cos(degree), 0,
            0, 0, 0, 1
    );

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

void Engine::Render() {
    std::fill_n(pixels, width * height, 0x000000);

    for (auto &triangle: Triangles) {
        triangle->Draw();
    }
}
