#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "../core/entities/KhachHang.h"
#include "../core/entities/SanBong.h"
#include "../core/entities/LichDatSan.h"
#include "../core/utils/ThuatToan.h"
#include <string>
using namespace std;

class DataManager {
private:
    static const string DATA_DIR;
    static const string FILE_SAN;
    static const string FILE_KHACH;
    static const string FILE_LICH;

    // Helper: Tách chuỗi theo dấu '|'
    static int splitByPipe(const string& line, string out[], int maxN);

public:
    // Tải dữ liệu từ file
    static bool taiSanBong(MangDong<SanBong>& ds);
    static bool taiKhachHang(MangDong<KhachHang>& ds);
    static bool taiLichDatSan(MangDong<LichDatSan>& ds);

    // Lưu dữ liệu vào file
    static bool luuSanBong(const MangDong<SanBong>& ds);
    static bool luuKhachHang(const MangDong<KhachHang>& ds);
    static bool luuLichDatSan(const MangDong<LichDatSan>& ds);

    // Tải/Lưu tất cả
    static void taiTatCa(MangDong<SanBong>& dsSan,
                         MangDong<KhachHang>& dsKhach,
                         MangDong<LichDatSan>& dsLich);

    static void luuTatCa(const MangDong<SanBong>& dsSan,
                         const MangDong<KhachHang>& dsKhach,
                         const MangDong<LichDatSan>& dsLich);

    // ✅ MỚI: Backup/Restore
    static bool backupData(const string& backupName);
    static bool restoreData(const string& backupName);

    // ✅ MỚI: Xuất báo cáo
    static bool xuatBaoCaoCSV(const string& filename, const MangDong<LichDatSan>& ds);
};

#endif // DATAMANAGER_H
