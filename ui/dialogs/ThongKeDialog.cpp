#include "ThongKeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QMap>
#include <QDateTime>

ThongKeDialog::ThongKeDialog(QuanLyThueSan* sys, QWidget* parent)
    : QDialog(parent), heThong(sys) {
    setWindowTitle("📈 Thống kê");
    resize(800, 600);

    auto* layout = new QVBoxLayout(this);

    // ✅ Bộ lọc thời gian
    auto* filterGroup = new QGroupBox("🕐 Khoảng thời gian");
    auto* filterLayout = new QHBoxLayout(filterGroup);

    filterLayout->addWidget(new QLabel("Chọn kỳ:"));
    cboKhoangThoiGian = new QComboBox;
    cboKhoangThoiGian->addItems({
        "Tất cả", "Hôm nay", "Tuần này", "Tháng này", "Quý này", "Năm này", "Tùy chỉnh"
    });
    filterLayout->addWidget(cboKhoangThoiGian);

    filterLayout->addSpacing(20);
    filterLayout->addWidget(new QLabel("Từ ngày:"));
    dateFrom = new QDateEdit(QDate::currentDate().addMonths(-1));
    dateFrom->setDisplayFormat("dd/MM/yyyy");
    dateFrom->setCalendarPopup(true);
    dateFrom->setEnabled(false);
    filterLayout->addWidget(dateFrom);

    filterLayout->addWidget(new QLabel("Đến ngày:"));
    dateTo = new QDateEdit(QDate::currentDate());
    dateTo->setDisplayFormat("dd/MM/yyyy");
    dateTo->setCalendarPopup(true);
    dateTo->setEnabled(false);
    filterLayout->addWidget(dateTo);

    filterLayout->addStretch();
    layout->addWidget(filterGroup);

    // Tabs
    auto* tabWidget = new QTabWidget;
    layout->addWidget(tabWidget);

    // --- Tab Doanh thu ---
    auto* pageDoanhThu = new QWidget;
    auto* layoutDoanhThu = new QVBoxLayout(pageDoanhThu);
    textDoanhThu = new QTextBrowser;
    textDoanhThu->setReadOnly(true);
    layoutDoanhThu->addWidget(textDoanhThu);
    tabWidget->addTab(pageDoanhThu, "💰 Doanh thu");

    // --- Tab Tần suất sân ---
    auto* pageTanSuat = new QWidget;
    auto* layoutTanSuat = new QVBoxLayout(pageTanSuat);
    textTanSuat = new QTextBrowser;
    textTanSuat->setReadOnly(true);
    layoutTanSuat->addWidget(textTanSuat);
    tabWidget->addTab(pageTanSuat, "🏟️ Tần suất sân");

    // --- Nút đóng ---
    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    layout->addWidget(btnBox);

    // Kết nối sự kiện
    connect(cboKhoangThoiGian, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThongKeDialog::onKhoangThoiGianChanged);
    connect(dateFrom, &QDateEdit::dateChanged, this, &ThongKeDialog::onDateChanged);
    connect(dateTo, &QDateEdit::dateChanged, this, &ThongKeDialog::onDateChanged);

    // Khởi tạo thống kê
    capNhatThongKe();
}

void ThongKeDialog::onKhoangThoiGianChanged(int index) {
    bool tuyChinh = (cboKhoangThoiGian->currentText() == "Tùy chỉnh");
    dateFrom->setEnabled(tuyChinh);
    dateTo->setEnabled(tuyChinh);

    if (!tuyChinh) capNhatThongKe();
}

void ThongKeDialog::onDateChanged() {
    if (cboKhoangThoiGian->currentText() == "Tùy chỉnh")
        capNhatThongKe();
}

void ThongKeDialog::capNhatThongKe() {
    tinhDoanhThu(textDoanhThu);
    tinhTanSuat(textTanSuat);
}

// ===========================================================
// =============== HÀM TÍNH DOANH THU ========================
// ===========================================================
void ThongKeDialog::tinhDoanhThu(QTextBrowser* text) {
    const auto& dsLich = heThong->getDanhSachLich();
    const auto& dsSan = heThong->getDanhSachSan();

    if (dsLich.getKichThuoc() == 0) {
        text->setHtml("<h3>Chưa có lịch đặt nào.</h3>");
        return;
    }

    // Xác định khoảng thời gian
    time_t from = 0, to = time(nullptr);
    QString tenKy = cboKhoangThoiGian->currentText();

    if (tenKy == "Hôm nay") {
        QDate today = QDate::currentDate();
        from = QDateTime(today, QTime(0, 0)).toSecsSinceEpoch();
        to = QDateTime(today, QTime(23, 59, 59)).toSecsSinceEpoch();
    } else if (tenKy == "Tuần này") {
        QDate today = QDate::currentDate();
        QDate startWeek = today.addDays(-(today.dayOfWeek() - 1));
        from = QDateTime(startWeek, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Tháng này") {
        QDate today = QDate::currentDate();
        QDate startMonth(today.year(), today.month(), 1);
        from = QDateTime(startMonth, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Quý này") {
        QDate today = QDate::currentDate();
        int thangDauQuy = ((today.month() - 1) / 3) * 3 + 1;
        QDate startQuarter(today.year(), thangDauQuy, 1);
        from = QDateTime(startQuarter, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Năm này") {
        QDate startYear(QDate::currentDate().year(), 1, 1);
        from = QDateTime(startYear, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Tùy chỉnh") {
        from = QDateTime(dateFrom->date(), QTime(0, 0)).toSecsSinceEpoch();
        to = QDateTime(dateTo->date(), QTime(23, 59, 59)).toSecsSinceEpoch();
    }

    // Thống kê
    double tongDoanhThu = 0;
    int soLichDaDat = 0, soLichDaHuy = 0;
    QMap<QString, double> doanhThuTheoSan;

    for (int i = 0; i < dsLich.getKichThuoc(); ++i) {
        const LichDatSan& lich = dsLich[i];

        // Lọc thời gian
        if (tenKy != "Tất cả") {
            time_t tLich = lich.getThoiGianBatDau();
            if (tLich < from || tLich > to) continue;
        }

        if (lich.getTrangThaiDat() == "Da Dat" && lich.getTrangThaiTT() == "Da Thanh Toan") {
            tongDoanhThu += lich.getTongTien();
            soLichDaDat++;
            QString maSan = QString::fromStdString(lich.getMaSan());
            doanhThuTheoSan[maSan] += lich.getTongTien();
        } else if (lich.getTrangThaiDat() == "Da Huy") {
            soLichDaHuy++;
        }
    }

    // Hiển thị
    QString html = QString("<h2>📊 Thống kê doanh thu - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color: #666;'>Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }

    html += QString("<p><b>💰 Tổng doanh thu:</b> %1 VND</p>").arg(QString::number(tongDoanhThu, 'f', 0));
    html += QString("<p>✅ Số lịch đặt thành công: %1</p>").arg(soLichDaDat);
    html += QString("<p>❌ Số lịch đã hủy: %1</p>").arg(soLichDaHuy);

    if (!doanhThuTheoSan.isEmpty()) {
        html += "<br><h3>📋 Doanh thu theo sân</h3>";
        html += "<table border='1' cellspacing='0' cellpadding='6' style='width:100%; border-collapse:collapse;'>";
        html += "<tr style='background-color:#2196F3; color:white;'><th>Mã sân</th><th>Tên sân</th><th>Doanh thu (VND)</th></tr>";

        QMapIterator<QString, double> it(doanhThuTheoSan);
        while (it.hasNext()) {
            it.next();
            QString tenSan = "N/A";
            for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
                if (dsSan[i].getMaSan() == it.key().toStdString()) {
                    tenSan = QString::fromStdString(dsSan[i].getTenSan());
                    break;
                }
            }
            html += QString("<tr><td>%1</td><td>%2</td><td style='text-align:right;'>%3</td></tr>")
                        .arg(it.key())
                        .arg(tenSan)
                        .arg(QString::number(it.value(), 'f', 0));
        }
        html += "</table>";
    }

    text->setHtml(html);
}

// ===========================================================
// =============== HÀM TÍNH TẦN SUẤT SÂN =====================
// ===========================================================
void ThongKeDialog::tinhTanSuat(QTextBrowser* text) {
    const auto& dsSan = heThong->getDanhSachSan();
    const auto& dsLich = heThong->getDanhSachLich();

    if (dsSan.getKichThuoc() == 0) {
        text->setHtml("<h3>Chưa có sân bóng nào.</h3>");
        return;
    }

    // Khoảng thời gian
    time_t from = 0, to = time(nullptr);
    QString tenKy = cboKhoangThoiGian->currentText();

    if (tenKy == "Hôm nay") {
        QDate today = QDate::currentDate();
        from = QDateTime(today, QTime(0, 0)).toSecsSinceEpoch();
        to = QDateTime(today, QTime(23, 59, 59)).toSecsSinceEpoch();
    } else if (tenKy == "Tuần này") {
        QDate today = QDate::currentDate();
        QDate startWeek = today.addDays(-(today.dayOfWeek() - 1));
        from = QDateTime(startWeek, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Tháng này") {
        QDate today = QDate::currentDate();
        QDate startMonth(today.year(), today.month(), 1);
        from = QDateTime(startMonth, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Quý này") {
        QDate today = QDate::currentDate();
        int thangDauQuy = ((today.month() - 1) / 3) * 3 + 1;
        QDate startQuarter(today.year(), thangDauQuy, 1);
        from = QDateTime(startQuarter, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Năm này") {
        QDate startYear(QDate::currentDate().year(), 1, 1);
        from = QDateTime(startYear, QTime(0, 0)).toSecsSinceEpoch();
    } else if (tenKy == "Tùy chỉnh") {
        from = QDateTime(dateFrom->date(), QTime(0, 0)).toSecsSinceEpoch();
        to = QDateTime(dateTo->date(), QTime(23, 59, 59)).toSecsSinceEpoch();
    }

    QString html = QString("<h2>📊 Thống kê tần suất đặt sân - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color:#666;'>Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }

    html += "<table border='1' cellspacing='0' cellpadding='6' style='width:100%; border-collapse:collapse;'>";
    html += "<tr style='background-color:#4CAF50; color:white;'><th>Mã sân</th><th>Tên sân</th><th>Số lần đặt</th><th>Tổng giờ đặt</th></tr>";

    int countSanDat = 0;
    for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
        const SanBong& san = dsSan[i];
        int soLanDat = 0;
        double tongGio = 0.0;

        for (int j = 0; j < dsLich.getKichThuoc(); ++j) {
            const LichDatSan& lich = dsLich[j];
            if (tenKy != "Tất cả") {
                time_t tLich = lich.getThoiGianBatDau();
                if (tLich < from || tLich > to) continue;
            }

            if (lich.getMaSan() == san.getMaSan() && lich.getTrangThaiDat() == "Da Dat") {
                soLanDat++;
                tongGio += difftime(lich.getThoiGianKetThuc(), lich.getThoiGianBatDau()) / 3600.0;
            }
        }

        if (soLanDat > 0) {
            countSanDat++;
            html += QString("<tr><td>%1</td><td>%2</td><td style='text-align:center;'>%3</td><td style='text-align:right;'>%4 giờ</td></tr>")
                        .arg(QString::fromStdString(san.getMaSan()))
                        .arg(QString::fromStdString(san.getTenSan()))
                        .arg(soLanDat)
                        .arg(QString::number(tongGio, 'f', 1));
        }
    }

    html += "</table>";

    if (countSanDat == 0)
        text->setHtml("<h3>Chưa có sân nào được đặt.</h3>");
    else
        text->setHtml(html);
}
