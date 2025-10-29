#include "DataManager.h"
#include "../core/utils/DateTimeUtils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

const string DataManager::DATA_DIR = "data/";
const string DataManager::FILE_SAN = "data/sanbong.txt";
const string DataManager::FILE_KHACH = "data/khachhang.txt";
const string DataManager::FILE_LICH = "data/lichdatsan.txt";
const string DataManager::FILE_DICHVU = "data/dichvu.txt";
const string DataManager::FILE_DONDICHVU = "data/dondichvu.txt";

int DataManager::splitByPipe(const string& line, string out[], int maxN) {
    int n = 0, start = 0;
    for (int i = 0; i <= (int)line.size(); ++i) {
        if (i == (int)line.size() || line[i] == '|') {
            if (n < maxN)
                out[n++] = line.substr(start, i - start);
            start = i + 1;
        }
    }
    return n;
}

bool DataManager::taiSanBong(MangDong<SanBong>& ds) {
    ifstream file(FILE_SAN);
    if (!file.is_open()) {
        cout << "Khong tim thay " << FILE_SAN << ". Tao du lieu mau...\n";
        ds.them(SanBong("A501", "San A5-1", 5, 200000, false));
        ds.them(SanBong("A502", "San A5-2", 5, 200000, false));
        ds.them(SanBong("A701", "San A7-1", 7, 400000, false));
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string c[5];
        int cnt = splitByPipe(line, c, 5);
        if (cnt < 4) continue;

        int loai = 0;
        double gia = 0;
        bool bt = false;

        try { loai = stoi(c[2]); } catch (...) { loai = 0; }
        try { gia = stod(c[3]); } catch (...) { gia = 0.0; }
        if (cnt >= 5) bt = (c[4] == "1");

        ds.them(SanBong(c[0], c[1], loai, gia, bt));
    }
    file.close();
    cout << "Da tai " << ds.getKichThuoc() << " san bong.\n";
    return true;
}

bool DataManager::taiKhachHang(MangDong<KhachHang>& ds) {
    ifstream file(FILE_KHACH);
    if (!file.is_open()) {
        cout << "Khong tim thay " << FILE_KHACH << ". Tao du lieu mau...\n";
        ds.them(KhachHang("KH1", "Nguyen Van A", "0901234567"));
        ds.them(KhachHang("KH2", "Tran Thi B", "0912345678"));
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string c[3];
        int cnt = splitByPipe(line, c, 3);
        if (cnt < 3) continue;

        ds.them(KhachHang(c[0], c[1], c[2]));
    }
    file.close();
    cout << "Da tai " << ds.getKichThuoc() << " khach hang.\n";
    return true;
}

bool DataManager::taiLichDatSan(MangDong<LichDatSan>& ds) {
    ifstream file(FILE_LICH);
    if (!file.is_open()) {
        cout << "Khong tim thay " << FILE_LICH << ".\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string c[9];
        int cnt = splitByPipe(line, c, 9);
        if (cnt < 6) continue;

        time_t batDau = DateTimeUtils::stringToTime(c[3]);
        time_t ketThuc = DateTimeUtils::stringToTime(c[4]);

        double tien = 0.0;
        try { tien = stod(c[5]); } catch (...) { tien = 0.0; }

        string ttDat = (cnt >= 7 && !c[6].empty()) ? c[6] : "Da Dat";
        string ttTT = (cnt >= 8 && !c[7].empty()) ? c[7] : "Chua Thanh Toan";
        time_t ttt = 0;
        if (cnt >= 9 && !c[8].empty() && c[8] != "N/A")
            ttt = DateTimeUtils::stringToTime(c[8]);

        LichDatSan lich(c[0], c[1], c[2], batDau, ketThuc, tien, ttDat);
        lich.setTrangThaiTT(ttTT);
        lich.setThoiGianThanhToan(ttt);
        ds.them(lich);
    }
    file.close();
    cout << "Da tai " << ds.getKichThuoc() << " lich dat.\n";
    return true;
}

bool DataManager::luuSanBong(const MangDong<SanBong>& ds) {
    ofstream file(FILE_SAN);
    if (!file.is_open()) return false;

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const SanBong& s = ds[i];
        file << s.getMaSan() << "|"
             << s.getTenSan() << "|"
             << s.getLoaiSan() << "|"
             << s.getGiaThue() << "|"
             << (s.getBaoTri() ? "1" : "0") << "\n";
    }
    file.close();
    return true;
}

bool DataManager::luuKhachHang(const MangDong<KhachHang>& ds) {
    ofstream file(FILE_KHACH);
    if (!file.is_open()) return false;

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const KhachHang& kh = ds[i];
        file << kh.getMaKH() << "|"
             << kh.getHoTen() << "|"
             << kh.getSdt() << "\n";
    }
    file.close();
    return true;
}

bool DataManager::luuLichDatSan(const MangDong<LichDatSan>& ds) {
    ofstream file(FILE_LICH);
    if (!file.is_open()) return false;

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const LichDatSan& l = ds[i];
        file << l.getMaLichDat() << "|"
             << l.getMaKH() << "|"
             << l.getMaSan() << "|"
             << DateTimeUtils::timeToString(l.getThoiGianBatDau()) << "|"
             << DateTimeUtils::timeToString(l.getThoiGianKetThuc()) << "|"
             << l.getTongTien() << "|"
             << l.getTrangThaiDat() << "|"
             << l.getTrangThaiTT() << "|"
             << (l.getThoiGianThanhToan() ?
                     DateTimeUtils::timeToString(l.getThoiGianThanhToan()) : "N/A")
             << "\n";
    }
    file.close();
    return true;
}

bool DataManager::taiDichVu(MangDong<DichVu>& ds) {
    ifstream file(FILE_DICHVU);
    if (!file.is_open()) {
        cout << "Khong tim thay " << FILE_DICHVU << ". Tao du lieu mau...\n";
        // Dữ liệu mẫu
        ds.them(DichVu("DV1", "Nuoc suoi", "Chai", 10000, 100, "Do uong", true));
        ds.them(DichVu("DV2", "Nuoc ngot", "Lon", 15000, 80, "Do uong", true));
        ds.them(DichVu("DV3", "Thue giay", "Doi", 50000, 20, "Thiet bi", true));
        ds.them(DichVu("DV4", "Thue bong", "Qua", 30000, 15, "Thiet bi", true));
        ds.them(DichVu("DV5", "Mi tom", "Goi", 20000, 50, "Do an", true));
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string c[7];
        int cnt = splitByPipe(line, c, 7);
        if (cnt < 6) continue;

        double donGia = 0.0;
        int ton = 0;
        bool phucVu = true;

        try { donGia = stod(c[3]); } catch (...) { donGia = 0.0; }
        try { ton = stoi(c[4]); } catch (...) { ton = 0; }
        if (cnt >= 7) phucVu = (c[6] == "1");

        ds.them(DichVu(c[0], c[1], c[2], donGia, ton, c[5], phucVu));
    }
    file.close();
    cout << "Da tai " << ds.getKichThuoc() << " dich vu.\n";
    return true;
}

bool DataManager::taiDonDichVu(MangDong<DonDichVu>& ds) {
    ifstream file(FILE_DONDICHVU);
    if (!file.is_open()) {
        cout << "Khong tim thay " << FILE_DONDICHVU << ".\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        string c[10];
        int cnt = splitByPipe(line, c, 10);
        if (cnt < 5) continue;

        time_t tgDat = DateTimeUtils::stringToTime(c[3]);
        
        DonDichVu don(c[0], c[1], c[2], tgDat);
        don.setTrangThai(cnt >= 5 ? c[4] : "Cho Xu Ly");

        // Tải chi tiết dịch vụ (nếu có)
        // Format: maDV1:tenDV1:soLuong1:donGia1;maDV2:tenDV2:soLuong2:donGia2
        if (cnt >= 6 && !c[5].empty()) {
            string chiTietStr = c[5];
            int start = 0;
            for (int i = 0; i <= (int)chiTietStr.size(); ++i) {
                if (i == (int)chiTietStr.size() || chiTietStr[i] == ';') {
                    string item = chiTietStr.substr(start, i - start);
                    if (!item.empty()) {
                        string parts[4];
                        int pCnt = 0, pStart = 0;
                        for (int j = 0; j <= (int)item.size(); ++j) {
                            if (j == (int)item.size() || item[j] == ':') {
                                if (pCnt < 4) parts[pCnt++] = item.substr(pStart, j - pStart);
                                pStart = j + 1;
                            }
                        }
                        if (pCnt >= 4) {
                            int sl = 0;
                            double gia = 0.0;
                            try { sl = stoi(parts[2]); } catch (...) { sl = 0; }
                            try { gia = stod(parts[3]); } catch (...) { gia = 0.0; }
                            don.themDichVu(parts[0], parts[1], sl, gia);
                        }
                    }
                    start = i + 1;
                }
            }
        }

        ds.them(don);
    }
    file.close();
    cout << "Da tai " << ds.getKichThuoc() << " don dich vu.\n";
    return true;
}

bool DataManager::luuDichVu(const MangDong<DichVu>& ds) {
    ofstream file(FILE_DICHVU);
    if (!file.is_open()) return false;

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const DichVu& dv = ds[i];
        file << dv.getMaDichVu() << "|"
             << dv.getTenDichVu() << "|"
             << dv.getDonVi() << "|"
             << dv.getDonGia() << "|"
             << dv.getSoLuongTon() << "|"
             << dv.getLoaiDichVu() << "|"
             << (dv.getConPhucVu() ? "1" : "0") << "\n";
    }
    file.close();
    return true;
}

bool DataManager::luuDonDichVu(const MangDong<DonDichVu>& ds) {
    ofstream file(FILE_DONDICHVU);
    if (!file.is_open()) return false;

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const DonDichVu& don = ds[i];
        file << don.getMaDon() << "|"
             << don.getMaLichDat() << "|"
             << don.getMaKhachHang() << "|"
             << DateTimeUtils::timeToString(don.getThoiGianDat()) << "|"
             << don.getTrangThai() << "|";

        // Lưu chi tiết dịch vụ
        const auto& chiTiet = don.getChiTiet();
        for (int j = 0; j < chiTiet.getKichThuoc(); ++j) {
            if (j > 0) file << ";";
            file << chiTiet[j].maDichVu << ":"
                 << chiTiet[j].tenDichVu << ":"
                 << chiTiet[j].soLuong << ":"
                 << chiTiet[j].donGia;
        }
        file << "\n";
    }
    file.close();
    return true;
}

void DataManager::taiTatCa(MangDong<SanBong>& dsSan,
                           MangDong<KhachHang>& dsKhach,
                           MangDong<LichDatSan>& dsLich,
                           MangDong<DichVu>& dsDichVu,
                           MangDong<DonDichVu>& dsDonDichVu) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    taiSanBong(dsSan);
    taiKhachHang(dsKhach);
    taiLichDatSan(dsLich);
    taiDichVu(dsDichVu);
    taiDonDichVu(dsDonDichVu);
}

void DataManager::luuTatCa(const MangDong<SanBong>& dsSan,
                           const MangDong<KhachHang>& dsKhach,
                           const MangDong<LichDatSan>& dsLich,
                           const MangDong<DichVu>& dsDichVu,
                           const MangDong<DonDichVu>& dsDonDichVu) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    luuSanBong(dsSan);
    luuKhachHang(dsKhach);
    luuLichDatSan(dsLich);
    luuDichVu(dsDichVu);
    luuDonDichVu(dsDonDichVu);

    cout << "Da luu toan bo du lieu.\n";
}

bool DataManager::backupData(const string& backupName) {
    string cmd;
#ifdef _WIN32
    cmd = "xcopy data data_backup_" + backupName + " /E /I /Y";
#else
    cmd = "cp -r data data_backup_" + backupName;
#endif
    return system(cmd.c_str()) == 0;
}

bool DataManager::restoreData(const string& backupName) {
    string cmd;
#ifdef _WIN32
    cmd = "xcopy data_backup_" + backupName + " data /E /I /Y";
#else
    cmd = "cp -r data_backup_" + backupName + "/* data/";
#endif
    return system(cmd.c_str()) == 0;
}

bool DataManager::xuatBaoCaoCSV(const string& filename, const MangDong<LichDatSan>& ds) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // Header
    file << "Ma Lich,Ma KH,Ma San,Bat Dau,Ket Thuc,Tien,TT Dat,TT Thanh Toan\n";

    // Data
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const LichDatSan& l = ds[i];
        file << l.getMaLichDat() << ","
             << l.getMaKH() << ","
             << l.getMaSan() << ","
             << DateTimeUtils::timeToString(l.getThoiGianBatDau()) << ","
             << DateTimeUtils::timeToString(l.getThoiGianKetThuc()) << ","
             << l.getTongTien() << ","
             << l.getTrangThaiDat() << ","
             << l.getTrangThaiTT() << "\n";
    }
    file.close();
    return true;
}
