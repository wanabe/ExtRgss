#include <windows.h>
#include "rgss.h"

#ifndef DLL_NAME
#define DLL_NAME "ext_rgss.dll"
#endif

extern VALUE mExtRgss;
VALUE old_call(VALUE self, ID mid, int argc, VALUE *argv);
