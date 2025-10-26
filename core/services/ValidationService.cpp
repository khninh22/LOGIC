#include "ValidationService.h"
#include <cctype>

bool ValidationService::kiemTraMaSanHopLe(const string& maSan) {
    return !maSan.empty() && maSan.length() <= 10;
}

bool ValidationService::kiemTraMaKHHopLe(const string& maKH) {
    return !maKH.empty() && maKH.length() <= 10;
}

bool ValidationService::kiemTraSDTHopLe(const string& sdt) {
    if (sdt.length() < 10 || sdt.length() > 11)
        return false;

    for (size_t i = 0; i < sdt.length(); ++i) {
        if (!isdigit(sdt[i]))
            return false;
    }
    return true;
}

bool ValidationService::kiemTraLoaiSanHopLe(int loai) {
    return loai == 5 || loai == 7 || loai == 11;
}
