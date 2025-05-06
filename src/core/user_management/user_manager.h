#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <services/encrypt.h>

#include "./services/database.h"
#include "./models/session.h"
#include "./models/user.h"

using namespace std;

class UserManager {
    private:
        DatabaseService *database;
        EncryptService *encryptService;

    public:
        bool addUser(const Session& session, User& new_user);
        bool deleteUser(const Session& session, const string& student_id);
        bool updateUser(const Session& session, const User& updated_user);
        bool updatePassword(const Session& session, const string& student_id, const string& new_password);
        optional<vector<User>> getListUsers(const Session& session);
        optional<User> getUser(const Session& session, const string& student_id);

        UserManager(DatabaseService* database, EncryptService* encrypt_service);
        ~UserManager();

};



#endif //USER_MANAGER_H
