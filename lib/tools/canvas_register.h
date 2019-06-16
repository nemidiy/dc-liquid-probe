#ifndef __GJ_UTILS_CANVAS_REGISTER_H__
#define __GJ_UTILS_CANVAS_REGISTER_H__

#include <screen.h>
#include <device.h>

namespace gj {

namespace utils {

struct CanvasRegister {

static Screen::canvas_func get_canvas_func(gj::atlas::Device* dev);
    
};

} // namespace utils

} // namespace gj

#endif