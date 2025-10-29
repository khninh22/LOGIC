#include "DichVuDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>

DichVuDialog::DichVuDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("🍔 Quản lý Dịch vụ");
    resize(1000, 600);

    auto* mainLayout = new QVBoxLayout(this);

    // Header
    auto* lblTitle = new QLabel("<h2>🍔 QUẢN LÝ DỊCH VỤ</h2>");
    lblTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblTitle);

    // Tìm kiếm và lọc
    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("🔍 Tìm kiếm:"));
    txtTimKiem = new QLineEdit;
    txtTimKiem->setPlaceholderText("Nhập tên hoặc mã dịch vụ...");
    searchLayout->addWidget(txtTimKiem);

    searchLayout->addWidget(new QLabel("📂 Loại:"));
    cboLocLoai = new QComboBox;
    cboLocLoai->addItems({"Tất cả", "Đồ uống", "Đồ ăn", "Thiết bị", "Khác"});
    searchLayout->addWidget(cboLocLoai);

    auto* btnRefresh = new QPushButton("🔄 Làm mới");
    searchLayout->addWidget(btnRefresh);
    searchLayout->addStretch();

    mainLayout->addLayout(searchLayout);

    // Content layout (Table + Form)
    auto* contentLayout = new QHBoxLayout;

    // Left: Table
    auto* leftGroup = new QGroupBox("📋 Danh sách dịch vụ");
    auto* leftLayout = new QVBoxLayout(leftGroup);
    
    table = new QTableWidget;
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"Mã", "Tên dịch vụ", "Đơn vị", "Đơn giá", 
                                      "Tồn kho", "Loại", "Trạng thái"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSortingEnabled(true);
    leftLayout->addWidget(table);
    
    contentLayout->addWidget(leftGroup, 2);

    // Right: Form
    auto* rightGroup = new QGroupBox("✏️ Thông tin dịch vụ");
    auto* rightLayout = new QVBoxLayout(rightGroup);
    
    auto* formLayout = new QFormLayout;
    
    txtMa = new QLineEdit;
    txtMa->setPlaceholderText("VD: DV1, DV2...");
    formLayout->addRow("Mã dịch vụ:", txtMa);
    
    txtTen = new QLineEdit;
    txtTen->setPlaceholderText("VD: Nước suối, Thuê giày...");
    formLayout->addRow("Tên dịch vụ:", txtTen);
    
    txtDonVi = new QLineEdit;
    txtDonVi->setPlaceholderText("VD: Chai, Đôi, Phần...");
    formLayout->addRow("Đơn vị:", txtDonVi);
    
    spinGia = new QDoubleSpinBox;
    spinGia->setRange(0, 10000000);
    spinGia->setSuffix(" VND");
    spinGia->setGroupSeparatorShown(true);
    formLayout->addRow("Đơn giá:", spinGia);
    
    spinTon = new QSpinBox;
    spinTon->setRange(0, 10000);
    formLayout->addRow("Số lượng tồn:", spinTon);
    
    cboLoai = new QComboBox;
    cboLoai->addItems({"Đồ uống", "Đồ ăn", "Thiết bị", "Khác"});
    formLayout->addRow("Loại:", cboLoai);
    
    rightLayout->addLayout(formLayout);
    rightLayout->addStretch();
    
    // Buttons
    auto* btnLayout = new QVBoxLayout;
    btnThem = new QPushButton("➕ Thêm mới");
    btnThem->setStyleSheet("QPushButton { padding: 10px; background-color: #4CAF50; color: white; font-weight: bold; border-radius: 5px; }");
    btnLayout->addWidget(btnThem);
    
    btnSua = new QPushButton("✏️ Sửa");
    btnSua->setStyleSheet("QPushButton { padding: 10px; background-color: #2196F3; color: white; font-weight: bold; border-radius: 5px; }");
    btnSua->setEnabled(false);
    btnLayout->addWidget(btnSua);
    
    btnXoa = new QPushButton("🗑️ Xóa");
    btnXoa->setStyleSheet("QPushButton { padding: 10px; background-color: #f44336; color: white; font-weight: bold; border-radius: 5px; }");
    btnXoa->setEnabled(false);
    btnLayout->addWidget(btnXoa);
    
    btnNhapHang = new QPushButton("📦 Nhập hàng");
    btnNhapHang->setStyleSheet("QPushButton { padding: 10px; background-color: #FF9800; color: white; font-weight: bold; border-radius: 5px; }");
    btnNhapHang->setEnabled(false);
    btnLayout->addWidget(btnNhapHang);
    
    auto* btnClear = new QPushButton("🔄 Làm mới form");
    btnLayout->addWidget(btnClear);
    
    btnLayout->addStretch();
    rightLayout->addLayout(btnLayout);
    
    contentLayout->addWidget(rightGroup, 1);
    mainLayout->addLayout(contentLayout);

    // Signals
    connect(btnThem, &QPushButton::clicked, this, &DichVuDialog::onThemMoi);
    connect(btnSua, &QPushButton::clicked, this, &DichVuDialog::onSua);
    connect(btnXoa, &QPushButton::clicked, this, &DichVuDialog::onXoa);
    connect(btnNhapHang, &QPushButton::clicked, this, &DichVuDialog::onNhapHang);
    connect(btnRefresh, &QPushButton::clicked, this, &DichVuDialog::onRefresh);
    connect(btnClear, &QPushButton::clicked, this, &DichVuDialog::clearInputs);
    connect(txtTimKiem, &QLineEdit::textChanged, this, &DichVuDialog::onTimKiem);
    connect(cboLocLoai, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &DichVuDialog::onLocLoai);
    connect(table, &QTableWidget::itemSelectionChanged, this, &DichVuDialog::onRowSelected);
    connect(table, &QTableWidget::doubleClicked, this, &DichVuDialog::onSua);

    loadDanhSach();
}

void DichVuDialog::loadDanhSach() {
    table->setRowCount(0);
    
    const auto& ds = heThong->getDanhSachDichVu();
    QString loaiFilter = cboLocLoai->currentText();
    QString searchText = txtTimKiem->text().toLower();
    
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const DichVu& dv = ds[i];
        
        // Lọc theo loại
        if (loaiFilter != "Tất cả" && 
            QString::fromStdString(dv.getLoaiDichVu()) != loaiFilter) {
            continue;
        }
        
        // Tìm kiếm
        if (!searchText.isEmpty()) {
            QString ma = QString::fromStdString(dv.getMaDichVu()).toLower();
            QString ten = QString::fromStdString(dv.getTenDichVu()).toLower();
            if (!ma.contains(searchText) && !ten.contains(searchText)) {
                continue;
            }
        }
        
        int row = table->rowCount();
        table->insertRow(row);
        
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dv.getMaDichVu())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dv.getTenDichVu())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dv.getDonVi())));
        
        auto* itemGia = new QTableWidgetItem(QString::number(dv.getDonGia(), 'f', 0) + " VND");
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(row, 3, itemGia);
        
        auto* itemTon = new QTableWidgetItem(QString::number(dv.getSoLuongTon()));
        itemTon->setTextAlignment(Qt::AlignCenter);
        // Cảnh báo tồn kho thấp
        if (dv.getSoLuongTon() <= 10) {
            itemTon->setBackground(QColor(255, 200, 200));
            itemTon->setForeground(Qt::red);
        }
        table->setItem(row, 4, itemTon);
        
        table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(dv.getLoaiDichVu())));
        
        QString trangThai = dv.getConPhucVu() ? "✅ Còn phục vụ" : "❌ Ngừng";
        auto* itemTT = new QTableWidgetItem(trangThai);
        itemTT->setTextAlignment(Qt::AlignCenter);
        if (!dv.getConPhucVu()) {
            itemTT->setForeground(Qt::red);
        }
        table->setItem(row, 6, itemTT);
    }
    
    table->resizeColumnsToContents();
}

void DichVuDialog::clearInputs() {
    txtMa->clear();
    txtTen->clear();
    txtDonVi->setText("Cái");
    spinGia->setValue(0);
    spinTon->setValue(0);
    cboLoai->setCurrentIndex(0);
    txtMa->setEnabled(true);
    btnThem->setEnabled(true);
    btnSua->setEnabled(false);
    btnXoa->setEnabled(false);
    btnNhapHang->setEnabled(false);
    table->clearSelection();
}

void DichVuDialog::onThemMoi() {
    QString ma = txtMa->text().trimmed();
    QString ten = txtTen->text().trimmed();
    QString donVi = txtDonVi->text().trimmed();
    double gia = spinGia->value();
    int ton = spinTon->value();
    QString loai = cboLoai->currentText();
    
    if (ma.isEmpty() || ten.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ mã và tên dịch vụ!");
        return;
    }
    
    if (heThong->themDichVuAPI(ma.toStdString(), ten.toStdString(), donVi.toStdString(),
                               gia, ton, loai.toStdString())) {
        QMessageBox::information(this, "Thành công", "Đã thêm dịch vụ mới!");
        loadDanhSach();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể thêm dịch vụ! Mã có thể đã tồn tại.");
    }
}

void DichVuDialog::onSua() {
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn dịch vụ cần sửa!");
        return;
    }
    
    QString ma = txtMa->text().trimmed();
    QString ten = txtTen->text().trimmed();
    QString donVi = txtDonVi->text().trimmed();
    double gia = spinGia->value();
    int ton = spinTon->value();
    QString loai = cboLoai->currentText();
    
    if (ten.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Tên dịch vụ không được để trống!");
        return;
    }
    
    if (heThong->suaDichVuAPI(ma.toStdString(), ten.toStdString(), donVi.toStdString(),
                              gia, ton, loai.toStdString())) {
        QMessageBox::information(this, "Thành công", "Đã cập nhật dịch vụ!");
        loadDanhSach();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể cập nhật dịch vụ!");
    }
}

void DichVuDialog::onXoa() {
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }
    
    QString ma = txtMa->text().trimmed();
    QString ten = txtTen->text().trimmed();
    
    auto reply = QMessageBox::question(this, "Xác nhận", 
        QString("Bạn có chắc muốn xóa dịch vụ '%1'?").arg(ten),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (heThong->xoaDichVuAPI(ma.toStdString())) {
            QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ!");
            loadDanhSach();
            clearInputs();
        } else {
            QMessageBox::warning(this, "Lỗi", 
                "Không thể xóa! Dịch vụ có thể đang được sử dụng trong đơn hàng.");
        }
    }
}

void DichVuDialog::onNhapHang() {
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn dịch vụ cần nhập hàng!");
        return;
    }
    
    QString ma = txtMa->text().trimmed();
    QString ten = txtTen->text().trimmed();
    
    bool ok;
    int soLuong = QInputDialog::getInt(this, "Nhập hàng", 
        QString("Nhập số lượng cho '%1':").arg(ten), 10, 1, 10000, 1, &ok);
    
    if (ok) {
        if (heThong->nhapHangAPI(ma.toStdString(), soLuong)) {
            QMessageBox::information(this, "Thành công", 
                QString("Đã nhập %1 %2!").arg(soLuong).arg(txtDonVi->text()));
            loadDanhSach();
            onRowSelected(); // Refresh selected row
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể nhập hàng!");
        }
    }
}

void DichVuDialog::onTimKiem() {
    loadDanhSach();
}

void DichVuDialog::onLocLoai(int) {
    loadDanhSach();
}

void DichVuDialog::onRefresh() {
    loadDanhSach();
    clearInputs();
}

void DichVuDialog::onRowSelected() {
    if (table->selectedItems().isEmpty()) {
        clearInputs();
        return;
    }
    
    int row = table->currentRow();
    if (row < 0) return;
    
    txtMa->setText(table->item(row, 0)->text());
    txtTen->setText(table->item(row, 1)->text());
    txtDonVi->setText(table->item(row, 2)->text());
    
    QString giaStr = table->item(row, 3)->text();
    giaStr.remove(" VND").remove(",");
    spinGia->setValue(giaStr.toDouble());
    
    spinTon->setValue(table->item(row, 4)->text().toInt());
    
    QString loai = table->item(row, 5)->text();
    cboLoai->setCurrentText(loai);
    
    txtMa->setEnabled(false);
    btnThem->setEnabled(false);
    btnSua->setEnabled(true);
    btnXoa->setEnabled(true);
    btnNhapHang->setEnabled(true);
}
