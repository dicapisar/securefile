//
// Created by Diego Camilo Pinto Sarmiento on 4/5/2025.
//

#include "report.h"

Report::Report() : id(0), encrypted_file_id(0), encrypted_file_name(""), user_id(0), user_name(""), student_id(""), action(CREATE), action_date("") {}

Report::Report(int id, int encrypted_file_id, string &encrypted_file_name, int user_id, string &user_name, string &student_id, Actions action, string &action_date)
    : id(id), encrypted_file_id(encrypted_file_id), encrypted_file_name(encrypted_file_name), user_id(user_id), user_name(user_name), student_id(student_id), action(action), action_date(action_date) {}

Report::~Report() = default;
