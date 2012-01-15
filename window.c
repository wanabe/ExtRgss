#include "ext_rgss.h"
#include "graphics.h"
#include "window.h"
#include "rect.h"
#include "bitmap.h"

static void Window__mark(void *ptr) {
}

static void Window__free(void *ptr) {
  if(ptr) free(ptr);
}

static size_t Window__memsize(const void *ptr) {
  return ptr ? sizeof(Window) : 0;
}

const rb_data_type_t Window_data_type = {
  "window", {Window__mark, Window__free, Window__memsize,},
};

static VALUE Window_s_alloc(VALUE klass) {
  VALUE obj;
  Window *window;

  obj = TypedData_Make_Struct(klass, Window, &Window_data_type, window);

  return obj;
}

static VALUE Window_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE windows = rb_ivar_get(mGraphics, rb_intern("@windows"));
  VALUE bmp_argv[2] = {INT2FIX(1), INT2FIX(1)};
  VALUE contents = rb_class_new_instance(2, bmp_argv, cBitmap);
  VALUE cursor_rect = rb_class_new_instance(0, NULL, cRect);
  Window *window = EXT_WINDOW(self);

  rb_ary_push(windows, self);

  contents = old_call(self, rb_intern("contents"), 0, NULL); /* TODO */
  rb_ivar_set(self, rb_intern("@contents"), contents);
  window->contents = contents;

  cursor_rect = old_call(self, rb_intern("cursor_rect"), 0, NULL); /* TODO */
  rb_ivar_set(self, rb_intern("@cursor_rect"), cursor_rect);
  Rect__add_ref(cursor_rect, self);
  window->cursor_rect = cursor_rect;

  window->x = window->y = 0;
  window->ox = window->oy = 0;
  window->disposed = 0;
  window->visible = 1;

  {
    VERTEX *v = window->vertex_data;
    v[0].x = v[2].x = 0;
    v[0].y = v[1].y = 0;
    v[1].x = v[3].x = 100;
    v[2].y = v[3].y = 100;
    v[0].z = v[1].z = v[2].z = v[3].z = 0;
    v[0].u = v[2].u = 0;
    v[0].v = v[1].v = 0;
    v[1].u = v[3].u = 1;
    v[2].v = v[3].v = 1;
  }

  return self;
}

static VALUE Window_contents_set(VALUE self, VALUE contents) {
  Window *window = EXT_WINDOW(self);

  window->contents = contents;
  old_call(self, rb_intern("contents="), 1, &contents);
  return contents;
}

static VALUE Window_contents(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return window->contents;
}

static VALUE Window_dispose(VALUE self) {
  Window *window = EXT_WINDOW(self);

  if(!window->disposed) {
    VALUE windows = rb_ivar_get(mGraphics, rb_intern("@windows"));
    rb_ary_delete(windows, self);
    Rect__remove_ref(window->cursor_rect, self);
    old_call(self, rb_intern("dispose"), 0, NULL);
    window->disposed = 1;
  }
  return self;
}

void Init_ExtWindow() {
  VALUE cOldWindow = rb_const_get(rb_cObject, rb_intern("Window"));
  VALUE cWindow = rb_define_class_under(mExtRgss, "Window", rb_cObject);
  rb_const_set(rb_cObject, rb_intern("OldWindow"), cOldWindow);
  rb_const_set(rb_cObject, rb_intern("Window"), cWindow);

  rb_define_alloc_func(cWindow, Window_s_alloc);
  rb_define_method(cWindow, "initialize", Window_initialize, -1);
  rb_define_method(cWindow, "contents=", Window_contents_set, 1);
  rb_define_method(cWindow, "contents", Window_contents, 0);
  rb_define_method(cWindow, "dispose", Window_dispose, 0);
}
