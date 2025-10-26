#include "QuanLyThongKe.h"
#include <iostream>
#include <iomanip>

QuanLyThongKe::QuanLyThongKe(MangDong<LichDatSan>* dsLich, MangDong<SanBong>* dsSan)
    : dsLichDatSan(dsLich), dsSanBong(dsSan) {}

bool QuanLyThongKe::trongKhoang(time_t t, time_t from, time_t to) const {
    if (from == 0 && to == 0) return true; // Tất cả
    return t >= from && t <= to;
}

ThongKeDoanhThu QuanLyThongKe::tinhDoanhThu(KhoangThoiGian khoang, time_t from, time_t to) const {
    ThongKeDoanhThu kq;

    // Xác định khoảng thời gian
    time_t now = time(nullptr);
    tm* nowTm = localtime(&now);

    if (khoang == HOM_NAY) {
        tm start = *nowTm;
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;
        from = mktime(&start);
        to = now;
    }
    else if (khoang == TUAN_NAY) {
        tm start = *nowTm;
        start.tm_mday -= nowTm->tm_wday; // Đầu tuần
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;
        from = mktime(&start);
        to = now;
    }
    else if (khoang == THANG_NAY) {
        tm start = *nowTm;
        start.tm_mday = 1;
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;
        from = mktime(&start);
        to = now;
    }
    else if (khoang == QUY_NAY) {
        tm start = *nowTm;
        start.tm_mon = (nowTm->tm_mon / 3) * 3; // Đầu quý
        start.tm_mday = 1;
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;
        from = mktime(&start);
        to = now;
    }
    else if (khoang == NAM_NAY) {
        tm start = *nowTm;
        start.tm_mon = 0; start.tm_mday = 1;
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;
        from = mktime(&start);
        to = now;
    }

    // Tính toán
    for (int i = 0; i < dsLichDatSan->getKichThuoc(); ++i) {
        const LichDatSan& lich = (*dsLichDatSan)[i];

        if (!trongKhoang(lich.getThoiGianBatDau(), from, to)) continue;

        if (lich.getTrangThaiDat() == "Da Dat" &&
            lich.getTrangThaiTT() == "Da Thanh Toan") { // ✅ FIX: Chỉ tính đã thanh toán

            kq.tongDoanhThu += lich.getTongTien();
            kq.soLichDaDat++;

            string maSan = lich.getMaSan();
            kq.doanhThuTheoSan[maSan] += lich.getTongTien();
        }
        else if (lich.getTrangThaiDat() == "Da Huy") {
            kq.soLichDaHuy++;
        }
    }

    return kq;
}

MangDong<ThongKeSan> QuanLyThongKe::thongKeTanSuat(KhoangThoiGian khoang,
                                                   time_t from, time_t to) const {
    MangDong<ThongKeSan> kq;

    for (int i = 0; i < dsSanBong->getKichThuoc(); ++i) {
        const SanBong& san = (*dsSanBong)[i];

        ThongKeSan tk;
        tk.maSan = san.getMaSan();
        tk.tenSan = san.getTenSan();

        for (int j = 0; j < dsLichDatSan->getKichThuoc(); ++j) {
            const LichDatSan& lich = (*dsLichDatSan)[j];

            if (lich.getMaSan() != san.getMaSan()) continue;
            if (!trongKhoang(lich.getThoiGianBatDau(), from, to)) continue;
            if (lich.getTrangThaiDat() != "Da Dat") continue;

            tk.soLanDat++;
            tk.tongGio += difftime(lich.getThoiGianKetThuc(),
                                   lich.getThoiGianBatDau()) / 3600.0;
        }

        if (tk.soLanDat > 0) {
            kq.them(tk);
        }
    }

    return kq;
}

double QuanLyThongKe::tinhDoanhThuKyTruoc(KhoangThoiGian khoang) const {
    // TODO: Implement logic tính kỳ trước
    return 0.0;
}

double QuanLyThongKe::tinhTyLeThayDoi(KhoangThoiGian khoang) const {
    double kyNay = tinhDoanhThu(khoang).tongDoanhThu;
    double kyTruoc = tinhDoanhThuKyTruoc(khoang);

    if (kyTruoc == 0) return 0.0;
    return ((kyNay - kyTruoc) / kyTruoc) * 100.0;
}

MangDong<ThongKeSan> QuanLyThongKe::topSanPhoBien(int top) const {
    MangDong<ThongKeSan> ds = thongKeTanSuat();

    // Bubble sort giảm dần theo số lần đặt
    for (int i = 0; i < ds.getKichThuoc() - 1; ++i) {
        for (int j = 0; j < ds.getKichThuoc() - i - 1; ++j) {
            if (ds[j].soLanDat < ds[j + 1].soLanDat) {
                ThongKeSan temp = ds[j];
                ds[j] = ds[j + 1];
                ds[j + 1] = temp;
            }
        }
    }

    // Lấy top
    MangDong<ThongKeSan> kq;
    for (int i = 0; i < ds.getKichThuoc() && i < top; ++i) {
        kq.them(ds[i]);
    }
    return kq;
}

void QuanLyThongKe::hienThiDoanhThu(KhoangThoiGian khoang) const {
    ThongKeDoanhThu tk = tinhDoanhThu(khoang);

    cout << "\n========== THONG KE DOANH THU ==========\n";
    cout << "Tong doanh thu: " << fixed << setprecision(0)
         << tk.tongDoanhThu << " VND\n";
    cout << "So lich dat thanh cong: " << tk.soLichDaDat << "\n";
    cout << "So lich da huy: " << tk.soLichDaHuy << "\n\n";

    if (!tk.doanhThuTheoSan.empty()) {
        cout << "Doanh thu theo san:\n";
        for (auto& p : tk.doanhThuTheoSan) {
            cout << "  " << p.first << ": " << p.second << " VND\n";
        }
    }
}

void QuanLyThongKe::hienThiTanSuat(KhoangThoiGian khoang) const {
    MangDong<ThongKeSan> ds = thongKeTanSuat(khoang);

    cout << "\n========== THONG KE TAN SUAT ==========\n";
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        cout << ds[i].maSan << " - " << ds[i].tenSan
             << ": " << ds[i].soLanDat << " lan, "
             << fixed << setprecision(1) << ds[i].tongGio << " gio\n";
    }
}
