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

void UI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void UI::showWelcomeMessage() {

    const char* message = R"(
Welcome to:
    █▀▀ █▀▀ █▀▀ █ █ █▀▄ █▀▀   █▀▀ ▀█▀ █   █▀▀
    ▀▀█ █▀▀ █   █ █ █▀▄ █▀▀   █▀▀  █  █   █▀▀
    ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀ ▀ ▀ ▀▀▀   ▀   ▀▀▀ ▀▀▀ ▀▀▀
Created by:
    Name: Christophe Nicolas Bourgeonnier, Diego Camilo Pinto Sarmiento, Mayerli Almario Arevalo
    Student ID: A00173434 , A00151853, A00143310
)";

    cout << message << endl;
}

void UI::showExitMessage() {
    cout << "\n👋 ¡Bye!\n\n";
}

int UI::showMenu(const vector<string>& options) {
    clearScreen();
    showMessage("Select an option:", MessageType::Info);
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  " << setw(2) << (i + 1) << ") " << options[i] << "\n";
    }
    cout << "> " << flush;

    int option_selected;
    cin >> option_selected;
    while (true) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            showMessage("Invalid input. Please enter a number.", MessageType::Error);
            cout << "> " << flush;
            cin >> option_selected;
        } else if (option_selected < 1 || option_selected > static_cast<int>(options.size())) {
            showMessage("Invalid option. Please try again.", MessageType::Error);
            cout << "> " << flush;
            cin >> option_selected;
        } else {
            break;
        }
    }
    return option_selected;
}

void UI::showTableWithInformation(
    const vector<string>& headers,
    const vector<map<string,string>>& rows
) {
    size_t cols = headers.size();
    vector<size_t> width(cols);
    // Calcular anchos de columna
    for (size_t i = 0; i < cols; ++i) {
        width[i] = headers[i].size();
    }
    for (const auto& row : rows) {
        for (size_t i = 0; i < cols; ++i) {
            auto it = row.find(headers[i]);
            if (it != row.end()) {
                width[i] = max(width[i], it->second.size());
            }
        }
    }

    // Borde superior
    cout << "┌";
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < width[i] + 2; ++j) cout << "─";
        cout << (i + 1 < cols ? "┬" : "┐");
    }
    cout << "\n";

    // Fila de encabezados
    cout << "│";
    for (size_t i = 0; i < cols; ++i) {
        cout << " " << left << setw(width[i]) << headers[i] << " ";
        cout << "│";
    }
    cout << "\n";

    // Separador de encabezados
    cout << "├";
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < width[i] + 2; ++j) cout << "─";
        cout << (i + 1 < cols ? "┼" : "┤");
    }
    cout << "\n";

    // Filas de datos
    for (const auto& row : rows) {
        cout << "│";
        for (size_t i = 0; i < cols; ++i) {
            string value;
            auto it = row.find(headers[i]);
            if (it != row.end()) value = it->second;
            cout << " " << left << setw(width[i]) << value << " ";
            cout << "│";
        }
        cout << "\n";
    }

    // Borde inferior
    cout << "└";
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < width[i] + 2; ++j) cout << "─";
        cout << (i + 1 < cols ? "┴" : "┘");
    }
    cout << "\n";
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

