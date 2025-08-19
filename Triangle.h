//
// Created by Jake Pappas on 8/19/2025.
//

#ifndef PIXELRENDERER_TRIANGLE_H
#define PIXELRENDERER_TRIANGLE_H


using std::vector, glm::mat4, glm::vec3, glm::vec2, glm::vec4;

class Triangle {
public:
    vec4 p1{};
    vec4 p2{};
    vec4 p3{};
    mat4 projection{};
    mat4 rotation{};
    int screenWidth;
    int screenHeight;

    vec2 p2d1{};
    vec2 p2d2{};
    vec2 p2d3{};

    uint32_t *pixels;

    Triangle(vec4 p1, vec4 p2, vec4 p3, mat4 &projection, mat4 &rotation, int screenWidth, int screenHeight,
             uint32_t *pixels) {
        this->projection = projection;
        this->rotation = rotation;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        this->p2d1 = ApplyTransformations(p1);
        this->p2d2 = ApplyTransformations(p2);
        this->p2d3 = ApplyTransformations(p3);

        this->pixels = pixels;
    }

    ~Triangle() = default;

    vec2 ApplyTransformations(vec4 p) {
        vec4 projected = projection * rotation * p;
        if(projected.w!=0){
            vec3 ndc = vec3(projected) / projected.w;
            int x = static_cast<int>(round((ndc.x + 1.0f) * 0.5f * screenWidth));
            int y = static_cast<int>(round((1.0f - (ndc.y + 1.0f) * 0.5f) * screenHeight));
            return {x, y};
        }else{
            return{0,0};
        }

    }

    void SetRotation(float degreeRotation){
        std::cout<<degreeRotation<<std::endl;
        std::cout<<p2d1.x << " " << p2d1.y <<std::endl;
        float degree = degreeRotation * M_PI / 180.0f;
        rotation = mat4(
                cos(degree), 0, sin(degree), 0,
                0, 1, 0, 0,
                -sin(degree), 0, cos(degree), 0,
                0, 0, 0, 1
        );
        this->p2d1 = ApplyTransformations(p1);
        this->p2d2 = ApplyTransformations(p2);
        this->p2d3 = ApplyTransformations(p3);

    }


    void ColorPixel(int x, int y) {
        uint8_t r = 255;
        uint8_t g = 255;
        uint8_t b = 255;
        pixels[y * screenWidth + x] = (r << 16) | (g << 8) | b;
    }

    void DrawLine(vec2 p1, vec2 p2) {
        int x0 = p1.x;
        int y0 = p1.y;
        int x1 = p2.x;
        int y1 = p2.y;

        bool steep = abs(y1 - y0) > abs(x1 - x0);

        if (steep) {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }

        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        int dy = abs(y1 - y0);
        int error = dx / 2;
        int ystep = (y0 < y1) ? 1 : -1;
        int y = y0;

        for (int x = x0; x <= x1; x++) {
            if (steep) {
                if (y >= 0 && y <= screenHeight &&
                    x >= 0 && x <= screenWidth) {
                    ColorPixel(y,x);
                }
            } else {
                if (x >= 0 && x <= screenHeight &&
                    y >= 0 && y <= screenWidth) {
                    ColorPixel(x,y);
                }
            }

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }

    void Draw() {
        DrawLine(p2d1,p2d2);
        DrawLine(p2d1,p2d3);
        DrawLine(p2d2,p2d3);
    }

};


#endif //PIXELRENDERER_TRIANGLE_H
