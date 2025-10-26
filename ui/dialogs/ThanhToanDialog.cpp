#include "ThanhToanDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QHeaderView>

ThanhToanDialog::ThanhToanDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("Thanh toán hóa đơn");
    resize(1000, 600);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("<h3>Các lịch đặt chưa thanh toán</h3>"));

    // Table
    tblLich = new QTableWidget;
    tblLich->setColumnCount(7);
    tblLich->setHorizontalHeaderLabels({
        "Mã lịch", "Mã KH", "Mã sân", "Bắt đầu", "Kết thúc",
        "Tiền", "TT Đặt"
    });
    tblLich->horizontalHeader()->setStretchLastSection(true);
    tblLich->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblLich->setSelectionMode(QAbstractItemView::SingleSelection);
    tblLich->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblLich->setColumnWidth(5, 120);

    layout->addWidget(tblLich, 1);

    // Buttons
    auto* btnLayout = new QHBoxLayout;
    btnThanhToan = new QPushButton("✅ Xác nhận thanh toán");
    btnThanhToan->setStyleSheet("padding: 8px; font-weight: bold; background-color: #4CAF50; color: white;");

    auto* btnClose = new QDialogButtonBox(QDialogButtonBox::Close);

    btnLayout->addWidget(btnThanhToan);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);

    layout->addLayout(btnLayout);

    connect(btnThanhToan, &QPushButton::clicked, this, &ThanhToanDialog::onThanhToan);
    connect(btnClose, &QDialogButtonBox::rejected, this, &QDialog::accept);

    refreshTable();
}

void ThanhToanDialog::refreshTable() {
    const auto& ds = heThong->getDanhSachLich();
    tblLich->setRowCount(0);

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        // Chỉ hiển thị lịch "Đã đặt" và "Chưa thanh toán"
        if (ds[i].getTrangThaiDat() == "Da Dat" && ds[i].getTrangThaiTT() == "Chua Thanh Toan") {
            int row = tblLich->rowCount();
            tblLich->insertRow(row);

            tblLich->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds[i].getMaLichDat())));
            tblLich->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds[i].getMaKH())));
            tblLich->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(ds[i].getMaSan())));
            tblLich->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(
                                         QuanLyThueSan::formatDateTime(ds[i].getThoiGianBatDau()))));
            tblLich->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(
                                         QuanLyThueSan::formatDateTime(ds[i].getThoiGianKetThuc()))));

            auto* tienItem = new QTableWidgetItem(QString::number(ds[i].getTongTien(), 'f', 0) + " VND");
            tienItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            tblLich->setItem(row, 5, tienItem);

            tblLich->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(ds[i].getTrangThaiDat())));
        }
    }

    if (tblLich->rowCount() == 0) {
        tblLich->setRowCount(1);
        auto* item = new QTableWidgetItem("Không có hóa đơn nào chờ thanh toán.");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        tblLich->setItem(0, 0, item);
        tblLich->setSpan(0, 0, 1, 7);
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
    QString tien = tblLich->item(selectedRow, 5)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       QString("Bạn có chắc muốn thanh toán hóa đơn <b>%1</b><br>Số tiền: <b>%2</b>?")
                                           .arg(maLich).arg(tien),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    // Gọi API thanh toán (đã bao gồm xuất file .txt)
    if (heThong->thanhToanAPI(maLich.toStdString())) {
        QMessageBox::information(this, "Thành công",
                                 QString("Đã thanh toán thành công hóa đơn %1.\n✅ Đã xuất file vào thư mục 'hoadon'.").arg(maLich));
        refreshTable(); // Tải lại bảng (hóa đơn sẽ biến mất)
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể thanh toán. Lịch có thể đã bị hủy hoặc đã thanh toán.");
    }
}
