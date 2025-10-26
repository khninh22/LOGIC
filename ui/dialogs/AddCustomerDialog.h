#ifndef ADDCUSTOMERDIALOG_H
#define ADDCUSTOMERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "../../QuanLyThueSan.h"

class AddCustomerDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddCustomerDialog(QuanLyThueSan* sys, QWidget* parent = nullptr);

    QString getNewMaKH() const { return _newMaKH; }

private slots:
    void onSave();

private:
    QuanLyThueSan* heThong;
    QLineEdit *edtMa, *edtTen, *edtSDT;
    QString _newMaKH;
};

#endif // ADDCUSTOMERDIALOG_H
