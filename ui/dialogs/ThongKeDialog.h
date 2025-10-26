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
    void tinhDoanhThu(QTextBrowser* text);
    void tinhTanSuat(QTextBrowser* text);

    QuanLyThueSan* heThong;
    QComboBox* cboKhoangThoiGian;
    QDateEdit* dateFrom;
    QDateEdit* dateTo;
    QTextBrowser* textDoanhThu;
    QTextBrowser* textTanSuat;
};

#endif // THONGKEDIALOG_H
