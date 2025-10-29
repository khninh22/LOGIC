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
    setWindowTitle("📈 Thống kê & Báo cáo");
    resize(1000, 700);

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

    // --- Tab Tổng quan ---
    auto* pageTongQuan = new QWidget;
    auto* layoutTongQuan = new QVBoxLayout(pageTongQuan);
    textTongQuan = new QTextBrowser;
    textTongQuan->setReadOnly(true);
    layoutTongQuan->addWidget(textTongQuan);
    tabWidget->addTab(pageTongQuan, "📊 Tổng quan");

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

    // --- Tab Khách hàng ---
    auto* pageKhachHang = new QWidget;
    auto* layoutKhachHang = new QVBoxLayout(pageKhachHang);
    textKhachHang = new QTextBrowser;
    textKhachHang->setReadOnly(true);
    layoutKhachHang->addWidget(textKhachHang);
    tabWidget->addTab(pageKhachHang, "👥 Khách hàng");

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
    tinhTongQuan(textTongQuan);
    tinhDoanhThu(textDoanhThu);
    tinhTanSuat(textTanSuat);
    tinhKhachHang(textKhachHang);
}

// ===========================================================
// =============== HÀM TÍNH TỔNG QUAN ========================
// ===========================================================
void ThongKeDialog::tinhTongQuan(QTextBrowser* text) {
    const auto& dsLich = heThong->getDanhSachLich();
    const auto& dsSan = heThong->getDanhSachSan();
    const auto& dsKH = heThong->getDanhSachKhach();
    const auto& dsDV = heThong->getDanhSachDichVu();

    // Xác định khoảng thời gian
    time_t from = 0, to = time(nullptr);
    QString tenKy = cboKhoangThoiGian->currentText();
    xacDinhKhoangThoiGian(from, to, tenKy);

    // Thống kê tổng quan
    int tongLichDat = 0, tongLichDaHuy = 0, tongLichThanhToan = 0, tongLichChuaThanhToan = 0;
    double tongDoanhThu = 0, tongTienSan = 0, tongTienDichVu = 0;

    for (int i = 0; i < dsLich.getKichThuoc(); ++i) {
        const LichDatSan& lich = dsLich[i];
        
        // Lọc thời gian
        if (tenKy != "Tất cả") {
            time_t tLich = lich.getThoiGianBatDau();
            if (tLich < from || tLich > to) continue;
        }

        if (lich.getTrangThaiDat() == "Da Dat") {
            tongLichDat++;
            double tienDV = heThong->tinhTongTienDonAPI(lich.getMaLichDat());
            
            if (lich.getTrangThaiTT() == "Da Thanh Toan") {
                tongLichThanhToan++;
                tongTienSan += lich.getTongTien();
                tongTienDichVu += tienDV;
                tongDoanhThu += (lich.getTongTien() + tienDV);
            } else {
                tongLichChuaThanhToan++;
            }
        } else if (lich.getTrangThaiDat() == "Da Huy") {
            tongLichDaHuy++;
        }
    }

    // Hiển thị HTML
    QString html = QString("<h2 style='color:#1976D2;'>📊 BÁO CÁO TỔNG QUAN - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color: #666; font-size:14px;'>📅 Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }
    html += "<hr style='border:1px solid #ddd;'>";

    // Card-style statistics
    html += "<div style='display:flex; flex-wrap:wrap; gap:15px; margin:20px 0;'>";
    
    // Card 1: Doanh thu
    html += QString("<div style='flex:1; min-width:200px; background:#4CAF50; color:white; padding:20px; border-radius:8px;'>"
                   "<div style='font-size:16px; margin-bottom:5px;'>💰 Tổng doanh thu (VND)</div>"
                   "<div style='font-size:32px; font-weight:bold;'>%1</div>"
                   "</div>").arg(QString::number(tongDoanhThu, 'f', 0));

    // Card 2: Lịch đặt
    html += QString("<div style='flex:1; min-width:200px; background:#2196F3; color:white; padding:20px; border-radius:8px;'>"
                   "<div style='font-size:16px; margin-bottom:5px;'>📅 Lịch đã đặt</div>"
                   "<div style='font-size:32px; font-weight:bold;'>%1</div>"
                   "</div>").arg(tongLichDat);

    // Card 3: Đã thanh toán
    html += QString("<div style='flex:1; min-width:200px; background:#FF9800; color:white; padding:20px; border-radius:8px;'>"
                   "<div style='font-size:16px; margin-bottom:5px;'>✅ Đã thanh toán</div>"
                   "<div style='font-size:32px; font-weight:bold;'>%1</div>"
                   "</div>").arg(tongLichThanhToan);

    // Card 4: Chưa thanh toán
    html += QString("<div style='flex:1; min-width:200px; background:#9C27B0; color:white; padding:20px; border-radius:8px;'>"
                   "<div style='font-size:16px; margin-bottom:5px;'>⏳ Chưa thanh toán</div>"
                   "<div style='font-size:32px; font-weight:bold;'>%1</div>"
                   "</div>").arg(tongLichChuaThanhToan);

    html += "</div>";

    // Chi tiết doanh thu
    html += "<div style='background:#f5f5f5; padding:15px; border-radius:5px; margin:15px 0;'>";
    html += "<h3 style='margin-top:0; color:#1976D2;'>💵 Chi tiết doanh thu</h3>";
    html += QString("<p><b>🏟️ Tiền thuê sân:</b> %1 VND (%2%)</p>")
                .arg(QString::number(tongTienSan, 'f', 0))
                .arg(tongDoanhThu > 0 ? QString::number(tongTienSan * 100 / tongDoanhThu, 'f', 1) : "0");
    html += QString("<p><b>🍔 Tiền dịch vụ:</b> %1 VND (%2%)</p>")
                .arg(QString::number(tongTienDichVu, 'f', 0))
                .arg(tongDoanhThu > 0 ? QString::number(tongTienDichVu * 100 / tongDoanhThu, 'f', 1) : "0");
    html += "</div>";

    // Thống kê lịch
    html += "<div style='background:#f5f5f5; padding:15px; border-radius:5px; margin:15px 0;'>";
    html += "<h3 style='margin-top:0; color:#1976D2;'>📋 Thống kê lịch đặt</h3>";
    int tongLich = tongLichDat + tongLichDaHuy;
    html += QString("<p><b>📊 Tổng số lịch:</b> %1</p>").arg(tongLich);
    html += QString("<p><b>✅ Đã đặt:</b> %1 (%2%)</p>")
                .arg(tongLichDat)
                .arg(tongLich > 0 ? QString::number(tongLichDat * 100.0 / tongLich, 'f', 1) : "0");
    html += QString("<p><b>❌ Đã hủy:</b> %1 (%2%)</p>")
                .arg(tongLichDaHuy)
                .arg(tongLich > 0 ? QString::number(tongLichDaHuy * 100.0 / tongLich, 'f', 1) : "0");
    html += QString("<p><b>💳 Tỷ lệ thanh toán:</b> %1% (%2/%3)</p>")
                .arg(tongLichDat > 0 ? QString::number(tongLichThanhToan * 100.0 / tongLichDat, 'f', 1) : "0")
                .arg(tongLichThanhToan)
                .arg(tongLichDat);
    html += "</div>";

    // Thông tin hệ thống
    html += "<div style='background:#e3f2fd; padding:15px; border-radius:5px; margin:15px 0;'>";
    html += "<h3 style='margin-top:0; color:#1976D2;'>ℹ️ Thông tin hệ thống</h3>";
    html += QString("<p><b>🏟️ Tổng số sân:</b> %1</p>").arg(dsSan.getKichThuoc());
    html += QString("<p><b>👥 Tổng khách hàng:</b> %1</p>").arg(dsKH.getKichThuoc());
    html += QString("<p><b>🍔 Tổng dịch vụ:</b> %1</p>").arg(dsDV.getKichThuoc());
    html += "</div>";

    text->setHtml(html);
}

// ===========================================================
// =============== HÀM XÁC ĐỊNH KHOẢNG THỜI GIAN =============
// ===========================================================
void ThongKeDialog::xacDinhKhoangThoiGian(time_t& from, time_t& to, const QString& tenKy) {
    from = 0;
    to = time(nullptr);
    
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
    time_t from, to;
    QString tenKy = cboKhoangThoiGian->currentText();
    xacDinhKhoangThoiGian(from, to, tenKy);

    // Thống kê
    double tongDoanhThu = 0, tongTienSan = 0, tongTienDichVu = 0;
    int soLichDaDat = 0, soLichDaHuy = 0;
    QMap<QString, double> doanhThuTheoSan;
    QMap<QString, double> tienSanTheoSan;
    QMap<QString, double> tienDVTheoSan;

    for (int i = 0; i < dsLich.getKichThuoc(); ++i) {
        const LichDatSan& lich = dsLich[i];

        // Lọc thời gian
        if (tenKy != "Tất cả") {
            time_t tLich = lich.getThoiGianBatDau();
            if (tLich < from || tLich > to) continue;
        }

        if (lich.getTrangThaiDat() == "Da Dat" && lich.getTrangThaiTT() == "Da Thanh Toan") {
            double tienSan = lich.getTongTien();
            double tienDV = heThong->tinhTongTienDonAPI(lich.getMaLichDat());
            double tong = tienSan + tienDV;
            
            tongTienSan += tienSan;
            tongTienDichVu += tienDV;
            tongDoanhThu += tong;
            soLichDaDat++;
            
            QString maSan = QString::fromStdString(lich.getMaSan());
            doanhThuTheoSan[maSan] += tong;
            tienSanTheoSan[maSan] += tienSan;
            tienDVTheoSan[maSan] += tienDV;
        } else if (lich.getTrangThaiDat() == "Da Huy") {
            soLichDaHuy++;
        }
    }

    // Hiển thị
    QString html = QString("<h2 style='color:#4CAF50;'>� THỐNG KÊ DOANH THU - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color: #666;'>📅 Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }
    html += "<hr>";

    html += QString("<div style='background:#e8f5e9; padding:20px; border-radius:8px; margin:15px 0;'>"
                   "<h3 style='margin-top:0; color:#2E7D32;'>📈 Tổng kết</h3>"
                   "<p style='font-size:18px;'><b>💵 Tổng doanh thu:</b> <span style='color:#4CAF50; font-size:24px; font-weight:bold;'>%1 VND</span></p>"
                   "<p><b>🏟️ Tiền thuê sân:</b> %2 VND</p>"
                   "<p><b>🍔 Tiền dịch vụ:</b> %3 VND</p>"
                   "<p><b>✅ Số lịch đã thanh toán:</b> %4</p>"
                   "<p><b>❌ Số lịch đã hủy:</b> %5</p>"
                   "</div>")
        .arg(QString::number(tongDoanhThu, 'f', 0))
        .arg(QString::number(tongTienSan, 'f', 0))
        .arg(QString::number(tongTienDichVu, 'f', 0))
        .arg(soLichDaDat)
        .arg(soLichDaHuy);

    if (!doanhThuTheoSan.isEmpty()) {
        html += "<h3 style='color:#1976D2;'>📋 Chi tiết doanh thu theo sân</h3>";
        html += "<table border='1' cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse;'>";
        html += "<tr style='background-color:#1976D2; color:white;'>"
                "<th>Mã sân</th><th>Tên sân</th>"
                "<th>Tiền sân (VND)</th><th>Tiền DV (VND)</th>"
                "<th>Tổng (VND)</th><th>% Doanh thu</th></tr>";

        QMapIterator<QString, double> it(doanhThuTheoSan);
        while (it.hasNext()) {
            it.next();
            QString maSan = it.key();
            QString tenSan = "N/A";
            for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
                if (dsSan[i].getMaSan() == maSan.toStdString()) {
                    tenSan = QString::fromStdString(dsSan[i].getTenSan());
                    break;
                }
            }
            double phanTram = (tongDoanhThu > 0) ? (it.value() * 100.0 / tongDoanhThu) : 0;
            html += QString("<tr>"
                           "<td>%1</td><td>%2</td>"
                           "<td style='text-align:right;'>%3</td>"
                           "<td style='text-align:right;'>%4</td>"
                           "<td style='text-align:right; font-weight:bold;'>%5</td>"
                           "<td style='text-align:center;'>%6%</td>"
                           "</tr>")
                        .arg(maSan)
                        .arg(tenSan)
                        .arg(QString::number(tienSanTheoSan[maSan], 'f', 0))
                        .arg(QString::number(tienDVTheoSan[maSan], 'f', 0))
                        .arg(QString::number(it.value(), 'f', 0))
                        .arg(QString::number(phanTram, 'f', 1));
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
    time_t from, to;
    QString tenKy = cboKhoangThoiGian->currentText();
    xacDinhKhoangThoiGian(from, to, tenKy);

    QString html = QString("<h2 style='color:#FF9800;'>🏟️ THỐNG KÊ TẦN SUẤT ĐẶT SÂN - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color:#666;'>📅 Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }
    html += "<hr>";

    html += "<table border='1' cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse;'>";
    html += "<tr style='background-color:#FF9800; color:white;'>"
            "<th>Mã sân</th><th>Tên sân</th>"
            "<th>Lượt đặt</th><th>Lượt hủy</th>"
            "<th>Tổng giờ đặt</th><th>Tỷ lệ hủy</th></tr>";

    int countSanDat = 0;
    for (int i = 0; i < dsSan.getKichThuoc(); ++i) {
        const SanBong& san = dsSan[i];
        int soLanDat = 0, soLanHuy = 0;
        double tongGio = 0.0;

        for (int j = 0; j < dsLich.getKichThuoc(); ++j) {
            const LichDatSan& lich = dsLich[j];
            if (tenKy != "Tất cả") {
                time_t tLich = lich.getThoiGianBatDau();
                if (tLich < from || tLich > to) continue;
            }

            if (lich.getMaSan() == san.getMaSan()) {
                if (lich.getTrangThaiDat() == "Da Dat") {
                    soLanDat++;
                    tongGio += difftime(lich.getThoiGianKetThuc(), lich.getThoiGianBatDau()) / 3600.0;
                } else if (lich.getTrangThaiDat() == "Da Huy") {
                    soLanHuy++;
                }
            }
        }

        int tongLuot = soLanDat + soLanHuy;
        if (tongLuot > 0) {
            countSanDat++;
            double tyLeHuy = (tongLuot > 0) ? (soLanHuy * 100.0 / tongLuot) : 0;
            QString rowColor = (tyLeHuy > 30) ? "background-color:#ffebee;" : "";
            
            html += QString("<tr style='%1'>"
                           "<td>%2</td><td>%3</td>"
                           "<td style='text-align:center; font-weight:bold; color:#4CAF50;'>%4</td>"
                           "<td style='text-align:center; font-weight:bold; color:#f44336;'>%5</td>"
                           "<td style='text-align:right;'>%6 giờ</td>"
                           "<td style='text-align:center;'>%7%</td>"
                           "</tr>")
                        .arg(rowColor)
                        .arg(QString::fromStdString(san.getMaSan()))
                        .arg(QString::fromStdString(san.getTenSan()))
                        .arg(soLanDat)
                        .arg(soLanHuy)
                        .arg(QString::number(tongGio, 'f', 1))
                        .arg(QString::number(tyLeHuy, 'f', 1));
        }
    }

    html += "</table>";

    if (countSanDat == 0)
        html += "<p style='color:#999; text-align:center; margin-top:20px;'>Chưa có sân nào được đặt trong kỳ này.</p>";
    else
        html += "<p style='color:#666; margin-top:15px;'><i>💡 Ghi chú: Dòng màu hồng nhạt là sân có tỷ lệ hủy > 30%</i></p>";

    text->setHtml(html);
}

// ===========================================================
// =============== HÀM THỐNG KÊ KHÁCH HÀNG ===================
// ===========================================================
void ThongKeDialog::tinhKhachHang(QTextBrowser* text) {
    const auto& dsKH = heThong->getDanhSachKhach();
    const auto& dsLich = heThong->getDanhSachLich();

    if (dsKH.getKichThuoc() == 0) {
        text->setHtml("<h3>Chưa có khách hàng nào.</h3>");
        return;
    }

    // Khoảng thời gian
    time_t from, to;
    QString tenKy = cboKhoangThoiGian->currentText();
    xacDinhKhoangThoiGian(from, to, tenKy);

    struct ThongKeKH {
        QString maKH, tenKH, sdt;
        int soLanDat = 0, soLanHuy = 0;
        double tongChiTieu = 0;
    };

    QMap<QString, ThongKeKH> mapKH;

    // Khởi tạo danh sách KH
    for (int i = 0; i < dsKH.getKichThuoc(); ++i) {
        ThongKeKH tk;
        tk.maKH = QString::fromStdString(dsKH[i].getMaKH());
        tk.tenKH = QString::fromStdString(dsKH[i].getHoTen());
        tk.sdt = QString::fromStdString(dsKH[i].getSdt());
        mapKH[tk.maKH] = tk;
    }

    // Thống kê
    for (int i = 0; i < dsLich.getKichThuoc(); ++i) {
        const LichDatSan& lich = dsLich[i];
        
        if (tenKy != "Tất cả") {
            time_t tLich = lich.getThoiGianBatDau();
            if (tLich < from || tLich > to) continue;
        }

        QString maKH = QString::fromStdString(lich.getMaKH());
        if (mapKH.contains(maKH)) {
            if (lich.getTrangThaiDat() == "Da Dat") {
                mapKH[maKH].soLanDat++;
                if (lich.getTrangThaiTT() == "Da Thanh Toan") {
                    double tienSan = lich.getTongTien();
                    double tienDV = heThong->tinhTongTienDonAPI(lich.getMaLichDat());
                    mapKH[maKH].tongChiTieu += (tienSan + tienDV);
                }
            } else if (lich.getTrangThaiDat() == "Da Huy") {
                mapKH[maKH].soLanHuy++;
            }
        }
    }

    QString html = QString("<h2 style='color:#9C27B0;'>� THỐNG KÊ KHÁCH HÀNG - %1</h2>").arg(tenKy);
    if (tenKy == "Tùy chỉnh") {
        html += QString("<p style='color:#666;'>📅 Từ %1 đến %2</p>")
                    .arg(dateFrom->date().toString("dd/MM/yyyy"))
                    .arg(dateTo->date().toString("dd/MM/yyyy"));
    }
    html += "<hr>";

    html += "<table border='1' cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse;'>";
    html += "<tr style='background-color:#9C27B0; color:white;'>"
            "<th>Mã KH</th><th>Tên KH</th><th>SĐT</th>"
            "<th>Lượt đặt</th><th>Lượt hủy</th>"
            "<th>Tổng chi tiêu (VND)</th></tr>";

    int countKHCoGiaoDich = 0;
    QMapIterator<QString, ThongKeKH> it(mapKH);
    while (it.hasNext()) {
        it.next();
        const ThongKeKH& tk = it.value();
        
        if (tk.soLanDat > 0 || tk.soLanHuy > 0) {
            countKHCoGiaoDich++;
            QString rowColor = (tk.tongChiTieu > 1000000) ? "background-color:#fff9c4;" : "";
            
            html += QString("<tr style='%1'>"
                           "<td>%2</td><td>%3</td><td>%4</td>"
                           "<td style='text-align:center; color:#4CAF50; font-weight:bold;'>%5</td>"
                           "<td style='text-align:center; color:#f44336; font-weight:bold;'>%6</td>"
                           "<td style='text-align:right; font-weight:bold;'>%7</td>"
                           "</tr>")
                        .arg(rowColor)
                        .arg(tk.maKH)
                        .arg(tk.tenKH)
                        .arg(tk.sdt)
                        .arg(tk.soLanDat)
                        .arg(tk.soLanHuy)
                        .arg(QString::number(tk.tongChiTieu, 'f', 0));
        }
    }

    html += "</table>";

    if (countKHCoGiaoDich == 0)
        html += "<p style='color:#999; text-align:center; margin-top:20px;'>Chưa có khách hàng nào có giao dịch trong kỳ này.</p>";
    else
        html += "<p style='color:#666; margin-top:15px;'><i>💡 Ghi chú: Dòng màu vàng nhạt là khách hàng VIP (chi tiêu > 1 triệu VND)</i></p>";

    text->setHtml(html);
}
