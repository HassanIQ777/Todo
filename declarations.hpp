#include "libutils/src/color.hpp"
#include "libutils/src/funcs.hpp"
#include "libutils/src/strutils.hpp"

#include <algorithm>

using funcs::print;

inline std::string TITLE = "To-Do";

inline std::string trimStr(std::string text, int WIDTH) {
  if (text.length() <= WIDTH) {
    return text;
  }
  return text.substr(0, WIDTH - 2) + "··";
}

inline void printTodos(std::vector<std::string> &todos, bool line_wrapping) {
  const int WIDTH = funcs::getTerminalWidth();
   const std::string BORDER_CHAR = "─";
  //const std::string BORDER_CHAR = "-";
  std::string BORDER;
  for (int i = 0; i < WIDTH; i++) {
    BORDER += BORDER_CHAR;
  }
  std::string TOP_BORDER_LINING;
  std::string title = TITLE; // im sorry this sucks
  for (int i = 0; i < ((WIDTH - title.size()) / 2); i++) {
    TOP_BORDER_LINING += BORDER_CHAR;
  }
  std::string TOP_BORDER = TOP_BORDER_LINING + title + TOP_BORDER_LINING;
  if (WIDTH % 2 == 0) {
    TOP_BORDER += BORDER_CHAR;
  }
  // if(title.size()%2 == 1) TOP_BORDER[TOP_BORDER.size() - 1] = '';

  print(color::_BOLD, color::TXT_RED, TOP_BORDER, color::_RESET, "\n");

  int size = todos.size();
  if (size == 0) {
    print(color::_ITALIC, "You have nothing to do...", color::_RESET, "\n");
  }

  for (unsigned int i = 0; i < size; i++) { // im sorry for this abomination
    std::string index = funcs::str(i + 1) + " ";
    std::string t_todo = todos[i];
    std::string full = index + t_todo;
    std::string trimmed = trimStr(full, WIDTH);
    std::string to_print;
    if (line_wrapping) {
      to_print = trimmed;
    } else {
      to_print = full;
    }

    // cout << style(_BOLD, TXT_YELLOW) << index << _RESET << style(_BOLD,
    // TXT_CYAN) << t_todo << _RESET << endl;
    to_print.insert(2, color::_RESET +
                           color::style(color::_BOLD, color::TXT_CYAN));
    to_print.insert(0, color::style(color::_BOLD, color::TXT_YELLOW));
    print(to_print, "\n");
  }

  print(color::_BOLD, color::TXT_RED, BORDER, color::_RESET, "\n");
}

inline bool addItem(std::vector<std::string> &todos, std::string item) {
  if (item == "") {
    return false;
  }
  item = strutils::trim(item);
  todos.push_back(item);
  sort(todos.begin(), todos.end());
  return true;
}

inline bool delItem(std::vector<std::string> &todos, int item_number) {
  if (item_number <= 0 || item_number > todos.size() || todos.size() == 0) {
    return false;
  }
  todos.erase(todos.begin() + item_number - 1);
  return true;
}
