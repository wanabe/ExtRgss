#include "ext_rgss.h"
#include "graphics.h"
#include "bitmap.h"
#include "rect.h"
#include "window.h"

VALUE mExtRgss;

VALUE old_call(VALUE self, ID mid, int argc, VALUE *argv) {
  return rb_funcall2(rb_ivar_get(self, rb_intern("@old")), mid, argc, argv);
}

void Init_ExtRgss() {
  mExtRgss = rb_define_module("ExtRgss");
  Init_ExtGraphics();
  Init_ExtBitmap();
  Init_ExtSprite();
  Init_ExtRect();
  Init_ExtWindow();
}
