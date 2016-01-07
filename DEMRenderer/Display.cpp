#include "stdafx.h"
#include "Display.h"

#define ONE_SECOND_MS 1000

Display::Display(const unsigned int width, const unsigned int height, const unsigned int fps, const string& title) :
	m_width(width), m_height(height), m_fps(fps), m_title(title), m_isClosed(true), m_camera(nullptr), m_transform(nullptr)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(
		m_title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		cerr << "Failed to initialize window!" << endl;
	}
	else {
		m_isClosed = false;
	}
}


Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::setCamera(Camera *camera) {
	this->m_camera = camera;
}

void Display::setTransform(Transform *transform) {
	this->m_transform = transform;
}

void Display::Update() {
	SDL_GL_SwapWindow(m_window);

	SDL_Event event;

	Uint32 start = SDL_GetTicks();

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_isClosed = true;
		}
		if (m_camera != nullptr) {
			if (event.type == SDL_MOUSEMOTION) {
				m_prevMouseXCoord = m_curMouseXCoord;
				m_prevMouseYCoord = m_curMouseYCoord;
				m_curMouseXCoord = event.motion.x;
				m_curMouseYCoord = event.motion.y;
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					m_camera->moveRight((float)(m_curMouseXCoord - m_prevMouseXCoord) / (m_width));
					m_camera->moveUp((float)(m_curMouseYCoord - m_prevMouseYCoord) / (m_height));
				}
				else
				{
					if (m_transform != nullptr) {
						if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
							m_transform->rotateWith(vec3((float)(m_curMouseYCoord - m_prevMouseYCoord) / (m_height), 
								(float)(m_curMouseXCoord - m_prevMouseXCoord) / (m_width), 0));
						}
					}
				}
			}
			if (event.type == SDL_MOUSEWHEEL) {
				if (event.wheel.y > 0) {
					m_camera->zoomWith(0.05f);
				}
				else {
					m_camera->zoomWith(-0.05f);
				}
			}
		}
	}

	Uint32 end = SDL_GetTicks();
	Uint32 delay = end - start;

	if (ONE_SECOND_MS / m_fps > delay) {
		SDL_Delay(ONE_SECOND_MS / m_fps - delay);
	}

}
