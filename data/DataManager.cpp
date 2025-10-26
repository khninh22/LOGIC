#include "DataManager.h"
#include "../core/utils/DateTimeUtils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

const string DataManager::DATA_DIR = "data/";
const string DataManager::FILE_SAN = "data/sanbong.txt";
const string DataManager::FILE_KHACH = "data/khachhang.txt";
const string DataManager::FILE_LICH = "data/lichdatsan.txt";

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

void DataManager::taiTatCa(MangDong<SanBong>& dsSan,
                           MangDong<KhachHang>& dsKhach,
                           MangDong<LichDatSan>& dsLich) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    taiSanBong(dsSan);
    taiKhachHang(dsKhach);
    taiLichDatSan(dsLich);
}

void DataManager::luuTatCa(const MangDong<SanBong>& dsSan,
                           const MangDong<KhachHang>& dsKhach,
                           const MangDong<LichDatSan>& dsLich) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    luuSanBong(dsSan);
    luuKhachHang(dsKhach);
    luuLichDatSan(dsLich);

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
