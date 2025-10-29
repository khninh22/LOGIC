#include "ThanhToanDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QTextBrowser>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>

ThanhToanDialog::ThanhToanDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("💳 Thanh toán hóa đơn");
    resize(1200, 700);

    auto* mainLayout = new QHBoxLayout(this);
    
    // LEFT: Danh sách hóa đơn
    auto* leftWidget = new QWidget;
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->addWidget(new QLabel("<h3>💰 Các lịch đặt chưa thanh toán</h3>"));

    // Table
    tblLich = new QTableWidget;
    tblLich->setColumnCount(8);
    tblLich->setHorizontalHeaderLabels({
        "Mã lịch", "Mã KH", "Tên KH", "Mã sân", "Bắt đầu", "Kết thúc",
        "Tổng tiền", "TT Đặt"
    });
    tblLich->horizontalHeader()->setStretchLastSection(true);
    tblLich->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblLich->setSelectionMode(QAbstractItemView::SingleSelection);
    tblLich->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblLich->setColumnWidth(2, 150);
    tblLich->setColumnWidth(6, 120);

    leftLayout->addWidget(tblLich, 1);

    // Buttons
    auto* btnLayout = new QHBoxLayout;
    btnThanhToan = new QPushButton("✅ Xác nhận thanh toán");
    btnThanhToan->setStyleSheet("padding: 10px 20px; font-weight: bold; background-color: #4CAF50; color: white; border-radius: 5px;");
    
    btnInHoaDon = new QPushButton("🖨️ In hóa đơn");
    btnInHoaDon->setStyleSheet("padding: 10px 20px; font-weight: bold; background-color: #2196F3; color: white; border-radius: 5px;");
    btnInHoaDon->setEnabled(false);

    auto* btnClose = new QPushButton("❌ Đóng");
    btnClose->setStyleSheet("padding: 10px 20px;");

    btnLayout->addWidget(btnThanhToan);
    btnLayout->addWidget(btnInHoaDon);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);

    leftLayout->addLayout(btnLayout);
    mainLayout->addWidget(leftWidget, 2);
    
    // RIGHT: Chi tiết hóa đơn
    auto* rightGroup = new QGroupBox("📋 Chi tiết hóa đơn");
    auto* rightLayout = new QVBoxLayout(rightGroup);
    
    txtChiTiet = new QTextBrowser;
    txtChiTiet->setStyleSheet("font-family: 'Courier New', monospace; font-size: 11pt;");
    rightLayout->addWidget(txtChiTiet);
    
    mainLayout->addWidget(rightGroup, 1);

    connect(btnThanhToan, &QPushButton::clicked, this, &ThanhToanDialog::onThanhToan);
    connect(btnInHoaDon, &QPushButton::clicked, this, &ThanhToanDialog::onInHoaDon);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    connect(tblLich, &QTableWidget::itemSelectionChanged, this, &ThanhToanDialog::onRowSelected);

    refreshTable();
}

void ThanhToanDialog::refreshTable() {
    const auto& ds = heThong->getDanhSachLich();
    const auto& dsKH = heThong->getDanhSachKhach();
    tblLich->setRowCount(0);

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        // Chỉ hiển thị lịch "Đã đặt" và "Chưa thanh toán"
        if (ds[i].getTrangThaiDat() == "Da Dat" && ds[i].getTrangThaiTT() == "Chua Thanh Toan") {
            int row = tblLich->rowCount();
            tblLich->insertRow(row);

            tblLich->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds[i].getMaLichDat())));
            tblLich->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds[i].getMaKH())));
            
            // Tìm tên KH
            QString tenKH = "N/A";
            for (int j = 0; j < dsKH.getKichThuoc(); ++j) {
                if (dsKH[j].getMaKH() == ds[i].getMaKH()) {
                    tenKH = QString::fromStdString(dsKH[j].getHoTen());
                    break;
                }
            }
            tblLich->setItem(row, 2, new QTableWidgetItem(tenKH));
            
            tblLich->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(ds[i].getMaSan())));
            tblLich->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(
                                         QuanLyThueSan::formatDateTime(ds[i].getThoiGianBatDau()))));
            tblLich->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(
                                         QuanLyThueSan::formatDateTime(ds[i].getThoiGianKetThuc()))));

            // Tính tổng tiền bao gồm dịch vụ
            double tongTien = ds[i].getTongTien();
            double tienDichVu = heThong->tinhTongTienDonAPI(ds[i].getMaLichDat());
            double tongCong = tongTien + tienDichVu;
            
            auto* tienItem = new QTableWidgetItem(QString::number(tongCong, 'f', 0) + " VND");
            tienItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            tblLich->setItem(row, 6, tienItem);

            tblLich->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(ds[i].getTrangThaiDat())));
        }
    }

    if (tblLich->rowCount() == 0) {
        tblLich->setRowCount(1);
        auto* item = new QTableWidgetItem("Không có hóa đơn nào chờ thanh toán.");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        tblLich->setItem(0, 0, item);
        tblLich->setSpan(0, 0, 1, 8);
        btnThanhToan->setEnabled(false);
    } else {
        btnThanhToan->setEnabled(true);
    }
}

void ThanhToanDialog::onThanhToan() {
    auto selectedItems = tblLich->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một hóa đơn để thanh toán.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    QString maLich = tblLich->item(selectedRow, 0)->text();
    QString tien = tblLich->item(selectedRow, 6)->text();  // Cột 6 là tổng tiền

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       QString("Bạn có chắc muốn thanh toán hóa đơn <b>%1</b><br>Số tiền: <b>%2</b>?")
                                           .arg(maLich).arg(tien),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    // Gọi API thanh toán (đã bao gồm xuất file .txt)
    if (heThong->thanhToanAPI(maLich.toStdString())) {
        QMessageBox::information(this, "Thành công",
                                 QString("Đã thanh toán thành công hóa đơn %1.\n✅ Đã xuất file vào thư mục 'hoadon'.").arg(maLich));
        
        // Lưu dữ liệu tự động
        heThong->luuDuLieu();
        
        refreshTable(); // Tải lại bảng (hóa đơn sẽ biến mất)
        txtChiTiet->clear();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể thanh toán. Lịch có thể đã bị hủy hoặc đã thanh toán.");
    }
}

void ThanhToanDialog::onRowSelected() {
    auto selectedItems = tblLich->selectedItems();
    if (selectedItems.isEmpty() || tblLich->rowCount() == 0) {
        txtChiTiet->clear();
        btnInHoaDon->setEnabled(false);
        return;
    }
    
    int row = selectedItems.first()->row();
    if (tblLich->item(row, 0)->flags() & Qt::ItemIsSelectable) {
        QString maLich = tblLich->item(row, 0)->text();
        hienThiChiTiet(maLich.toStdString());
        btnInHoaDon->setEnabled(true);
    }
}

void ThanhToanDialog::hienThiChiTiet(const std::string& maLich) {
    const auto& dsLich = heThong->getDanhSachLich();
    const LichDatSan* lich = nullptr;
    
    for (int i = 0; i < dsLich.getKichThuoc(); ++i) {
        if (dsLich[i].getMaLichDat() == maLich) {
            lich = &dsLich[i];
            break;
        }
    }
    
    if (!lich) return;
    
    // Tìm thông tin khách hàng
    const auto& dsKH = heThong->getDanhSachKhach();
    QString tenKH = "N/A", sdtKH = "N/A";
    for (int i = 0; i < dsKH.getKichThuoc(); ++i) {
        if (dsKH[i].getMaKH() == lich->getMaKH()) {
            tenKH = QString::fromStdString(dsKH[i].getHoTen());
            sdtKH = QString::fromStdString(dsKH[i].getSdt());
            break;
        }
    }
    
    // Tìm thông tin sân
    const auto& dsSan = heThong->getDanhSachSan();
    QString tenSan = "N/A";
    for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
        if (dsSan[i].getMaSan() == lich->getMaSan()) {
            tenSan = QString::fromStdString(dsSan[i].getTenSan());
            break;
        }
    }
    
    // Lấy đơn dịch vụ (nếu có)
    auto dsDon = heThong->layDonTheoLichAPI(maLich);
    double tongTienDichVu = 0;
    
    QString html = QString(
        "<html><body style='font-family: Arial;'>"
        "<div style='text-align: center; border-bottom: 2px solid #333; padding-bottom: 10px;'>"
        "<h2 style='margin: 5px;'>🏟️ HÓA ĐƠN ĐẶT SÂN</h2>"
        "<p style='margin: 0;'><i>Hệ thống quản lý thuê sân bóng</i></p>"
        "</div>"
        "<br>"
        "<table style='width: 100%%; border-collapse: collapse;'>"
        "<tr><td style='width: 40%%;'><b>Mã hóa đơn:</b></td><td>%1</td></tr>"
        "<tr><td><b>Ngày lập:</b></td><td>%2</td></tr>"
        "<tr><td><b>Khách hàng:</b></td><td>%3</td></tr>"
        "<tr><td><b>Số điện thoại:</b></td><td>%4</td></tr>"
        "</table>"
        "<hr>"
        "<h3 style='background-color: #2196F3; color: white; padding: 8px;'>📋 THÔNG TIN ĐẶT SÂN</h3>"
        "<table style='width: 100%%;'>"
        "<tr><td style='width: 40%%;'><b>Sân:</b></td><td>%5 - %6</td></tr>"
        "<tr><td><b>Thời gian:</b></td><td>%7</td></tr>"
        "<tr><td><b>Đến:</b></td><td>%8</td></tr>"
        "<tr><td><b>Tiền sân:</b></td><td style='color: #2196F3; font-weight: bold;'>%9 VND</td></tr>"
        "</table>"
    ).arg(QString::fromStdString(maLich))
     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
     .arg(tenKH)
     .arg(sdtKH)
     .arg(QString::fromStdString(lich->getMaSan()))
     .arg(tenSan)
     .arg(QString::fromStdString(QuanLyThueSan::formatDateTime(lich->getThoiGianBatDau())))
     .arg(QString::fromStdString(QuanLyThueSan::formatDateTime(lich->getThoiGianKetThuc())))
     .arg(QString::number((qlonglong)lich->getTongTien()));
    
    // Thêm phần dịch vụ nếu có
    if (dsDon.getKichThuoc() > 0) {
        html += "<hr><h3 style='background-color: #FF9800; color: white; padding: 8px;'>🍔 DỊCH VỤ ĐÃ ĐẶT</h3>";
        html += "<table style='width: 100%%; border: 1px solid #ddd;'>";
        html += "<tr style='background-color: #f0f0f0;'>"
                "<th style='border: 1px solid #ddd; padding: 8px;'>Tên dịch vụ</th>"
                "<th style='border: 1px solid #ddd; padding: 8px;'>Đơn giá</th>"
                "<th style='border: 1px solid #ddd; padding: 8px;'>SL</th>"
                "<th style='border: 1px solid #ddd; padding: 8px;'>Thành tiền</th>"
                "</tr>";
        
        for (int i = 0; i < dsDon.getKichThuoc(); ++i) {
            const auto& chiTiet = dsDon[i].getChiTiet();
            for (int j = 0; j < chiTiet.getKichThuoc(); ++j) {
                html += QString("<tr>"
                    "<td style='border: 1px solid #ddd; padding: 5px;'>%1</td>"
                    "<td style='border: 1px solid #ddd; padding: 5px; text-align: right;'>%2</td>"
                    "<td style='border: 1px solid #ddd; padding: 5px; text-align: center;'>%3</td>"
                    "<td style='border: 1px solid #ddd; padding: 5px; text-align: right;'><b>%4</b></td>"
                    "</tr>")
                    .arg(QString::fromStdString(chiTiet[j].tenDichVu))
                    .arg(QString::number((qlonglong)chiTiet[j].donGia))
                    .arg(chiTiet[j].soLuong)
                    .arg(QString::number((qlonglong)chiTiet[j].thanhTien));
                tongTienDichVu += chiTiet[j].thanhTien;
            }
        }
        
        html += QString("<tr style='background-color: #FFF3E0;'>"
                "<td colspan='3' style='border: 1px solid #ddd; padding: 8px; text-align: right;'><b>Tổng tiền dịch vụ:</b></td>"
                "<td style='border: 1px solid #ddd; padding: 8px; text-align: right; color: #FF9800; font-weight: bold;'>%1 VND</td>"
                "</tr></table>")
            .arg(QString::number((qlonglong)tongTienDichVu));
    }
    
    // Tổng cộng
    double tongCong = lich->getTongTien() + tongTienDichVu;
    html += QString(
        "<hr>"
        "<div style='text-align: right; font-size: 16pt;'>"
        "<p style='margin: 5px;'><b>TỔNG CỘNG:</b> "
        "<span style='color: #4CAF50; font-size: 20pt;'>%1 VND</span></p>"
        "</div>"
        "<hr>"
        "<p style='text-align: center; color: #666;'>"
        "<i>⚠️ Vui lòng kiểm tra kỹ thông tin trước khi thanh toán</i><br>"
        "Cảm ơn quý khách đã sử dụng dịch vụ!"
        "</p>"
        "</body></html>"
    ).arg(QString::number((qlonglong)tongCong));
    
    txtChiTiet->setHtml(html);
}

void ThanhToanDialog::onInHoaDon() {
    auto selectedItems = tblLich->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một hóa đơn.");
        return;
    }
    
    int row = selectedItems.first()->row();
    QString maLich = tblLich->item(row, 0)->text();
    
    // In bằng QPrinter
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);
    
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument doc;
        doc.setHtml(txtChiTiet->toHtml());
        doc.print(&printer);
        QMessageBox::information(this, "Thành công", "Đã in hóa đơn!");
    }
}
