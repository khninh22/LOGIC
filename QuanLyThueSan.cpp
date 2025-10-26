#include "QuanLyThueSan.h"
#include "core/utils/DateTimeUtils.h"

QuanLyThueSan::QuanLyThueSan() {
    // Tải dữ liệu
    taiDuLieu();

    // Khởi tạo managers
    mgrKhach = new QuanLyKhachHang(&dsKhachHang);
    mgrSan = new QuanLySanBong(&dsSanBong);
    mgrLich = new QuanLyLichDat(&dsLichDatSan, &dsSanBong);
    mgrThongKe = new QuanLyThongKe(&dsLichDatSan, &dsSanBong);
}

QuanLyThueSan::~QuanLyThueSan() {
    delete mgrKhach;
    delete mgrSan;
    delete mgrLich;
    delete mgrThongKe;
}

// ==================== API CHO GUI - KHÁCH HÀNG ====================

bool QuanLyThueSan::themKhachHangAPI(const string& ma, const string& ten, const string& sdt) {
    return mgrKhach->themKhachHang(ma, ten, sdt);
}

bool QuanLyThueSan::suaKhachHangAPI(const string& ma, const string& ten, const string& sdt) {
    return mgrKhach->suaKhachHang(ma, ten, sdt);
}

bool QuanLyThueSan::xoaKhachHangAPI(const string& ma) {
    // Kiểm tra còn lịch đặt hiệu lực không
    time_t now = time(nullptr);
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); ++i) {
        const LichDatSan& l = dsLichDatSan[i];
        if (l.getMaKH() == ma && l.getTrangThaiDat() == "Da Dat" &&
            difftime(l.getThoiGianKetThuc(), now) > 0) {
            return false; // Còn lịch đặt hiệu lực
        }
    }

    return mgrKhach->xoaKhachHang(ma);
}

// ==================== API CHO GUI - SÂN BÓNG ====================

bool QuanLyThueSan::themSanBongAPI(const string& ma, const string& ten, int loai, double gia) {
    return mgrSan->themSanBong(ma, ten, loai, gia);
}

bool QuanLyThueSan::suaSanBongAPI(const string& ma, const string& ten, int loai, double gia) {
    return mgrSan->suaSanBong(ma, ten, loai, gia);
}

bool QuanLyThueSan::xoaSanBongAPI(const string& ma) {
    // Kiểm tra còn lịch đặt hiệu lực không
    time_t now = time(nullptr);
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); ++i) {
        const LichDatSan& l = dsLichDatSan[i];
        if (l.getMaSan() == ma && l.getTrangThaiDat() == "Da Dat" &&
            difftime(l.getThoiGianKetThuc(), now) > 0) {
            return false; // Còn lịch đặt hiệu lực
        }
    }

    return mgrSan->xoaSanBong(ma);
}

bool QuanLyThueSan::doiBaoTriAPI(const string& ma, bool trangThai) {
    if (trangThai) {
        // Nếu bật bảo trì, kiểm tra lịch sắp tới
        time_t now = time(nullptr);
        for (int i = 0; i < dsLichDatSan.getKichThuoc(); ++i) {
            const LichDatSan& l = dsLichDatSan[i];
            if (l.getMaSan() == ma && l.getTrangThaiDat() == "Da Dat" &&
                difftime(l.getThoiGianBatDau(), now) > 0) {
                return false; // Có lịch sắp tới
            }
        }
        return mgrSan->batBaoTri(ma);
    } else {
        return mgrSan->tatBaoTri(ma);
    }
}

// ==================== API CHO GUI - LỊCH ĐẶT ====================

bool QuanLyThueSan::datSanAPI(const string& maKH, const string& maSan,
                              time_t tBD, time_t tKT,
                              string& outMaLD, double& outTien) {
    return mgrLich->datSan(maKH, maSan, tBD, tKT, outMaLD, outTien);
}

bool QuanLyThueSan::huyLichDatAPI(const string& maLich) {
    return mgrLich->huyLichDat(maLich);
}

bool QuanLyThueSan::thanhToanAPI(const string& maLich) {
    return mgrLich->thanhToan(maLich);
}

bool QuanLyThueSan::isTrungLich(const string& maSan, time_t tBD, time_t tKT) const {
    return mgrLich->kiemTraTrungLich(maSan, tBD, tKT);
}

// ==================== UTILS ====================

time_t QuanLyThueSan::parseDateTime(const string& s) {
    return DateTimeUtils::stringToTime(s);
}

string QuanLyThueSan::formatDateTime(time_t t) {
    return DateTimeUtils::timeToString(t);
}

// ==================== FILE I/O ====================

void QuanLyThueSan::taiDuLieu() {
    DataManager::taiTatCa(dsSanBong, dsKhachHang, dsLichDatSan);
}

void QuanLyThueSan::luuDuLieu() {
    DataManager::luuTatCa(dsSanBong, dsKhachHang, dsLichDatSan);
}

bool QuanLyThueSan::backup(const string& backupName) {
    return DataManager::backupData(backupName);
}

bool QuanLyThueSan::restore(const string& backupName) {
    return DataManager::restoreData(backupName);
}
