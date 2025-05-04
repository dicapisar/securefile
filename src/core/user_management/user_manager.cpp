//
// Created by Diego Camilo Pinto Sarmiento on 3/5/2025.
//

#include "user_manager.h"

#include "./services/database.h"
#include "./models/session.h"
#include "./models/user.h"

using namespace std;

UserManager::UserManager(DatabaseService* database) : database(database) {}
UserManager::~UserManager() = default;

bool UserManager::addUser(const Session& session, const User& new_user) {
    return true;
}

bool UserManager::deleteUser(const Session& session, const string& student_id) {
    return true;
}

bool UserManager::updateUser(const Session& session, const string& student_id, const User& updated_user) {
    return true;
}

bool UserManager::updatePassword(const Session& session, const string& student_id, const string& new_password) {
    return true;
}

optional<vector<User>> UserManager::getListUsers(const Session& session) {
    return nullopt;
}
