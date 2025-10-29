#include "LichSuDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHeaderView>

LichSuDialog::LichSuDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("Lịch sử đặt sân");
    resize(1000, 600);

    auto* layout = new QVBoxLayout(this);

    auto* filterLayout = new QHBoxLayout;
    cboFilter = new QComboBox;
    cboFilter->addItems({"Tất cả", "Đã đặt", "Đã hủy", "Đã thanh toán", "Chưa thanh toán"});
    filterLayout->addWidget(new QLabel("Lọc:"));
    filterLayout->addWidget(cboFilter);
    filterLayout->addStretch();

    tblLich = new QTableWidget;
    tblLich->setColumnCount(8);
    tblLich->setHorizontalHeaderLabels({
        "Mã lịch", "Mã KH", "Mã sân", "Bắt đầu", "Kết thúc",
        "Tiền", "TT Đặt", "TT Thanh toán"
    });
    tblLich->horizontalHeader()->setStretchLastSection(true);
    tblLich->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblLich->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblLich->setColumnWidth(5, 120);

    layout->addLayout(filterLayout);
    layout->addWidget(tblLich);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Close);
    layout->addWidget(btnBox);

    connect(cboFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LichSuDialog::refreshTable);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::accept);

    refreshTable();
}

void LichSuDialog::refreshTable() {
    const auto& ds = heThong->getDanhSachLich();
    tblLich->setRowCount(0);

    QString filter = cboFilter->currentText();

    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        if (filter == "Đã đặt" && ds[i].getTrangThaiDat() != "Da Dat") continue;
        if (filter == "Đã hủy" && ds[i].getTrangThaiDat() != "Da Huy") continue;
        if (filter == "Đã thanh toán" && ds[i].getTrangThaiTT() != "Da Thanh Toan") continue;
        if (filter == "Chưa thanh toán" && ds[i].getTrangThaiTT() != "Chua Thanh Toan") continue;

        int row = tblLich->rowCount();
        tblLich->insertRow(row);

        tblLich->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds[i].getMaLichDat())));
        tblLich->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds[i].getMaKH())));
        tblLich->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(ds[i].getMaSan())));
        tblLich->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(
                                     QuanLyThueSan::formatDateTime(ds[i].getThoiGianBatDau()))));
        tblLich->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(
                                     QuanLyThueSan::formatDateTime(ds[i].getThoiGianKetThuc()))));

        // Tính tổng tiền bao gồm cả dịch vụ
        double tongTien = ds[i].getTongTien();
        double tienDichVu = heThong->tinhTongTienDonAPI(ds[i].getMaLichDat());
        double tongCong = tongTien + tienDichVu;
        
        auto* tienItem = new QTableWidgetItem(QString::number(tongCong, 'f', 0) + " VND");
        tienItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tblLich->setItem(row, 5, tienItem);

        tblLich->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(ds[i].getTrangThaiDat())));
        tblLich->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(ds[i].getTrangThaiTT())));
    }
}
