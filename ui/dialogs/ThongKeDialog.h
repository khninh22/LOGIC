#ifndef THONGKEDIALOG_H
#define THONGKEDIALOG_H

#include <QDialog>
#include <QTextBrowser>
#include <QComboBox>
#include <QDateEdit>
#include "../../QuanLyThueSan.h"

class ThongKeDialog : public QDialog {
    Q_OBJECT
public:
    explicit ThongKeDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void onKhoangThoiGianChanged(int index);
    void onDateChanged();
    void capNhatThongKe();

private:
    void tinhTongQuan(QTextBrowser* text);
    void tinhDoanhThu(QTextBrowser* text);
    void tinhTanSuat(QTextBrowser* text);
    void tinhKhachHang(QTextBrowser* text);
    void xacDinhKhoangThoiGian(time_t& from, time_t& to, const QString& tenKy);

    QuanLyThueSan* heThong;
    QComboBox* cboKhoangThoiGian;
    QDateEdit* dateFrom;
    QDateEdit* dateTo;
    QTextBrowser* textTongQuan;
    QTextBrowser* textDoanhThu;
    QTextBrowser* textTanSuat;
    QTextBrowser* textKhachHang;
};

#endif // THONGKEDIALOG_H
