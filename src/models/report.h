#ifndef REPORT_H
#define REPORT_H

#include <string>
using namespace std;

enum Actions {
    CREATE,
    DELETE,
    ENCRYPT,
    DECRYPT,
    SHARE,
};

class Report {
    public:
        int id;
        int encrypted_file_id;
        string encrypted_file_name;
        int user_id;
        string user_name;
        string student_id;
        Actions action;
        string action_date;

        Report();
        ~Report();

        Report(int id, int encrypted_file_id, string& encrypted_file_name, int user_id, string& user_name, string& student_id, Actions action, string& action_date);
};



#endif //REPORT_H
