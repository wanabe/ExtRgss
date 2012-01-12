#include "ext_rgss.h"
#include "bitmap.h"

static void BitmapData__update(RgssBitmapData *bmpdata) {
  BitmapExtData *extdata;

  if (!bmpdata) {
    return;
  }
  extdata = BITMAP_EXTDATA(bmpdata);
  if(!extdata->changed) {
    return;
  }

  extdata->changed = 0;
  Graphics__update_texture(extdata->texture, bmpdata->buffer, bmpdata->info->biWidth, bmpdata->info->biHeight);
}

void Bitmap__init_extdata(VALUE bmp) {
  RgssBitmapData *bmpdata;
  BitmapExtData *extdata;

  bmpdata = RGSS_BITMAPDATA(bmp);
  bmpdata->info->biClrImportant = (DWORD)(extdata = ALLOC(BitmapExtData)); /* TODO: should free */

  Graphics__create_texture(&extdata->texture, bmpdata->info->biWidth, bmpdata->info->biHeight);
  extdata->changed = 1;
  BitmapData__update(bmpdata);
}

static VALUE Bitmap_initialize(int argc, VALUE *argv, VALUE self) {
  rb_funcall2(self, rb_intern("old_initialize"), argc, argv);
  Bitmap__init_extdata(self);
  return self;
}

void Init_ExtBitmap() {
  VALUE cBitmap = rb_const_get(rb_cObject, rb_intern("Bitmap"));
  rb_define_alias(cBitmap, "old_initialize", "initialize");
  rb_define_method(cBitmap, "initialize", Bitmap_initialize, -1);
}
