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
    mat4 modelMatrix{};
    int screenWidth;
    int screenHeight;
    vec3 color;

    vec2 p2d1{};
    vec2 p2d2{};
    vec2 p2d3{};
    vec2 points[3];

    uint32_t *pixels;

    Triangle(vec4 p1, vec4 p2, vec4 p3, mat4 &projection, mat4 &modelMatrix, int screenWidth, int screenHeight,
             uint32_t *pixels, vec3 color) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;

        this->projection = projection;
        this->modelMatrix = modelMatrix;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        ApplyAllTransformations(modelMatrix);

        this->pixels = pixels;
        points[0] = p2d1;
        points[1] = p2d2;
        points[2] = p2d3;

        this->color = color;
    }

    ~Triangle() = default;

    void ApplyAllTransformations(mat4 newModelMatrix) {
        this->p2d1 = ApplyTransformations(p1, newModelMatrix);
        this->p2d2 = ApplyTransformations(p2, newModelMatrix);
        this->p2d3 = ApplyTransformations(p3, newModelMatrix);
        points[0] = p2d1;
        points[1] = p2d2;
        points[2] = p2d3;

    }

    vec2 ApplyTransformations(vec4 p, mat4 newModelMatrix) const {
        vec4 projected = projection * newModelMatrix * p;
        if (projected.w != 0) {
            vec3 ndc = vec3(projected) / projected.w;
            int x = static_cast<int>(round((ndc.x + 1.0f) * 0.5f * screenWidth));
            int y = static_cast<int>(round((1.0f - (ndc.y + 1.0f) * 0.5f) * screenHeight));
            return {x, y};
        } else {
            return {0, 0};
        }

    }

    void ColorPixel(int x, int y) {
        uint8_t r = color.x;
        uint8_t g = color.y;
        uint8_t b = color.z;
        pixels[y * screenWidth + x] = (r << 16) | (g << 8) | b;
    }

    void sortEm() {
        if (points[0].y > points[1].y) std::swap(points[0], points[1]);
        if (points[1].y > points[2].y) std::swap(points[1], points[2]);
        if (points[0].y > points[1].y) std::swap(points[0], points[1]);
    }


    void FillBottomFlatTriangle(vec2 v1, vec2 v2, vec2 v3)
    {
        float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
        float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

        float curx1 = v1.x;
        float curx2 = v1.x;

        for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
        {
            DrawLine(vec2(curx1, scanlineY), vec2(curx2, scanlineY));
            curx1 += invslope1;
            curx2 += invslope2;
        }
    }

    void FillTopFlatTriangle(vec2 v1, vec2 v2, vec2 v3)
    {
        float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
        float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

        float curx1 = v3.x;
        float curx2 = v3.x;

        for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
        {
            DrawLine(vec2(curx1, scanlineY), vec2(curx2, scanlineY));
            curx1 -= invslope1;
            curx2 -= invslope2;
        }
    }


    void Fill(){
        // x4 = x1 + ((y2-y1)/(y3-y1))*(x3-x1)
        sortEm();
        vec2 v1 = points[0];
        vec2 v2 = points[1];
        vec2 v3 = points[2];
        if(v2.y==v3.y){
            FillBottomFlatTriangle(v1,v2,v3);
        }else if(v1.y == v2.y){
            FillTopFlatTriangle(v1,v2,v3);
        }else{
            float x4 = v1.x + ((v2.y-v1.y)/(v3.y-v1.y))*(v3.x-v1.x);
//            DrawLine(v2, vec2(x4,v2.y));

            FillBottomFlatTriangle(v1,v2, vec2(x4,v2.y));
            FillTopFlatTriangle(v2,vec2(x4,v2.y),v3);
        }

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
                if (y >= 0 && y < screenWidth &&
                    x >= 0 && x < screenHeight) {
                    ColorPixel(y, x);
                }
            } else {
                if (x >= 0 && x < screenWidth &&
                    y >= 0 && y < screenHeight) {
                    ColorPixel(x, y);
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
        Fill();
        DrawLine(p2d1, p2d2);
        DrawLine(p2d1, p2d3);
        DrawLine(p2d2, p2d3);
    }


};


#endif //PIXELRENDERER_TRIANGLE_H
