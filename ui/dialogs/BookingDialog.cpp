#include "BookingDialog.h"
#include "AddCustomerDialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

BookingDialog::BookingDialog(QuanLyThueSan* sys, const QString& maSan, const QString& tenSan,
                             const QDate& date, const QTime& start, const QTime& end,
                             QWidget* parent)
    : QDialog(parent), heThong(sys), maSan(maSan), ngay(date), gioBD(start), gioKT(end) {

    setWindowTitle("Đặt sân");
    setModal(true);
    resize(500, 350);

    auto* layout = new QVBoxLayout(this);

    // Thông tin sân
    auto* grpInfo = new QGroupBox("Thông tin đặt sân");
    auto* infoLayout = new QGridLayout(grpInfo);

    infoLayout->addWidget(new QLabel("<b>Sân:</b>"), 0, 0);
    infoLayout->addWidget(new QLabel(QString("%1 - %2").arg(maSan).arg(tenSan)), 0, 1);
    infoLayout->addWidget(new QLabel("<b>Ngày:</b>"), 1, 0);
    infoLayout->addWidget(new QLabel(date.toString("dd/MM/yyyy")), 1, 1);
    infoLayout->addWidget(new QLabel("<b>Thời gian:</b>"), 2, 0);
    infoLayout->addWidget(new QLabel(QString("%1 - %2").arg(start.toString("HH:mm")).arg(end.toString("HH:mm"))), 2, 1);

    int phut = start.secsTo(end) / 60;
    infoLayout->addWidget(new QLabel("<b>Thời lượng:</b>"), 3, 0);
    infoLayout->addWidget(new QLabel(QString("%1 phút").arg(phut)), 3, 1);

    layout->addWidget(grpInfo);

    // Form khách hàng
    auto* grpKH = new QGroupBox("Thông tin khách hàng");
    auto* khLayout = new QGridLayout(grpKH);

    cboKhach = new QComboBox;
    khLayout->addWidget(new QLabel("Khách hàng:"), 0, 0);
    khLayout->addWidget(cboKhach, 0, 1);

    layout->addWidget(grpKH);
    layout->addStretch();

    // Buttons
    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btnBox->button(QDialogButtonBox::Ok)->setText("✅ Đặt sân");
    btnBox->button(QDialogButtonBox::Cancel)->setText("❌ Hủy");
    layout->addWidget(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, this, &BookingDialog::onConfirm);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(cboKhach, QOverload<int>::of(&QComboBox::activated),
            this, &BookingDialog::onKhachHangSelected);

    loadKhachHangComboBox();
}

void BookingDialog::loadKhachHangComboBox() {
    QString maKHHienTai = cboKhach->currentData().toString();
    cboKhach->clear();

    // Mục 0: Thêm mới
    cboKhach->addItem("--- Thêm khách hàng mới ---");
    cboKhach->setItemData(0, QVariant(""));

    // Tải danh sách
    const auto& dsKH = heThong->getDanhSachKhach();
    for (int i = 0; i < dsKH.getKichThuoc(); ++i) {
        const KhachHang& kh = dsKH[i];
        QString displayText = QString::fromStdString(kh.getMaKH() + " - " + kh.getHoTen());
        QString maKH = QString::fromStdString(kh.getMaKH());
        cboKhach->addItem(displayText, QVariant(maKH));
    }

    // Tự động chọn lại
    if (!maKHHienTai.isEmpty()) {
        int index = cboKhach->findData(maKHHienTai);
        if (index != -1) {
            cboKhach->setCurrentIndex(index);
        }
    }
}

void BookingDialog::onKhachHangSelected(int index) {
    if (index == 0) {
        AddCustomerDialog dlg(heThong, this);
        if (dlg.exec() == QDialog::Accepted) {
            loadKhachHangComboBox();
            QString maKHVuaTao = dlg.getNewMaKH();
            int newIndex = cboKhach->findData(maKHVuaTao);
            if (newIndex != -1) {
                cboKhach->setCurrentIndex(newIndex);
            }
        } else {
            cboKhach->setCurrentIndex(0);
        }
    }
}

void BookingDialog::onConfirm() {
    QString maKH_q = cboKhach->currentData().toString();

    if (maKH_q.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn khách hàng hoặc thêm khách hàng mới!");
        return;
    }

    std::string maKH = maKH_q.toStdString();

    // Đặt sân
    QString dtStr = ngay.toString("dd/MM/yyyy") + " " + gioBD.toString("HH:mm");
    time_t tBD = QuanLyThueSan::parseDateTime(dtStr.toStdString());
    time_t tKT = tBD + gioBD.secsTo(gioKT);

    std::string maLD;
    double tien = 0;

    if (!heThong->datSanAPI(maKH, maSan.toStdString(), tBD, tKT, maLD, tien)) {
        QMessageBox::critical(this, "Lỗi", "Không thể đặt sân!\n(Trùng lịch hoặc lỗi hệ thống)");
        return;
    }

    // Thành công
    QString msg = QString(
                      "✅ <b>Đặt sân thành công!</b><br><br>"
                      "📋 Mã lịch: <b>%1</b><br>"
                      "🏟️ Sân: <b>%2</b><br>"
                      "📅 Ngày: <b>%3</b><br>"
                      "🕐 Giờ: <b>%4 - %5</b><br>"
                      "💰 Tiền: <b>%6 VND</b><br><br>"
                      "⚠️ <i>Vui lòng thanh toán trước khi sử dụng sân.</i>"
                      ).arg(QString::fromStdString(maLD))
                      .arg(maSan)
                      .arg(ngay.toString("dd/MM/yyyy"))
                      .arg(gioBD.toString("HH:mm"))
                      .arg(gioKT.toString("HH:mm"))
                      .arg(QString::number((qlonglong)tien));

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Đặt sân thành công");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    accept();
}
