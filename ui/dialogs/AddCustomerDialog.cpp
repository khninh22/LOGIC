#include "AddCustomerDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

AddCustomerDialog::AddCustomerDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys), _newMaKH("") {

    setWindowTitle("Thêm khách hàng mới");
    setModal(true);

    auto* layout = new QGridLayout(this);

    edtMa = new QLineEdit;
    edtTen = new QLineEdit;
    edtSDT = new QLineEdit;

    // ✅ FIX: Không dùng số 0 đệm
    QString maMacDinh = QString("KH%1").arg(heThong->getDanhSachKhach().getKichThuoc() + 1);
    edtMa->setText(maMacDinh);
    edtTen->setPlaceholderText("Nguyễn Văn A");
    edtSDT->setPlaceholderText("09xxxxxxxx");

    layout->addWidget(new QLabel("Mã KH:"), 0, 0);
    layout->addWidget(edtMa, 0, 1);
    layout->addWidget(new QLabel("Họ tên:"), 1, 0);
    layout->addWidget(edtTen, 1, 1);
    layout->addWidget(new QLabel("SĐT:"), 2, 0);
    layout->addWidget(edtSDT, 2, 1);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btnBox->button(QDialogButtonBox::Ok)->setText("Lưu");
    layout->addWidget(btnBox, 3, 0, 1, 2);

    connect(btnBox, &QDialogButtonBox::accepted, this, &AddCustomerDialog::onSave);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void AddCustomerDialog::onSave() {
    std::string ma = edtMa->text().toStdString();
    std::string ten = edtTen->text().toStdString();
    std::string sdt = edtSDT->text().toStdString();

    if (ma.empty() || ten.empty()) {
        QMessageBox::warning(this, "Lỗi", "Mã KH và Tên không được để trống!");
        return;
    }

    if (!heThong->themKhachHangAPI(ma, ten, sdt)) {
        QMessageBox::critical(this, "Lỗi",
                              "Không thể thêm khách hàng!\n"
                              "Nguyên nhân có thể do:\n"
                              "- Mã KH đã tồn tại\n"
                              "- SĐT không hợp lệ (phải 10-11 số)");
        return;
    }

    _newMaKH = QString::fromStdString(ma);
    QMessageBox::information(this, "Thành công", "Đã thêm khách hàng mới!");
    accept();
}
