#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include "include.hpp"

enum {
    N = 12, /* 
        THIS IS HOW FAR AWAY SNAPING WILL HAPPEN 
     */ 
    NC = 8 /* 
        THIS IS HOW FAR AWAY WINDOW TO WINDOW CORNER SNAPPING WILL HAPPEN 
     */
};

enum Direction {
    NEXT,
    PREV
};


struct size_pos {
    uint16_t x, y, width, height;
};

struct client {                  
	xcb_window_t win;
    xcb_window_t frame;            
	int16_t x, y;                   
	uint16_t width,height;          
	uint8_t  depth;  
    size_pos ogsize;
    bool ismax;
    uint16_t desktop;
};

struct win_data {
    xcb_window_t win;
    uint16_t x, y, width, height;
};

struct desktop {
    std::vector<client *> current_clients;
    uint16_t desktop;
    const uint16_t x = 0;
    const uint16_t y = 0;
    uint16_t width;
    uint16_t height;
};

#endif