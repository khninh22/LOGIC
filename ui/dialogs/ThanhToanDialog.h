#ifndef THANHTOADIALOG_H
#define THANHTOADIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include "../../QuanLyThueSan.h"

class ThanhToanDialog : public QDialog {
    Q_OBJECT
public:
    explicit ThanhToanDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void refreshTable();
    void onThanhToan();

private:
    QuanLyThueSan* heThong;
    QTableWidget* tblLich;
    QPushButton* btnThanhToan;
};

#endif // THANHTOADIALOG_H
