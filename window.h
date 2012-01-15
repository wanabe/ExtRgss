typedef struct {
  VALUE contents;
  VALUE cursor_rect;
  VERTEX vertex_data[8];
  int x, y, ox, oy;
  int width, height;
  int disposed, visible;
  int openness;
} Window;
#define EXT_WINDOW(val) ((Window*)(DATA_PTR(val)))

extern const rb_data_type_t Window_data_type;

void Init_ExtWindow();
//void Window__update_rect(Window *window);
