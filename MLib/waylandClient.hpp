#ifndef WAYLANDCLIENT_HPP
#define WAYLANDCLIENT_HPP

#ifdef WAYLANDCLIENT_HPP

#include <cstring>
#include <wayland-client-protocol.h>
#include <wayland-client.hpp>
#include <iostream>
#include <wayland-server-protocol.h>
#include "wayland-xdg-shell-client-protocol.h"
// #include "xdg-shell-client-protocol.h"
#include "xdg-shell-client-protocol.hpp"
#include "wayland-client-protocol-extra.hpp"
#include "wayland-client-protocol.hpp"
#include "wayland-client-protocol-unstable.hpp"

// /usr/share/waylandpp/protocols/xdg-shell.xml
// /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml

// wayland-scanner client-header /usr/share/waylandpp/protocols/xdg-shell.xml xdg-shell-client-protocol.hpp
// wayland-scanner private-code /usr/share/waylandpp/protocols/xdg-shell.xml xdg-shell-client-protocol.cpp

// wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell-client-protocol.hpp
// wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell-client-protocol.cpp

#endif

class WaylandClient {
public:
    WaylandClient() {
        display = wl_display_connect(nullptr);
        if (!display) {
            throw std::runtime_error("Failed to connect to Wayland display.");
        }
        std::cout << "WaylandClient: display: " << display << "\n";

        registry = wl_display_get_registry(display);
        wl_registry_add_listener(registry, &registry_listener, this);
        wl_display_roundtrip(display); // Ensure the registry is fully processed

        // xdg_wm_base_get_xdg_surface(registry, display);

        std::cout << "WaylandClient: registry: " << registry << "\n";

    }
    ~WaylandClient() {
        if (registry) {
            wl_registry_destroy(registry);
        }
        if (display) {
            wl_display_disconnect(display);
        }
    }
    void run() {
        while (wl_display_dispatch(display) != -1) {
            // Handle events
        }
    }
    void create_surface() {
        if (!compositor) {
            std::cerr << "Compositor is not initialized." << std::endl;
            return;
        }
        std::cout << "run: compositor: " << compositor << "\n";

        surface = wl_compositor_create_surface(compositor);
        if (!surface) {
            std::cerr << "Failed to create surface." << std::endl;
            return;
        }

        std::cout << "run: created surface: " << surface << "\n";
        // Further surface configuration goes here...
        // For example, creating a shell surface or an xdg surface.
    }
private:
    static void handle_global(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
        std::cout << "handle_global: New interface: name: " << name << " interface: " << interface << std::endl;

        WaylandClient *client = static_cast<WaylandClient*>(data);

        if (strcmp(interface, "wl_compositor") == 0) {
            client->compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 1));
            std::cout << "handle_global: client->wl_compositor: " << client->compositor << "\n";
        }
        else if (strcmp(interface, "wl_seat") == 0) {
            client->seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 1));
            std::cout << "handle_global: client->wl_seat: " << client->seat << "\n";
        }
    }
    static void handle_global_remove(void *data, wl_registry *registry, uint32_t name) {
    }

    wl_registry_listener registry_listener = {
        handle_global,
        handle_global_remove
    };

    wl_display *display = nullptr;
    wl_registry *registry = nullptr;

    wl_surface *surface = nullptr;
    wl_seat *seat = nullptr;
    wl_compositor *compositor = nullptr;

    xdg_wm_base *xdg_base = nullptr;
};

// wl_registry_listener WaylandClient::registry_listener = {
//     handle_global,
//     handle_global_remove
// };

#endif
