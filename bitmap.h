#include "graphics.h"

typedef struct {
  LPDIRECT3DTEXTURE9 texture;
  int changed;
} BitmapExtData;
#define BITMAP_EXTDATA(bmpdata) ((BitmapExtData*)((bmpdata)->info->biClrImportant))

extern VALUE cBitmap;

extern void Bitmap__init_extdata(VALUE bmp);
extern void Init_ExtBitmap();
