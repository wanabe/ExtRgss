#include "ext_rgss.h"
#include "bitmap.h"

VALUE cBitmap;

void BitmapData__update(RgssBitmapData *bmpdata) {
  BitmapExtData *extdata;

  extdata = BITMAP_EXTDATA(bmpdata);
  if(!extdata->changed) {
    return;
  }

  extdata->changed = 0;
  Graphics__update_texture(extdata->texture, bmpdata->buffer, bmpdata->info->biWidth, bmpdata->info->biHeight);
}

/* from Hacker's Delight http://www.hackersdelight.org/HDcode/clp2.c.txt */
static unsigned int clp2(unsigned int x) {
  x = x - 1;
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >>16);
  return x + 1;
}

void Bitmap__init_extdata(VALUE bmp) {
  RgssBitmapData *bmpdata;
  BitmapExtData *extdata;

  bmpdata = RGSS_BITMAPDATA(bmp);
  bmpdata->info->biClrImportant = (DWORD)(extdata = ALLOC(BitmapExtData)); /* TODO: should free */

  extdata->texw = clp2(bmpdata->info->biWidth);
  extdata->texh = clp2(bmpdata->info->biHeight);
  Graphics__create_texture(&extdata->texture, extdata->texw, extdata->texh);
  extdata->changed = 1;
  BitmapData__update(bmpdata);
}

static VALUE Bitmap_initialize(int argc, VALUE *argv, VALUE self) {
  rb_funcall2(self, rb_intern("old_initialize"), argc, argv);
  Bitmap__init_extdata(self);
  return self;
}

static VALUE Bitmap_draw_text(int argc, VALUE *argv, VALUE self) {
  BitmapExtData *extdata = BITMAP_EXTDATA(RGSS_BITMAPDATA(self));
  rb_funcall2(self, rb_intern("old_draw_text"), argc, argv);
  extdata->changed = 1;
  return self;
}

static VALUE Bitmap_clear(VALUE self) {
  BitmapExtData *extdata = BITMAP_EXTDATA(RGSS_BITMAPDATA(self));
  rb_funcall2(self, rb_intern("old_clear"), 0, NULL);
  extdata->changed = 1;
  return self;
}

void Init_ExtBitmap() {
  cBitmap = rb_const_get(rb_cObject, rb_intern("Bitmap"));
  rb_define_alias(cBitmap, "old_initialize", "initialize");
  rb_define_method(cBitmap, "initialize", Bitmap_initialize, -1);
  rb_define_alias(cBitmap, "old_draw_text", "draw_text");
  rb_define_method(cBitmap, "draw_text", Bitmap_draw_text, -1);
  rb_define_alias(cBitmap, "old_clear", "clear");
  rb_define_method(cBitmap, "clear", Bitmap_clear, 0);
}
