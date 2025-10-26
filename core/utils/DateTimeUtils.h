#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <string>
#include <ctime>
using namespace std;

class DateTimeUtils {
public:
    // Parse "dd/mm/yyyy HH:MM" -> time_t
    static time_t stringToTime(const string& dateTimeStr);

    // Format time_t -> "dd/mm/yyyy HH:MM"
    static string timeToString(time_t t);

    // Parse date "dd/mm/yyyy" -> tm
    static bool parseDate(const string& dateStr, tm& outTm);

    // Parse time "HH:MM" -> hour, minute
    static bool parseTime(const string& timeStr, int& hour, int& minute);

    // Make time_t from tm + hour + minute
    static time_t makeTimeOnDay(const tm& baseDay, int hour, int minute);

    // Format time_t -> "dd/mm/yyyy HH:MM" (alias)
    static string formatTime(time_t t);
};

#endif // DATETIMEUTILS_H
