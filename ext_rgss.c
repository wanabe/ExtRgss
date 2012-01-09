#include "ext_rgss.h"
#include "graphics.h"
#include "bitmap.h"

VALUE mExtRgss;

void Init_ExtRgss() {
  mExtRgss = rb_define_module("ExtRgss");
  Init_ExtGraphics();
  Init_ExtBitmap();
  Init_ExtSprite();
}
