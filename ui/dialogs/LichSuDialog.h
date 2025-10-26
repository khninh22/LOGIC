#ifndef LICHSUDIALOG_H
#define LICHSUDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include "../../QuanLyThueSan.h"

class LichSuDialog : public QDialog {
    Q_OBJECT
public:
    explicit LichSuDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

private slots:
    void refreshTable();

private:
    QuanLyThueSan* heThong;
    QTableWidget* tblLich;
    QComboBox* cboFilter;
};

#endif // LICHSUDIALOG_H
