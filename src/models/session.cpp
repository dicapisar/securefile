//
// Created by Diego Camilo Pinto Sarmiento on 3/5/2025.
//

#include "session.h"
#include <string>

Session::Session() : user_id(0), user_name(""), user_email(""), user_student_id(""), is_admin(false), is_logged(false) {}

Session::Session(int user_id, string user_name, string user_email, string user_student_id, bool user_is_admin, bool is_logged)
    : user_id(user_id), user_name(user_name), user_email(user_email), user_student_id(user_student_id), is_admin(user_is_admin), is_logged(is_logged) {}

Session::~Session() = default;