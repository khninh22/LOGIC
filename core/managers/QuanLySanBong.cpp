#include "QuanLySanBong.h"
#include "../services/ValidationService.h"
#include <iostream>

QuanLySanBong::QuanLySanBong(MangDong<SanBong>* ds)
    : dsSanBong(ds) {}

SanBong* QuanLySanBong::timSanBong(const string& maSan) {
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        if ((*dsSanBong)[i].getMaSan() == maSan) {
            return &(*dsSanBong)[i];
        }
    }
    return nullptr;
}

bool QuanLySanBong::themSanBong(const string& ma, const string& ten, int loai, double gia) {
    // Validate
    if (!ValidationService::kiemTraMaSanHopLe(ma)) return false;
    if (ten.empty()) return false;
    if (!ValidationService::kiemTraLoaiSanHopLe(loai)) return false;
    if (gia < 0) return false;

    // Kiểm tra trùng mã
    if (timSanBong(ma) != nullptr) return false;

    // Thêm vào danh sách
    dsSanBong->them(SanBong(ma, ten, loai, gia, false));
    return true;
}

bool QuanLySanBong::suaSanBong(const string& ma, const string& ten, int loai, double gia) {
    SanBong* san = timSanBong(ma);
    if (!san) return false;

    // Validate và cập nhật
    if (!ten.empty()) san->setTenSan(ten);
    if (ValidationService::kiemTraLoaiSanHopLe(loai)) san->setLoaiSan(loai);
    if (gia >= 0) san->setGiaThue(gia);

    return true;
}

bool QuanLySanBong::xoaSanBong(const string& ma) {
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        if ((*dsSanBong)[i].getMaSan() == ma) {
            dsSanBong->xoaTheoViTri(i);
            return true;
        }
    }
    return false;
}

bool QuanLySanBong::batBaoTri(const string& ma) {
    SanBong* san = timSanBong(ma);
    if (!san) return false;
    if (san->getBaoTri()) return false; // Đã bảo trì rồi

    san->setBaoTri(true);
    return true;
}

bool QuanLySanBong::tatBaoTri(const string& ma) {
    SanBong* san = timSanBong(ma);
    if (!san) return false;
    if (!san->getBaoTri()) return false; // Không bảo trì

    san->setBaoTri(false);
    return true;
}

MangDong<SanBong> QuanLySanBong::laySanTheoLoai(int loai) const {
    MangDong<SanBong> kq;
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        if ((*dsSanBong)[i].getLoaiSan() == loai) {
            kq.them((*dsSanBong)[i]);
        }
    }
    return kq;
}

void QuanLySanBong::hienThiTatCa() const {
    if (dsSanBong->getKichThuoc() == 0) {
        cout << "\nChua co san bong nao trong he thong.\n";
        return;
    }

    cout << "\n========== DANH SACH SAN BONG ==========\n";
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        (*dsSanBong)[i].hienThi();
    }
}

void QuanLySanBong::hienThiDangBaoTri() const {
    cout << "\n========== SAN DANG BAO TRI ==========\n";
    bool found = false;
    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        if ((*dsSanBong)[i].getBaoTri()) {
            (*dsSanBong)[i].hienThi();
            found = true;
        }
    }
    if (!found) cout << "(khong co san nao)\n";
}
