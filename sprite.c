#include "ext_rgss.h"
#include "graphics.h"
#include "sprite.h"

static void Sprite__mark(void *ptr) {
}

static void Sprite__free(void *ptr) {
}

static size_t Sprite__memsize(const void *ptr) {
  return ptr ? sizeof(Sprite) : 0;
}

static const rb_data_type_t Sprite_data_type = {
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
  rb_ary_push(sprites, self);
  return self;
}

static VALUE Sprite_bitmap_set(VALUE self, VALUE bitmap) {
  Sprite *sprite = EXT_SPRITE(self);

  rb_ivar_set(self, "@bitmap", bitmap);
  sprite->bitmap = bitmap;
  return bitmap;
}

static VALUE Sprite_bitmap(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return sprite->bitmap;
}

static VALUE Sprite_dispose(VALUE self) {
  VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));

  rb_ary_delete(sprites, self);

  return self;
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
}
