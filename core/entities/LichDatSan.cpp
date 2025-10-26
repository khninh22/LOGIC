#include "LichDatSan.h"
#include <iomanip>

// Helper function to format time
static string formatTime(time_t t) {
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

LichDatSan::LichDatSan(string ma, string maKH_, string maSan_,
                       time_t batDau, time_t ketThuc,
                       double tongTien_, string trangThaiDat_)
    : maLichDat(ma), maKH(maKH_), maSan(maSan_),
    thoiGianBatDau(batDau), thoiGianKetThuc(ketThuc),
    tongTien(tongTien_), trangThaiDat(trangThaiDat_),
    trangThaiTT("Chua Thanh Toan"), thoiGianThanhToan(0) {}

string LichDatSan::getMaLichDat() const { return maLichDat; }
string LichDatSan::getMaSan() const { return maSan; }
string LichDatSan::getMaKH() const { return maKH; }
time_t LichDatSan::getThoiGianBatDau() const { return thoiGianBatDau; }
time_t LichDatSan::getThoiGianKetThuc() const { return thoiGianKetThuc; }
double LichDatSan::getTongTien() const { return tongTien; }
string LichDatSan::getTrangThaiDat() const { return trangThaiDat; }
string LichDatSan::getTrangThaiTT() const { return trangThaiTT; }
time_t LichDatSan::getThoiGianThanhToan() const { return thoiGianThanhToan; }

void LichDatSan::setTrangThaiDat(const string& s) { trangThaiDat = s; }
void LichDatSan::setTrangThaiTT(const string& s) { trangThaiTT = s; }
void LichDatSan::setThoiGianThanhToan(time_t t) { thoiGianThanhToan = t; }

void LichDatSan::hienThi() const {
    cout << left
         << setw(12) << maLichDat
         << setw(10) << maKH
         << setw(10) << maSan
         << setw(18) << formatTime(thoiGianBatDau)
         << setw(18) << formatTime(thoiGianKetThuc)
         << right << setw(12) << fixed << setprecision(0)
         << to_string((long long)tongTien).insert(
                to_string((long long)tongTien).length() - 3, ",") << " VND"
         << left
         << setw(14) << trangThaiDat
         << setw(16) << trangThaiTT
         << setw(20) << (thoiGianThanhToan ? formatTime(thoiGianThanhToan) : "N/A")
         << endl;
}
