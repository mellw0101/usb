#include "structs.hpp"
#define main_cpp
#include "include.hpp"

Logger log;
XCB xcb;

std::vector<client *> client_list; // global list of clients
std::vector<desktop *> desktop_list;

desktop * cur_d;
client * focused_client;
win_data data;

static xcb_connection_t * conn;
static xcb_ewmh_connection_t * ewmh; 
static const xcb_setup_t * setup;
static xcb_screen_iterator_t iter;
static xcb_screen_t * screen;

namespace get {
    client * 
    client_from_win(const xcb_window_t * w) 
    {
        for (const auto & c : client_list) 
        {
            if (* w == c->win) 
            {
                return c;
            }
        }
        return nullptr; /*

            RETURN 'nullptr' BECAUSE THE
            WINDOW DOES NOT BELONG TO ANY 
            CLIENT IN THE CLIENT LIST
         */ 
    }
}

xcb_atom_t
get_atom(const char * atom_name) 
{
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom
    (
        conn, 
        0, 
        strlen(atom_name), 
        atom_name
    );
    
    xcb_intern_atom_reply_t * reply = xcb_intern_atom_reply(conn, cookie, NULL);
    
    if (!reply) 
    {
        return XCB_ATOM_NONE;
    } 

    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

class wm {
    public:
        static void 
        focus_input(client * & c)
        {
            if (!c)
            {
                LOG_warning("client was nullptr");
                return;
            }
            xcb_set_input_focus
            (
                conn, 
                XCB_INPUT_FOCUS_POINTER_ROOT, 
                c->win, 
                XCB_CURRENT_TIME
            );
            flush_server(__func__);
        }

        static void 
        unfocus_client(client * c)
        {
            // LOG_func
            if (c == nullptr)
            {
                LOG_warning("client was nullptr");
                return;
            }
            xcb_unmap_window(conn, c->win);
            xcb_map_window(conn, c->win);
        }

        static void 
        focus_client(client * & c)
        {    
            // LOG_func
            if (c == nullptr)
            {
                LOG_warning("client was nullptr");
                return;
            }
            raise_client(c);
            focus_input(c);
            focused_client = c;
        }

        static void 
        setWindowSize(client * c) 
        {
            xcb_configure_window
            (
                conn, 
                c->win, 
                XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, 
                (const uint32_t[2])
                {
                    static_cast<const uint32_t>(c->width), 
                    static_cast<const uint32_t>(c->height)
                }
            );
            xcb_flush(conn);
        }

        static void 
        setWindowPosition(client * c) 
        {
            xcb_configure_window
            (
                conn, 
                c->win, 
                XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, 
                (const uint32_t[2])
                {
                    static_cast<uint32_t>(c->x), 
                    static_cast<uint32_t>(c->y)           
                }
            );
            xcb_flush(conn);
        }

        static void
        launchTerminal() 
        {
            if (fork()) 
            {
                return;
            }
            setsid();
            execlp("/usr/bin/konsole", "/usr/bin/konsole");
        }

        static void
        kill_session()
        {
            if (fork() == 0) 
            {
                setsid();
                xcb_disconnect(conn);
                execlp("/bin/mwm-KILL", "mwm-KILL", (char *)nullptr);
                exit(1); // Exit if exec fails
            }
        }

        static void 
        flush_server(const char * function_name)
        {
            const uint8_t & status = xcb_flush(conn);
            if (status == 0) 
            {
                Log::ERROR(function_name, "]:[Failed to flush server");
                return;
            }
        }

        static void 
        raise_client(client * & c) 
        {
            uint32_t values[1] = 
            {
                XCB_STACK_MODE_ABOVE
            };
            xcb_configure_window
            (
                conn,
                c->win,
                XCB_CONFIG_WINDOW_STACK_MODE, 
                values
            );
            flush_server(__func__);
        }

        static void
        save_ogsize(client * & c)
        {
            c->ogsize.x         = c->x; 
            c->ogsize.y         = c->y;
            c->ogsize.width     = c->width;
            c->ogsize.height    = c->height;
        }

        /* TODO */
        // MUST CHECK VALUES SOMETHING IS OFF
        static void 
        update_client(client * & c) 
        {
            if (c == nullptr)
            {
                return;
            }

            uint16_t x, y, width, height;
            getWindowSize(c->win, x, y, width, height);
            c->x        = x;
            c->y        = y;
            c->width    = width;
            c->height   = height;
        }

        static void 
        getWindowSize(xcb_window_t window, uint16_t & x, uint16_t & y, uint16_t & width, uint16_t & height) 
        {
            xcb_get_geometry_cookie_t geometry_cookie = xcb_get_geometry(conn, window);
            xcb_get_geometry_reply_t * geometry = xcb_get_geometry_reply(conn, geometry_cookie, nullptr);

            if (geometry) 
            {
                x = geometry->x;
                y = geometry->y;
                width = geometry->width;
                height = geometry->height;
                free(geometry);
            } 
            else 
            {
                width = height = x = y = 200;
            }
        }

        static void
        ungrab_button(client * c, const uint8_t & mouse_button, const uint16_t & modifiers)
        {
            xcb_ungrab_button(
                conn,
                mouse_button, // Button to ungrab (e.g., left mouse button)
                c->win,
                modifiers // Modifiers to ungrab (e.g., Shift, Control, Alt, Super)
            );
            xcb_flush(conn); // Flush the request to the X server
        }

        static void
        grab_button(client * c, const uint8_t & mouse_button, const uint16_t & modifiers) 
        {
            LOG_func
            xcb_grab_button(
                conn, 
                1, // 'owner_events'. Set to 0 for no event propagation
                c->win, 
                XCB_EVENT_MASK_BUTTON_PRESS, // Event mask
                XCB_GRAB_MODE_ASYNC, // Pointer mode
                XCB_GRAB_MODE_ASYNC, // Keyboard mode
                XCB_NONE, // Confine to window: none
                XCB_NONE, // Cursor: none
                mouse_button, 
                modifiers
            );
            xcb_flush(conn); // Flush the request to the X server
        }

        static void
        cycle_focus()
        {
            bool focus = false;
            for (auto & c : client_list)
            {
                if (c)
                {
                    if (c == focused_client)
                    {
                        focus = true;
                        continue;
                    }
                    
                    if (focus)
                    {
                        focus_client(c);
                        return;  
                    }
                }
            }
        }
};

class focus_client {
    public:
        focus_client(client * & c) : c(c)
        {
            // LOG_func
            if (c == nullptr)
            {
                LOG_warning("client was nullptr");
                return;
            }
            raise_client(c);
            focus_input(c);
            focused_client = c;
        }

        focus_client(client * & c, bool is = true) : c(c)
        {
            // LOG_func
            if (c == nullptr)
            {
                LOG_warning("client was nullptr");
                return;
            }
            raise_client(c);
            focus_input(c);
            focused_client = c;
        }

    private:
        client * & c;

        void  
        raise_client(client * & c) 
        {
            uint32_t values[1] = 
            {
                XCB_STACK_MODE_ABOVE
            };
            xcb_configure_window
            (
                conn,
                c->win,
                XCB_CONFIG_WINDOW_STACK_MODE, 
                values
            );
            flush_server(__func__);
        }
        
        void 
        flush_server(const char * function_name)
        {
            const uint8_t & status = xcb_flush(conn);
            if (status == 0) 
            {
                Log::ERROR(function_name, "]:[Failed to flush server");
                return;
            }
        }

        void 
        focus_input(client * & c)
        {
            if (!c)
            {
                LOG_warning("client was nullptr");
                return;
            }
            xcb_set_input_focus
            (
                conn, 
                XCB_INPUT_FOCUS_POINTER_ROOT, 
                c->win, 
                XCB_CURRENT_TIME
            );
            flush_server(__func__);
        }
};

enum show_hide {
    SHOW,
    HIDE
};

void 
show_hide_client(client * c, const show_hide & mode) 
{
    switch (mode) 
    {
        case HIDE:
        {
            xcb_unmap_window(conn, c->win);
            break;
        }

        case SHOW:
        {
            xcb_map_window(conn, c->win);
            break;
        }
    }
    xcb_flush(conn);    
}

class mv_client {
    public:
        mv_client(client * & c, const uint16_t & start_x, const uint16_t & start_y) 
        : c(c), start_x(start_x), start_y(start_y)
        {
            log.log(FUNC, __func__);
            grab_pointer();

            run();
            xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
            xcb_flush(conn);
        }

    private:
        client * & c;
        const uint16_t & start_x;
        const uint16_t & start_y;
        bool shouldContinue = true;
        xcb_generic_event_t * ev;

        void 
        grab_pointer()
        {
            xcb_grab_pointer_cookie_t cookie = xcb_grab_pointer
            (
                conn,
                false, // owner_events: false to not propagate events to other clients
                c->win,
                XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION,
                XCB_GRAB_MODE_ASYNC,
                XCB_GRAB_MODE_ASYNC,
                XCB_NONE,
                XCB_NONE, 
                XCB_CURRENT_TIME
            );

            xcb_grab_pointer_reply_t * reply = xcb_grab_pointer_reply(conn, cookie, NULL);
            if (!reply || reply->status != XCB_GRAB_STATUS_SUCCESS) 
            {
                log.log(ERROR, __func__, "Could not grab pointer");
                free(reply);
                return;
            }
            free(reply); 
        }
    
        void 
        move_client(const uint16_t & x, const uint16_t & y)
        {
            xcb_configure_window
            (
                conn,
                c->win,
                XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                (const uint32_t[2])
                {
                    static_cast<const uint32_t>(x), 
                    static_cast<const uint32_t>(y)
                }
            );
        }

        /* DEFENITIONS TO REDUCE REDUNDENT CODE IN 'snap' FUNCTION */
        #define RIGHT   screen->width_in_pixels  - c->width
        #define BOTTOM  screen->height_in_pixels - c->height

        void 
        snap(const int16_t & x, const int16_t & y)
        {
            // WINDOW TO WINDOW SNAPPING 
            for (const auto & cli : cur_d->current_clients)
            {
                // SNAP WINDOW TO 'RIGHT' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((x > cli->x + cli->width - N && x < cli->x + cli->width + N) 
                    && (y + c->height > cli->y && y < cli->y + cli->height))
                {
                    // SNAP WINDOW TO 'RIGHT_TOP' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (y > cli->y - NC && y < cli->y + NC)
                    {  
                        move_client(cli->x + cli->width, cli->y);
                        return;
                    }

                    // SNAP WINDOW TO 'RIGHT_BOTTOM' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (y + c->height > cli->y + cli->height - NC && y + c->height < cli->y + cli->height + NC)
                    {
                        move_client(cli->x + cli->width, (cli->y + cli->height) - c->height);
                        return;
                    }

                    move_client(cli->x + cli->width, y);
                    return;
                }

                // SNAP WINSOW TO 'LEFT' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((x + c->width > cli->x - N && x + c->width < cli->x + N) 
                    && (y + c->height > cli->y && y < cli->y + cli->height))
                {
                    // SNAP WINDOW TO 'LEFT_TOP' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (y > cli->y - NC && y < cli->y + NC)
                    {  
                        move_client(cli->x - c->width, cli->y);
                        return;
                    }

                    // SNAP WINDOW TO 'LEFT_BOTTOM' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (y + c->height > cli->y + cli->height - NC && y + c->height < cli->y + cli->height + NC)
                    {
                        move_client(cli->x - c->width, (cli->y + cli->height) - c->height);
                        return;
                    }                

                    move_client(cli->x - c->width, y);
                    return;
                }

                // SNAP WINDOW TO 'BOTTOM' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((y > cli->y + cli->height - N && y < cli->y + cli->height + N) 
                    && (x + c->width > cli->x && x < cli->x + cli->width))
                {
                    // SNAP WINDOW TO 'BOTTOM_LEFT' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (x > cli->x - NC && x < cli->x + NC)
                    {  
                        move_client(cli->x, cli->y + cli->height);
                        return;
                    }

                    // SNAP WINDOW TO 'BOTTOM_RIGHT' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (x + c->width > cli->x + cli->width - NC && x + c->width < cli->x + cli->width + NC)
                    {
                        move_client((cli->x + cli->width) - c->width, cli->y + cli->height);
                        return;
                    }

                    move_client(x ,cli->y + cli->height);
                    return;
                }

                // SNAP WINDOW TO 'TOP' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((y + c->height > cli->y - N && y + c->height < cli->y + N) 
                 && (x + c->width > cli->x && x < cli->x + cli->width))
                {
                    // SNAP WINDOW TO 'TOP_LEFT' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (x > cli->x - NC && x < cli->x + NC)
                    {  
                        move_client(cli->x, cli->y - c->height);
                        return;
                    }

                    // SNAP WINDOW TO 'TOP_RIGHT' CORNER OF NON_CONROLLED WINDOW WHEN APPROPRIET
                    if (x + c->width > cli->x + cli->width - NC && x + c->width < cli->x + cli->width + NC)
                    {
                        move_client((cli->x + cli->width) - c->width, cli->y - c->height);
                        return;
                    }

                    move_client(x ,cli->y - c->height);
                    return;
                }
            }

            // WINDOW TO EDGE OF SCREEN SNAPPING
            if (((x < N) && (x > -N)) && ((y < N) && (y > -N)))
            {
                move_client(0, 0);
            }
            else if ((x < RIGHT + N && x > RIGHT - N) && (y < N && y > -N))
            {
                move_client(RIGHT, 0);
            }
            else if ((y < BOTTOM + N && y > BOTTOM - N) && (x < N && x > -N))
            {
                move_client(0, BOTTOM);
            }
            else if ((x < N) && (x > -N))
            { 
                move_client(0, y);
            }
            else if (y < N && y > -N)
            {
                move_client(x, 0);
            }
            else if ((x < RIGHT + N && x > RIGHT - N) && (y < BOTTOM + N && y > BOTTOM - N))
            {
                move_client(RIGHT, BOTTOM);
            }
            else if ((x < RIGHT + N) && (x > RIGHT - N))
            { 
                move_client(RIGHT, y);
            }
            else if (y < BOTTOM + N && y > BOTTOM - N)
            {
                move_client(x, BOTTOM);
            }
            else 
            {
                move_client(x, y);
            }
        }

        void
        run() /*
            THIS IS THE MAIN EVENT LOOP FOR 'mv_client'
         */ 
        {
            while (shouldContinue) 
            {
                ev = xcb_wait_for_event(conn);
                if (!ev) 
                {
                    continue;
                }

                switch (ev->response_type & ~0x80) 
                {
                    case XCB_MOTION_NOTIFY: 
                    {
                        const auto * e = reinterpret_cast<const xcb_motion_notify_event_t *>(ev);
                        const int16_t & new_x = e->root_x - start_x;
                        const int16_t & new_y = e->root_y - start_y;
                        
                        if (isTimeToRender())
                        {
                            snap(new_x, new_y);
                            xcb_flush(conn);
                        }
                        break;
                    }
                    case XCB_BUTTON_RELEASE:
                    {
                        shouldContinue = false;
                        wm::update_client(c);
                        break;
                    }
                }
                free(ev);
            }
        }
        
        const double frameRate = 120.0; /* 
            FRAMERATE 
         */
        std::chrono::high_resolution_clock::time_point lastUpdateTime = std::chrono::high_resolution_clock::now(); /*
            HIGH_PRECISION_CLOCK AND TIME_POINT 
         */
        const double frameDuration = 1000.0 / frameRate; /* 
            DURATION IN MILLISECONDS THAT EACH FRAME SHOULD LAST 
        */

        bool 
        isTimeToRender() 
        {
            auto currentTime = std::chrono::high_resolution_clock::now(); /*
                CALCULATE ELAPSED TIME SINCE THE LAST UPDATE
             */ 
            const std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastUpdateTime;

            if (elapsedTime.count() >= frameDuration) /*
                CHECK IF THE ELAPSED TIME EXCEEDS THE FRAME DURATION
             */ 
            {
                
                lastUpdateTime = currentTime; /*
                    UPDATE THE LAST_UPDATE_TIME TO THE 
                    CURRENT TIME FOR THE NEXT CHECK
                 */ 
                return true; /*
                    RETURN TRUE IF IT'S TIME TO RENDER
                 */ 
            }
            return false; /*
                RETURN FALSE IF NOT ENOUGH TIME HAS PASSED
             */ 
        }
};

void 
move_desktop(const uint8_t & n)
{
    if (cur_d == desktop_list[n - 1])
    {
        return;
    }
    
    for (const auto & c : cur_d->current_clients)
    {
        if (c)
        {
            if (c->desktop == cur_d->desktop)
            {
                show_hide_client(c, HIDE);
            }
        }
    }

    cur_d = desktop_list[n - 1];
    for (const auto & c : cur_d->current_clients)
    {
        if (c)
        {
            show_hide_client(c, SHOW);           
        }
    }
}

void
move_to_next_desktop()
{
    LOG_func
    if (cur_d->desktop == desktop_list.size())
    {
        return;
    }

    move_desktop(cur_d->desktop + 1);
}

void
move_to_previus_desktop()
{
    LOG_func
    if (cur_d->desktop == 1)
    {
        return;
    }

    move_desktop(cur_d->desktop - 1);
}

void
move_to_next_desktop_w_app()
{
    LOG_func
    if (cur_d->desktop == desktop_list.size())
    {
        return;
    }

    if (focused_client)
    {
        focused_client->desktop = cur_d->desktop + 1;
    }

    move_desktop(cur_d->desktop + 1);
}

void
move_to_previus_desktop_w_app()
{
    LOG_func
    if (cur_d->desktop == 1)
    {
        return;
    }

    if (focused_client)
    {
        focused_client->desktop = cur_d->desktop - 1;
    }

    move_desktop(cur_d->desktop - 1);
    wm::raise_client(focused_client);
}

class resize_client {
    public:
        /* 
            THE REASON FOR THE 'retard_int' IS BECUSE WITHOUT IT 
            I CANNOT CALL THIS CLASS LIKE THIS 'resize_client(c)' 
            INSTEAD I WOULD HAVE TO CALL IT LIKE THIS 'resize_client rc(c)'
            AND NOW WITH THE 'retard_int' I CAN CALL IT LIKE THIS 'resize_client(c, 0)'
            */
        resize_client(client * & c , int retard_int) 
        : c(c) 
        {
            grab_pointer();
            
            teleport_mouse(c->x + c->width, c->y + c->height); /* 
                MOVE THE MOUSE TO THE BOTTOM-RIGHT CORNER OF THE WINDOW
             */
            run();
            
            // Release the pointer grab and flush the connection
            xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
            xcb_flush(conn);
        }

    private:
        client * & c;

        void
        grab_pointer()
        {
            // Grab the pointer to track mouse events for window resizing
            xcb_grab_pointer_cookie_t cookie = xcb_grab_pointer
            (
                conn,
                false, // owner_events: false to not propagate events to other clients
                c->win,
                XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION,
                XCB_GRAB_MODE_ASYNC,
                XCB_GRAB_MODE_ASYNC,
                XCB_NONE,
                XCB_NONE,
                XCB_CURRENT_TIME
            );

            // CHECK IF THE POINTER GRAB WAS SUCCESSFULL
            xcb_grab_pointer_reply_t* reply = xcb_grab_pointer_reply(conn, cookie, NULL);
            if (!reply || reply->status != XCB_GRAB_STATUS_SUCCESS) 
            {
                LOG_error("Could not grab pointer");
                free(reply);
                return;
            }
            free(reply);
        }

        void 
        teleport_mouse(const uint16_t & x, const uint16_t & y) 
        {
            xcb_warp_pointer
            (
                conn, 
                XCB_NONE, 
                screen->root, 
                0, 
                0, 
                0, 
                0, 
                x, 
                y
            );
            xcb_flush(conn);
        }

        void
        resize_win(const uint16_t & width, const uint16_t & height)
        {
            // CONFIGURE THE WINDOW WITH THE NEW WIDTH AND HEIGHT
            xcb_configure_window 
            (
                conn,
                c->win,
                XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                (const uint32_t[2])
                {
                    static_cast<const uint32_t &>(width), 
                    static_cast<const uint32_t &>(height)
                }
            );
        }

        void
        snap(const uint16_t & x, const uint16_t & y)
        {
            // WINDOW TO WINDOW SNAPPING 
            for (const auto & cli : cur_d->current_clients)
            {
                
                // SNAP WINSOW TO 'LEFT' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((x > cli->x - N && x < cli->x + N) 
                 && (y + c->height > cli->y && y < cli->y + cli->height))
                {
                    resize_win(cli->x - c->x, y - c->y);
                    return;
                }

                // SNAP WINDOW TO 'TOP' BORDER OF 'NON_CONTROLLED' WINDOW
                if ((y > cli->y - N && y < cli->y + N) 
                 && (x + c->width > cli->x && x < cli->x + cli->width))
                {
                    resize_win(x - c->x ,cli->y - c->y);
                    return;
                }
            }
            resize_win(x - c->x, y - c->y);
        }

        void /* 
            THIS IS THE MAIN EVENT LOOP FOR 'resize_client'
         */
        run()
        {
            xcb_generic_event_t * ev;
            bool shouldContinue = true;

            // Wait for motion events and handle window resizing
            while (shouldContinue) 
            {
                ev = xcb_wait_for_event(conn);
                if (!ev) 
                {
                    continue;
                }

                switch (ev->response_type & ~0x80) 
                {
                    case XCB_MOTION_NOTIFY: 
                    {
                        const auto * e = reinterpret_cast<const xcb_motion_notify_event_t *>(ev);
                        if (isTimeToRender())
                        {
                            snap(e->root_x, e->root_y);
                            xcb_flush(conn); 
                        }
                        break;
                    }
                    case XCB_BUTTON_RELEASE: 
                    {
                        shouldContinue = false;                        
                        wm::update_client(c); 
                        break;
                    }
                }
                // Free the event memory after processing
                free(ev); 
            }
        }

        /* FRAMERATE */
        const double frameRate = 120.0;

        /* HIGH_PRECISION_CLOCK AND TIME_POINT */
        std::chrono::high_resolution_clock::time_point lastUpdateTime = std::chrono::high_resolution_clock::now();
        
        /* DURATION IN MILLISECONDS THAT EACH FRAME SHOULD LAST */
        const double frameDuration = 1000.0 / frameRate; 
        
        bool 
        isTimeToRender() 
        {
            // CALCULATE ELAPSED TIME SINCE THE LAST UPDATE
            const auto & currentTime = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double, std::milli> & elapsedTime = currentTime - lastUpdateTime;

            /*
                CHECK IF THE ELAPSED TIME EXCEEDS THE FRAME DURATION
             */ 
            if (elapsedTime.count() >= frameDuration) 
            {
                // UPDATE THE LAST_UPDATE_TIME TO THE 
                // CURRENT TIME FOR THE NEXT CHECK
                lastUpdateTime = currentTime; 
                
                // RETURN TRUE IF IT'S TIME TO RENDER
                return true; 
            }
            // RETURN FALSE IF NOT ENOUGH TIME HAS PASSED
            return false; 
        }
};

namespace borrowed {
    bool
    getgeom(const xcb_drawable_t * win, int16_t * x, int16_t * y, uint16_t * width, uint16_t * height, uint8_t * depth)
    {
        xcb_get_geometry_reply_t * geom = xcb_get_geometry_reply(conn,
                xcb_get_geometry(conn, * win), NULL);

        if (NULL == geom)
            return false;

        * x = geom->x;
        * y = geom->y;
        * width = geom->width;
        * height = geom->height;
        * depth = geom->depth;

        free(geom);
        return true;
    }
    
    void
    moveresize(xcb_drawable_t win, const uint16_t & x, const uint16_t & y, const uint16_t & width, const uint16_t & height)
    {
        if (win == screen->root || win == 0)
        {
            return;
        }
            
        xcb_configure_window
        (
            conn,
            win,
            XCB_CONFIG_WINDOW_WIDTH | 
            XCB_CONFIG_WINDOW_HEIGHT,
            (const uint32_t[2])
            {
                static_cast<const uint32_t &>(width),
                static_cast<const uint32_t &>(height)
            }
        );

        xcb_configure_window
        (
            conn,
            win,
            XCB_CONFIG_WINDOW_X |
            XCB_CONFIG_WINDOW_Y,
            (const uint32_t[2])
            {
                static_cast<const uint32_t &>(x),
                static_cast<const uint32_t &>(y)
            }
        );
        xcb_flush(conn);
    }

    void
    unmax(client * c)
    {
        if (c == nullptr)
        {
            return;
        }

        c->x        = c->ogsize.x;
        c->y        = c->ogsize.y;
        c->width    = c->ogsize.width;
        c->height   = c->ogsize.height;
        c->ismax    = false;

        moveresize
        (
            c->win,
            c->x, 
            c->y,
            c->width,
            c->height
        );
    }

    void
    unmaxwin(client * c)
    {
        unmax(c);
        xcb_change_property
        (
            conn,
            XCB_PROP_MODE_REPLACE,
            c->win,
            ewmh->_NET_WM_STATE, 
            XCB_ATOM_ATOM,
            32,
            0,
            0
        );
    }

    void 
    maxwin(client * c, const uint8_t & with_offsets) 
    {
        if (c == nullptr)
        {
            LOG_error("client is nullptr");
            return;
        }
        if (c->ismax)
        {
            unmaxwin(c);
            return;
        }

        const uint16_t & mon_x      = 0; 
        const uint16_t & mon_y      = 0;
        const uint16_t & mon_width  = screen->width_in_pixels;
        const uint16_t & mon_height = screen->height_in_pixels;

        show_hide_client(c, HIDE);
        wm::save_ogsize(c);

        moveresize
        (
            c->win, 
            mon_x, 
            mon_y, 
            mon_width, 
            mon_height
        );
        wm::update_client(c);

        if (!with_offsets) 
        {
            xcb_change_property
            (
                conn,
                XCB_PROP_MODE_REPLACE,
                c->win,
                ewmh->_NET_WM_STATE,
                XCB_ATOM_ATOM,
                32,
                1,
                &ewmh->_NET_WM_STATE_FULLSCREEN
            );
        }
        c->ismax = true;
        xcb_flush(conn);
        show_hide_client(c, SHOW);
        wm::focus_client(c);
    }
}

std::string
get_name(client * c)
{
    log.log(FUNC, __func__);
    xcb_get_property_reply_t * reply;
    unsigned int reply_len;
    char * name;

    reply = xcb_get_property_reply
    (
        conn, 
        xcb_get_property
        (
            conn, 
            false,
            c->win, 
            get_atom
            (
                "WM_NAME"
            ),
            XCB_GET_PROPERTY_TYPE_ANY, 
            0,
            60
        ), 
        NULL
    );

    if (!reply || xcb_get_property_value_length(reply) == 0)
    {
        if (reply != nullptr)
        {
            log.log(ERROR, __func__, "reply length is = 0");
            free(reply);
            return "";
        }

        log.log(ERROR, __func__, "reply == nullptr");
        return "";
    }

    reply_len = xcb_get_property_value_length(reply);
	name = static_cast<char *>(malloc(sizeof(char) * (reply_len+1)));
	memcpy(name, xcb_get_property_value(reply), reply_len);
	name[reply_len] = '\0';

    if (reply)
    {
        free(reply);
    }

    log.log(INFO, __func__, "name = " + std::string(name)); 
    std::string sname = std::string(name);
    free(name);

    return sname;
}

class WinManager {
    public:
        static void 
        manage_new_window(const xcb_drawable_t & w) 
        {
            client * c = make_client(w);
            if (!c)
            {
                return;
            }
            
            wm::setWindowPosition(c);
            wm::setWindowSize(c);

            // MAP THE WINDOW
            XCB_map_window(w);
            // xcb_map_window(conn, c->win);  
            xcb_flush(conn);

            apply_event_mask(c);

            // GRAB MOUSE BUTTONS FOR THE WINDOW 
            // SO WINDOW CAN BE INTERACTED WITH
            grab_buttons(c, 
            {
                {   L_MOUSE_BUTTON,     ALT },
                {   R_MOUSE_BUTTON,     ALT }
            });
            
            // GRAB KEYS FOR THE WINDOW SO THAT KEYBINDINGS WORK
            grab_keys(c, 
            {
                {   T,          ALT | CTRL              },
                {   Q,          ALT | SHIFT             },
                {   F11,        NULL                    },
                {   N_1,        ALT                     },
                {   N_2,        ALT                     },
                {   N_3,        ALT                     },
                {   N_4,        ALT                     },
                {   N_5,        ALT                     },
                {   R_ARROW,    CTRL | SUPER            },
                {   L_ARROW,    CTRL | SUPER            },
                {   R_ARROW,    CTRL | SUPER | SHIFT    },
                {   L_ARROW,    CTRL | SUPER | SHIFT    },
                {   TAB,        ALT                     }
            });

            // make_frame(c);
            get_name(c);
            wm::focus_client(c);
        }

    private:
        static void
        grab_buttons(client * c, std::initializer_list<std::pair<const uint8_t, const uint16_t>> bindings)
        {
            for (const auto & binding : bindings)
            {
                const uint8_t & button = binding.first;
                const uint16_t & modifier = binding.second;
                xcb_grab_button
                (
                    conn, 
                    
                    // 'OWNER_EVENTS : SET TO 0 FOR NO EVENT PROPAGATION'
                    1, 
                    c->win, 
                    
                    // EVENT MASK
                    XCB_EVENT_MASK_BUTTON_PRESS, 
                    
                    // POINTER MODE
                    XCB_GRAB_MODE_ASYNC, 
                    
                    // KEYBOARD MODE
                    XCB_GRAB_MODE_ASYNC, 
                    
                    // CONFINE TO WINDOW
                    XCB_NONE, 
                    
                    // CURSOR
                    XCB_NONE, 
                    button, 
                    modifier    
                );
                // FLUSH THE REQUEST TO THE X SERVER
                xcb_flush(conn); 
            }
        }

        static void 
        grab_keys(client * c, std::initializer_list<std::pair<const uint32_t, const uint16_t>> bindings) 
        {
            xcb_key_symbols_t * keysyms = xcb_key_symbols_alloc(conn);
        
            if (!keysyms) 
            {
                LOG_error("keysyms could not get initialized");
                return;
            }

            for (const auto & binding : bindings) 
            {
                xcb_keycode_t * keycodes = xcb_key_symbols_get_keycode(keysyms, binding.first);

                if (keycodes)
                {
                    for (auto * kc = keycodes; * kc; kc++) 
                    {
                        xcb_grab_key
                        (
                            // CONNECTION TO THE X SERVER
                            conn,
                            
                            // 'OWNER_EVENTS' SET TO 1 TO ALLOW EVENT PROPAGATION 
                            1,

                            // KEYS WILL BE GRABBED ON THIS WINDOW          
                            c->win,
                            
                            // MODIFIER MASK
                            binding.second, 
                            
                            // KEYCODE
                            *kc,        

                            // POINTER MODE
                            XCB_GRAB_MODE_ASYNC, 
                            
                            // KEYBOARD MODE
                            XCB_GRAB_MODE_ASYNC  
                        );
                    }
                    // FREE THE MEMORY THAT WAS ALLOCATED FOR THE KEYCODES
                    free(keycodes);
                }
            }
            xcb_key_symbols_free(keysyms);
            
            // FLUSH THE REQUEST TO THE X SERVER
            // SO THAT THE X SERVER HANDELS THIS REQUEST NOW
            xcb_flush(conn); 
        }

        static void
        apply_event_mask(client * c)
        {
            xcb_change_window_attributes
            (
                conn,
                c->win,
                XCB_CW_EVENT_MASK,
                (const uint32_t[1])
                {
                    XCB_EVENT_MASK_FOCUS_CHANGE
                }
            );

        }

        static bool 
        is_exclusive_fullscreen(client * c) 
        {
            xcb_ewmh_get_atoms_reply_t wm_state;
            if (xcb_ewmh_get_wm_state_reply(ewmh, xcb_ewmh_get_wm_state(ewmh, c->win), & wm_state, NULL)) 
            {
                for (unsigned int i = 0; i < wm_state.atoms_len; i++) 
                {
                    if ((wm_state.atoms[i] == ewmh->_NET_WM_STATE_FULLSCREEN) 
                    && (i + 1 < wm_state.atoms_len) 
                    && (wm_state.atoms[i + 1] == XCB_NONE)) 
                    {
                        // EXCLUSIVE FULLSCREEN STATE DETECTED
                        xcb_ewmh_get_atoms_reply_wipe(& wm_state);
                        return true;
                    }
                }
                xcb_ewmh_get_atoms_reply_wipe(& wm_state);
            }
            return false;
        }

        static client *
        make_client(const xcb_window_t & win) 
        {
            client * c = static_cast<client *>(malloc(sizeof(client)));
            
            if (!c) 
            {
                log.log(ERROR, __func__, "Could not allocate memory for client");
                return nullptr;
            }

            c->win      = win;
            c->height   = (data.height < 300) ? 300 : data.height;
            c->width    = (data.width < 400)  ? 400 : data.width;
            c->x        = (data.x <= 0)       ? (screen->width_in_pixels / 2)  - (c->width / 2)  : data.x;
            c->y        = (data.y <= 0)       ? (screen->height_in_pixels / 2) - (c->height / 2) : data.y;
            
            // DEFAULT DEPTH IS 24 BIT
            // THIS CAN BE ADJUSTED IF NESSESARY
            c->depth    = 24; 
            c->desktop  = cur_d->desktop;

            if (is_exclusive_fullscreen(c)) 
            {
                c->x = 0;
                c->y = 0;
                c->width = screen->width_in_pixels;
                c->height = screen->height_in_pixels;
                c->ismax = true;
            }
            else 
            {
                c->ismax = false;
            }
        
            client_list.push_back(c);
            cur_d->current_clients.push_back(c);
            return c;
        }

        static void 
        make_frame(client * c)
        {
            // CREATE A FRAME WINDOW
            c->frame = xcb_generate_id(conn);
            xcb_create_window
            (
                conn, 
                XCB_COPY_FROM_PARENT, 
                c->frame, 
                screen->root, 
                0, 
                0, 
                c->width, 
                c->height + 20, 
                0, 
                XCB_WINDOW_CLASS_INPUT_OUTPUT, 
                screen->root_visual, 
                0, 
                NULL
            );

            // SET THE BACKGROUND COLOR TO WHITE
            xcb_change_window_attributes
            (
                conn, 
                c->frame, 
                XCB_CW_BACK_PIXEL, 
                (const uint32_t[1])
                {
                    screen->white_pixel
                }
            );
            
            // REPARENT THE PROGRAM_WINDOW TO THE FRAME_WINDOW
            xcb_reparent_window
            (
                conn, 
                c->win, 
                c->frame, 
                0, 
                20
            ); 
            
            // MAP THE FRAME WINDOW SO IT BECOMES VISIBLE
            xcb_map_window(conn, c->frame);

            // FLUSH THE REQUEST TO THE X SERVER 
            // FOR EMIDIET HANDELING OF THE REQUEST
            xcb_flush(conn); 
        }
};

class Event {
    public:
        Event()
        {
            initialize_keysyms();
        }

        ~Event() 
        {
            if (keysyms) 
            {
                xcb_key_symbols_free(keysyms);
            }
        }

        void 
        handler(const xcb_generic_event_t * ev)
        {
            switch (ev->response_type & ~0x80) 
            {
                case XCB_KEY_PRESS: 
                {
                    key_press_handler(ev);
                    break;
                }
                
                case XCB_MAP_NOTIFY: 
                {
                    map_notify_handler(ev);
                    break;
                }
                
                case XCB_MAP_REQUEST: 
                {
                    map_req_handler(ev);
                    break;
                }
                
                case XCB_BUTTON_PRESS: 
                {
                    button_press_handler(ev);
                    break;
                }

                case XCB_CONFIGURE_REQUEST:
                {
                    configure_request_handler(ev);
                    break;
                }

                case XCB_FOCUS_IN:
                {
                    focus_in_handler(ev);
                    break;
                }

                case XCB_FOCUS_OUT:
                {
                    focus_out_handler(ev);
                    break;
                }

                /* 
                    TODO 
                    THIS ACTS WIERDLY   
                 */
                case XCB_CLIENT_MESSAGE: 
                {
                    const auto & e = reinterpret_cast<const xcb_client_message_event_t *>(ev);
                    log_event_response_typ();
                    break;
                }

                case XCB_DESTROY_NOTIFY:
                {
                    destroy_notify_handler(ev);
                    break;
                }
            }
        }

    private:
        xcb_key_symbols_t * keysyms;
        /*
            VARIABELS TO STORE KEYCODES
         */ 
        xcb_keycode_t t{},q{},f{},f11{},n_1{},n_2{},n_3{},n_4{},n_5{},r_arrow{},l_arrow{},tab{}; 
        
        void 
        key_press_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_key_press_event_t *>(ev);
            
            /*
                CHECK IF 'ALT+CTRL+T' WAS PRESSED
                AND IF SO LAUNCH TERMINAL   
             */         
            if ((e->detail == t) 
             && (e->state & CTRL) 
             && (e->state & ALT)) 
            {
                log.log(INFO, __func__, "ALT+CTRL+T");
                wm::launchTerminal();
            }

            /*
                CHECK IF 'ALT+SHIFT+Q' WAS PRESSED
                AND IF SO LAUNCH KILL_SESSION
             */ 
            if ((e->detail == q) 
             && (e->state & SHIFT) 
             && (e->state & ALT)) 
            {
                log.log(INFO, __func__, "ALT+SHIFT+Q");
                wm::kill_session();
            }

            /*
                CHECK IF 'F11' WAS PRESSED
                AND IF SO TOGGLE FULLSCREEN
             */ 
            if (e->detail == f11)
            {
                log.log(INFO, __func__, "F11");
                client *c = get::client_from_win(&e->event);
                borrowed::maxwin(c, 0);
            }

            /*
                CHECK IF 'ALT+1' WAS PRESSED
                AND IF SO MOVE TO DESKTOP 1
             */
            if ((e->detail == n_1) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+1");
                move_desktop(1);
            }

            /*
                CHECK IF 'ALT+2' WAS PRESSED
                AND IF SO MOVE TO DESKTOP 2
             */ 
            if ((e->detail == n_2) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+2");
                move_desktop(2);
            }

            /*
                CHECK IF 'ALT+3' WAS PRESSED
                AND IF SO MOVE TO DESKTOP 3
             */ 
            if ((e->detail == n_3) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+3");
                move_desktop(3);
            }

            /*
                CHECK IF 'ALT+4' WAS PRESSED
                AND IF SO MOVE TO DESKTOP 4
             */
            if ((e->detail == n_4) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+4");
                move_desktop(4);
            }

            /*
                CHECK IF 'ALT+5' WAS PRESSED
                IF SO MOVE TO DESKTOP 5
             */
            if ((e->detail == n_5) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+5");
                move_desktop(5);
            }

            /*
                CHECK IF 'CTRL+SUPER+LEFT' OR 'CTRL+SUPER+SHIFT+LEFT' WAS PRESSED
                IF 'CTRL+SUPER+LEFT' WAS PRESSED MOVE TO PREVIOUS DESKTOP
                IF 'CTRL+SUPER+SHIFT+LEFT' WAS PRESSED MOVE TO PREVIOUS DESKTOP WITH FOCUSED APP
             */
            if ((e->detail == r_arrow) 
             && (e->state & CTRL | SUPER))
            {
                if (e->state & SHIFT)
                {
                    move_to_next_desktop_w_app();
                    return;
                }
                move_to_next_desktop();
            }

            /*
                CHECK IF 'CTRL+SUPER+RIGHT' OR 'CTRL+SUPER+SHIFT+RIGHT' WAS PRESSED
                IF 'CTRL+SUPER+RIGHT' WAS PRESSED MOVE TO NEXT DESKTOP
                IF 'CTRL+SUPER+SHIFT+RIGHT' WAS PRESSED MOVE TO NEXT DESKTOP WITH FOCUSED APP
             */
            if ((e->detail == l_arrow) 
             && (e->state & CTRL | SUPER))
            {
                if (e->state & SHIFT)
                {
                    move_to_previus_desktop_w_app();
                    return;
                }
                move_to_previus_desktop();
            }

            /*
                CHECK IF 'ALT+TAB' WAS PRESSED
                IF SO CYCLE FOCUS
             */
            if ((e->detail == tab) 
             && (e->state & ALT))
            {
                wm::cycle_focus();
            }
        }

        void /* 
            INITIALIZES KEYBOARD KEY SYMBOLS AND STORES 
            THE KEYCODES UNTIL SESSION IS KILLED 
         */
        initialize_keysyms() 
        {
            keysyms = xcb_key_symbols_alloc(conn);
            if (keysyms) 
            {
                xcb_keycode_t * t_keycodes          = xcb_key_symbols_get_keycode(keysyms, T);
                xcb_keycode_t * q_keycodes          = xcb_key_symbols_get_keycode(keysyms, Q);
                xcb_keycode_t * f_keycodes          = xcb_key_symbols_get_keycode(keysyms, F);
                xcb_keycode_t * f11_keycodes        = xcb_key_symbols_get_keycode(keysyms, F11);
                xcb_keycode_t * n_1_keycodes        = xcb_key_symbols_get_keycode(keysyms, N_1);
                xcb_keycode_t * n_2_keycodes        = xcb_key_symbols_get_keycode(keysyms, N_2);
                xcb_keycode_t * n_3_keycodes        = xcb_key_symbols_get_keycode(keysyms, N_3);
                xcb_keycode_t * n_4_keycodes        = xcb_key_symbols_get_keycode(keysyms, N_4);
                xcb_keycode_t * n_5_keycodes        = xcb_key_symbols_get_keycode(keysyms, N_5);
                xcb_keycode_t * r_arrow_keycodes    = xcb_key_symbols_get_keycode(keysyms, R_ARROW);
                xcb_keycode_t * l_arrow_keycodes    = xcb_key_symbols_get_keycode(keysyms, L_ARROW);
                xcb_keycode_t * tab_keycodes        = xcb_key_symbols_get_keycode(keysyms, TAB);
                
                if (t_keycodes) 
                {
                    t = * t_keycodes;
                    free(t_keycodes);
                }
                
                if (q_keycodes) 
                {
                    q = * q_keycodes;
                    free(q_keycodes);
                }
                
                if (f_keycodes)
                {
                    f = * f_keycodes;
                    free(f_keycodes);
                }

                if (f11_keycodes)
                {
                    f11 = * f11_keycodes;
                    free(f11_keycodes);
                }

                if (n_1_keycodes)
                {
                    n_1 = * n_1_keycodes;
                    free(n_1_keycodes);
                }

                if (n_2_keycodes)
                {
                    n_2 = * n_2_keycodes;
                    free(n_2_keycodes);
                }

                if (n_3_keycodes)
                {
                    n_3 = * n_3_keycodes;
                    free(n_3_keycodes);
                }

                if (n_4_keycodes)
                {
                    n_4 = * n_4_keycodes;
                    free(n_4_keycodes);
                }

                if (n_5_keycodes)
                {
                    n_5 = * n_5_keycodes;
                    free(n_5_keycodes);
                }

                if (r_arrow_keycodes)
                {
                    r_arrow = * r_arrow_keycodes;
                    free(r_arrow_keycodes);
                }

                if (l_arrow_keycodes)
                {
                    l_arrow = * l_arrow_keycodes;
                    free(l_arrow_keycodes);
                }

                if (tab_keycodes)
                {
                    tab = * tab_keycodes;
                    free(tab_keycodes);
                }
            }
        }
        
        void 
        map_notify_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_map_notify_event_t *>(ev);
            
            client * c = get::client_from_win(& e->event);
            if (c)
            {
                wm::update_client(c);
            }
        }
        
        void 
        map_req_handler(const xcb_generic_event_t * ev) 
        {
            const auto * e = reinterpret_cast<const xcb_map_request_event_t *>(ev);
            WinManager::manage_new_window( e->window);
        }
        
        void 
        button_press_handler(const xcb_generic_event_t * ev) 
        {
            const auto * e = reinterpret_cast<const xcb_button_press_event_t *>(ev);
            
            client * c = get::client_from_win(& e->event);
            if (!c)
            {
                log.log(ERROR, __func__, "c == null");
                return;
            }
            
            if ((e->detail == L_MOUSE_BUTTON) 
             && (e->event != screen->root) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+L_MOUSE_BUTTON");
                wm::raise_client(c);
                mv_client(c, e->event_x, e->event_y);
                wm::focus_client(c);
            }

            if ((e->detail == R_MOUSE_BUTTON) 
             && (e->event != screen->root) 
             && (e->state & ALT))
            {
                log.log(INFO, __func__, "ALT+R_MOUSE_BUTTON");
                wm::raise_client(c);
                resize_client(c, 0);
                wm::focus_client(c);
            }

            if ((e->detail == L_MOUSE_BUTTON) 
             && (e->event != screen->root) 
             && (c != focused_client))
            {
                log.log(INFO, __func__, "L_MOUSE_BUTTON");
                focus_client(c, 0);
            } 
        }

        void
        configure_request_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_configure_request_event_t *>(ev);
            data.width     = e->width;
            data.height    = e->height;
            data.x         = e->x;
            data.y         = e->y;
        }

        void
        focus_in_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_focus_in_event_t *>(ev);
            
            client * c = get::client_from_win(& e->event);
            if (!c)
            {
                return;
            }

            wm::ungrab_button(c, L_MOUSE_BUTTON, 0);
            focused_client = c;
        }

        void
        focus_out_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_focus_out_event_t *>(ev);
            
            client * c = get::client_from_win(& e->event);
            if (!c)
            {
                return;
            }
            
            if (c == focused_client)
            {
                return;
            }
            
            wm::grab_button(c, L_MOUSE_BUTTON, 0);
        }

        void
        destroy_notify_handler(const xcb_generic_event_t * ev)
        {
            const auto * e = reinterpret_cast<const xcb_destroy_notify_event_t *>(ev);

            client * c = get::client_from_win(& e->event);
            if (c)
            {
                free(c);
                focused_client = nullptr;
            }
        }
};

void /* 
    THIS IS THE MAIN EVENT LOOP
    FOR THE 'WINDOW_MANAGER' 
 */
run() 
{
    // INITIALIZES THE EVENT CLASS AS EVENT
    Event event;
    while (true)
    {
        xcb_generic_event_t * ev = xcb_wait_for_event(conn);
        
        if (!ev) 
        {
            continue;
        }

        // SEND THE EVENT TO THE EVENT HANDLER
        event.handler(ev);
        
        // DEALLOCATE THE MEMORY THE EVENT
        // USED AFTER IT HAS BEEN HANDLED
        free(ev);
    }
}

void /* 
    INITIALIZES AN EWMH CONNECTION 
 */
ewmh_init()
{
    if (!(ewmh = static_cast<xcb_ewmh_connection_t *>(calloc(1, sizeof(xcb_ewmh_connection_t)))))
    {
        LOG_error("ewmh faild to initialize");
        wm::kill_session();
    }    
    xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(conn, ewmh);
    if (!(xcb_ewmh_init_atoms_replies(ewmh, cookie, 0)))
    {
        LOG_error("xcb_ewmh_init_atoms_replies:faild")
        exit(1);
    }
}

void
make_desktop(const uint16_t & n)
{
    desktop * d = new desktop;
    d->desktop  = n;
    d->width    = screen->width_in_pixels;
    d->height   = screen->height_in_pixels;
    cur_d       = d;
    desktop_list.push_back(d);
}

void 
configureRootWindow() 
{
    // MAKE THE ROOT WINDOW THE SIZE OF THE SCREEN AND BLACK
    xcb_change_window_attributes
    (
        conn,
        screen->root,
        XCB_CW_BACK_PIXEL,
        (const uint32_t[1])
        {
            // SET THE BACKROUND TO BLACK
            screen->black_pixel 
        }
    );

    // APPLY THE EVENT MASKS TO THE ROOT WINDOW
    xcb_change_window_attributes
    (
        conn,
        screen->root,
        XCB_CW_EVENT_MASK,
        (const uint32_t[1])
        {
            XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT    | 
            XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY          | 
            XCB_EVENT_MASK_ENTER_WINDOW                 | 
            XCB_EVENT_MASK_LEAVE_WINDOW                 | 
            XCB_EVENT_MASK_STRUCTURE_NOTIFY             |
            XCB_EVENT_MASK_BUTTON_PRESS                 |
            XCB_EVENT_MASK_BUTTON_RELEASE               |
            XCB_EVENT_MASK_POINTER_MOTION               |
            XCB_EVENT_MASK_KEY_PRESS                    |
            XCB_EVENT_MASK_FOCUS_CHANGE                 |
            XCB_EVENT_MASK_KEY_RELEASE
        }
    );

    // CLEAR THE ROOT WINDOW TO APPLY THE CHANGES
    xcb_clear_area
    (
        conn, 
        0,
        screen->root,
        0, 
        0,
        screen->width_in_pixels,
        screen->height_in_pixels
    );

    // FLUSH TO MAKE X SERVER HANDEL REQUEST NOW
    xcb_flush(conn);
}

bool 
setSubstructureRedirectMask() 
{
    // ATTEMPT TO SET THE SUBSTRUCTURE REDIRECT MASK
    xcb_void_cookie_t cookie = xcb_change_window_attributes_checked
    (
        conn,
        screen->root,
        XCB_CW_EVENT_MASK,
        (const uint32_t[1])
        {
            XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
        }
    );

    // CHECK IF ANOTHER WINDOW MANAGER IS RUNNING
    xcb_generic_error_t * error = xcb_request_check(conn, cookie);
    if (error) 
    {
        LOG_error("Error: Another window manager is already running or failed to set SubstructureRedirect mask.") 
        free(error);
        return false;
    }
    return true;
}

void 
setDefaultCursor() 
{
    xcb_cursor_context_t *ctx;
    if (xcb_cursor_context_new(conn, screen, &ctx) < 0) 
    {
        LOG_error("Failed to create cursor context")
        return;
    }

    xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, "left_ptr");
    if (!cursor) 
    {
        LOG_error("Failed to load cursor")
        xcb_cursor_context_free(ctx);
        return;
    }

    xcb_change_window_attributes
    (
        conn,
        screen->root, 
        XCB_CW_CURSOR, 
        (const uint32_t[1])
        {
            cursor
        }
    );
    xcb_cursor_context_free(ctx);
}

const int8_t 
setup_wm()
{
    conn = xcb_connect(nullptr, nullptr);
    
    if (xcb_connection_has_error(conn)) 
    {
        LOG_error("Error opening XCB connection")
        return -1;
    }

    setup = xcb_get_setup(conn);
    iter = xcb_setup_roots_iterator(setup);
    screen = iter.data;

    setSubstructureRedirectMask(); 
    configureRootWindow();
    setDefaultCursor();

    ewmh_init();

    /* 
        MAKE ('5') DESKTOPS 
        
        THIS IS JUST FOR 
        DEBUGING AND TESTING 
        IN THE FUTURE I WILL IMPLEMENT
        A WAY TO ADD AND REMOVE 
        DESKTOPS DURING RUNTIME
     */
    make_desktop(1);
    make_desktop(2);
    make_desktop(3);
    make_desktop(4);
    make_desktop(5);

    move_desktop(1);
    return 0;
}

int
main() 
{
    LOG_start()
    const int8_t & err = setup_wm();
    if (err != 0)
    {
        return err;
    }
    run();
    xcb_disconnect(conn);
    return 0;
}