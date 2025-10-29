#ifndef DICHVU_H
#define DICHVU_H

#include <string>
#include <iostream>
using namespace std;

/**
 * DichVu - Dịch vụ bổ sung cho khách hàng
 * VD: Nước uống, thuê giày, đồ ăn, thuê bóng...
 */
class DichVu {
private:
    string maDichVu;     // VD: DV001, DV002...
    string tenDichVu;    // VD: "Nước suối", "Thuê giày"
    string donVi;        // VD: "Chai", "Đôi", "Phần"
    double donGia;       // Giá mỗi đơn vị
    int soLuongTon;      // Số lượng tồn kho
    string loaiDichVu;   // VD: "Đồ uống", "Đồ ăn", "Thiết bị"
    bool conPhucVu;      // Còn phục vụ hay không

public:
    // Constructors
    DichVu(string ma = "", string ten = "", string dv = "Cái",
           double gia = 0.0, int ton = 0, string loai = "Khác",
           bool phucVu = true);

    // Getters
    string getMaDichVu() const;
    string getTenDichVu() const;
    string getDonVi() const;
    double getDonGia() const;
    int getSoLuongTon() const;
    string getLoaiDichVu() const;
    bool getConPhucVu() const;

    // Setters
    void setTenDichVu(const string& ten);
    void setDonVi(const string& dv);
    void setDonGia(double gia);
    void setSoLuongTon(int ton);
    void setLoaiDichVu(const string& loai);
    void setConPhucVu(bool phucVu);

    // Methods
    bool tangSoLuong(int them);      // Nhập hàng
    bool giamSoLuong(int giam);      // Bán hàng
    double tinhTien(int soLuong) const;  // Tính tiền cho số lượng

    // Display
    void hienThi() const;
};

#endif // DICHVU_H
