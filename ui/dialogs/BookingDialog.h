#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDate>
#include <QTime>
#include <QTableWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include "../../QuanLyThueSan.h"

class BookingDialog : public QDialog {
    Q_OBJECT
public:
    BookingDialog(QuanLyThueSan* sys, const QString& maSan, const QString& tenSan,
                  const QDate& date, const QTime& start, const QTime& end,
                  QWidget* parent = nullptr);

private:
    void loadKhachHangComboBox();
    void loadDichVu();
    void updateTongTien();

private slots:
    void onKhachHangSelected(int index);
    void onThemDichVu();
    void onXoaDichVu();
    void onConfirm();
    void onDichVuToggled(bool checked);

private:
    QuanLyThueSan* heThong;
    QString maSan;
    QDate ngay;
    QTime gioBD, gioKT;
    QComboBox* cboKhach;
    
    // Dịch vụ
    QCheckBox* chkDichVu;
    QComboBox* cboDichVu;
    QSpinBox* spinSoLuong;
    QTableWidget* tblDichVu;
    QPushButton* btnThemDV;
    QPushButton* btnXoaDV;
    QLabel* lblTienSan;
    QLabel* lblTienDichVu;
    QLabel* lblTongTien;
    
    double tienSan;
    double tienDichVu;
};

#endif // BOOKINGDIALOG_H
