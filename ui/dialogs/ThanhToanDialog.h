#ifndef THANHTOADIALOG_H
#define THANHTOADIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QTextBrowser>
#include "../../QuanLyThueSan.h"

class ThanhToanDialog : public QDialog {
    Q_OBJECT
public:
    explicit ThanhToanDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void refreshTable();
    void onThanhToan();
    void onRowSelected();
    void onInHoaDon();

private:
    void hienThiChiTiet(const std::string& maLich);
    
    QuanLyThueSan* heThong;
    QTableWidget* tblLich;
    QPushButton* btnThanhToan;
    QPushButton* btnInHoaDon;
    QTextBrowser* txtChiTiet;
};

#endif // THANHTOADIALOG_H
