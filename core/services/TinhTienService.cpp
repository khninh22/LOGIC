#include "TinhTienService.h"
#include "../utils/DateTimeUtils.h"

double TinhTienService::donGiaBanNgay(int loai) {
    if (loai == 5) return 200000.0;
    if (loai == 7) return 400000.0;
    if (loai == 11) return 800000.0;
    return 0.0;
}

double TinhTienService::donGiaBanToi(int loai) {
    if (loai == 5) return 300000.0;
    if (loai == 7) return 500000.0;
    if (loai == 11) return 1000000.0;
    return 0.0;
}

double TinhTienService::tinhTienTheoKhoang(const SanBong& san, time_t tBD, time_t tKT) {
    if (tKT <= tBD) return 0.0;

    // Tách theo mốc 17:00
    tm d = *localtime(&tBD);
    tm moc = d;
    moc.tm_hour = 17;
    moc.tm_min = 0;
    moc.tm_sec = 0;
    time_t tMoc = mktime(&moc);

    double tong = 0.0;

    // Giờ ban ngày (trước 17:00)
    if (tBD < tMoc) {
        time_t endDay = (tKT < tMoc) ? tKT : tMoc;
        double gio = difftime(endDay, tBD) / 3600.0;
        tong += gio * donGiaBanNgay(san.getLoaiSan());
    }

    // Giờ ban tối (sau 17:00)
    if (tKT > tMoc) {
        time_t startNight = (tBD > tMoc) ? tBD : tMoc;
        double gio = difftime(tKT, startNight) / 3600.0;
        tong += gio * donGiaBanToi(san.getLoaiSan());
    }

    return tong;
}

bool TinhTienService::khoangTrongGioMoCua(const tm& ngay, time_t tBD, time_t tKT) {
    time_t tMo = DateTimeUtils::makeTimeOnDay(ngay, GIO_MO_CUA, 0);
    time_t tDong = DateTimeUtils::makeTimeOnDay(ngay, GIO_DONG_CUA, 0);

    if (tBD < tMo || tKT > tDong) return false;
    return tKT > tBD;
}
