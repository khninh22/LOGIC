#include "QuanLyKhachHang.h"
#include "../services/ValidationService.h"
#include <iostream>

QuanLyKhachHang::QuanLyKhachHang(MangDong<KhachHang>* ds)
    : dsKhachHang(ds) {}

KhachHang* QuanLyKhachHang::timKhachHang(const string& maKH) {
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getMaKH() == maKH) {
            return &(*dsKhachHang)[i];
        }
    }
    return nullptr;
}

KhachHang* QuanLyKhachHang::timKhachHangLinhHoat(const string& thongTin) {
    // Ưu tiên tìm theo mã KH
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getMaKH() == thongTin) {
            return &(*dsKhachHang)[i];
        }
    }

    // Tìm theo số điện thoại
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getSdt() == thongTin) {
            return &(*dsKhachHang)[i];
        }
    }

    // Tìm theo tên
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getHoTen().find(thongTin) != string::npos) {
            return &(*dsKhachHang)[i];
        }
    }

    return nullptr;
}

bool QuanLyKhachHang::themKhachHang(const string& ma, const string& ten, const string& sdt) {
    // Validate
    if (!ValidationService::kiemTraMaKHHopLe(ma)) return false;
    if (ten.empty()) return false;
    if (!sdt.empty() && !ValidationService::kiemTraSDTHopLe(sdt)) return false;

    // Kiểm tra trùng mã
    if (timKhachHang(ma) != nullptr) return false;

    // Thêm vào danh sách
    dsKhachHang->them(KhachHang(ma, ten, sdt));
    return true;
}

bool QuanLyKhachHang::suaKhachHang(const string& ma, const string& ten, const string& sdt) {
    KhachHang* kh = timKhachHang(ma);
    if (!kh) return false;

    // Validate
    if (ten.empty()) return false;
    if (!sdt.empty() && !ValidationService::kiemTraSDTHopLe(sdt)) return false;

    // Vì KhachHang không có setter, tạo đối tượng mới và thay thế
    string sdtMoi = sdt.empty() ? kh->getSdt() : sdt;
    KhachHang thay(ma, ten, sdtMoi);

    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getMaKH() == ma) {
            (*dsKhachHang)[i] = thay;
            return true;
        }
    }
    return false;
}

bool QuanLyKhachHang::xoaKhachHang(const string& ma) {
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        if ((*dsKhachHang)[i].getMaKH() == ma) {
            dsKhachHang->xoaTheoViTri(i);
            return true;
        }
    }
    return false;
}

string QuanLyKhachHang::sinhMaKHTiepTheo() const {
    // ✅ FIX: Không dùng số 0 đệm, chỉ số thứ tự
    return "KH" + to_string(dsKhachHang->getKichThuoc() + 1);
}

void QuanLyKhachHang::hienThiTatCa() const {
    if (dsKhachHang->getKichThuoc() == 0) {
        cout << "\nChua co khach hang nao trong he thong.\n";
        return;
    }

    cout << "\n========== DANH SACH KHACH HANG ==========\n";
    for (int i = 0; i < dsKhachHang->getKichThuoc(); ++i) {
        (*dsKhachHang)[i].hienThi();
    }
}
