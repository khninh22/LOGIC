#include "DateTimeUtils.h"

// Helper: Check if character is digit
static bool isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

// Helper: Parse integer from substring
static bool parseIntRange(const string& s, int l, int r, int& out) {
    long val = 0;
    if (l < 0 || r >= (int)s.size() || l > r) return false;

    for (int i = l; i <= r; ++i) {
        if (!isDigitChar(s[i])) return false;
        val = val * 10 + (s[i] - '0');
        if (val > 1000000000) return false;
    }
    out = (int)val;
    return true;
}

bool DateTimeUtils::parseDate(const string& dateStr, tm& outTm) {
    int p1 = -1, p2 = -1;
    for (int i = 0; i < (int)dateStr.size(); ++i) {
        if (dateStr[i] == '/') {
            if (p1 == -1) p1 = i;
            else { p2 = i; break; }
        }
    }
    if (p1 == -1 || p2 == -1) return false;

    int dd, mm, yyyy;
    if (!parseIntRange(dateStr, 0, p1 - 1, dd)) return false;
    if (!parseIntRange(dateStr, p1 + 1, p2 - 1, mm)) return false;
    if (!parseIntRange(dateStr, p2 + 1, (int)dateStr.size() - 1, yyyy)) return false;

    if (dd < 1 || dd > 31 || mm < 1 || mm > 12 || yyyy < 1900 || yyyy > 3000)
        return false;

    outTm = {};
    outTm.tm_mday = dd;
    outTm.tm_mon = mm - 1;
    outTm.tm_year = yyyy - 1900;
    return true;
}

bool DateTimeUtils::parseTime(const string& timeStr, int& hour, int& minute) {
    int sep = -1;
    for (int i = 0; i < (int)timeStr.size(); ++i) {
        if (timeStr[i] == ':' || timeStr[i] == ' ') {
            sep = i;
            break;
        }
    }
    if (sep == -1) return false;

    if (!parseIntRange(timeStr, 0, sep - 1, hour)) return false;
    if (!parseIntRange(timeStr, sep + 1, (int)timeStr.size() - 1, minute)) return false;

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) return false;
    return true;
}

time_t DateTimeUtils::makeTimeOnDay(const tm& baseDay, int hour, int minute) {
    tm t = baseDay;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = 0;
    return mktime(&t);
}

time_t DateTimeUtils::stringToTime(const string& dateTimeStr) {
    size_t sp = dateTimeStr.find(' ');
    if (sp == string::npos) return 0;

    string d = dateTimeStr.substr(0, sp);
    string hm = dateTimeStr.substr(sp + 1);

    tm day = {};
    if (!parseDate(d, day)) return 0;

    int hh, mm;
    if (!parseTime(hm, hh, mm)) return 0;

    return makeTimeOnDay(day, hh, mm);
}

string DateTimeUtils::timeToString(time_t t) {
    if (t == 0) return "N/A";
    char buffer[20];
#ifdef _WIN32
    tm local_tm;
    localtime_s(&local_tm, &t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &local_tm);
#else
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", localtime(&t));
#endif
    return string(buffer);
}

string DateTimeUtils::formatTime(time_t t) {
    return timeToString(t);
}
