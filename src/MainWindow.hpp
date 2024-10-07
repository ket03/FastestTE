#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

#define SIZE_WINDOW_X 640
#define SIZE_WINDOW_Y 480
#define SIZE_SETTINGS_WINDOW_X 300
#define SIZE_SETTINGS_WINDOW_Y 400
#define SIDEBAR_WIDTH 200
#define TOPBAR_HEIGHT 30
#define FILTER "*.txt"

#define SETTINGS                                                                                                                                     \
  "CMD`,  Settings/Exit\n"                                                                                                                           \
  "CMD`+  Text size+\n"                                                                                                                              \
  "CMD`-  Text size-\n"                                                                                                                              \
  "CMD`f  Search\n"                                                                                                                                  \
  "CMD`o  Open file\n"                                                                                                                               \
  "CMD`s  Save file\n"

using std::unordered_map, std::string, std::ifstream, std::ofstream, std::stoi;

void Parse(unordered_map<string, string> &data, const string &path);

struct RGB {
  u_char red;
  u_char green;
  u_char blue;
};

class Appearance {
public:
  unordered_map<string, string> config;
  RGB interface_color;
  RGB text_editor_color;
  RGB text_color;
  RGB cursor_color;
  u_char text_size;

  Appearance() {
    Parse(config, "settings.txt");
    interface_color.red = stoi(config["INTERFACE_COLOR_RED"]);
    interface_color.green = stoi(config["INTERFACE_COLOR_GREEN"]);
    interface_color.blue = stoi(config["INTERFACE_COLOR_BLUE"]);

    text_editor_color.red = stoi(config["TEXT_EDITOR_COLOR_RED"]);
    text_editor_color.green = stoi(config["TEXT_EDITOR_COLOR_GREEN"]);
    text_editor_color.blue = stoi(config["TEXT_EDITOR_COLOR_BLUE"]);

    text_color.red = stoi(config["TEXT_COLOR_RED"]);
    text_color.green = stoi(config["TEXT_COLOR_GREEN"]);
    text_color.blue = stoi(config["TEXT_COLOR_BLUE"]);

    cursor_color.red = stoi(config["CURSOR_COLOR_RED"]);
    cursor_color.green = stoi(config["CURSOR_COLOR_GREEN"]);
    cursor_color.blue = stoi(config["CURSOR_COLOR_BLUE"]);

    text_size = stoi(config["TEXT_SIZE"]);
  }
};

#endif // MAINWINDOW_HPP