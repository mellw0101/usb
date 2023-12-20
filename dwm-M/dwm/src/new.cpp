#include <xcb/xcb.h>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>


// Function to launch a terminal
void launchTerminal() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execlp("xterm", "xterm", NULL); // Replace "xterm" with your preferred terminal
        exit(EXIT_FAILURE); // Exit if exec fails
    } else if (pid > 0) {
        // Parent process
        wait( nullptr );                                    // Optionally wait for the terminal to close
    }
}


int main() {
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);
    if (xcb_connection_has_error(connection)) {
        fprintf(stderr, "Error opening display\n");
        exit(-1);
    }



    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    xcb_window_t window = xcb_generate_id(connection);
    uint32_t values[] = {screen->black_pixel, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS};
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, screen->width_in_pixels, screen->height_in_pixels, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                      XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);

    xcb_map_window(connection, window);

	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(connection);
    xcb_keycode_t *keycode_t = xcb_key_symbols_get_keycode(keysyms, XK_t);

    while (*keycode_t) {
        xcb_grab_key(connection, 1, screen->root, XCB_MOD_MASK_ANY, *keycode_t, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
        keycode_t++;
    }
    xcb_flush(connection);

    xcb_flush(connection);

    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE:
                // Handle expose event
                break;
			case XCB_MOTION_NOTIFY:
            {
                auto *motion = (xcb_motion_notify_event_t *)event;
            }
            break;
			case XCB_KEY_PRESS:
			{
				auto *kp = (xcb_key_press_event_t *)event;
				if (kp->detail == *keycode_t && (kp->state & XCB_MOD_MASK_CONTROL) && (kp->state & XCB_MOD_MASK_1)) {
					launchTerminal();
				}
			}
			break;
        }
        free(event);
    }

    xcb_disconnect(connection);
    return 0;
}
