#include <wayland-server-protocol.h>
#include <wayland-server.h>
#include <unistd.h>
#include <iostream>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace mlib {
    class WaylandCompositor {
    public:
        WaylandCompositor() {
            initEGL();
            initOpenGL();
            initWayland();
        }
        ~WaylandCompositor() {
            // Clean up Wayland resources
        }

        void run() {
            while (true) {  // Replace with a condition for a controlled exit
                // Draw the background
                drawBackground();

                // Swap the buffers to display the rendered background
                if (egl_surface != EGL_NO_SURFACE) {
                    eglSwapBuffers(egl_display, egl_surface);
                }

                wl_display_flush_clients(display);

                // You might also want to add a mechanism to control the loop frequency,
                // like sleeping for a short duration, to avoid consuming too much CPU.
                usleep(16000);  // Example: Sleep for ~16ms (~60 FPS)
            }
        }

    private:
        struct wl_display* display;
        struct wl_event_loop* event_loop;

        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
        EGLSurface egl_surface;

        void initWayland() {
            display = wl_display_create();
            event_loop = wl_display_get_event_loop(display);
            // Set up Wayland protocols, listeners, and other initializations
        }
        void drawBackground() {
            // Clear the screen with a light blue color
            glClearColor(0.68f, 0.85f, 0.9f, 1.0f); // Light blue color
            glClear(GL_COLOR_BUFFER_BIT);
            // Swap buffers, handle EGL surface, etc.

        }
        void initEGL() {
            egl_display = eglGetDisplay((EGLNativeDisplayType) display);
            if (egl_display == EGL_NO_DISPLAY) {
                std::cerr << "Failed to get EGL display" << std::endl;
                return;
            }

            if (!eglInitialize(egl_display, nullptr, nullptr)) {
                std::cerr << "Failed to initialize EGL" << std::endl;
                return;
            }

            // Choose an EGL config
            // Here, we just choose the first available config. You may need to choose more carefully depending on your needs
            EGLint num_configs;
            EGLConfig egl_config;
            eglChooseConfig(egl_display, nullptr, &egl_config, 1, &num_configs);

            // Create an EGL context
            egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, nullptr);
            if (egl_context == EGL_NO_CONTEXT) {
                std::cerr << "Failed to create EGL context" << std::endl;
                return;
            }

            eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_context);
        }
        void initOpenGL() {
            // Set up OpenGL states
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // Other OpenGL initializations as necessary
        }
        void createEGLSurface(struct wl_surface* wlSurface) {
            const EGLint egl_surface_attribs[] = { EGL_NONE };

            egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)wlSurface, egl_surface_attribs);
            if (egl_surface == EGL_NO_SURFACE) {
                std::cerr << "Failed to create EGL surface" << std::endl;
                return;
            }

            if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context)) {
                std::cerr << "Failed to make EGL context current" << std::endl;
                eglDestroySurface(egl_display, egl_surface);
                return;
            }
        }
        static void newClientHandler(struct wl_client* client, void* data) {
            // Handle new client connections
        }
    };
}

