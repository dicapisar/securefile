#include "report_management.h"

#include "./services/database.h"
#include "./models/session.h"
#include "./models/report.h"

ReportManagement::ReportManagement(DatabaseService* database) : database(database) {}

ReportManagement::~ReportManagement() = default;

bool ReportManagement::createReport(const Report& report) {

  // 1. Create the report using the database service

  // 2. Check if the report was created successfully

  // 3. If the report was created successfully, return true

  return false;
}

optional<vector<Report>> ReportManagement::getListReports(const Session& session) {

  // 1. Get the list of reports from the database service (using the getReportsByOwnerID method with the session's user ID)

  // 2. Check if the list of reports was retrieved successfully

  // 3. If the list of reports was retrieved successfully, return the list of reports

  return nullopt;
}