#include "report_management.h"

#include "./services/database.h"
#include "./models/session.h"
#include "./models/report.h"

ReportManagement::ReportManagement(const DatabaseService& database) : database(database) {}

ReportManagement::~ReportManagement() = default;

bool ReportManagement::createReport(const Report& report) {
  return false;
}

optional<vector<Report>> ReportManagement::getListReports(const Session& session) {
  return nullopt;
}