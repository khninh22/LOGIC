#ifndef DICHVUDIALOG_H
#define DICHVUDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "../../QuanLyThueSan.h"

class DichVuDialog : public QDialog {
    Q_OBJECT
public:
    explicit DichVuDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void onThemMoi();
    void onSua();
    void onXoa();
    void onNhapHang();
    void onTimKiem();
    void onLocLoai(int index);
    void onRefresh();
    void onRowSelected();

private:
    void loadDanhSach();
    void clearInputs();

    QuanLyThueSan* heThong;
    
    QTableWidget* table;
    QLineEdit* txtMa;
    QLineEdit* txtTen;
    QLineEdit* txtDonVi;
    QDoubleSpinBox* spinGia;
    QSpinBox* spinTon;
    QComboBox* cboLoai;
    QComboBox* cboLocLoai;
    QLineEdit* txtTimKiem;
    
    QPushButton* btnThem;
    QPushButton* btnSua;
    QPushButton* btnXoa;
    QPushButton* btnNhapHang;
};

#endif // DICHVUDIALOG_H
