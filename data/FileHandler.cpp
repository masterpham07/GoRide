#include "data/FileHandler.h"
#include <fstream>
#include <sstream>

using namespace std;

string FileHandler::csvEscape(const string& field) {
    bool needQuote = field.find(',') != string::npos ||
                     field.find('"') != string::npos ||
                     field.find('\n') != string::npos;
    if (!needQuote) return field;

    string escaped = "\"";
    for (char c : field) {
        if (c == '"') escaped += "\"\"";
        else escaped += c;
    }
    escaped += "\"";
    return escaped;
}

void FileHandler::appendTripToFile(const Trip& t, const string& path) {
    ofstream f(path, ios::app);
    if (!f.is_open()) return;
    f << csvEscape(t.tripId) << "," << csvEscape(t.riderId) << "," << csvEscape(t.driverId) << ","
      << csvEscape(t.pickup.label) << "," << csvEscape(t.dropoff.label) << ","
      << t.distanceKm << "," << t.fare << "," << csvEscape(statusToStr(t.status)) << "\n";
    f.close();
}

void FileHandler::ensureHeader(const string &path)
{
    ifstream check(path);
    bool exists = check.good();
    check.close();
    if (!exists)
    {
        ofstream f(path);
        // THÊM BOM (Byte Order Mark) ĐỂ EXCEL ĐỌC ĐÚNG TIẾNG VIỆT
        f << "\xEF\xBB\xBF";

        // Ghi dòng tiêu đề ngay sau đó
        f << "TripId,RiderId,DriverId,Pickup,Dropoff,DistanceKm,Fare,Status\n";
    }
}
