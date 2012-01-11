#include "ext_rgss.h"

typedef struct {
} Sprite_t;

static void Sprite__mark(void *ptr) {
}

static void Sprite__free(void *ptr) {
}

static size_t Sprite__memsize(const void *ptr) {
  return ptr ? sizeof(Sprite_t) : 0;
}

static const rb_data_type_t Sprite_data_type = {
  "sprite", {Sprite__mark, Sprite__free, Sprite__memsize,},
};

static VALUE Sprite_s_alloc(VALUE klass) {
  VALUE obj;
  Sprite_t *sprite;

  obj = TypedData_Make_Struct(klass, Sprite_t, &Sprite_data_type, sprite);

  return obj;
}

void Init_ExtSprite() {
  VALUE cOldSprite = rb_const_get(rb_cObject, rb_intern("Sprite"));
  VALUE cSprite = rb_define_class_under(mExtRgss, "Sprite", rb_cObject);
  rb_const_set(rb_cObject, rb_intern("OldSprite"), cOldSprite);
  rb_const_set(rb_cObject, rb_intern("Sprite"), cSprite);

  rb_define_alloc_func(cSprite, Sprite_s_alloc);
}
