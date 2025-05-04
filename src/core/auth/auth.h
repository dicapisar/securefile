//
// Created by Diego Camilo Pinto Sarmiento on 3/5/2025.
//

#ifndef AUTH_H
#define AUTH_H

#include "./services/database.h"
#include "./models/session.h"

class Auth {
    private:
    DatabaseService *database;
    public:
        Session login(const string &student_id, const string &password);
        bool logout(Session& session);

        Auth(DatabaseService* db);
        ~Auth();
};



#endif //AUTH_H
