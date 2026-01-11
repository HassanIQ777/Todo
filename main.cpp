/* To-Do list program by HassanIQ777 */

#include "declarations.hpp"
#include "libutils/src/cliparser.hpp"
#include "libutils/src/file.hpp"
#include "libutils/src/log.hpp"

int main(const int argc, char *argv[]) {
  const std::string VERSION = "1.5"; // as of 2026 Jan 10
  const CLIParser parser(argc, argv);

  if (argc != 2 || parser.m_hasFlag("-h")) {
    // help message
    funcs::print("Usage:\n  ", parser.m_getArg(0), "  [PATH TO TODO FOLDER]",
                 "\n  ", "-v    to show version number", "\n  ",
                 "-h    to show this message", "\n");

    exit(1);
  }

  if (parser.m_hasFlag("-v")) {
    funcs::print("Todo version ", VERSION, "\n");
    exit(2);
  }

  std::string todos_folder = parser.m_getArg(1);
  if (!File::m_isdirectory(todos_folder)) {
    Log::m_error("'" + todos_folder + "' is not a directory.");
  }

  if (todos_folder[todos_folder.size()] != '/') {
    todos_folder += "/";
  }

  const std::string todos_path = todos_folder + "todo.txt";
  const std::string info_path = todos_folder + "info.ini";
  const std::string log_path = todos_folder + "logs.txt";

  if (!File::m_isfile(todos_path)) {
    File::m_createfile(todos_path); // create empty todo.txt
  }

  if (!File::m_isfile(info_path)) {
    File::m_createfile(info_path); // create empty info.ini
    File::m_appendline(info_path, "line_wrapping=1");
    File::m_appendline(info_path, "title=To-Do");
  }

  if (!File::m_isfile(log_path)) {
    File::m_createfile(log_path); // create empty logs.txt
  }

  std::vector<std::string> todos =
      File::m_readfile(todos_path); // read the list

  bool running = true;
  bool line_wrapping =
      stoi(File::m_getFromINI(info_path, "line_wrapping")) != 0;
  TITLE = File::m_getFromINI(info_path, "title");

  while (running) {
    funcs::clearTerminal();
    printTodos(todos, line_wrapping);

    print("type 'h' for help", "\n");
    std::string key = funcs::lowercase(funcs::getKeyPress());

    if (key == "q" || key == "\033") { // quit
      running = false;
    } else if (key == "h") { // help message
      print("'+' to add a To-Do\n", "'-' to tick off a To-Do\n",
            "'s' soft line wrapping = ", std::boolalpha, line_wrapping, "\n",
            "'q' to quit", "\n");
      funcs::getKeyPress();
    } else if (key == "=" || key == "+") { // add an item
      print("Add To-Do: ");
      std::string todo;
      std::getline(std::cin, todo);
      todo = strutils::trim(todo);
      if (addItem(todos, todo)) {
        std::string date = funcs::currentTime();
        funcs::replaceChar(date, ' ', '_');
        funcs::replaceChar(date, ':', '_');
        File::m_appendline(log_path, date + ":added:" + todo);
      }
    } else if (key == "-" || key == "_") { // delete an item
      print("To-Do number: ");
      int number;
      std::cin >> number;
      std::string to_delete = todos[number - 1];
      if (delItem(todos, number)) {
        std::string date = funcs::currentTime();
        funcs::replaceChar(date, ' ', '_');
        funcs::replaceChar(date, ':', '_');
        File::m_appendline(log_path, date + ":removed:" + to_delete);
      }
    } else if (key == "s") {
      line_wrapping = !line_wrapping;
      File::m_writeToINI(info_path, "line_wrapping", funcs::str(line_wrapping));
    }

    File::m_writefile(todos_path, todos);
  }
  print("\n");
  funcs::printCentered("Thanks for using Todo\n");
  funcs::printCentered("By HassanIQ777\n\n");
}
