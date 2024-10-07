#include "MainWindow.hpp"
// filter файлов + linenumber_bgcolor/linenumber_fgcolor - работают не по доке

Fl_Window *globalWindow;
Fl_Window *globalSettingsWindow;
Fl_Text_Editor *globalTextEditor;
Fl_Box *globalSidebar;
string PATH;
Appearance *globalAppearance;

void ColorPickerCallback(Fl_Widget *widget) {
  uchar r, g, b;
  if (fl_color_chooser("Choose Color", r, g, b))
    Fl_Color chosen_color = fl_rgb_color(r, g, b);
}

void ShowSettingsWindow() {
  int center_x = (globalWindow->w() - SIZE_SETTINGS_WINDOW_X) / 2;
  int center_y = (globalWindow->h() - SIZE_SETTINGS_WINDOW_Y) / 2;
  globalSettingsWindow->position(center_x, center_y);
  if (globalSettingsWindow->shown()) {
    globalSettingsWindow->hide();
  } else { 
    globalSettingsWindow->show();
    globalSettingsWindow->take_focus();
  } 
}

void ChangeTextSize(char symbol) {
  int curr_pos = globalTextEditor->buffer()->length();
  symbol == '+' ? globalAppearance->text_size++ : globalAppearance->text_size--;
  globalTextEditor->textsize(globalAppearance->text_size);
  globalTextEditor->buffer()->append(" ");
  globalTextEditor->buffer()->remove(curr_pos, curr_pos + 1);
}

void OpenFileChooser() {
  Fl_Native_File_Chooser file_chooser;
  file_chooser.filter(FILTER);
  if (file_chooser.show() == 0) {
    PATH = file_chooser.filename();
    globalTextEditor->buffer()->text("");
    ifstream file_input(PATH);
    if (file_input.is_open()) {
      std::filesystem::path filePath(PATH);
      const char *fileName = filePath.filename().c_str();
      globalSidebar->copy_label(fileName);

      string line;
      while (getline(file_input, line))
        globalTextEditor->buffer()->append((line + "\n").c_str());
    }
    globalTextEditor->insert_position(globalTextEditor->buffer()->length() - 1);
  }
}

void SaveFile() {
  if (PATH.empty()) {
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    file_chooser.filter(FILTER);
    if (file_chooser.show() == 0) {
      PATH = file_chooser.filename();
      string extension = file_chooser.filter_value() == 0 ? ".txt" : file_chooser.filter_value() == 1 ? ".py" : ".cpp";
      if (PATH.rfind('.') == string::npos)
        PATH += extension;
      ofstream file(PATH);
      if (file.is_open())
        file << globalTextEditor->buffer()->text();
    }
  } else {
    ofstream file_output(PATH);
    if (file_output.is_open())
      file_output << globalTextEditor->buffer()->text();
  }
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

int handle_shortcuts(int event) {
  if (event == FL_SHORTCUT && Fl::event_state(FL_COMMAND))
    switch (Fl::event_key()) {
    case '+':
      ChangeTextSize('+');
      return 1;
    case '-':
      ChangeTextSize('-');
      return 1;
    case ',':
      ShowSettingsWindow();
      return 1;
    case 'o':
      OpenFileChooser();
      return 1;
    case 's':
      SaveFile();
      return 1;
    case 'f':
      return 1;
    }
  return 0;
}

void InitWindow(Appearance &appearance) {
  Fl_Window *window = new Fl_Window((Fl::w() - SIZE_WINDOW_X) / 2, (Fl::h() - SIZE_WINDOW_Y) / 2, SIZE_WINDOW_X, SIZE_WINDOW_Y, "Text Editor");
  globalWindow = window;

  Fl_Box *sidebar = new Fl_Box(0, 0, SIDEBAR_WIDTH, SIZE_WINDOW_Y);
  sidebar->box(FL_FLAT_BOX);
  sidebar->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));
  sidebar->labelcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  globalSidebar = sidebar;

  Fl_Box *separator = new Fl_Box(SIDEBAR_WIDTH - 1, 0, 2, SIZE_WINDOW_Y);
  separator->box(FL_FLAT_BOX);
  separator->color(fl_rgb_color(appearance.interface_color.red + 13, appearance.interface_color.green + 13, appearance.interface_color.blue + 13));

  Fl_Box *topbar = new Fl_Box(SIDEBAR_WIDTH, 0, SIZE_WINDOW_X - SIDEBAR_WIDTH, TOPBAR_HEIGHT);
  topbar->box(FL_FLAT_BOX);
  topbar->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));

  int center_x = (SIZE_WINDOW_X - SIZE_SETTINGS_WINDOW_X) / 2;
  int center_y = (SIZE_WINDOW_Y - SIZE_SETTINGS_WINDOW_Y) / 2;
  Fl_Window *settings_window = new Fl_Window(center_x, center_y, SIZE_SETTINGS_WINDOW_X, SIZE_SETTINGS_WINDOW_Y);
  Fl_Box *settings = new Fl_Box(0, 0, SIZE_SETTINGS_WINDOW_X, SIZE_SETTINGS_WINDOW_Y, SETTINGS);

  Fl_Button *interface_color = new Fl_Button(SIZE_SETTINGS_WINDOW_X / 2, 0, 175, 40, "Change interface color");
  interface_color->callback(ColorPickerCallback);
  Fl_Button *text_editor_color = new Fl_Button(SIZE_SETTINGS_WINDOW_X / 2, 130, 175, 40, "Change text-field color");
  text_editor_color->callback(ColorPickerCallback);
  Fl_Button *text_color = new Fl_Button(SIZE_SETTINGS_WINDOW_X / 2, 130, 175, 40, "Change text color");
  text_color->callback(ColorPickerCallback);
  Fl_Button *cursor_color = new Fl_Button(SIZE_SETTINGS_WINDOW_X / 2, 130, 175, 40, "Change cursor color");
  cursor_color->callback(ColorPickerCallback);
  Fl_Button *reset_settings = new Fl_Button(SIZE_SETTINGS_WINDOW_X / 2, 130, 175, 40, "Reset settings");
  reset_settings->callback(ColorPickerCallback);

  settings->box(FL_FLAT_BOX);
  settings->labelcolor(fl_rgb_color(appearance.text_color.red, appearance.text_color.green, appearance.text_color.blue));
  settings->labelsize(appearance.text_size);
  settings->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));
  settings_window->color(fl_rgb_color(appearance.interface_color.red, appearance.interface_color.green, appearance.interface_color.blue));
  settings_window->size_range(SIZE_SETTINGS_WINDOW_X, SIZE_SETTINGS_WINDOW_Y, SIZE_SETTINGS_WINDOW_X + 1, SIZE_SETTINGS_WINDOW_Y + 1);
  settings_window->hide();
  settings_window->end();
  globalSettingsWindow = settings_window;

  Fl_Text_Buffer *textBuffer = new Fl_Text_Buffer();
  Fl_Text_Editor *textEditor = new Fl_Text_Editor(SIDEBAR_WIDTH - 1, TOPBAR_HEIGHT - 2, SIZE_WINDOW_X - SIDEBAR_WIDTH + 3, SIZE_WINDOW_Y - TOPBAR_HEIGHT + 4);
  textEditor->buffer(textBuffer);
  globalTextEditor = textEditor;

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

  window->resizable(textEditor);
  window->size_range(640, 480);
  window->end();
  window->show();
}

int main() {
  Appearance appearance;
  globalAppearance = &appearance;
  InitWindow(appearance);
  Fl::add_handler(handle_shortcuts);
  return Fl::run();
}