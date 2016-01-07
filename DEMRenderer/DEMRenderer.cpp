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

#define WIDTH 800
#define HEGHT 600
#define FPS 60
#define ASPECT_RATIO (float)(WIDTH) / (float)(HEGHT)
#define TITLE "DEMRenderer"

using namespace std;

int main(int argc, char** argv)
{
	Display display(WIDTH, HEGHT, FPS, TITLE);

	glm::vec3 vertices[] = { glm::vec3(-0.5f,  -0.5f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f) };

	Shader shader("./Shaders/basicshader");
	//Mesh mesh(vertices, extent< decltype(vertices) >::value);

	float counter = 0.0f;

	Transform transform;
	Camera camera(vec3(0, 0, -3), 70.0f, ASPECT_RATIO, 0.01f, 100.0f);
	display.setCamera(&camera);
	display.setTransform(&transform);

    Dem dem;
    try {
        dem.Open("Dems/DEM_30m/bushkill_pa.dem");
        dem.Parse();
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

    Mesh mesh(dem.GetVertexMap(), dem.GetVertexMapSize());

	while (!display.isWindowClosed())
	{
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//transform.setPos(vec3(sinf(counter), 0, 0));
		//transform.setScale(vec3(sinf(counter), 0, 0));
		//transform.setRot(vec3(0, 0, counter));

		shader.bind();
		shader.update(transform, camera);
		mesh.draw();

		display.Update();
		counter += 0.01f;
	}

    return EXIT_SUCCESS;
}

