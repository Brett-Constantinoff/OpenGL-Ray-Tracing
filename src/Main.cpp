#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "Window.h"
#include "RayTracer.h"

int main()
{
    // create window
    std::shared_ptr<Window> window = std::make_shared<Window>(1080, 16.0f / 9.0f, "Raytracing");

    // create ray tracer
    RayTracer rayTracer(window);
    rayTracer.render();

    return 0;
}