#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "./services/database.h"
#include "./models/session.h"
#include "./models/user.h"

using namespace std;

class UserManager {
    private:
        DatabaseService database;

    public:
        bool addUser(const Session& session, const User& new_user);
        bool deleteUser(const Session& session, const string& student_id);
        bool updateUser(const Session& session, const string& student_id, const User& updated_user);
        bool updatePassword(const Session& session, const string& student_id, const string& new_password);
        optional<vector<User>> getListUsers(const Session& session);

        UserManager(const DatabaseService& database);
        ~UserManager();

};



#endif //USER_MANAGER_H
