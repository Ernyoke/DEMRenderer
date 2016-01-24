// DEMRenderer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <GL\glew.h>

#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Dem.h"

constexpr int WIDTH = 800;
constexpr int HEGHT = 600;
constexpr int FPS = 60;
constexpr float ASPECT_RATIO = (float)(WIDTH) / (float)(HEGHT);
constexpr char* TITLE = "DEMRenderer";

int main(int argc, char** argv)
{
    bool result{ false };

    float counter{ 0.0f };

    Dem dem;
    try {
        dem.Open("Dems/DEM_30m/bushkill_pa.dem");
        dem.Parse();
        result = true;
    }
    catch (FileTypeException &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (FileOpenException &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception!" << endl;
    }

    if (result) {
        //set up the display frame
        Display display(WIDTH, HEGHT, FPS, TITLE);
        Shader shader("./Shaders/basicshader");

        //set up the camera
        Camera camera(glm::vec3(0, 0, -3), 70.0f, ASPECT_RATIO, 0.01f, 100.0f);
        display.SetCamera(&camera);

        //set up the transformation vector
        Transform transform;
        display.SetTransform(&transform);

        //get the surface from the DEM file
        Mesh mesh(dem.GetSurfaceMap(), dem.GetSurfaceMapSize(), dem.GetNormals());

        //Mesh mesh(vertices, extent< decltype(vertices) >::value);

        while (!display.IsWindowClosed())
        {
            glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.Update(transform, camera);
            mesh.DrawElevations();

            display.Update();
            counter += 0.01f;
        }
    }

    return EXIT_SUCCESS;
}

