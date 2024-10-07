#include "MainWindow.hpp"
// filter файлов + linenumber_bgcolor/linenumber_fgcolor - работают не по доке
Fl_Text_Editor *globalTextEditor;
Appearance *globalAppearance;
Fl_Window *globalSettingsWindow;
Fl_Window *globalWindow;
Fl_Box *globalSidebar;
string PATH;

void InitWindow(Appearance &appearance);
void InitSecondWindow(Appearance &appearance);
void Parse(unordered_map<string, string> &config, const string &path);
void ColorPickerCallback(Fl_Widget *widget);

int main() {
  Appearance appearance;
  globalAppearance = &appearance;
  InitWindow(appearance);
  Fl::add_handler(handle_shortcuts);
  return Fl::run();
}

void InitWindow(Appearance &appearance) {
  int center_x = (Fl::w() - MAINWINDOW_WIDTH) / 2;
  int center_y = (Fl::h() - MAINWINDOW_HEIGHT) / 2;
  Fl_Window *window = new Fl_Window(center_x, center_y, MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT, "Text Editor");
  window->size_range(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
  globalWindow = window;


  Fl_Box *sidebar = new Fl_Box(0, 0, SIDEBAR_WIDTH, MAINWINDOW_HEIGHT);
  sidebar->box(FL_FLAT_BOX);
  sidebar->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));
  sidebar->labelcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  globalSidebar = sidebar;


  Fl_Box *separator = new Fl_Box(SIDEBAR_WIDTH - 1, 0, 2, MAINWINDOW_HEIGHT);
  separator->box(FL_FLAT_BOX);
  separator->color(fl_rgb_color(appearance.interface_color.red + 13, appearance.interface_color.green + 13, appearance.interface_color.blue + 13));


  Fl_Box *topbar = new Fl_Box(SIDEBAR_WIDTH, 0, MAINWINDOW_WIDTH - SIDEBAR_WIDTH, TOPBAR_HEIGHT);
  topbar->box(FL_FLAT_BOX);
  topbar->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));


  Fl_Text_Buffer *textBuffer = new Fl_Text_Buffer();
  Fl_Text_Editor *textEditor = new Fl_Text_Editor(SIDEBAR_WIDTH - 1, TOPBAR_HEIGHT - 2, MAINWINDOW_WIDTH - SIDEBAR_WIDTH + 3, MAINWINDOW_HEIGHT - TOPBAR_HEIGHT + 4);
  textEditor->buffer(textBuffer);
  textEditor->box(FL_NO_BOX);
  textEditor->textcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  textEditor->color(fl_rgb_color(appearance.text_editor_color.red, appearance.text_editor_color.green, appearance.text_editor_color.blue));
  textEditor->cursor_color(fl_rgb_color(appearance.cursor_color.red, appearance.cursor_color.green, appearance.cursor_color.blue));
  textEditor->textsize(appearance.text_size);
  textEditor->textfont(FL_COURIER);
  textEditor->cursor_style(5);                           //
  textEditor->selection_color(fl_rgb_color(56, 58, 60)); //
  textEditor->linenumber_width(20);                      //
  textEditor->linenumber_align(FL_ALIGN_RIGHT);
  textEditor->linenumber_bgcolor(fl_rgb_color(appearance.text_editor_color.red, appearance.text_editor_color.green, appearance.text_editor_color.blue));
  textEditor->linenumber_fgcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  textEditor->linenumber_size(appearance.text_size);
  globalTextEditor = textEditor;


  window->resizable(textEditor);
  window->show();


  InitSecondWindow(appearance);
}

void InitSecondWindow(Appearance &appearance) {
  int center_x = (Fl::w() - SECONDWINDOW_WIDTH) / 2;
  int center_y = (Fl::h() - SECONDWINDOW_HEIGHT) / 2;
  Fl_Window *settings_window = new Fl_Window(center_x, center_y, SECONDWINDOW_WIDTH, SECONDWINDOW_HEIGHT);
  Fl_Box *settings_widget = new Fl_Box(0, 0, SECONDWINDOW_WIDTH, SECONDWINDOW_HEIGHT, SETTINGS);


  center_x = (SECONDWINDOW_WIDTH - BUTTON_WIDTH) / 2;
  center_y = (SECONDWINDOW_HEIGHT - BUTTON_HEIGHT) / 2;
  Fl_Button *interface_color = new Fl_Button(center_x, center_y, BUTTON_WIDTH, BUTTON_HEIGHT, "Change interface color");
  interface_color->callback(ColorPickerCallback);
  interface_color->color(fl_rgb_color(85, 85, 85));
  interface_color->box(FL_UP_BOX);
  interface_color->down_box(FL_DOWN_BOX);
  interface_color->down_color(fl_rgb_color(70, 70, 70));
  interface_color->labelcolor(fl_rgb_color(180, 180, 180));

  Fl_Button *text_editor_color = new Fl_Button(center_x, center_y + BUTTON_HEIGHT + 5, BUTTON_WIDTH, BUTTON_HEIGHT, "Change text-field color");
  text_editor_color->callback(ColorPickerCallback);
  text_editor_color->color(fl_rgb_color(85, 85, 85));
  text_editor_color->box(FL_UP_BOX);
  text_editor_color->down_box(FL_DOWN_BOX);
  text_editor_color->down_color(fl_rgb_color(70, 70, 70));
  text_editor_color->labelcolor(fl_rgb_color(180, 180, 180));

  Fl_Button *text_color = new Fl_Button(center_x, center_y + (BUTTON_HEIGHT * 2) + 10, BUTTON_WIDTH, BUTTON_HEIGHT, "Change text color");
  text_color->callback(ColorPickerCallback);
  text_color->color(fl_rgb_color(85, 85, 85));
  text_color->box(FL_UP_BOX);
  text_color->down_box(FL_DOWN_BOX);
  text_color->down_color(fl_rgb_color(70, 70, 70));
  text_color->labelcolor(fl_rgb_color(180, 180, 180));

  Fl_Button *cursor_color = new Fl_Button(center_x, center_y + (BUTTON_HEIGHT * 3) + 15, BUTTON_WIDTH, BUTTON_HEIGHT, "Change cursor color");
  cursor_color->callback(ColorPickerCallback);
  cursor_color->color(fl_rgb_color(85, 85, 85));
  cursor_color->box(FL_UP_BOX);
  cursor_color->down_box(FL_DOWN_BOX);
  cursor_color->down_color(fl_rgb_color(70, 70, 70));
  cursor_color->labelcolor(fl_rgb_color(180, 180, 180));

  Fl_Button *reset_settings = new Fl_Button(center_x, center_y + (BUTTON_HEIGHT * 4) + 20, BUTTON_WIDTH, BUTTON_HEIGHT, "Reset settings");
  reset_settings->callback(ColorPickerCallback);
  reset_settings->color(fl_rgb_color(85, 85, 85));
  reset_settings->box(FL_UP_BOX);
  reset_settings->down_box(FL_DOWN_BOX);
  reset_settings->down_color(fl_rgb_color(70, 70, 70));
  reset_settings->labelcolor(fl_rgb_color(180, 180, 180));


  settings_widget->labelcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  settings_widget->labelsize(appearance.text_size);
  settings_widget->align(FL_ALIGN_TOP | FL_ALIGN_INSIDE);


  settings_window->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));
  settings_window->size_range(SECONDWINDOW_WIDTH, SECONDWINDOW_HEIGHT, SECONDWINDOW_WIDTH, SECONDWINDOW_HEIGHT);
  settings_window->hide();
  globalSettingsWindow = settings_window;
}

void Parse(unordered_map<string, string> &config, const string &path) {
  ifstream file(path);
  string line;
  if (file.is_open())
    while (getline(file, line)) {
      size_t equalPos = line.find('=');
      if (equalPos != string::npos) {
        string key = line.substr(0, equalPos);
        string value = line.substr(equalPos + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        config[key] = value;
      }
    }
}

void ColorPickerCallback(Fl_Widget *widget) {
  uchar r, g, b;
  if (fl_color_chooser("Choose Color", r, g, b))
     fl_rgb_color(r, g, b);
}