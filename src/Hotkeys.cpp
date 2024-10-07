#include "MainWindow.hpp"

void SaveFile();
void OpenFileChooser();
void ShowSettingsWindow();
void ChangeTextSize(char symbol);

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

void ShowSettingsWindow() {
  globalSettingsWindow->shown() ? globalSettingsWindow->hide() : globalSettingsWindow->show();
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
