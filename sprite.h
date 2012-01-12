extern void Init_ExtSprite();
typedef struct {
  VALUE bitmap;
  VERTEX vertex_data[4];
} Sprite;
#define EXT_SPRITE(val) ((Sprite*)(DATA_PTR(val)))
