//
// Created by Diego Camilo Pinto Sarmiento on 19/3/2025.
//

#ifndef USER_H
#define USER_H

#include <iostream>
using namespace std;


class User {
public:
  int id;
  string student_id;
  string name;
  string email;
  string password;
  bool is_admin;
  string created_at;

  User();
  ~User();

  User(int id, const string& student_id, const string& name, const string& email, const string& password,
       bool is_admin, const string& created_at);
};



#endif //USER_H
