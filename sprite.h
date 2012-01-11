extern void Init_ExtSprite();
typedef struct {
  VALUE bitmap;
} Sprite;
#define EXT_SPRITE(val) ((Sprite*)(DATA_PTR(val)))
