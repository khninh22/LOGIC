#ifndef THUATTOAN_H
#define THUATTOAN_H

#include <iostream>
#include <string>
using namespace std;

// ============================
//  MẢNG ĐỘNG TỰ CÀI ĐẶT
// ============================

template <typename T>
class MangDong {
private:
    T* data;           // con trỏ tới mảng dữ liệu
    int kichThuoc;     // số phần tử hiện có
    int sucChua;       // dung lượng tối đa hiện tại

    void moRong() {
        int moi = (sucChua == 0) ? 2 : sucChua * 2;
        T* temp = new T[moi];
        for (int i = 0; i < kichThuoc; i++)
            temp[i] = data[i];
        delete[] data;
        data = temp;
        sucChua = moi;
    }

public:
    // 1) Ctor mặc định + đủ tham số
    MangDong(int sucChuaBanDau = 5)
        : data(new T[sucChuaBanDau > 0 ? sucChuaBanDau : 5]),
        kichThuoc(0),
        sucChua(sucChuaBanDau > 0 ? sucChuaBanDau : 5) {}

    // 2) Copy ctor (deep copy)
    MangDong(const MangDong& o)
        : data(new T[o.sucChua]),
        kichThuoc(o.kichThuoc),
        sucChua(o.sucChua) {
        for (int i = 0; i < kichThuoc; ++i) data[i] = o.data[i];
    }

    // 3) Copy assignment (deep copy + self-assign guard)
    MangDong& operator=(const MangDong& o) {
        if (this == &o) return *this;
        T* newData = new T[o.sucChua];
        for (int i = 0; i < o.kichThuoc; ++i) newData[i] = o.data[i];
        delete[] data;
        data = newData;
        kichThuoc = o.kichThuoc;
        sucChua   = o.sucChua;
        return *this;
    }

    // 4) Destructor
    ~MangDong() { delete[] data; }

    void them(const T& phanTu) {
        if (kichThuoc >= sucChua)
            moRong();
        data[kichThuoc++] = phanTu;
    }

    int getKichThuoc() const {
        return kichThuoc;
    }

    T& operator[](int i) {
        return data[i];
    }

    const T& operator[](int i) const {
        return data[i];
    }

    bool xoaTheoViTri(int index) {
        if (index < 0 || index >= kichThuoc)
            return false;
        for (int i = index; i < kichThuoc - 1; i++)
            data[i] = data[i + 1];
        kichThuoc--;
        return true;
    }
};

// ============================
//  HÀM TÌM KIẾM / KIỂM TRA TRÙNG
// ============================

// tìm phần tử theo mã (dùng cho class có getMaKH() hoặc getMaSan())
template <typename T>
int timTheoMa(const MangDong<T>& ds, const string& ma) {
    for (int i = 0; i < ds.getKichThuoc(); i++) {
        if ((ds[i].getMaKH() == ma) || (ds[i].getMaSan() == ma))
            return i;
    }
    return -1;
}

// kiểm tra mã trùng
template <typename T>
bool maDaTonTai(const MangDong<T>& ds, const string& ma) {
    return timTheoMa(ds, ma) != -1;
}

// ============================
//  HÀM SẮP XẾP (BUBBLE SORT)
// ============================

template <typename T>
void sapXep(MangDong<T>& ds, bool (*soSanh)(const T&, const T&)) {
    int n = ds.getKichThuoc();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (soSanh(ds[j + 1], ds[j])) {
                T temp = ds[j];
                ds[j] = ds[j + 1];
                ds[j + 1] = temp;
            }
        }
    }
}

#endif
