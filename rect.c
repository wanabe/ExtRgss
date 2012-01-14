#include "ext_rgss.h"
#include "graphics.h"
#include "sprite.h"

VALUE cRect;
static VALUE (*Rect_set_)(int, VALUE*, VALUE);

VALUE Rect_set(int argc, VALUE *argv, VALUE self) {
  VALUE refs = rb_ivar_get(self, rb_intern("@refs"));
  RgssRect *rect = RGSS_RECT(self);

  Rect_set_(argc, argv, self);
  if(argc == 1) {
    RgssRect *src = RGSS_RECT(argv[0]);
    rect->x = src->x;
    rect->y = src->y;
    rect->w = src->w;
    rect->h = src->h;
  } else {
    VALUE x, y, w, h;
    rb_scan_args(argc, argv, "4", &x, &y, &w, &h);
    rect->x = FIX2LONG(x);
    rect->y = FIX2LONG(y);
    rect->w = FIX2LONG(w);
    rect->h = FIX2LONG(h);
  }

  if(RTEST(refs)) {
    switch(TYPE(refs)) {
    case T_DATA:
      if(RTYPEDDATA_TYPE(refs) == &Sprite_data_type) {
        Sprite *sprite = EXT_SPRITE(refs);
        Sprite__update_rect(sprite);
      }
    }
  }

  return self;
}

void Rect__add_ref(VALUE self, VALUE ref) {
  VALUE refs = rb_ivar_get(self, rb_intern("@refs"));
  if(RTEST(refs)) {
    rb_raise(rb_eNotImpError, "TODO: Rect__add_ref is under construction");
  } else {
    rb_ivar_set(self, rb_intern("@refs"), ref);
  }
}

void Rect__remove_ref(VALUE self, VALUE ref) {
  VALUE refs = rb_ivar_get(self, rb_intern("@refs"));
  if(refs == ref) {
    rb_ivar_set(self, rb_intern("@refs"), Qnil);
  } else {
    rb_p(refs);
    rb_p(ref);
    rb_raise(rb_eNotImpError, "TODO: Rect__remove_ref is under construction");
  }
}

void Init_ExtRect() {
  cRect = rb_const_get(rb_cObject, rb_intern("Rect"));
  Rect_set_ = get_instance_method(cRect, "set");
  rb_define_method(cRect, "set", Rect_set, -1);
}
