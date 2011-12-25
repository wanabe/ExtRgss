#include "ext_rgss.h"
#include "graphics.h"

VALUE mExtRgss;

void Init_ExtRgss() {
  mExtRgss = rb_define_module("ExtRgss");
  Init_ExtGraphics();
}
