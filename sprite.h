typedef struct {
  VALUE bitmap;
  VALUE src_rect;
  VERTEX vertex_data[4];
  int x, y, ox, oy;
  int disposed, visible;
} Sprite;
#define EXT_SPRITE(val) ((Sprite*)(DATA_PTR(val)))

extern const rb_data_type_t Sprite_data_type;

void Init_ExtSprite();
void Sprite__update_rect(Sprite *sprite);
