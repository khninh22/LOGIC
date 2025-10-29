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
    mgrDichVu = new QuanLyDichVu(&dsDichVu, &dsDonDichVu);
}

QuanLyThueSan::~QuanLyThueSan() {
    delete mgrKhach;
    delete mgrSan;
    delete mgrLich;
    delete mgrThongKe;
    delete mgrDichVu;
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
    DataManager::taiTatCa(dsSanBong, dsKhachHang, dsLichDatSan, dsDichVu, dsDonDichVu);
}

void QuanLyThueSan::luuDuLieu() {
    DataManager::luuTatCa(dsSanBong, dsKhachHang, dsLichDatSan, dsDichVu, dsDonDichVu);
}

bool QuanLyThueSan::backup(const string& backupName) {
    return DataManager::backupData(backupName);
}

bool QuanLyThueSan::restore(const string& backupName) {
    return DataManager::restoreData(backupName);
}

// ==================== API DỊCH VỤ ====================

bool QuanLyThueSan::themDichVuAPI(const string& ma, const string& ten, const string& donVi,
                                  double donGia, int ton, const string& loai) {
    return mgrDichVu->themDichVu(ma, ten, donVi, donGia, ton, loai);
}

bool QuanLyThueSan::suaDichVuAPI(const string& ma, const string& ten, const string& donVi,
                                 double donGia, int ton, const string& loai) {
    return mgrDichVu->suaDichVu(ma, ten, donVi, donGia, ton, loai);
}

bool QuanLyThueSan::xoaDichVuAPI(const string& ma) {
    return mgrDichVu->xoaDichVu(ma);
}

bool QuanLyThueSan::nhapHangAPI(const string& ma, int soLuong) {
    return mgrDichVu->nhapHang(ma, soLuong);
}

bool QuanLyThueSan::taoDonDichVuAPI(const string& maLich, const string& maKH, string& outMaDon) {
    return mgrDichVu->taoDonDichVu("", maLich, maKH, outMaDon);
}

bool QuanLyThueSan::themDichVuVaoDonAPI(const string& maDon, const string& maDV, int soLuong) {
    return mgrDichVu->themDichVuVaoDon(maDon, maDV, soLuong);
}

bool QuanLyThueSan::xoaDichVuKhoiDonAPI(const string& maDon, const string& maDV) {
    return mgrDichVu->xoaDichVuKhoiDon(maDon, maDV);
}

bool QuanLyThueSan::huyDonDichVuAPI(const string& maDon) {
    return mgrDichVu->huyDon(maDon);
}

bool QuanLyThueSan::daGiaoDonAPI(const string& maDon) {
    return mgrDichVu->daGiaoDon(maDon);
}

MangDong<DonDichVu> QuanLyThueSan::layDonTheoLichAPI(const string& maLich) const {
    return mgrDichVu->layDonTheoLich(maLich);
}

double QuanLyThueSan::tinhTongTienDonAPI(const string& maLich) const {
    // Lấy tất cả đơn dịch vụ của lịch đặt này
    MangDong<DonDichVu> dsDon = mgrDichVu->layDonTheoLich(maLich);
    double tongTien = 0.0;
    for (int i = 0; i < dsDon.getKichThuoc(); ++i) {
        tongTien += dsDon[i].getTongTien();
    }
    return tongTien;
}
