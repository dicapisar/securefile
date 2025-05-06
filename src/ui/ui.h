// src/ui/ui.h
#ifndef UI_H
#define UI_H

#include <vector>
#include <string>
#include <map>

enum class MessageType {
    Info,
    Success,
    Warning,
    Error
};

class UI {
public:
    static void clearScreen();
    static void showWelcomeMessage();
    static void showExitMessage();
    static int showMenu(const std::vector<std::string>& options);
    static void showTableWithInformation(const std::vector<std::string>& headers, const std::vector<std::map<std::string, std::string>>& rows);
    static void showMessage(const std::string& message, MessageType type = MessageType::Info);
};

#endif // UI_H