#ifndef XCB_HPP
#define XCB_HPP
#include "include.hpp"

class XCB {
    public:
        void
        map_window(xcb_connection_t * & conn, const xcb_window_t & win, const std::string & func)
        {
            cookie = xcb_map_window
            (
                conn, 
                win
            );
            
            error = xcb_request_check
            (
                conn, 
                cookie
            );
            if (error)
            {
                log.log(ERROR, func, "FAILED TO MAP WINDOW");
                free(error);
            }
        }

    private:
        Logger log;
        xcb_void_cookie_t cookie;
        xcb_generic_error_t * error ;
};

#endif