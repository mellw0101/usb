#include "waylandClient.h"
#include "debug.h"

WaylandClient::WaylandClient() {
    display = wl_display_connect(nullptr);
    if (display == nullptr) {
        DEBUG::output("Can't connect to Wayland display.");
        exit(EXIT_FAILURE);
    }
    qDebug() << display;

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    qDebug() << registry;
    // Here you can create a surface or window and manage its geometry
}

WaylandClient::~WaylandClient() {
    if (compositor) wl_compositor_destroy(compositor);
    if (registry) wl_registry_destroy(registry);
    if (display) wl_display_disconnect(display);
}
void WaylandClient::global_registry_handler(void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
    WaylandClient *client = static_cast<WaylandClient*>(data);

    if (strcmp(interface, "wl_compositor") == 0) {
        client->compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, id, &wl_compositor_interface, 1));
    }
    // Add other interfaces you need, e.g., wl_shell, etc.
}

void WaylandClient::global_registry_remover(void *data, wl_registry *registry, uint32_t id) {
    // Implement if needed
}
wl_registry_listener WaylandClient::registry_listener = {
    global_registry_handler,
    global_registry_remover
};

void WaylandClient::createSurface() {
    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        std::cerr << "Failed to create a surface." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Further configuration of the surface goes here
}
// Add other Wayland objects you need, e.g., wl_surface, wl_shell_surface, etc.
