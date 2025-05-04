#ifndef REPORT_MANAGEMENT_H
#define REPORT_MANAGEMENT_H

#include "./services/database.h"
#include "./models/session.h"
#include "./models/report.h"

class ReportManagement {
  private:
    DatabaseService *database;

  public:
    bool createReport(const Report& report);
    optional<vector<Report>> getListReports(const Session& session);

    ReportManagement(DatabaseService* database);
    ~ReportManagement();
};



#endif //REPORT_MANAGEMENT_H
