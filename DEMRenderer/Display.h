#pragma once

#include <iostream>
#include "SDL.h"
#include "GL\glew.h"

#include "Camera.h"
#include "Transform.h"


class Display
{
public:
    explicit Display(const unsigned int width, const unsigned int height, const unsigned int fps, const string& title);

    //make this class noncopyable
    Display(const Display&) = delete;
    Display operator=(const Display&) = delete;

    void SetCamera(Camera *camera);
    void SetTransform(Transform *transform);

    void Update();
    inline const bool IsWindowClosed() { return m_isClosed; }

    virtual ~Display();

private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_fps;
    string m_title;
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_isClosed;
    Camera *m_camera;
    Transform *m_transform;

    int m_prevMouseXCoord;
    int m_prevMouseYCoord;
    int m_curMouseXCoord;
    int m_curMouseYCoord;
};

