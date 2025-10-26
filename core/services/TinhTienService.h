#ifndef TINHTIENSERVICE_H
#define TINHTIENSERVICE_H

#include "../entities/SanBong.h"
#include <ctime>

class TinhTienService {
public:
    // Cấu hình giờ mở/đóng cửa
    static const int GIO_MO_CUA = 6;
    static const int GIO_DONG_CUA = 22;

    // Bảng giá theo loại sân và khung giờ
    static double donGiaBanNgay(int loaiSan);   // 06:00-17:00
    static double donGiaBanToi(int loaiSan);    // 17:00-22:00

    // Tính tiền cho khoảng thời gian [tBD, tKT)
    static double tinhTienTheoKhoang(const SanBong& san, time_t tBD, time_t tKT);

    // Kiểm tra khoảng giờ có trong giờ mở cửa không
    static bool khoangTrongGioMoCua(const tm& ngay, time_t tBD, time_t tKT);
};

#endif // TINHTIENSERVICE_H
