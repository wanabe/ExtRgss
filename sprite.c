#include "ext_rgss.h"

void Init_ExtSprite() {
  VALUE cOldSprite = rb_const_get(rb_cObject, rb_intern("Sprite"));
  VALUE cSprite = rb_define_class_under(mExtRgss, "Sprite", rb_cObject);
  rb_const_set(rb_cObject, rb_intern("OldSprite"), cOldSprite);
  rb_const_set(rb_cObject, rb_intern("Sprite"), cSprite);
}
