// src/ui/ui.cpp
#include "ui.h"
#include <iostream>
#include <iomanip>


using namespace std;

static const map<MessageType, string> COLOR_CODE = {
    { MessageType::Error,   "\033[91m" },  // Red
    { MessageType::Success, "\033[92m" },  // Green
    { MessageType::Warning, "\033[93m" },  // Yellow
    { MessageType::Info,    "\033[97m" }   // White
};

static const map<MessageType, string> PREFIX = {
    { MessageType::Error,   "❌ [ERROR] "   },
    { MessageType::Success, "✅ [SUCCESS] " },
    { MessageType::Warning, "⚠️  [WARN] "   },
    { MessageType::Info,    "ℹ️  [INFO] "   }
};

void UI::showWelcomeMessage() {

    const char* message = R"(
Welcome to:
    █▀▀ █▀▀ █▀▀ █ █ █▀▄ █▀▀   █▀▀ ▀█▀ █   █▀▀
    ▀▀█ █▀▀ █   █ █ █▀▄ █▀▀   █▀▀  █  █   █▀▀
    ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀ ▀▀▀   ▀   ▀▀▀ ▀▀▀ ▀▀▀
Created by:
    Name: Christophe Nicolas Bourgeonnier, Diego Camilo Pinto Sarmiento, Mayerli Almario Arevalo
    Student ID: A00173434 , A00151853, A00XXXXXX
)";

    cout << message << endl;
}

void UI::showExitMessage() {
    cout << "\n👋 ¡Adiós!\n\n";
}

int UI::showMenu(const vector<string>& options) {
    cout << "Select an option:\n";
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  " << setw(2) << (i + 1) << ") " << options[i] << "\n";
    }
    cout << "> ";

    int option_selected;
    cin >> option_selected;

    while (true) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            showMessage("Invalid input. Please enter a number.", MessageType::Error);
            cout << "> ";
            cin >> option_selected;
        } else {
            if (option_selected < 1 || option_selected > (int)options.size()) {
                showMessage("Invalid option. Please try again.", MessageType::Error);
                cout << "> ";
                cin >> option_selected;
            } else {
                break;
            }
        }
    }

    return option_selected;
}

void UI::showTableWithInformation(const vector<string>& headers, const vector<map<string,string>>& rows) {
    size_t cols = headers.size();
    vector<size_t> width(cols);

    for (size_t i = 0; i < cols; ++i) {
        width[i] = headers[i].size();
    }
    for (auto& row : rows) {
        for (size_t i = 0; i < cols; ++i) {
            auto it = row.find(headers[i]);
            if (it != row.end()) {
                width[i] = max(width[i], it->second.size());
            }
        }
    }

    for (size_t i = 0; i < cols; ++i) {
        cout << left << setw(width[i] + 2) << headers[i];
    }
    cout << "\n";

    for (size_t i = 0; i < cols; ++i) {
        cout << string(width[i] + 2, '-');
    }
    cout << "\n";

    for (auto& row : rows) {
        for (size_t i = 0; i < cols; ++i) {
            string value = "";
            auto it = row.find(headers[i]);
            if (it != row.end()) value = it->second;
            cout << left << setw(width[i] + 2) << value;
        }
        cout << "\n";
    }
}

void UI::showMessage(const string& message, MessageType type){
    const std::string& color  = COLOR_CODE.at(type);
    const std::string& prefix = PREFIX.at(type);
    const std::string reset   = "\033[0m";

    std::cout
        << "\n"
        << color << prefix << message << reset
        << "\n";
}

