//
// Created by Diego Camilo Pinto Sarmiento on 3/5/2025.
//

#ifndef SESSION_H
#define SESSION_H

#include <string>

using namespace std;

class Session {
    public:
        int user_id;
        string user_name;
        string user_email;
        string user_student_id;
        bool is_admin;
        bool is_logged;

        Session();
        ~Session();

        Session(int user_id, string user_name, string user_email, string user_student_id, bool user_is_admin, bool is_logged);
};



#endif //SESSION_H
