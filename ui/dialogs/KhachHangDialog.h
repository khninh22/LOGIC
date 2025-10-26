#ifndef KHACHHANGDIALOG_H
#define KHACHHANGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../../QuanLyThueSan.h"

class KhachHangDialog : public QDialog {
    Q_OBJECT
public:
    explicit KhachHangDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void refreshTable();
    void onSearch(const QString& text);
    void onThem();
    void onSua();
    void onXoa();

private:
    QuanLyThueSan* heThong;
    QTableWidget* tblKhach;
    QLineEdit* edtSearch;
    QPushButton *btnThem, *btnSua, *btnXoa, *btnRefresh;
};

#endif // KHACHHANGDIALOG_H
