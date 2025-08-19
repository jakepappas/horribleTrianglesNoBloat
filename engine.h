//
// Created by Jake Pappas on 8/19/2025.
//

#ifndef PIXELRENDERER_ENGINE_H
#define PIXELRENDERER_ENGINE_H


#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include "resources/glm/glm.hpp"
#include "resources/glm/gtc/matrix_transform.hpp"
#include "Triangle.h"

using glm::vec2, glm::vec3, glm::vec4, glm::mat4;


class Engine {
private:

    vec4 p1 = vec4(0.5, 0.5, 5, 1);
    vec4 p2 = vec4(-0.5, -0.5, 5, 1);
    vec4 p3 = vec4(-0.5, 0.5, 5, 1);
    vec4 p4 = vec4(0.5, -0.5, 5, 1);

    vec4 p5 = vec4(0.5, 0.5, 10, 1);
    vec4 p6 = vec4(-0.5, -0.5, 10, 1);
    vec4 p7 = vec4(-0.5, 0.5, 10, 1);
    vec4 p8 = vec4(0.5, -0.5, 10, 1);






    uint32_t *pixels;

    const unsigned int width = 800, height = 800;

    const float Far = 100.0f;
    const float Near = 1.0f;
    const float fov = 110.0f;
    const float aspect = 1.0f;
    const float f = 1.0f / tan((fov * M_PI / 180.0f) / 2.0f);

    mat4 perspective = mat4(
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (Far + Near) / (Near - Far), (2 * Far * Near) / (Near - Far),
            0, 0, -1, 0
    );

    float degreeRotation = 0;


    std::vector<std::unique_ptr<Triangle>> Triangles;

public:
    Engine(uint32_t *pixels);

    ~Engine();

    /// @brief Initializes the shapes to be rendered.
    void InitShapes();

    void ProcessInput();

    void Update();

    void Render();


};

#endif //PIXELRENDERER_ENGINE_H
