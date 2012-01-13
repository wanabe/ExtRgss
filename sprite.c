#include "ext_rgss.h"
#include "graphics.h"
#include "sprite.h"

VALUE cRect;

static void Sprite__mark(void *ptr) {
}

static void Sprite__free(void *ptr) {
  if(ptr) free(ptr);
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

static VALUE old_call(VALUE self, ID mid, int argc, VALUE *argv) {
  return rb_funcall2(rb_ivar_get(self, rb_intern("@old")), mid, argc, argv);
}

static VALUE Sprite_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));
  VALUE src_rect = rb_class_new_instance(0, NULL, cRect);
  Sprite *sprite = EXT_SPRITE(self);

  rb_ary_push(sprites, self);
  old_call(self, rb_intern("initialize"), argc, argv);

  src_rect = old_call(self, rb_intern("src_rect"), 0, NULL); /* TODO */
  rb_ivar_set(self, rb_intern("@src_rect"), src_rect);
  sprite->src_rect = src_rect;

  return self;
}

static VALUE Sprite_bitmap_set(VALUE self, VALUE bitmap) {
  Sprite *sprite = EXT_SPRITE(self);
  RgssRect *rect = RGSS_RECT(sprite->src_rect);
  RgssBitmapData *bmpdata = RGSS_BITMAPDATA(bitmap);
  
  rb_ivar_set(self, "@bitmap", bitmap);
  sprite->bitmap = bitmap;
  Graphics__update_vertex(bitmap, sprite->vertex_data);
  old_call(self, rb_intern("bitmap="), 1, &bitmap);
  rect->x = rect->y = 0;
  rect->w = bmpdata->info->biWidth;
  rect->h = bmpdata->info->biHeight;

  return bitmap;
}

static VALUE Sprite_bitmap(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  old_call(self, rb_intern("bitmap"), 0, NULL);

  return sprite->bitmap;
}

static VALUE Sprite_dispose(VALUE self) {
  VALUE sprites = rb_ivar_get(mGraphics, rb_intern("@sprites"));

  /*VALUE src_rect;
  unsigned char *ptr;
  int i;
  src_rect = old_call(self, rb_intern("src_rect"), 0, NULL);
  ptr = (char*)(DATA_PTR(src_rect));
  for(i = 0; i < 50; i++) {
    printf("%02x ", *ptr++);
  }
  rb_p(src_rect);*/
  rb_ary_delete(sprites, self);
  old_call(self, rb_intern("dispose"), 0, NULL);

  return self;
}

static VALUE Sprite_src_rect_set(VALUE self, VALUE src_rect) {
  Sprite *sprite = EXT_SPRITE(self);

  /*char *ptr = (char*)src_rect;
  for(; ptr < src_rect + 5; ptr++) {
    printf("%02x ", *ptr);
  }
  printf("src_rect=\n");*/

  rb_ivar_set(self, "@src_rect", src_rect);
  old_call(self, rb_intern("src_rect="), 1, &src_rect);
  sprite->src_rect = src_rect;
  return src_rect;
}

static VALUE Sprite_src_rect(VALUE self) {
  Sprite *sprite = EXT_SPRITE(self);

  return sprite->src_rect;
}

void Init_ExtSprite() {
  VALUE cOldSprite = rb_const_get(rb_cObject, rb_intern("Sprite"));
  VALUE cSprite = rb_define_class_under(mExtRgss, "Sprite", rb_cObject);
  cRect = rb_const_get(rb_cObject, rb_intern("Rect"));
  rb_const_set(rb_cObject, rb_intern("OldSprite"), cOldSprite);
  rb_const_set(rb_cObject, rb_intern("Sprite"), cSprite);

  rb_define_alloc_func(cSprite, Sprite_s_alloc);
  rb_define_method(cSprite, "initialize", Sprite_initialize, -1);
  rb_define_method(cSprite, "bitmap=", Sprite_bitmap_set, 1);
  rb_define_method(cSprite, "bitmap", Sprite_bitmap, 0);
  rb_define_method(cSprite, "dispose", Sprite_dispose, 0);
  rb_define_method(cSprite, "src_rect=", Sprite_src_rect_set, 1);
  rb_define_method(cSprite, "src_rect", Sprite_src_rect, 0);
}
