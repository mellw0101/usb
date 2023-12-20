#ifndef WAYLANDCLIET_H
#define WAYLANDCLIET_H

#include <cstring>
#include <iostream>
#include <wayland-client.h>

class WaylandClient {
public:
    WaylandClient();

    ~WaylandClient();

    void createSurface();

private:
    static void global_registry_handler(void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t version);

    static void global_registry_remover(void *data, wl_registry *registry, uint32_t id);

    static wl_registry_listener registry_listener;

    wl_display *display = nullptr;
    wl_registry *registry = nullptr;
    wl_compositor *compositor = nullptr;
    wl_surface *surface = nullptr;

    // Add other Wayland objects you need, e.g., wl_surface, wl_shell_surface, etc.
};

#endif // WAYLANDCLIET_H
