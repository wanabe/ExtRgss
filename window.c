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

static void Window__update_vertex(Window *window) {
  VALUE contents = window->contents;
  RgssBitmapData *bmpdata = RGSS_BITMAPDATA(contents);
  BitmapExtData *extdata = BITMAP_EXTDATA(bmpdata);
  VERTEX *v = window->vertex_data;
  v[0].z = v[1].z = v[2].z = v[3].z = 0;
  v[0].x = v[2].x = window->x - 0.5;
  v[0].y = v[1].y = window->y - 0.5;
  v[1].x = v[3].x = window->x + window->width - 0.5;
  v[2].y = v[3].y = window->y + window->height - 0.5;
  v[0].u = v[2].u = -window->width;
  v[0].v = v[1].v = -window->height;
  v[1].u = v[3].u = window->width;
  v[2].v = v[3].v = window->height;
  v[0].w = v[1].w = v[2].w = v[3].w = extdata->texw;
  v[0].h = v[1].h = v[2].h = v[3].h = extdata->texh;
  v[0].b1 = v[1].b1 = v[2].b1 = v[3].b1 = window->openness;
  v[0].b2 = v[1].b2 = v[2].b2 = v[3].b2 = window->opacity;
  v[0].b3 = v[1].b3 = v[2].b3 = v[3].b3 = window->contents_opacity;
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
  Bitmap__init_extdata(contents);

  cursor_rect = old_call(self, rb_intern("cursor_rect"), 0, NULL); /* TODO */
  rb_ivar_set(self, rb_intern("@cursor_rect"), cursor_rect);
  Rect__add_ref(cursor_rect, self);
  window->cursor_rect = cursor_rect;

  switch(argc) {
  case 0:
    window->x = window->y = window->width = window->height = 0;
    break;
  case 4:
    window->x = FIX2INT(argv[0]);
    window->y = FIX2INT(argv[1]);
    window->width = FIX2INT(argv[2]);
    window->height = FIX2INT(argv[3]);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0, 4)", argc);
  }
  window->ox = window->oy = 0;
  window->disposed = 0;
  window->visible = 1;
  window->openness = 255;
  window->opacity = window->contents_opacity = 255;

  Window__update_vertex(window);

  return self;
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

static VALUE Window_x_set(VALUE self, VALUE x) {
  Window *window = EXT_WINDOW(self);

  window->x = NUM2LONG(x);
  Window__update_vertex(window);
  old_call(self, rb_intern("x="), 1, &x);
  return x;
}

static VALUE Window_x(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return INT2FIX(window->x);
}

static VALUE Window_y_set(VALUE self, VALUE y) {
  Window *window = EXT_WINDOW(self);

  window->y = NUM2LONG(y);
  Window__update_vertex(window);
  old_call(self, rb_intern("y="), 1, &y);
  return y;
}

static VALUE Window_y(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return INT2FIX(window->y);
}

static VALUE Window_openness_set(VALUE self, VALUE openness) {
  Window *window = EXT_WINDOW(self);
  int o;

  o = NUM2LONG(openness);
  if(o < 0) {
    o = 0;
  } else if(o > 255) {
    o = 255;
  }
  window->openness = o;
  openness = INT2FIX(o);
  Window__update_vertex(window);
  old_call(self, rb_intern("openness="), 1, &openness);
  return openness;
}

static VALUE Window_openness(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return INT2FIX(window->openness);
}

static VALUE Window_opacity_set(VALUE self, VALUE opacity) {
  Window *window = EXT_WINDOW(self);
  int o;

  o = NUM2LONG(opacity);
  if(o < 0) {
    o = 0;
  } else if(o > 255) {
    o = 255;
  }
  window->opacity = o;
  opacity = INT2FIX(o);
  Window__update_vertex(window);
  old_call(self, rb_intern("opacity="), 1, &opacity);
  return opacity;
}

static VALUE Window_opacity(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return INT2FIX(window->opacity);
}

static VALUE Window_contents_opacity_set(VALUE self, VALUE contents_opacity) {
  Window *window = EXT_WINDOW(self);
  int o;

  o = NUM2LONG(contents_opacity);
  if(o < 0) {
    o = 0;
  } else if(o > 255) {
    o = 255;
  }
  window->contents_opacity = o;
  contents_opacity = INT2FIX(o);
  Window__update_vertex(window);
  old_call(self, rb_intern("contents_opacity="), 1, &contents_opacity);
  return contents_opacity;
}

static VALUE Window_contents_opacity(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return INT2FIX(window->contents_opacity);
}

static VALUE Window_contents_set(VALUE self, VALUE contents) {
  Window *window = EXT_WINDOW(self);

  window->contents = contents;
  old_call(self, rb_intern("contents="), 1, &contents);
  Window__update_vertex(window);
  return contents;
}

static VALUE Window_contents(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return window->contents;
}

static VALUE Window_windowskin_set(VALUE self, VALUE skin) {
  Window *window = EXT_WINDOW(self);

  window->skin = skin;
  old_call(self, rb_intern("windowskin="), 1, &skin);
  return skin;
}

static VALUE Window_windowskin(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return window->skin;
}

static VALUE Window_visible_set(VALUE self, VALUE visible) {
  Window *window = EXT_WINDOW(self);

  window->visible = !!RTEST(visible);
  old_call(self, rb_intern("visible="), 1, &visible);

  return visible;
}

static VALUE Window_visible(VALUE self) {
  Window *window = EXT_WINDOW(self);

  return window->visible ? Qtrue : Qfalse;
}

void Init_ExtWindow() {
  VALUE cOldWindow = rb_const_get(rb_cObject, rb_intern("Window"));
  VALUE cWindow = rb_define_class_under(mExtRgss, "Window", rb_cObject);
  rb_const_set(rb_cObject, rb_intern("OldWindow"), cOldWindow);
  rb_const_set(rb_cObject, rb_intern("Window"), cWindow);

  rb_define_alloc_func(cWindow, Window_s_alloc);
  rb_define_method(cWindow, "initialize", Window_initialize, -1);
  rb_define_method(cWindow, "dispose", Window_dispose, 0);
  rb_define_method(cWindow, "x=", Window_x_set, 1);
  rb_define_method(cWindow, "x", Window_x, 0);
  rb_define_method(cWindow, "y=", Window_y_set, 1);
  rb_define_method(cWindow, "y", Window_y, 0);
  rb_define_method(cWindow, "openness=", Window_openness_set, 1);
  rb_define_method(cWindow, "openness", Window_openness, 0);
  rb_define_method(cWindow, "opacity=", Window_opacity_set, 1);
  rb_define_method(cWindow, "opacity", Window_opacity, 0);
  rb_define_method(cWindow, "contents_opacity=", Window_contents_opacity_set, 1);
  rb_define_method(cWindow, "contents_opacity", Window_contents_opacity, 0);
  rb_define_method(cWindow, "contents=", Window_contents_set, 1);
  rb_define_method(cWindow, "contents", Window_contents, 0);
  rb_define_method(cWindow, "windowskin=", Window_windowskin_set, 1);
  rb_define_method(cWindow, "windowskin", Window_windowskin, 0);
  rb_define_method(cWindow, "visible=", Window_visible_set, 1);
  rb_define_method(cWindow, "visible", Window_visible, 0);
}
