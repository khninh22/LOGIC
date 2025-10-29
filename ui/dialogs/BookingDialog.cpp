#include "BookingDialog.h"
#include "AddCustomerDialog.h"
#include "../../core/services/TinhTienService.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QTableWidget>
#include <QFrame>

BookingDialog::BookingDialog(QuanLyThueSan* sys, const QString& maSan, const QString& tenSan,
                             const QDate& date, const QTime& start, const QTime& end,
                             QWidget* parent)
    : QDialog(parent), heThong(sys), maSan(maSan), ngay(date), gioBD(start), gioKT(end),
      tienSan(0), tienDichVu(0) {

    setWindowTitle("🏟️ Đặt sân và dịch vụ");
    setModal(true);
    resize(700, 650);

    auto* layout = new QVBoxLayout(this);

    // Thông tin sân
    auto* grpInfo = new QGroupBox("📋 Thông tin đặt sân");
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
    auto* grpKH = new QGroupBox("👤 Thông tin khách hàng");
    auto* khLayout = new QGridLayout(grpKH);

    cboKhach = new QComboBox;
    khLayout->addWidget(new QLabel("Khách hàng:"), 0, 0);
    khLayout->addWidget(cboKhach, 0, 1);

    layout->addWidget(grpKH);
    
    // ===== PHẦN DỊCH VỤ MỚI =====
    auto* grpDichVu = new QGroupBox("🍔 Đặt dịch vụ kèm (tùy chọn)");
    auto* dvLayout = new QVBoxLayout(grpDichVu);
    
    chkDichVu = new QCheckBox("Có đặt dịch vụ kèm");
    dvLayout->addWidget(chkDichVu);
    
    auto* dvWidget = new QWidget;
    auto* dvFormLayout = new QHBoxLayout(dvWidget);
    dvFormLayout->setContentsMargins(0, 0, 0, 0);
    
    dvFormLayout->addWidget(new QLabel("Dịch vụ:"));
    cboDichVu = new QComboBox;
    cboDichVu->setMinimumWidth(200);
    cboDichVu->setEnabled(false);
    dvFormLayout->addWidget(cboDichVu, 1);
    
    dvFormLayout->addWidget(new QLabel("Số lượng:"));
    spinSoLuong = new QSpinBox;
    spinSoLuong->setRange(1, 100);
    spinSoLuong->setValue(1);
    spinSoLuong->setEnabled(false);
    dvFormLayout->addWidget(spinSoLuong);
    
    btnThemDV = new QPushButton("➕ Thêm");
    btnThemDV->setEnabled(false);
    btnThemDV->setStyleSheet("QPushButton { padding: 5px 15px; background-color: #4CAF50; color: white; font-weight: bold; border-radius: 3px; }");
    dvFormLayout->addWidget(btnThemDV);
    
    dvLayout->addWidget(dvWidget);
    
    // Bảng dịch vụ đã chọn
    tblDichVu = new QTableWidget;
    tblDichVu->setColumnCount(5);
    tblDichVu->setHorizontalHeaderLabels({"Mã", "Tên dịch vụ", "Đơn giá", "SL", "Thành tiền"});
    tblDichVu->horizontalHeader()->setStretchLastSection(true);
    tblDichVu->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblDichVu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblDichVu->setMaximumHeight(150);
    tblDichVu->setEnabled(false);
    dvLayout->addWidget(tblDichVu);
    
    btnXoaDV = new QPushButton("🗑️ Xóa dịch vụ đã chọn");
    btnXoaDV->setEnabled(false);
    btnXoaDV->setStyleSheet("QPushButton { padding: 5px; background-color: #f44336; color: white; }");
    dvLayout->addWidget(btnXoaDV);
    
    layout->addWidget(grpDichVu);

    // Tổng tiền
    auto* grpTien = new QGroupBox("💰 Tổng tiền");
    auto* tienLayout = new QGridLayout(grpTien);
    
    tienLayout->addWidget(new QLabel("Tiền sân:"), 0, 0);
    lblTienSan = new QLabel("0 VND");
    lblTienSan->setStyleSheet("font-weight: bold; color: #2196F3;");
    tienLayout->addWidget(lblTienSan, 0, 1, Qt::AlignRight);
    
    tienLayout->addWidget(new QLabel("Tiền dịch vụ:"), 1, 0);
    lblTienDichVu = new QLabel("0 VND");
    lblTienDichVu->setStyleSheet("font-weight: bold; color: #FF9800;");
    tienLayout->addWidget(lblTienDichVu, 1, 1, Qt::AlignRight);
    
    auto* separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    tienLayout->addWidget(separator, 2, 0, 1, 2);
    
    tienLayout->addWidget(new QLabel("<b>TỔNG CỘNG:</b>"), 3, 0);
    lblTongTien = new QLabel("0 VND");
    lblTongTien->setStyleSheet("font-weight: bold; font-size: 16px; color: #4CAF50;");
    tienLayout->addWidget(lblTongTien, 3, 1, Qt::AlignRight);
    
    layout->addWidget(grpTien);

    // Buttons
    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btnBox->button(QDialogButtonBox::Ok)->setText("✅ Xác nhận đặt");
    btnBox->button(QDialogButtonBox::Cancel)->setText("❌ Hủy");
    layout->addWidget(btnBox);

    // Signals
    connect(btnBox, &QDialogButtonBox::accepted, this, &BookingDialog::onConfirm);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(cboKhach, QOverload<int>::of(&QComboBox::activated),
            this, &BookingDialog::onKhachHangSelected);
    connect(chkDichVu, &QCheckBox::toggled, this, &BookingDialog::onDichVuToggled);
    connect(btnThemDV, &QPushButton::clicked, this, &BookingDialog::onThemDichVu);
    connect(btnXoaDV, &QPushButton::clicked, this, &BookingDialog::onXoaDichVu);

    loadKhachHangComboBox();
    loadDichVu();
    updateTongTien();
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

void BookingDialog::loadDichVu() {
    cboDichVu->clear();
    
    const auto& ds = heThong->getDanhSachDichVu();
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        const DichVu& dv = ds[i];
        if (dv.getConPhucVu() && dv.getSoLuongTon() > 0) {
            QString text = QString("%1 - %2 (%3 VND)")
                .arg(QString::fromStdString(dv.getMaDichVu()))
                .arg(QString::fromStdString(dv.getTenDichVu()))
                .arg(QString::number(dv.getDonGia(), 'f', 0));
            cboDichVu->addItem(text, QString::fromStdString(dv.getMaDichVu()));
        }
    }
}

void BookingDialog::updateTongTien() {
    // Tính tiền sân
    QString dtStr = ngay.toString("dd/MM/yyyy") + " " + gioBD.toString("HH:mm");
    time_t tBD = QuanLyThueSan::parseDateTime(dtStr.toStdString());
    time_t tKT = tBD + gioBD.secsTo(gioKT);
    
    const auto& dsSan = heThong->getDanhSachSan();
    for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
        if (dsSan[i].getMaSan() == maSan.toStdString()) {
            tienSan = TinhTienService::tinhTienTheoKhoang(dsSan[i], tBD, tKT);
            break;
        }
    }
    
    // Tính tiền dịch vụ
    tienDichVu = 0;
    for (int row = 0; row < tblDichVu->rowCount(); ++row) {
        QString thanhTienStr = tblDichVu->item(row, 4)->text();
        thanhTienStr.remove(" VND").remove(",");
        tienDichVu += thanhTienStr.toDouble();
    }
    
    // Hiển thị
    lblTienSan->setText(QString::number((qlonglong)tienSan) + " VND");
    lblTienDichVu->setText(QString::number((qlonglong)tienDichVu) + " VND");
    lblTongTien->setText(QString::number((qlonglong)(tienSan + tienDichVu)) + " VND");
}

void BookingDialog::onDichVuToggled(bool checked) {
    cboDichVu->setEnabled(checked);
    spinSoLuong->setEnabled(checked);
    btnThemDV->setEnabled(checked);
    tblDichVu->setEnabled(checked);
    btnXoaDV->setEnabled(checked && tblDichVu->rowCount() > 0);
}

void BookingDialog::onThemDichVu() {
    if (cboDichVu->count() == 0) {
        QMessageBox::warning(this, "Lỗi", "Không có dịch vụ nào!");
        return;
    }
    
    QString maDV = cboDichVu->currentData().toString();
    int soLuong = spinSoLuong->value();
    
    // Tìm dịch vụ
    const auto& ds = heThong->getDanhSachDichVu();
    const DichVu* dv = nullptr;
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        if (ds[i].getMaDichVu() == maDV.toStdString()) {
            dv = &ds[i];
            break;
        }
    }
    
    if (!dv) return;
    
    // Kiểm tra tồn kho
    if (dv->getSoLuongTon() < soLuong) {
        QMessageBox::warning(this, "Lỗi", 
            QString("Không đủ tồn kho! Chỉ còn %1 %2")
            .arg(dv->getSoLuongTon())
            .arg(QString::fromStdString(dv->getDonVi())));
        return;
    }
    
    // Kiểm tra đã có trong bảng chưa
    for (int row = 0; row < tblDichVu->rowCount(); ++row) {
        if (tblDichVu->item(row, 0)->text() == maDV) {
            QMessageBox::warning(this, "Lỗi", "Dịch vụ này đã được thêm!");
            return;
        }
    }
    
    // Thêm vào bảng
    int row = tblDichVu->rowCount();
    tblDichVu->insertRow(row);
    
    tblDichVu->setItem(row, 0, new QTableWidgetItem(maDV));
    tblDichVu->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dv->getTenDichVu())));
    tblDichVu->setItem(row, 2, new QTableWidgetItem(QString::number(dv->getDonGia(), 'f', 0) + " VND"));
    tblDichVu->setItem(row, 3, new QTableWidgetItem(QString::number(soLuong)));
    
    double thanhTien = dv->getDonGia() * soLuong;
    tblDichVu->setItem(row, 4, new QTableWidgetItem(QString::number((qlonglong)thanhTien) + " VND"));
    
    tblDichVu->resizeColumnsToContents();
    updateTongTien();
    btnXoaDV->setEnabled(true);
}

void BookingDialog::onXoaDichVu() {
    if (tblDichVu->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }
    
    int row = tblDichVu->currentRow();
    tblDichVu->removeRow(row);
    updateTongTien();
    
    if (tblDichVu->rowCount() == 0) {
        btnXoaDV->setEnabled(false);
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

    // Đặt dịch vụ kèm (nếu có)
    std::string maDon;
    bool coDichVu = chkDichVu->isChecked() && tblDichVu->rowCount() > 0;
    
    if (coDichVu) {
        // Tạo đơn dịch vụ
        if (heThong->taoDonDichVuAPI(maLD, maKH, maDon)) {
            // Thêm từng dịch vụ vào đơn
            bool success = true;
            for (int row = 0; row < tblDichVu->rowCount(); ++row) {
                QString maDV = tblDichVu->item(row, 0)->text();
                int soLuong = tblDichVu->item(row, 3)->text().toInt();
                
                if (!heThong->themDichVuVaoDonAPI(maDon, maDV.toStdString(), soLuong)) {
                    success = false;
                    break;
                }
            }
            
            if (!success) {
                QMessageBox::warning(this, "Cảnh báo", 
                    "Đặt sân thành công nhưng có lỗi khi đặt một số dịch vụ!");
            }
        }
    }

    // Thành công
    QString msg = QString(
                      "✅ <b>Đặt sân thành công!</b><br><br>"
                      "📋 Mã lịch: <b>%1</b><br>"
                      "🏟️ Sân: <b>%2</b><br>"
                      "📅 Ngày: <b>%3</b><br>"
                      "🕐 Giờ: <b>%4 - %5</b><br>"
                      "💰 Tiền sân: <b>%6 VND</b><br>"
                      ).arg(QString::fromStdString(maLD))
                      .arg(maSan)
                      .arg(ngay.toString("dd/MM/yyyy"))
                      .arg(gioBD.toString("HH:mm"))
                      .arg(gioKT.toString("HH:mm"))
                      .arg(QString::number((qlonglong)tien));
    
    if (coDichVu && !maDon.empty()) {
        msg += QString(
                   "🍔 Tiền dịch vụ: <b>%1 VND</b><br>"
                   "💵 <b>TỔNG CỘNG: %2 VND</b><br><br>"
                   ).arg(QString::number((qlonglong)tienDichVu))
                   .arg(QString::number((qlonglong)(tien + tienDichVu)));
    }
    
    msg += "<br>⚠️ <i>Vui lòng thanh toán trước khi sử dụng sân.</i>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Đặt sân thành công");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    // Lưu dữ liệu tự động
    heThong->luuDuLieu();

    accept();
}
