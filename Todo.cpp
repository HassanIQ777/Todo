/* To-Do list program by HassanIQ777 */

#include "libutils/src/file.hpp"
#include "libutils/src/color.hpp"
#include "libutils/src/funcs.hpp"
#include "libutils/src/cliparser.hpp"
#include "libutils/src/log.hpp"

#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace color;

// function definitions
std::string trimStr(std::string text, int WIDTH);
void printTodos(vector<string> &todos, bool cutLongText);
bool addItem(vector<string> &todos, string item);
bool delItem(vector<string> &todos, int item_number);
//#####################

string TITLE = "To-Do";

int main(int argc, char *argv[])
{
	const string VERSION = "v1.3.4.1 release"; // as of 2025 Jun 30
	CLIParser parser(argc, argv);

	if (argc != 2 || parser.m_hasFlag("-h"))
	{
		// help message
		funcs::print("Usage:\n  ",
					 parser.m_getArg(0), "  [PATH TO TODO FOLDER]", "\n  ",
					 "-v    to show version number", "\n  ",
					 "-h    to show this message", "\n");

		exit(1);
	}

	if (parser.m_hasFlag("-v"))
	{
		funcs::print("Todo version ", VERSION, "\n");
		exit(2);
	}

	string todos_folder = parser.m_getArg(1);
	if (!File::m_isdirectory(todos_folder))
	{
		Log::m_error("'" + todos_folder + "' is not a directory.");
	}

	if (todos_folder[todos_folder.size()] != '/')
	{
		todos_folder += "/";
	}

	string todos_path = todos_folder + "todo.txt";
	string info_path = todos_folder + "info.ini";
	string log_path = todos_folder + "logs.txt";

	if (!File::m_isfile(todos_path))
	{
		File::m_checkfile(todos_path, true); // create file if it doesn't exist yet
		File::m_appendline(info_path, "cut_long_text=1");
		File::m_appendline(info_path, "title=To-Do");
	}

	if (!File::m_isfile(info_path))
	{
		File::m_checkfile(info_path, true);
	}

	if (!File::m_isfile(log_path))
	{
		File::m_checkfile(log_path, true);
	}

	vector<string> todos = File::m_readfile(todos_path); // read the list

	bool running = true;
	bool cut_long_text = stoi(File::m_getFromINI(info_path, "cut_long_text"));
	TITLE = File::m_getFromINI(info_path,"title");

	while (running)
	{
		funcs::clearTerminal();
		printTodos(todos, cut_long_text);

		cout << "type 'h' for help" << endl;
		string key = funcs::lowercase(funcs::getKeyPress());

		if (key == "q")
		{ // quit
			running = false;
			break;
		}
		else if (key == "h")
		{ // help message
			cout << "'a' to add a To-Do\n"
				 << "'-' to tick off a To-Do\n"
				 << "'c' to cut off long text = " << boolalpha << cut_long_text << "\n"
				 //<< "remember = " << boolalpha << remember << "\n"
				 << "'q' to quit" << endl;
			funcs::getKeyPress();
		}
		else if (key == "a")
		{ // add an item
			cout << "Add To-Do: ";
			string todo;
			getline(cin, todo);
			if (addItem(todos, todo))
			{
				string date = funcs::currentTime();
				funcs::replaceChar(date, ' ', '_');
				funcs::replaceChar(date, ':', '_');
				File::m_appendline(log_path, date + ":added:" + todo);
			}
		}
		else if (key == "-")
		{ // delete an item
			cout << "To-Do number: ";
			int number;
			cin >> number;
			string to_delete = todos[number - 1];
			if (delItem(todos, number))
			{
				string date = funcs::currentTime();
				funcs::replaceChar(date, ' ', '_');
				funcs::replaceChar(date, ':', '_');
				File::m_appendline(log_path, date + ":removed:" + to_delete);
			}
		}
		else if (key == "c")
		{
			cut_long_text = !cut_long_text;
			File::m_writeToINI(info_path, "cut_long_text", funcs::str(cut_long_text));
		}

		File::m_writefile(todos_path, todos);
	}
	cout << endl;
	funcs::printCentered("Thanks for using Todo\n", 10);
	funcs::printCentered("By HassanIQ777\n\n", 10);
}

std::string trimStr(std::string text, int WIDTH)
{
	if (text.length() <= WIDTH)
	{
		return text;
	}
	return text.substr(0, WIDTH - 2) + "··";
}

void printTodos(vector<string> &todos, bool cutLongText)
{
	const int WIDTH = funcs::getTerminalWidth();
	const string BORDER_CHAR = "─";
	string BORDER;
	for (int i = 0; i < WIDTH; i++)
	{
		BORDER += BORDER_CHAR;
	}
	string TOP_BORDER_LINING;
	string title = TITLE; // im sorry this sucks
	for (int i = 0; i < ((WIDTH - title.size()) / 2); i++)
	{
		TOP_BORDER_LINING += BORDER_CHAR;
	}
	string TOP_BORDER = TOP_BORDER_LINING + title + TOP_BORDER_LINING;
	if (WIDTH % 2 == 0)
	{
		TOP_BORDER += BORDER_CHAR;
	}
	//if(title.size()%2 == 1) TOP_BORDER[TOP_BORDER.size() - 1] = '';

	cout << _BOLD << TXT_RED << TOP_BORDER << _RESET << endl;

	int size = todos.size();
	if (size == 0)
	{
		cout << _ITALIC << "You have nothing to do..." << _RESET << endl;
	}

	for (unsigned int i = 0; i < size; i++)
	{
		string index = funcs::str(i + 1) + " ";
		string t_todo = todos[i];
		string full = index + t_todo;
		string trimmed = trimStr(full, WIDTH);
		string to_print;
		if (cutLongText)
		{
			to_print = trimmed;
		}
		else
		{
			to_print = full;
		}

		//cout << style(_BOLD, TXT_YELLOW) << index << _RESET << style(_BOLD, TXT_CYAN) << t_todo << _RESET << endl;
		to_print.insert(2, _RESET + style(_BOLD, TXT_CYAN)); //69 hahaja
		to_print.insert(0, style(_BOLD, TXT_YELLOW));
		cout << to_print << endl;
	}

	cout << _BOLD << TXT_RED << BORDER << _RESET << endl;
}

bool addItem(vector<string> &todos, string item)
{
	if (item == "")
	{
		return false;
	}
	todos.push_back(item);
	sort(todos.begin(), todos.end());
	return true;
}

bool delItem(vector<string> &todos, int item_number)
{
	if (item_number <= 0 || item_number > todos.size())
	{
		return false;
	}
	todos.erase(todos.begin() + item_number - 1);
	return true;
}