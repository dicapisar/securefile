//
// Created by Diego Camilo Pinto Sarmiento on 3/5/2025.
//

#include "user_manager.h"

#include <services/encrypt.h>

#include "./services/database.h"
#include "./models/session.h"
#include "./models/user.h"

using namespace std;

UserManager::UserManager(DatabaseService* database, EncryptService* encryptService) : database(database), encryptService(encryptService) {}
UserManager::~UserManager() = default;

bool UserManager::addUser(const Session& session, User& new_user) {
    if (!session.is_admin) {
        return false;
    }

    string hashed_password = encryptService->hashPassword(new_user.password);
    new_user.password = hashed_password;

    bool is_ok = database->createUser(new_user);

    return is_ok;
}

bool UserManager::deleteUser(const Session& session, const string& student_id) {
    if (!session.is_admin) {
        return false;
    }

    bool is_ok = database->deleteUser(student_id);

    return is_ok;
}

bool UserManager::updateUser(const Session& session, const User& updated_user) {
    if (!session.is_admin) {
        return false;
    }

    bool is_ok = database->updateUser(updated_user);

    return is_ok;

}

bool UserManager::updatePassword(const Session& session, const string& student_id, const string& new_password) {
    if (!session.is_admin) {
        return false;
    }

    bool is_ok = database->updateUserPassword(student_id, new_password);

    return is_ok;
}

optional<vector<User>> UserManager::getListUsers(const Session& session) {
    if (!session.is_admin) {
        return nullopt;
    }

    vector<User> usersList = database->getUsers();

    if (usersList.empty()) {
        return nullopt;
    }

    return usersList;
}

optional<User> UserManager::getUser(const Session& session, const string& student_id) {
    if (!session.is_admin) {
        return nullopt;
    }

    vector<User> usersList = database->getUsers();

    for (const User& user : usersList) {
        if (user.student_id == student_id) {
            return user;
        }
    }

    return nullopt;
}
