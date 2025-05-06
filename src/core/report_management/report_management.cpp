#include "report_management.h"

#include "./services/database.h"
#include "./models/session.h"
#include "./models/report.h"

ReportManagement::ReportManagement(DatabaseService* database) : database(database) {}

ReportManagement::~ReportManagement() = default;

bool ReportManagement::createReport(const Report& report) {

    // 1. Create the report using the database service
    bool OkReport = database->saveReport(report);
    // 2. Check if the report was created successfully

    return OkReport;
}

optional<vector<Report>> ReportManagement::getListReports(const Session& session) {

    // 1. Get the list of reports from the database service (using the getReportsByOwnerID method with the session's user ID)
    auto reportsOpt = database->getReportsByOwnerID(session.user_id);
    // 2. Check if the list of reports was retrieved successfully
    if (!reportsOpt.has_value()) {
        return nullopt;
    }
    // 3. If the list of reports was retrieved successfully, return the list of reports

    return reportsOpt;
}