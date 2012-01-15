#include "ext_rgss.h"
#include "graphics.h"
#include "sprite.h"
#include "rect.h"

static void Sprite__mark(void *ptr) {
}

static void Sprite__free(void *ptr) {
  if(ptr) free(ptr);
}

static size_t Sprite__memsize(const void *ptr) {
  return ptr ? sizeof(Sprite) : 0;
}

const rb_data_type_t Sprite_data_type = {
  "sprite", {Sprite__mark, Sprite__free, Sprite__memsize,},
};

static VALUE Sprite_s_alloc(VALUE klass) {
  VALUE obj;
  Sprite *sprite;

  obj = TypedData_Make_Struct(klass, Sprite, &Sprite_data_type, sprite);

  return obj;
}

static VALUE Sprite_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));
  VALUE src_rect = rb_class_new_instance(0, NULL, cRect);
  Sprite *sprite = EXT_SPRITE(self);

  rb_ary_push(sprites, self);
  old_call(self, rb_intern("initialize"), argc, argv);

  src_rect = old_call(self, rb_intern("src_rect"), 0, NULL); /* TODO */
  rb_ivar_set(self, rb_intern("@src_rect"), src_rect);
  Rect__add_ref(src_rect, self);
  sprite->src_rect = src_rect;

  sprite->x = sprite->y = 0;
  sprite->ox = sprite->oy = 0;
  sprite->disposed = 0;
  sprite->visible = 1;

  return self;
}

void Sprite__update_rect(Sprite *sprite) {
  RgssBitmapData *bmpdata = RGSS_BITMAPDATA(sprite->bitmap);
  RgssRect *rect = RGSS_RECT(sprite->src_rect);

  Graphics__update_vertex(sprite->vertex_data, sprite->x - sprite->ox, sprite->y - sprite->oy, bmpdata->info->biWidth, bmpdata->info->biHeight, rect);
}

static VALUE Sprite_bitmap_set(VALUE self, VALUE bitmap) {
  Sprite *sprite = EXT_SPRITE(self);
  RgssRect *rect = RGSS_RECT(sprite->src_rect);
  RgssBitmapData *bmpdata = RGSS_BITMAPDATA(bitmap);
  
  rb_ivar_set(self, rb_intern("@bitmap"), bitmap);
  sprite->bitmap = bitmap;
  old_call(self, rb_intern("bitmap="), 1, &bitmap);
  rect->x = rect->y = 0;
  rect->w = bmpdata->info->biWidth;
  rect->h = bmpdata->info->biHeight;
  Sprite__update_rect(sprite);

  return bitmap;
}

static VALUE Sprite_bitmap(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  old_call(self, rb_intern("bitmap"), 0, NULL);

  return sprite->bitmap;
}

static VALUE Sprite_dispose(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  if(!sprite->disposed) {
    VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));
    rb_ary_delete(sprites, self);
    Rect__remove_ref(sprite->src_rect, self);
    old_call(self, rb_intern("dispose"), 0, NULL);
    sprite->disposed = 1;
  }

  return self;
}

static VALUE Sprite_src_rect_set(VALUE self, VALUE src_rect) {
  Sprite *sprite = EXT_SPRITE(self);

  rb_ivar_set(self, rb_intern("@src_rect"), src_rect);
  old_call(self, rb_intern("src_rect="), 1, &src_rect);
  Rect__remove_ref(sprite->src_rect, self);
  Rect__add_ref(src_rect, self);
  sprite->src_rect = src_rect;
  return src_rect;
}

static VALUE Sprite_src_rect(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return sprite->src_rect;
}

static VALUE Sprite_x_set(VALUE self, VALUE x) {
  Sprite *sprite = EXT_SPRITE(self);

  sprite->x = NUM2LONG(x);
  Sprite__update_rect(sprite);
  old_call(self, rb_intern("x="), 1, &x);

  return x;
}

static VALUE Sprite_x(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return INT2FIX(sprite->x);
}

static VALUE Sprite_y_set(VALUE self, VALUE y) {
  Sprite *sprite = EXT_SPRITE(self);

  sprite->y = NUM2LONG(y);
  Sprite__update_rect(sprite);
  old_call(self, rb_intern("y="), 1, &y);

  return y;
}

static VALUE Sprite_y(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return INT2FIX(sprite->y);
}

static VALUE Sprite_ox_set(VALUE self, VALUE ox) {
  Sprite *sprite = EXT_SPRITE(self);

  sprite->ox = FIX2LONG(ox);
  Sprite__update_rect(sprite);
  old_call(self, rb_intern("ox="), 1, &ox);

  return ox;
}

static VALUE Sprite_ox(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return INT2FIX(sprite->ox);
}

static VALUE Sprite_oy_set(VALUE self, VALUE oy) {
  Sprite *sprite = EXT_SPRITE(self);

  sprite->oy = FIX2LONG(oy);
  Sprite__update_rect(sprite);
  old_call(self, rb_intern("oy="), 1, &oy);

  return oy;
}

static VALUE Sprite_oy(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return INT2FIX(sprite->oy);
}

static VALUE Sprite_visible_set(VALUE self, VALUE visible) {
  Sprite *sprite = EXT_SPRITE(self);

  sprite->visible = !!RTEST(visible);
  old_call(self, rb_intern("visible="), 1, &visible);

  return visible;
}

static VALUE Sprite_visible(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return sprite->visible ? Qtrue : Qfalse;
}

void Init_ExtSprite() {
  VALUE cOldSprite = rb_const_get(rb_cObject, rb_intern("Sprite"));
  VALUE cSprite = rb_define_class_under(mExtRgss, "Sprite", rb_cObject);
  rb_const_set(rb_cObject, rb_intern("OldSprite"), cOldSprite);
  rb_const_set(rb_cObject, rb_intern("Sprite"), cSprite);

  rb_define_alloc_func(cSprite, Sprite_s_alloc);
  rb_define_method(cSprite, "initialize", Sprite_initialize, -1);
  rb_define_method(cSprite, "bitmap=", Sprite_bitmap_set, 1);
  rb_define_method(cSprite, "bitmap", Sprite_bitmap, 0);
  rb_define_method(cSprite, "dispose", Sprite_dispose, 0);
  rb_define_method(cSprite, "src_rect=", Sprite_src_rect_set, 1);
  rb_define_method(cSprite, "src_rect", Sprite_src_rect, 0);
  rb_define_method(cSprite, "x=", Sprite_x_set, 1);
  rb_define_method(cSprite, "x", Sprite_x, 0);
  rb_define_method(cSprite, "y=", Sprite_y_set, 1);
  rb_define_method(cSprite, "y", Sprite_y, 0);
  rb_define_method(cSprite, "ox=", Sprite_ox_set, 1);
  rb_define_method(cSprite, "ox", Sprite_ox, 0);
  rb_define_method(cSprite, "oy=", Sprite_oy_set, 1);
  rb_define_method(cSprite, "oy", Sprite_oy, 0);
  rb_define_method(cSprite, "visible=", Sprite_visible_set, 1);
  rb_define_method(cSprite, "visible", Sprite_visible, 0);
}
