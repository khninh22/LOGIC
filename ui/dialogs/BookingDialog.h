#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDate>
#include <QTime>
#include "../../QuanLyThueSan.h"

class BookingDialog : public QDialog {
    Q_OBJECT
public:
    BookingDialog(QuanLyThueSan* sys, const QString& maSan, const QString& tenSan,
                  const QDate& date, const QTime& start, const QTime& end,
                  QWidget* parent = nullptr);

private:
    void loadKhachHangComboBox();

private slots:
    void onKhachHangSelected(int index);
    void onConfirm();

private:
    QuanLyThueSan* heThong;
    QString maSan;
    QDate ngay;
    QTime gioBD, gioKT;
    QComboBox* cboKhach;
};

#endif // BOOKINGDIALOG_H
