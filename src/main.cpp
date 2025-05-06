#include <iostream>
#include <vector>
#include <variant>
#include <core/file_management/file_management.h>
#include <core/user_management/user_manager.h>
#include <limits>
#include <regex>
#include <core/report_management/report_management.h>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

using namespace std;

#include "services/database.h"
#include  "services/file.h"
#include  "services/encrypt.h"
#include "core/auth/auth.h"
#include <string>
#include "ui/ui.h"

class Dependencies {
public:
    DatabaseService *database;
    EncryptService *encrypt;
    FileService *file;

    Dependencies(DatabaseService *database, EncryptService *encrypt, FileService *file) : database(database),
        encrypt(encrypt), file(file) {
    }

    ~Dependencies() {
        delete database;
        delete encrypt;
        delete file;
    }
};

Dependencies loadDependenciesTest() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hOut, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }
#endif

    cout << "👀 Validating installation of Libraries...\n";

    auto *database_service = new DatabaseService();
    database_service->load_SQL_file();

    auto *file_service = new FileService();
    auto *encrypt_service = new EncryptService();

    cout << "🎉 All Libraries Working Successfully.\n";

    Dependencies dependencies = Dependencies(database_service, encrypt_service, file_service);

    return dependencies;
}

bool isSecurePassword(const string& password) {
    static const regex pattern(
        R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\W_]).{8,}$)"
    );
    return regex_match(password, pattern);
}

bool isValidEmail(const string& email) {
    static const regex pattern(
        R"(^[\w.%+-]+@[\w.-]+\.[A-Za-z]{2,}$)"
    );
    return regex_match(email, pattern);
}

void start() {
    Dependencies dependencies = loadDependenciesTest();

    Auth auth{dependencies.database, dependencies.encrypt};
    UserManager user_manager{dependencies.database, dependencies.encrypt};
    ReportManagement report_management(dependencies.database);
    FileManagement file_management{
        dependencies.database, dependencies.encrypt, dependencies.file, (&report_management)
    };
    Session session;
    bool isLoggedIn = false;
    bool isRunning = true;

    UI::showWelcomeMessage();

    while (isRunning) {
        // Check if the user is logged in
        if (!isLoggedIn) {
            vector<string> loginOptions = {
                "Login",
                "Quit",
            };

            int optionSelected = UI::showMenu(loginOptions);
            UI::showMessage("You selected option " + to_string(optionSelected), MessageType::Info);


            if (optionSelected == 1) {
                UI::showMessage("Please enter your credentials", MessageType::Info);

                UI::showMessage("Student ID: ", MessageType::Info);
                string student_id;
                cin >> student_id;

                UI::showMessage("Password: ", MessageType::Info);
                string password;
                cin >> password;

                session = auth.login(student_id, password);

                if (session.is_logged) {
                    isLoggedIn = true;
                    UI::showMessage("Welcome " + session.user_name, MessageType::Success);
                } else {
                    UI::showMessage("Login failed", MessageType::Error);
                    UI::showMessage("Please try again", MessageType::Info);
                    continue;
                }
            } else {
                break;
            }
        }

        if (session.is_admin) {
            bool seeManageUsers = false;
            UI::showMessage("Do you want to manage users?", MessageType::Warning);

            vector<string> questionOptions = {
                "Yes", "No"
            };

            int optionSelected = UI::showMenu(questionOptions);
            UI::showMessage("You selected option " + to_string(optionSelected), MessageType::Info);

            if (optionSelected == 1) {
                seeManageUsers = true;

                while (seeManageUsers) {
                    UI::showMessage("Managing users...", MessageType::Info);
                    vector<string> userManagerOptions = {
                        "Create User",
                        "Update User",
                        "Delete User",
                        "List Users",
                        "Go to Main Menu"
                    };
                    optionSelected = UI::showMenu(userManagerOptions);

                    UI::showMessage("You selected option " + to_string(optionSelected), MessageType::Info);

                    switch (optionSelected) {
                        case 1: {
                            UI::showMessage("Create User...", MessageType::Info);

                            User newUser = User();

                            UI::showMessage("Please enter the student ID: ", MessageType::Info);
                            string student_id;
                            cin >> student_id;
                            newUser.student_id = student_id;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            UI::showMessage("Please enter the password: ", MessageType::Info);
                            // TODO: validate that the password is strong
                            string password;
                            getline(cin, password);
                            newUser.password = password;

                            while (!isSecurePassword(password)) {
                                UI::showMessage("Please enter a strong password: ", MessageType::Error);
                                getline(cin, password);
                                newUser.password = password;
                            }

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            UI::showMessage("Please enter the name: ", MessageType::Info);
                            string name;
                            getline(cin, name);
                            newUser.name = name;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            UI::showMessage("Please enter the email: ", MessageType::Info);
                            string email;
                            getline(cin, email);
                            newUser.email = email;

                            while (!isValidEmail(email)) {
                                UI::showMessage("Please enter a valid email: ", MessageType::Error);
                                getline(cin, email);
                                newUser.email = email;
                            }

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            bool is_ok = user_manager.addUser(session, newUser);

                            if (is_ok) {
                                UI::showMessage("User created successfully", MessageType::Success);
                            } else {
                                UI::showMessage("Error creating user, please try again", MessageType::Error);
                            }

                            break;
                        }
                        case 2: {
                            UI::showMessage("Update User...", MessageType::Info);

                            UI::showMessage("Please enter the student ID: ", MessageType::Info);
                            string student_id;
                            cin >> student_id;

                            optional<User> user = user_manager.getUser(session, student_id);
                            if (!user.has_value()) {
                                UI::showMessage("User not found", MessageType::Error);
                                break;
                            }

                            User userUpdated = user.value();

                            UI::showMessage("Please enter the new student id: ", MessageType::Info);
                            string new_student_id;
                            cin >> new_student_id;

                            UI::showMessage("Please enter the new name: ", MessageType::Info);
                            string new_name;
                            cin >> new_name;

                            UI::showMessage("Please enter the new email: ", MessageType::Info);
                            string new_email;
                            cin >> new_email;

                            userUpdated.student_id = student_id;
                            userUpdated.name = new_name;
                            userUpdated.email = new_email;

                            bool is_ok = user_manager.updateUser(session, userUpdated);

                            if (is_ok) {
                                UI::showMessage("User updated successfully", MessageType::Success);
                            } else {
                                UI::showMessage("Error updated user, please try again", MessageType::Error);
                            }

                            break;
                        }
                        case 3: {
                            UI::showMessage("Delete User...", MessageType::Info);

                            UI::showMessage("Please enter the student ID to delete: ", MessageType::Info);
                            string student_id;
                            cin >> student_id;

                            optional<User> user = user_manager.getUser(session, student_id);
                            if (!user.has_value()) {
                                UI::showMessage("User not found", MessageType::Error);
                                break;
                            }

                            UI::showMessage("Are you sure you want to delete the user?", MessageType::Warning);
                            questionOptions = {
                                "Yes", "No"
                            };
                            optionSelected = UI::showMenu(questionOptions);
                            UI::showMessage("You selected option " + to_string(optionSelected), MessageType::Info);
                            if (optionSelected == 2) {
                                UI::showMessage("User not deleted", MessageType::Info);
                                break;
                            }

                            bool is_ok = user_manager.deleteUser(session, student_id);

                            if (is_ok) {
                                UI::showMessage("User deleted successfully", MessageType::Success);
                            } else {
                                UI::showMessage("Error deleting user, please try again", MessageType::Error);
                            }

                            break;
                        }
                        case 4: {
                            UI::showMessage("List Users...", MessageType::Info);

                            optional<vector<User> > users = user_manager.getListUsers(session);
                            if (!users.has_value()) {
                                UI::showMessage("No users found", MessageType::Warning);
                                break;
                            }

                            // Show the list of users
                            vector<string> headers = {"ID", "Student ID", "Name", "Email", "Is Admin"};
                            vector<map<string, string> > rows;
                            int index = 1;
                            for (const auto &user: *users) {
                                map<string, string> row;
                                row["ID"] = to_string(index);
                                row["Student ID"] = user.student_id;
                                row["Name"] = user.name;
                                row["Email"] = user.email;
                                row["Is Admin"] = user.is_admin ? "Yes" : "No";
                                rows.push_back(row);
                                index++;
                            }
                            UI::showTableWithInformation(headers, rows);

                            break;
                        }
                        case 5: {
                            UI::showMessage("Going to Main Menu...", MessageType::Info);
                            seeManageUsers = false;
                            break;
                        }
                        default: {
                            UI::showMessage("Invalid option", MessageType::Error);
                        }
                    }
                }
            } else {
                seeManageUsers = false;
            }


            UI::showMessage("Continuing to file management...", MessageType::Info);
        }


        bool stillRunning = true;
        while (stillRunning && isLoggedIn) {
            // Show the main menu if the user is logged in
            vector<string> mainMenuOptions = {
                "Encrypt File",
                "Decrypt File",
                "Delete File",
                "List Files",
                "Share File",
                "See Report",
                "Logout"
            };

            int optionSelected = UI::showMenu(mainMenuOptions);
            UI::showMessage("You selected option " + to_string(optionSelected), MessageType::Info);

            switch (optionSelected) {
                case 1: {
                    UI::showMessage("Encrypting file...", MessageType::Info);

                    // Request the file name
                    UI::showMessage("Please enter the file name to encrypt: ", MessageType::Info);
                    string file_name;
                    cin >> file_name;

                    // Request the password
                    UI::showMessage("Please enter the password: ", MessageType::Info);
                    string password;
                    cin >> password;

                    bool is_ok = file_management.encryptFile(session, file_name, password);

                    if (is_ok) {
                        UI::showMessage("File encrypted successfully", MessageType::Success);
                    } else {
                        UI::showMessage("Error encrypting file", MessageType::Error);
                    }
                    break;
                }
                case 2: {
                    UI::showMessage("Decrypting file...", MessageType::Info);

                    // Get all encrypted files allowed to the user
                    optional<vector<EncryptedFile> > encrypted_files = file_management.getListEncryptedFiles(session);

                    if (!encrypted_files.has_value()) {
                        UI::showMessage("No encrypted files found", MessageType::Warning);
                        break;
                    }

                    // Show the list of encrypted files
                    vector<string> headers = {"ID", "File Name", "Owner"};
                    vector<map<string, string> > rows;

                    int index = 1;
                    for (const auto &file: *encrypted_files) {
                        map<string, string> row;
                        row["ID"] = to_string(index);
                        row["File Name"] = file.file_name;
                        row["Owner"] = file.owner.name;
                        rows.push_back(row);
                        index++;
                    }
                    UI::showTableWithInformation(headers, rows);

                    UI::showMessage("Please select the file ID to decrypt: ", MessageType::Info);
                    int file_id;
                    cin >> file_id;

                    if (file_id < 1 || file_id > rows.size()) {
                        UI::showMessage("Invalid file ID", MessageType::Error);
                        break;
                    }

                    EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                    if (fileSelected.password != "" && fileSelected.owner.id != session.user_id) {
                        UI::showMessage("Please enter the password: ", MessageType::Info);
                        string password;
                        cin >> password;

                        if (fileSelected.password != password) {
                            UI::showMessage("Invalid password", MessageType::Error);
                            break;
                        }
                    }

                    bool isOK = file_management.decryptFile(session, fileSelected.id, fileSelected.password);

                    if (!isOK) {
                        UI::showMessage("Error decrypting file", MessageType::Error);
                    } else {
                        UI::showMessage("File decrypted successfully", MessageType::Success);
                    }
                    break;
                }
                case 3: {
                    UI::showMessage("Deleting file...", MessageType::Info);

                    // 1. Get all encrypted files allowed to the user
                    optional<vector<EncryptedFile> > encrypted_files = file_management.getListEncryptedFiles(session);

                    if (!encrypted_files.has_value()) {
                        UI::showMessage("No encrypted files found", MessageType::Warning);
                        break;
                    }
                    // 2. remove the encrypted files which the owner is not the user of the session
                    vector<string> headers = {"ID", "File Name"};
                    vector<map<string, string> > rows;

                    int index = 1;
                    for (const auto &file: *encrypted_files) {
                        if (file.owner.id == session.user_id) {
                            map<string, string> row;
                            row["ID"] = to_string(index);
                            row["File Name"] = file.file_name;
                            rows.push_back(row);
                        }
                        index++;
                    }

                    // 3. Show the list of encrypted files
                    UI::showTableWithInformation(headers, rows);

                    // 4. Request the file ID to delete
                    UI::showMessage("Please select the file ID to delete: ", MessageType::Info);
                    int file_id;
                    cin >> file_id;

                    if (file_id < 1 || file_id > rows.size()) {
                        UI::showMessage("Invalid file ID", MessageType::Error);
                        break;
                    }

                    // 5. Get the path of the file selected
                    EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                    // 6. Call the delete function of file management
                    bool isOK = file_management.deleteFile(session, fileSelected.id, fileSelected.file_name);

                    // 7. show the message of success or error
                    if (!isOK) {
                        UI::showMessage("Error deleting file", MessageType::Error);
                    } else {
                        UI::showMessage("File deleted successfully", MessageType::Success);
                    }
                    break;
                }
                case 4: {
                    UI::showMessage("Listing files...", MessageType::Info);
                    // Get all encrypted files allowed to the user
                    optional<vector<EncryptedFile> > encrypted_files = file_management.getListEncryptedFiles(session);

                    if (!encrypted_files.has_value()) {
                        UI::showMessage("No encrypted files found", MessageType::Warning);
                        break;
                    }

                    // Show the list of encrypted files
                    vector<string> headers = {"ID", "File Name", "Owner"};
                    vector<map<string, string> > rows;

                    int index = 1;
                    for (const auto &file: *encrypted_files) {
                        map<string, string> row;
                        row["ID"] = to_string(index);
                        row["File Name"] = file.file_name;
                        row["Owner"] = file.owner.name;
                        rows.push_back(row);
                        index++;
                    }
                    UI::showTableWithInformation(headers, rows);
                    break;
                }
                case 5: {
                    UI::showMessage("Sharing file...", MessageType::Info);

                    // 1. Get all encrypted files allowed to the user
                    optional<vector<EncryptedFile> > encrypted_files = file_management.getListEncryptedFiles(session);

                    if (!encrypted_files.has_value()) {
                        UI::showMessage("No encrypted files found", MessageType::Warning);
                        break;
                    }

                    // 2. remove the encrypted files which the owner is not the user of the session
                    vector<string> headers = {"ID", "File Name"};
                    vector<map<string, string> > rows;

                    int index = 1;
                    for (const auto &file: *encrypted_files) {
                        if (file.owner.id == session.user_id) {
                            map<string, string> row;
                            row["ID"] = to_string(index);
                            row["File Name"] = file.file_name;
                            rows.push_back(row);
                        }
                        index++;
                    }

                    // 3. Show the list of encrypted files
                    UI::showTableWithInformation(headers, rows);

                    // 4. Request the file ID to share
                    UI::showMessage("Please select the file ID to share: ", MessageType::Info);
                    int file_id;
                    cin >> file_id;

                    if (file_id < 1 || file_id > rows.size()) {
                        UI::showMessage("Invalid file ID", MessageType::Error);
                        break;
                    }

                    // 5. Get the encrypted file selected
                    EncryptedFile fileSelected = encrypted_files.value()[file_id - 1];

                    // 6. Validate if the encrypted file has password
                    if (fileSelected.password == "") {
                        ;
                        UI::showMessage("The file does not have a password, please enter a password: ",
                                        MessageType::Info);
                        string password;
                        cin >> password;
                    }

                    // 7. If the file has password, request the password
                    if (fileSelected.password != "") {
                        UI::showMessage("The file has a password, please enter the password: ", MessageType::Info);
                        string password;
                        cin >> password;

                        if (fileSelected.password != password) {
                            UI::showMessage("Invalid password", MessageType::Error);
                            break;
                        }
                    }

                    // 8. Request the student ID to share
                    UI::showMessage("Please enter the student ID to share: ", MessageType::Info);
                    string student_id;
                    cin >> student_id;

                    // 9. Get the path of the file selected
                    // ??????

                    // 10. Call the share function of file management
                    bool isOK = file_management.shareFile(session, fileSelected.id, fileSelected.file_name, student_id,
                                                          fileSelected.password);

                    // 11. show the message of success or error
                    if (!isOK) {
                        UI::showMessage("Error sharing file", MessageType::Error);
                    } else {
                        UI::showMessage("File shared successfully", MessageType::Success);
                    }
                    break;
                }
                case 6: {
                    UI::showMessage("Generating report...", MessageType::Info);

                    // 1. Get all reports allowed to the user
                    optional<vector<Report> > reports = report_management.getListReports(session);

                    // 2. Show the list of reports
                    if (!reports.has_value()) {
                        UI::showMessage("No Reports found", MessageType::Warning);
                        break;
                    }

                    vector<string> headers = {
                        "ID", "ID File", "File Name", "User ID",
                        "Owner", "Student ID", "Action", "Date Action"
                    };
                    vector<map<string, string> > rows;
                    int index = 1;
                    for (const auto &rpt: *reports) {
                        map<string, string> row;
                        row["ID"] = to_string(index++);
                        row["ID File"] = to_string(rpt.encrypted_file_id);
                        row["File Name"] = rpt.encrypted_file_name;
                        row["User ID"] = to_string(rpt.user_id);
                        row["Owner"] = rpt.user_name;
                        row["Student ID"] = rpt.student_id;
                        row["Action"] = (rpt.action == CREATE
                                             ? "CREATE"
                                             : rpt.action == DELETE
                                                   ? "DELETE"
                                                   : rpt.action == ENCRYPT
                                                         ? "ENCRYPT"
                                                         : rpt.action == DECRYPT
                                                               ? "DECRYPT"
                                                               : "SHARE");
                        row["Date Action"] = rpt.action_date;
                        rows.push_back(row);
                    }
                    UI::showTableWithInformation(headers, rows);

                    break;
                }
                case 7: {
                    isLoggedIn = false;
                    stillRunning = false;
                    UI::showMessage("Logging out...", MessageType::Success);
                    break;
                }
                default: {
                    UI::showMessage("Invalid option", MessageType::Error);
                }
            }
        }

        if (!isLoggedIn) {
            break;
        }
    }

    UI::showExitMessage();
}

int main() {
    start();
    return 0;
}
