#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct vec3d
{
    float x, y, z;
};

struct triangle
{
    vec3d p[3]; 
};

struct mesh
{
    vector<triangle> tris;
};

struct mat4x4
{
    float m[4][4] = { 0 };
};

float GetAspectRatio(SDL_Window *window) {
    int w, h;
    SDL_GetWindowSize( window, &w, &h);
    return static_cast<float>(w) / static_cast<float>(h);
}

void MultiplyMatrixVector(const vec3d &i, vec3d&o, const mat4x4 &m) {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        o.x /= w;
        o.y /= w;
        o.z /= w;
    }

}

void DrawTriangle (SDL_Renderer* renderer) {
    SDL_RenderLine(renderer, 100, 100, 200, 300);
}

#define WIDTH 900
#define HEIGHT 600

int main() {
    SDL_Window *window;
    bool running = true;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("window", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not create window:", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);

    mesh meshCube;
    meshCube.tris = {
        
        // SOUTH
        {0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f},
    
        // EAST
        {1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f},

        // NORTH
        {1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f},

        // WEST
        {0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f},

        // TOP
        {0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f},

        // BOTTOM
        {1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f},

    };

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = GetAspectRatio(window);
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    mat4x4 matproj;
    matproj.m[0][0] = fAspectRatio * fFovRad;
    matproj.m[1][1] = fFovRad;
    matproj.m[2][2] = fFar / (fFar - fNear);
    matproj.m[2][3] = (-fFar * fNear) / (fFar - fNear);
    matproj.m[3][2] = 1.0f;

    
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        DrawTriangle(renderer);

        for (auto tri : meshCube.tris) {
            triangle triProjected;
                for(int i = 0; i < 3; i++) {
                    MultiplyMatrixVector(tri.p[i], triProjected.p[i], matproj);
                }
        }
        SDL_RenderPresent(renderer);
    }



    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
