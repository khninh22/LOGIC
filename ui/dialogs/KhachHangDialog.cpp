#include "KhachHangDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QHeaderView>

KhachHangDialog::KhachHangDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("Quản lý khách hàng");
    resize(800, 600);

    auto* layout = new QVBoxLayout(this);

    // Toolbar
    auto* toolbar = new QHBoxLayout;
    btnThem = new QPushButton("➕ Thêm mới");
    btnSua = new QPushButton("✏️ Sửa");
    btnXoa = new QPushButton("🗑️ Xóa");
    btnRefresh = new QPushButton("🔄 Làm mới");

    toolbar->addWidget(btnThem);
    toolbar->addWidget(btnSua);
    toolbar->addWidget(btnXoa);
    toolbar->addStretch();
    toolbar->addWidget(btnRefresh);

    // Search
    auto* searchLayout = new QHBoxLayout;
    edtSearch = new QLineEdit;
    edtSearch->setPlaceholderText("Tìm kiếm theo mã, tên, SĐT...");
    searchLayout->addWidget(new QLabel("🔍 Tìm kiếm:"));
    searchLayout->addWidget(edtSearch);

    // Table
    tblKhach = new QTableWidget;
    tblKhach->setColumnCount(3);
    tblKhach->setHorizontalHeaderLabels({"Mã KH", "Họ tên", "Số điện thoại"});
    tblKhach->horizontalHeader()->setStretchLastSection(true);
    tblKhach->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblKhach->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(toolbar);
    layout->addLayout(searchLayout);
    layout->addWidget(tblKhach);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(btnBox);

    connect(btnThem, &QPushButton::clicked, this, &KhachHangDialog::onThem);
    connect(btnSua, &QPushButton::clicked, this, &KhachHangDialog::onSua);
    connect(btnXoa, &QPushButton::clicked, this, &KhachHangDialog::onXoa);
    connect(btnRefresh, &QPushButton::clicked, this, &KhachHangDialog::refreshTable);
    connect(edtSearch, &QLineEdit::textChanged, this, &KhachHangDialog::onSearch);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::accept);

    refreshTable();
}

void KhachHangDialog::refreshTable() {
    const auto& ds = heThong->getDanhSachKhach();
    tblKhach->setRowCount(ds.getKichThuoc());

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        tblKhach->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(ds[i].getMaKH())));
        tblKhach->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(ds[i].getHoTen())));
        tblKhach->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(ds[i].getSdt())));
    }
}

void KhachHangDialog::onSearch(const QString& text) {
    for (int i = 0; i < tblKhach->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < tblKhach->columnCount(); ++j) {
            if (tblKhach->item(i, j)->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        tblKhach->setRowHidden(i, !match);
    }
}

void KhachHangDialog::onThem() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm khách hàng");
    auto* layout = new QGridLayout(&dlg);

    auto* edtMa = new QLineEdit;
    auto* edtTen = new QLineEdit;
    auto* edtSDT = new QLineEdit;

    // ✅ FIX: Không dùng số 0 đệm
    QString maMacDinh = QString("KH%1").arg(heThong->getDanhSachKhach().getKichThuoc() + 1);
    edtMa->setText(maMacDinh);

    layout->addWidget(new QLabel("Mã KH:"), 0, 0);
    layout->addWidget(edtMa, 0, 1);
    layout->addWidget(new QLabel("Họ tên:"), 1, 0);
    layout->addWidget(edtTen, 1, 1);
    layout->addWidget(new QLabel("SĐT:"), 2, 0);
    layout->addWidget(edtSDT, 2, 1);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(btnBox, 3, 0, 1, 2);

    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        std::string ma = edtMa->text().toStdString();
        std::string ten = edtTen->text().toStdString();
        std::string sdt = edtSDT->text().toStdString();

        if (ma.empty() || ten.empty() || sdt.empty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng điền đầy đủ thông tin!");
            return;
        }

        if (!heThong->themKhachHangAPI(ma, ten, sdt)) {
            QMessageBox::critical(this, "Lỗi", "Không thể thêm khách hàng!\n(Mã đã tồn tại hoặc thông tin không hợp lệ)");
            return;
        }

        QMessageBox::information(this, "Thành công", "Đã thêm khách hàng!");
        refreshTable();
    }
}

void KhachHangDialog::onSua() {
    auto selected = tblKhach->currentRow();
    if (selected < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn khách hàng cần sửa!");
        return;
    }

    QString maKH = tblKhach->item(selected, 0)->text();

    QDialog dlg(this);
    dlg.setWindowTitle("Sửa thông tin khách hàng");
    auto* layout = new QGridLayout(&dlg);

    auto* edtTen = new QLineEdit(tblKhach->item(selected, 1)->text());
    auto* edtSDT = new QLineEdit(tblKhach->item(selected, 2)->text());

    layout->addWidget(new QLabel("Mã KH:"), 0, 0);
    layout->addWidget(new QLabel(maKH), 0, 1);
    layout->addWidget(new QLabel("Họ tên:"), 1, 0);
    layout->addWidget(edtTen, 1, 1);
    layout->addWidget(new QLabel("SĐT:"), 2, 0);
    layout->addWidget(edtSDT, 2, 1);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(btnBox, 3, 0, 1, 2);

    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        std::string ma = maKH.toStdString();
        std::string ten = edtTen->text().toStdString();
        std::string sdt = edtSDT->text().toStdString();

        if (!heThong->suaKhachHangAPI(ma, ten, sdt)) {
            QMessageBox::critical(this, "Lỗi", "Không thể cập nhật thông tin!");
            return;
        }

        QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin!");
        refreshTable();
    }
}

void KhachHangDialog::onXoa() {
    auto selected = tblKhach->currentRow();
    if (selected < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn khách hàng cần xóa!");
        return;
    }

    QString maKH = tblKhach->item(selected, 0)->text();
    QString ten = tblKhach->item(selected, 1)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       QString("Bạn có chắc muốn xóa khách hàng:\n%1 - %2?").arg(maKH).arg(ten),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!heThong->xoaKhachHangAPI(maKH.toStdString())) {
            QMessageBox::critical(this, "Lỗi",
                                  "Không thể xóa khách hàng!\n"
                                  "(Khách hàng còn lịch đặt hiệu lực)");
            return;
        }
        QMessageBox::information(this, "Thành công", "Đã xóa khách hàng!");
        refreshTable();
    }
}
