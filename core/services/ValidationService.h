#ifndef VALIDATIONSERVICE_H
#define VALIDATIONSERVICE_H

#include <string>
using namespace std;

class ValidationService {
public:
    // Kiểm tra mã sân hợp lệ
    static bool kiemTraMaSanHopLe(const string& maSan);

    // Kiểm tra mã khách hàng hợp lệ
    static bool kiemTraMaKHHopLe(const string& maKH);

    // Kiểm tra số điện thoại hợp lệ (10-11 số)
    static bool kiemTraSDTHopLe(const string& sdt);

    // Kiểm tra loại sân hợp lệ (5, 7, 11)
    static bool kiemTraLoaiSanHopLe(int loai);
};

#endif // VALIDATIONSERVICE_H
