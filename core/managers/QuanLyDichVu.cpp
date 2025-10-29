#include "QuanLyDichVu.h"
#include "../services/ValidationService.h"
#include <iostream>
#include <sstream>
#include <iomanip>

QuanLyDichVu::QuanLyDichVu(MangDong<DichVu>* dsDV, MangDong<DonDichVu>* dsDon)
    : dsDichVu(dsDV), dsDonDichVu(dsDon) {}

// ==================== QUẢN LÝ DỊCH VỤ ====================

DichVu* QuanLyDichVu::timDichVu(const string& maDV) {
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getMaDichVu() == maDV) {
            return &(*dsDichVu)[i];
        }
    }
    return nullptr;
}

const DichVu* QuanLyDichVu::timDichVu(const string& maDV) const {
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getMaDichVu() == maDV) {
            return &(*dsDichVu)[i];
        }
    }
    return nullptr;
}

bool QuanLyDichVu::themDichVu(const string& ma, const string& ten, const string& donVi,
                              double donGia, int ton, const string& loai) {
    // Validate
    if (!ValidationService::kiemTraMa(ma) || ten.empty()) return false;
    if (donGia < 0 || ton < 0) return false;
    
    // Kiểm tra trùng mã
    if (timDichVu(ma) != nullptr) return false;
    
    // Thêm mới
    DichVu dv(ma, ten, donVi, donGia, ton, loai, true);
    dsDichVu->them(dv);
    return true;
}

bool QuanLyDichVu::suaDichVu(const string& ma, const string& ten, const string& donVi,
                             double donGia, int ton, const string& loai) {
    DichVu* dv = timDichVu(ma);
    if (!dv) return false;
    
    if (ten.empty() || donGia < 0 || ton < 0) return false;
    
    dv->setTenDichVu(ten);
    dv->setDonVi(donVi);
    dv->setDonGia(donGia);
    dv->setSoLuongTon(ton);
    dv->setLoaiDichVu(loai);
    return true;
}

bool QuanLyDichVu::xoaDichVu(const string& ma) {
    // Kiểm tra xem có đơn nào đang sử dụng không
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        const auto& chiTiet = (*dsDonDichVu)[i].getChiTiet();
        for (int j = 0; j < chiTiet.getKichThuoc(); ++j) {
            if (chiTiet[j].maDichVu == ma &&
                (*dsDonDichVu)[i].getTrangThai() != "Da Huy") {
                return false;  // Còn đơn đang sử dụng
            }
        }
    }
    
    // Xóa
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getMaDichVu() == ma) {
            return dsDichVu->xoaTheoViTri(i);
        }
    }
    return false;
}

bool QuanLyDichVu::doiTrangThaiPhucVu(const string& ma, bool phucVu) {
    DichVu* dv = timDichVu(ma);
    if (!dv) return false;
    dv->setConPhucVu(phucVu);
    return true;
}

bool QuanLyDichVu::nhapHang(const string& ma, int soLuong) {
    DichVu* dv = timDichVu(ma);
    if (!dv) return false;
    return dv->tangSoLuong(soLuong);
}

bool QuanLyDichVu::xuatHang(const string& ma, int soLuong) {
    DichVu* dv = timDichVu(ma);
    if (!dv) return false;
    return dv->giamSoLuong(soLuong);
}

MangDong<DichVu> QuanLyDichVu::locTheoLoai(const string& loai) const {
    MangDong<DichVu> kq;
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getLoaiDichVu() == loai) {
            kq.them((*dsDichVu)[i]);
        }
    }
    return kq;
}

MangDong<DichVu> QuanLyDichVu::locConPhucVu() const {
    MangDong<DichVu> kq;
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getConPhucVu()) {
            kq.them((*dsDichVu)[i]);
        }
    }
    return kq;
}

MangDong<DichVu> QuanLyDichVu::locSapHetHang(int nguongTon) const {
    MangDong<DichVu> kq;
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        if ((*dsDichVu)[i].getSoLuongTon() <= nguongTon && 
            (*dsDichVu)[i].getConPhucVu()) {
            kq.them((*dsDichVu)[i]);
        }
    }
    return kq;
}

// ==================== QUẢN LÝ ĐƠN DỊCH VỤ ====================

DonDichVu* QuanLyDichVu::timDonDichVu(const string& maDon) {
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        if ((*dsDonDichVu)[i].getMaDon() == maDon) {
            return &(*dsDonDichVu)[i];
        }
    }
    return nullptr;
}

const DonDichVu* QuanLyDichVu::timDonDichVu(const string& maDon) const {
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        if ((*dsDonDichVu)[i].getMaDon() == maDon) {
            return &(*dsDonDichVu)[i];
        }
    }
    return nullptr;
}

bool QuanLyDichVu::taoDonDichVu(const string& maDon, const string& maLich,
                                const string& maKH, string& outMaDon) {
    // Validate
    if (maLich.empty() || maKH.empty()) return false;
    
    // Sinh mã tự động nếu không truyền vào
    string ma = maDon.empty() ? sinhMaDonMoi() : maDon;
    
    // Kiểm tra trùng
    if (timDonDichVu(ma) != nullptr) return false;
    
    // Tạo đơn mới
    DonDichVu don(ma, maLich, maKH, time(nullptr));
    dsDonDichVu->them(don);
    outMaDon = ma;
    return true;
}

bool QuanLyDichVu::themDichVuVaoDon(const string& maDon, const string& maDV, int soLuong) {
    DonDichVu* don = timDonDichVu(maDon);
    if (!don) return false;
    
    DichVu* dv = timDichVu(maDV);
    if (!dv) return false;
    
    // Kiểm tra tồn kho
    if (dv->getSoLuongTon() < soLuong) return false;
    if (!dv->getConPhucVu()) return false;
    
    // Thêm vào đơn
    don->themDichVu(maDV, dv->getTenDichVu(), soLuong, dv->getDonGia());
    
    // Giảm tồn kho
    dv->giamSoLuong(soLuong);
    
    return true;
}

bool QuanLyDichVu::xoaDichVuKhoiDon(const string& maDon, const string& maDV) {
    DonDichVu* don = timDonDichVu(maDon);
    if (!don) return false;
    
    // Lấy số lượng để hoàn trả kho
    const auto& chiTiet = don->getChiTiet();
    int soLuongHoanTra = 0;
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        if (chiTiet[i].maDichVu == maDV) {
            soLuongHoanTra = chiTiet[i].soLuong;
            break;
        }
    }
    
    // Xóa khỏi đơn
    don->xoaDichVu(maDV);
    
    // Hoàn trả kho
    if (soLuongHoanTra > 0) {
        DichVu* dv = timDichVu(maDV);
        if (dv) {
            dv->tangSoLuong(soLuongHoanTra);
        }
    }
    
    return true;
}

bool QuanLyDichVu::capNhatSoLuongDon(const string& maDon, const string& maDV, int soLuongMoi) {
    DonDichVu* don = timDonDichVu(maDon);
    if (!don) return false;
    
    DichVu* dv = timDichVu(maDV);
    if (!dv) return false;
    
    // Lấy số lượng cũ
    const auto& chiTiet = don->getChiTiet();
    int soLuongCu = 0;
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        if (chiTiet[i].maDichVu == maDV) {
            soLuongCu = chiTiet[i].soLuong;
            break;
        }
    }
    
    if (soLuongCu == 0) return false;  // Không tìm thấy
    
    int chenhLech = soLuongMoi - soLuongCu;
    
    // Nếu tăng, kiểm tra tồn kho
    if (chenhLech > 0) {
        if (dv->getSoLuongTon() < chenhLech) return false;
        dv->giamSoLuong(chenhLech);
    } else if (chenhLech < 0) {
        // Nếu giảm, hoàn trả kho
        dv->tangSoLuong(-chenhLech);
    }
    
    // Cập nhật đơn
    don->capNhatSoLuong(maDV, soLuongMoi);
    return true;
}

bool QuanLyDichVu::huyDon(const string& maDon) {
    DonDichVu* don = timDonDichVu(maDon);
    if (!don) return false;
    
    // Hoàn trả tồn kho
    const auto& chiTiet = don->getChiTiet();
    for (int i = 0; i < chiTiet.getKichThuoc(); ++i) {
        DichVu* dv = timDichVu(chiTiet[i].maDichVu);
        if (dv) {
            dv->tangSoLuong(chiTiet[i].soLuong);
        }
    }
    
    don->setTrangThai("Da Huy");
    return true;
}

bool QuanLyDichVu::daGiaoDon(const string& maDon) {
    DonDichVu* don = timDonDichVu(maDon);
    if (!don) return false;
    don->setTrangThai("Da Giao");
    return true;
}

MangDong<DonDichVu> QuanLyDichVu::layDonTheoLich(const string& maLich) const {
    MangDong<DonDichVu> kq;
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        if ((*dsDonDichVu)[i].getMaLichDat() == maLich) {
            kq.them((*dsDonDichVu)[i]);
        }
    }
    return kq;
}

string QuanLyDichVu::sinhMaDichVuMoi() const {
    int maxNum = 0;
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        string ma = (*dsDichVu)[i].getMaDichVu();
        if (ma.length() >= 3 && ma.substr(0, 2) == "DV") {
            int num = 0;
            for (size_t j = 2; j < ma.length(); ++j) {
                if (ma[j] >= '0' && ma[j] <= '9') {
                    num = num * 10 + (ma[j] - '0');
                }
            }
            if (num > maxNum) maxNum = num;
        }
    }
    
    ostringstream oss;
    oss << "DV" << (maxNum + 1);
    return oss.str();
}

string QuanLyDichVu::sinhMaDonMoi() const {
    int maxNum = 0;
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        string ma = (*dsDonDichVu)[i].getMaDon();
        if (ma.length() >= 4 && ma.substr(0, 3) == "DDV") {
            int num = 0;
            for (size_t j = 3; j < ma.length(); ++j) {
                if (ma[j] >= '0' && ma[j] <= '9') {
                    num = num * 10 + (ma[j] - '0');
                }
            }
            if (num > maxNum) maxNum = num;
        }
    }
    
    ostringstream oss;
    oss << "DDV" << (maxNum + 1);
    return oss.str();
}

void QuanLyDichVu::hienThiDanhSachDichVu() const {
    using namespace std;
    cout << "\n========== DANH SÁCH DỊCH VỤ ==========" << endl;
    for (int i = 0; i < dsDichVu->getKichThuoc(); ++i) {
        (*dsDichVu)[i].hienThi();
        cout << "----------------------------------------" << endl;
    }
}

void QuanLyDichVu::hienThiDanhSachDon() const {
    using namespace std;
    cout << "\n========== DANH SÁCH ĐƠN DỊCH VỤ ==========" << endl;
    for (int i = 0; i < dsDonDichVu->getKichThuoc(); ++i) {
        (*dsDonDichVu)[i].hienThi();
    }
}

void QuanLyDichVu::hienThiDonTheoLich(const string& maLich) const {
    using namespace std;
    cout << "\n========== ĐƠN DỊCH VỤ CỦA LỊCH " << maLich << " ==========" << endl;
    auto dsDon = layDonTheoLich(maLich);
    for (int i = 0; i < dsDon.getKichThuoc(); ++i) {
        dsDon[i].hienThi();
    }
}
