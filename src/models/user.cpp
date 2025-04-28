//
// Created by Diego Camilo Pinto Sarmiento on 19/3/2025.
//

#include "user.h"
#include <string>

User::User() : id(0), student_id(""), name(""), email(""), password(""), is_admin(false), created_at("") {}

User::User(int id, const string& student_id, const string& name, const string& email, const string& password,
           bool is_admin, const string& created_at)
    : id(id), student_id(student_id), name(name), email(email), password(password),
      is_admin(is_admin), created_at(created_at) {}

User::~User() = default;

