#include "auth.h"
#include "./services/database.h"
#include "./models/session.h"

using namespace std;

Auth::Auth(const DatabaseService& database) : database(database) {}
Auth::~Auth() = default;

Session Auth::login(const string &student_id, const string &password) {
    Session session;
    auto users = database.getUsers();

    for (const auto& user : users) {
        if (user.student_id == student_id && user.password == password) { // TODO: Validate by hash no in text plain
            session.user_id = user.id;
            session.user_name = user.name;
            session.user_student_id = user.student_id;
            session.user_email = user.email;
            session.is_admin = user.is_admin;
            session.is_logged = true;
            return session;
        }
    }

    return session; // Return an empty session if login fails
}

bool Auth::logout(Session& session) {
    session.user_id = 0;
    session.user_name = "";
    session.user_student_id = "";
    session.user_email = "";
    session.is_admin = false;
    return true;
}
