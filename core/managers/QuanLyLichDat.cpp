#include "QuanLyLichDat.h"
#include "../services/TinhTienService.h"
#include "../utils/DateTimeUtils.h"
#include <iostream>
#include <fstream>

QuanLyLichDat::QuanLyLichDat(MangDong<LichDatSan>* dsLich, MangDong<SanBong>* dsSan)
    : dsLichDatSan(dsLich), dsSanBong(dsSan) {}

LichDatSan* QuanLyLichDat::timLichDat(const string& maLichDat) {
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        if ((*dsLichDatSan)[i].getMaLichDat() == maLichDat) {
            return &(*dsLichDatSan)[i];
        }
    }
    return nullptr;
}

bool QuanLyLichDat::kiemTraTrungLich(const string& maSan, time_t batDau, time_t ketThuc) const {
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        const LichDatSan& lich = (*dsLichDatSan)[i];
        if (lich.getMaSan() == maSan && lich.getTrangThaiDat() == "Da Dat") {
            if (batDau < lich.getThoiGianKetThuc() && ketThuc > lich.getThoiGianBatDau()) {
                return true;
            }
        }
    }
    return false;
}

MangDong<SanBong> QuanLyLichDat::laySanTrongKhoang(int loai, time_t tBD, time_t tKT) const {
    MangDong<SanBong> kq;
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        const SanBong& s = (*dsSanBong)[i];
        if (s.getLoaiSan() != loai || s.getBaoTri()) continue;

        if (!kiemTraTrungLich(s.getMaSan(), tBD, tKT)) {
            kq.them(s);
        }
    }
    return kq;
}

bool QuanLyLichDat::datSan(const string& maKH, const string& maSan,
                           time_t tBD, time_t tKT,
                           string& outMaLD, double& outTien) {
    // Tìm sân
    SanBong* san = nullptr;
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        if ((*dsSanBong)[i].getMaSan() == maSan) {
            san = &(*dsSanBong)[i];
            break;
        }
    }

    if (!san || tKT <= tBD) return false;
    if (kiemTraTrungLich(maSan, tBD, tKT)) return false;

    // Tính tiền
    double tong = TinhTienService::tinhTienTheoKhoang(*san, tBD, tKT);

    // Sinh mã lịch mới
    string maLD = sinhMaLichMoi();

    // Tạo lịch đặt
    LichDatSan lich(maLD, maKH, maSan, tBD, tKT, tong, "Da Dat");
    lich.setTrangThaiTT("Chua Thanh Toan");

    dsLichDatSan->them(lich);

    outMaLD = maLD;
    outTien = tong;
    return true;
}

bool QuanLyLichDat::huyLichDat(const string& maLichDat) {
    LichDatSan* lich = timLichDat(maLichDat);
    if (!lich) return false;
    if (lich->getTrangThaiDat() == "Da Huy") return false;

    lich->setTrangThaiDat("Da Huy");
    return true;
}

bool QuanLyLichDat::thanhToan(const string& maLichDat) {
    LichDatSan* lich = timLichDat(maLichDat);
    if (!lich) return false;
    if (lich->getTrangThaiDat() != "Da Dat") return false;
    if (lich->getTrangThaiTT() == "Da Thanh Toan") return false;

    lich->setTrangThaiTT("Da Thanh Toan");
    lich->setThoiGianThanhToan(time(nullptr));

    // Xuất hóa đơn
#ifdef _WIN32
    system("if not exist hoadon mkdir hoadon");
#else
    system("mkdir -p hoadon");
#endif

    ofstream fo(("hoadon/hoadon_" + lich->getMaLichDat() + ".txt").c_str());
    if (fo.is_open()) {
        fo << "==============================\n";
        fo << "    HOA DON THANH TOAN\n";
        fo << "==============================\n\n";
        fo << "Ma HD    : " << lich->getMaLichDat() << "\n";
        fo << "Ma KH    : " << lich->getMaKH() << "\n";
        fo << "Ma San   : " << lich->getMaSan() << "\n";
        fo << "Bat dau  : " << DateTimeUtils::timeToString(lich->getThoiGianBatDau()) << "\n";
        fo << "Ket thuc : " << DateTimeUtils::timeToString(lich->getThoiGianKetThuc()) << "\n";
        fo << "TT luc   : " << DateTimeUtils::timeToString(lich->getThoiGianThanhToan()) << "\n";
        fo << "------------------------------\n";
        fo << "Tong tien: " << lich->getTongTien() << " VND\n";
        fo << "==============================\n";
        fo.close();
    }

    return true;
}

string QuanLyLichDat::sinhMaLichMoi() const {
    return "LD" + to_string(dsLichDatSan->getKichThuoc() + 1);
}

void QuanLyLichDat::hienThiTatCa() const {
    if (dsLichDatSan->getKichThuoc() == 0) {
        cout << "\nChua co lich dat nao.\n";
        return;
    }

    cout << "\n========== DANH SACH LICH DAT ==========\n";
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        if ((*dsLichDatSan)[i].getTrangThaiDat() == "Da Dat") {
            (*dsLichDatSan)[i].hienThi();
        }
    }
}

void QuanLyLichDat::hienThiChuaThanhToan() const {
    cout << "\n========== LICH CHUA THANH TOAN ==========\n";
    bool found = false;
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        const LichDatSan& l = (*dsLichDatSan)[i];
        if (l.getTrangThaiDat() == "Da Dat" && l.getTrangThaiTT() == "Chua Thanh Toan") {
            l.hienThi();
            found = true;
        }
    }
    if (!found) cout << "(khong co)\n";
}

void QuanLyLichDat::hienThiTheoSan(const string& maSan) const {
    cout << "\n========== LICH DAT THEO SAN " << maSan << " ==========\n";
    bool found = false;
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        if ((*dsLichDatSan)[i].getMaSan() == maSan) {
            (*dsLichDatSan)[i].hienThi();
            found = true;
        }
    }
    if (!found) cout << "(khong co)\n";
}

void QuanLyLichDat::hienThiTheoKhach(const string& maKH) const {
    cout << "\n========== LICH DAT THEO KHACH " << maKH << " ==========\n";
    bool found = false;
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        if ((*dsLichDatSan)[i].getMaKH() == maKH) {
            (*dsLichDatSan)[i].hienThi();
            found = true;
        }
    }
    if (!found) cout << "(khong co)\n";
}
