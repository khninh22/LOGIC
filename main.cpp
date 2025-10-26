#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QScrollArea>
#include <QMouseEvent>
#include <QTabWidget>
#include <QTextBrowser>
#include <QMap>
#include <QFont>
#include <QDateTime>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QEnterEvent>
#endif

#include "QuanLyThueSan.h"

// ✅ INCLUDE CÁC DIALOG
#include "ui/dialogs/KhachHangDialog.h"
#include "ui/dialogs/BookingDialog.h"
#include "ui/dialogs/LichSuDialog.h"
#include "ui/dialogs/ThanhToanDialog.h"
#include "ui/dialogs/ThongKeDialog.h"

// ==================== CUSTOM WIDGET - Ô VUÔNG ĐẶT SÂN ====================
class TimeSlotWidget : public QWidget {
    Q_OBJECT
public:
    TimeSlotWidget(const QString& maSan, const QString& tenSan,
                   const QTime& start, const QTime& end,
                   bool available, QWidget* parent = nullptr)
        : QWidget(parent), maSan(maSan), startTime(start), endTime(end), isAvailable(available) {

        setFixedSize(100, 80);
        setCursor(available ? Qt::PointingHandCursor : Qt::ForbiddenCursor);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(5, 5, 5, 5);

        // Thời gian
        lblTime = new QLabel(QString("%1-%2").arg(start.toString("HH:mm")).arg(end.toString("HH:mm")));
        lblTime->setAlignment(Qt::AlignCenter);
        QFont timeFont = lblTime->font();
        timeFont.setBold(true);
        timeFont.setPointSize(11);
        lblTime->setFont(timeFont);

        // Tên sân
        lblSan = new QLabel(QString("%1\n%2").arg(maSan).arg(tenSan));
        lblSan->setAlignment(Qt::AlignCenter);
        lblSan->setWordWrap(true);
        QFont sanFont = lblSan->font();
        sanFont.setPointSize(9);
        lblSan->setFont(sanFont);

        layout->addWidget(lblTime);
        layout->addWidget(lblSan);

        setAutoFillBackground(true);
        setAttribute(Qt::WA_StyledBackground, true);
        updateStyle();
    }

    void setAvailable(bool avail) {
        isAvailable = avail;
        setCursor(avail ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
        updateStyle();
    }

    QString getMaSan() const { return maSan; }
    QTime getStartTime() const { return startTime; }
    QTime getEndTime() const { return endTime; }

signals:
    void clicked(const QString& maSan, const QTime& start, const QTime& end);

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (isAvailable) {
            emit clicked(maSan, startTime, endTime);
        }
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent* event) override {
#else
    void enterEvent(QEvent* event) override {
#endif
        if (isAvailable) {
            setStyleSheet(styleHover);
        }
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        updateStyle();
        QWidget::leaveEvent(event);
    }

private:
    void updateStyle() {
        if (isAvailable) {
            setStyleSheet(styleAvailable);
        } else {
            setStyleSheet(styleBusy);
        }
    }

    QString maSan;
    QTime startTime, endTime;
    bool isAvailable;
    QLabel *lblTime, *lblSan;

    const QString styleAvailable =
        "QWidget { background-color: #4CAF50; border: 2px solid #2E7D32; border-radius: 8px; } "
        "QLabel { background: transparent; color: white; }";

    const QString styleBusy =
        "QWidget { background-color: #A00000; border: 2px solid white; border-radius: 8px; } "
        "QLabel { background: transparent; color: white; text-decoration: line-through; }";

    const QString styleHover =
        "QWidget { background-color: #66BB6A; border: 3px solid #FFFFFF; border-radius: 8px; } "
        "QLabel { background: transparent; color: white; font-weight: bold; }";
};

// ==================== FORWARD DECLARATIONS ====================
class KhachHangDialog;
class LichSuDialog;
class AddCustomerDialog;
class BookingDialog;
class ThanhToanDialog;
class ThongKeDialog;

// ==================== MAIN WINDOW ====================
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void createToolBar();
    void createUI();
    void createStatusBar();
    void connectSignals();

    void refreshAll();
    void refreshStatusTable();
    void refreshBookingSlots();
    int getLoaiSanHienTai() const;

private slots:
    void onSlotClicked(const QString& maSan, const QTime& start, const QTime& end);
    void showKhachHang();
    void showLichSu();
    void showThanhToan();
    void showThongKe();

protected:
    void showEvent(QShowEvent* event) override;

private:
    QuanLyThueSan* heThong;
    QDateEdit* dateEdit;
    QComboBox* cboLoai;
    QPushButton* btnRefresh;
    QTableWidget* tblStatus;
    QScrollArea* scrollArea;
    QWidget* slotContainer;
    QGridLayout* slotLayout;
    QComboBox* cboGio;
};

// ==================== IMPLEMENTATION ====================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    heThong = new QuanLyThueSan();
    setWindowTitle("🏟️ Hệ thống quản lý thuê sân bóng v2.0");

    createMenuBar();
    createToolBar();
    createUI();
    createStatusBar();

    refreshAll();
    resize(1400, 800);
}

MainWindow::~MainWindow() {
    heThong->luuDuLieu();
    delete heThong;
}

void MainWindow::createMenuBar() {
    auto* menuFile = menuBar()->addMenu("&Tệp");
    auto* actSave = menuFile->addAction("💾 Lưu dữ liệu");
    auto* actBackup = menuFile->addAction("📦 Backup");
    auto* actExit = menuFile->addAction("🚪 Thoát");

    connect(actSave, &QAction::triggered, [this]() {
        heThong->luuDuLieu();
        statusBar()->showMessage("✅ Đã lưu dữ liệu!", 3000);
    });

    connect(actBackup, &QAction::triggered, [this]() {
        QString backupName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        if (heThong->backup(backupName.toStdString())) {
            QMessageBox::information(this, "Backup", "Đã backup thành công!");
        }
    });

    connect(actExit, &QAction::triggered, this, &QMainWindow::close);

    auto* menuQuanLy = menuBar()->addMenu("&Quản lý");
    auto* actThanhToan = menuQuanLy->addAction("💳 Thanh toán hóa đơn");
    auto* actThongKe = menuQuanLy->addAction("📈 Thống kê");
    menuQuanLy->addSeparator();
    auto* actKhach = menuQuanLy->addAction("👥 Khách hàng");
    auto* actLichSu = menuQuanLy->addAction("📋 Lịch sử đặt sân");

    connect(actThanhToan, &QAction::triggered, this, &MainWindow::showThanhToan);
    connect(actThongKe, &QAction::triggered, this, &MainWindow::showThongKe);
    connect(actKhach, &QAction::triggered, this, &MainWindow::showKhachHang);
    connect(actLichSu, &QAction::triggered, this, &MainWindow::showLichSu);

    auto* menuHelp = menuBar()->addMenu("&Trợ giúp");
    auto* actAbout = menuHelp->addAction("ℹ️ Giới thiệu");
    connect(actAbout, &QAction::triggered, [this]() {
        QMessageBox::about(this, "Giới thiệu",
                           "🏟️ <b>Hệ thống quản lý thuê sân bóng</b><br>"
                           "Phiên bản 2.0 - Modular Architecture<br><br>"
                           "Phát triển bởi: PBL2 Team<br>"
                           "✨ Tính năng mới:<br>"
                           "- Thống kê theo thời gian<br>"
                           "- Backup/Restore dữ liệu<br>"
                           "- Mã KH không thừa số 0");
    });
}

void MainWindow::createToolBar() {
    auto* toolbar = addToolBar("Main");
    toolbar->setMovable(false);

    auto* actRefresh = toolbar->addAction("🔄 Làm mới");
    toolbar->addSeparator();
    auto* actThanhToan = toolbar->addAction("💳 Thanh toán");
    auto* actThongKe = toolbar->addAction("📈 Thống kê");
    toolbar->addSeparator();
    auto* actKhach = toolbar->addAction("👥 Khách hàng");
    auto* actLichSu = toolbar->addAction("📋 Lịch sử");

    connect(actRefresh, &QAction::triggered, this, &MainWindow::refreshAll);
    connect(actThanhToan, &QAction::triggered, this, &MainWindow::showThanhToan);
    connect(actThongKe, &QAction::triggered, this, &MainWindow::showThongKe);
    connect(actKhach, &QAction::triggered, this, &MainWindow::showKhachHang);
    connect(actLichSu, &QAction::triggered, this, &MainWindow::showLichSu);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Sẵn sàng");
}

void MainWindow::createUI() {
    auto* central = new QWidget;
    setCentralWidget(central);
    auto* mainLayout = new QVBoxLayout(central);

    // Header
    auto* headerLayout = new QHBoxLayout;
    headerLayout->addWidget(new QLabel("<b>📅 Ngày:</b>"));

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumWidth(120);
    headerLayout->addWidget(dateEdit);

    headerLayout->addSpacing(20);
    headerLayout->addWidget(new QLabel("<b>⚽ Loại sân:</b>"));

    cboLoai = new QComboBox;
    cboLoai->addItems({"Sân 5 người", "Sân 7 người", "Sân 11 người"});
    cboLoai->setMinimumWidth(150);
    headerLayout->addWidget(cboLoai);

    btnRefresh = new QPushButton("🔄 Làm mới");
    btnRefresh->setStyleSheet("QPushButton { padding: 8px 20px; background-color: #2196F3; color: white; border-radius: 5px; font-weight: bold; }");
    headerLayout->addWidget(btnRefresh);

    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // Tab Widget
    auto* tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget, 1);

    // Bộ lọc giờ
    cboGio = new QComboBox;
    cboGio->addItem("Tất cả giờ");
    const int openHour = 6, closeHour = 22;
    for (int h = openHour; h < closeHour; ++h) {
        cboGio->addItem(QString("%1:00").arg(h, 2, 10, QChar('0')));
    }
    cboGio->setMinimumWidth(120);

    auto* cornerWidget = new QWidget;
    auto* cornerLayout = new QHBoxLayout(cornerWidget);
    cornerLayout->setContentsMargins(0, 5, 10, 5);
    cornerLayout->addWidget(new QLabel("🕐 Lọc giờ bắt đầu:"));
    cornerLayout->addWidget(cboGio);
    tabWidget->setCornerWidget(cornerWidget, Qt::TopRightCorner);

    // TAB 1: Đặt sân
    auto* pageBooking = new QWidget;
    auto* layoutBooking = new QVBoxLayout(pageBooking);
    layoutBooking->setContentsMargins(5, 5, 5, 5);
    layoutBooking->addWidget(new QLabel("<h3>🏟️ Đặt sân - Nhấn vào ô để đặt (khung 1 giờ)</h3>"));

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    slotContainer = new QWidget;
    slotLayout = new QGridLayout(slotContainer);
    slotLayout->setSpacing(10);
    slotLayout->setContentsMargins(10, 10, 10, 10);
    slotLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    scrollArea->setWidget(slotContainer);
    layoutBooking->addWidget(scrollArea, 1);

    // TAB 2: Trạng thái sân
    auto* pageStatus = new QWidget;
    auto* layoutStatus = new QVBoxLayout(pageStatus);
    layoutStatus->setContentsMargins(5, 5, 5, 5);
    layoutStatus->addWidget(new QLabel("<h3>📊 Trạng thái sân trong ngày (30 phút / ô)</h3>"));

    tblStatus = new QTableWidget;
    tblStatus->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblStatus->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblStatus->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tblStatus->setSelectionMode(QAbstractItemView::NoSelection);
    layoutStatus->addWidget(tblStatus, 1);

    auto* legendLayout = new QHBoxLayout;
    auto* lblLegend = new QLabel(
        "   <span style='color: green;'>✅ Trống</span>   "
        "<span style='color: red;'>❌ Đã đặt</span>   "
        );
    legendLayout->addWidget(lblLegend);
    legendLayout->addStretch();
    layoutStatus->addLayout(legendLayout);

    tabWidget->addTab(pageBooking, "🏟️ Đặt sân");
    tabWidget->addTab(pageStatus, "📊 Trạng thái sân");
}

void MainWindow::connectSignals() {
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshAll);
    connect(dateEdit, &QDateEdit::dateChanged, this, &MainWindow::refreshAll);
    connect(cboLoai, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::refreshAll);
    if (cboGio) {
        connect(cboGio, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::refreshAll);
    }
}

void MainWindow::showEvent(QShowEvent* event) {
    static bool firstShow = true;
    if (firstShow) {
        connectSignals();
        firstShow = false;
    }
    QMainWindow::showEvent(event);
}

void MainWindow::refreshAll() {
    refreshStatusTable();
    refreshBookingSlots();
    statusBar()->showMessage("✅ Đã làm mới dữ liệu", 2000);
}

void MainWindow::refreshStatusTable() {
    const int openHour = 6, closeHour = 22;
    int totalSlots = (closeHour - openHour) * 2;
    tblStatus->setColumnCount(totalSlots);

    QStringList headers;
    for (int h = openHour; h < closeHour; ++h) {
        headers << QString("%1:00").arg(h, 2, 10, QChar('0'))
        << QString("%1:30").arg(h, 2, 10, QChar('0'));
    }
    tblStatus->setHorizontalHeaderLabels(headers);

    int loai = getLoaiSanHienTai();
    const auto& ds = heThong->getDanhSachSan();
    QVector<const SanBong*> rows;
    for (int i = 0; i < ds.getKichThuoc(); ++i)
        if (ds[i].getLoaiSan() == loai && !ds[i].getBaoTri())
            rows.push_back(&ds[i]);

    tblStatus->setRowCount(rows.size());
    QStringList vlabels;
    for (auto* s : rows)
        vlabels << QString::fromStdString(s->getMaSan());
    tblStatus->setVerticalHeaderLabels(vlabels);

    QDate d = dateEdit->date();
    tm day{};
    day.tm_mday = d.day();
    day.tm_mon = d.month() - 1;
    day.tm_year = d.year() - 1900;

    auto makeTime = [&](int h, int m) {
        tm t = day;
        t.tm_hour = h;
        t.tm_min = m;
        t.tm_sec = 0;
        return mktime(&t);
    };

    for (int r = 0; r < rows.size(); ++r) {
        for (int c = 0; c < totalSlots; ++c) {
            int h = openHour + c / 2;
            int m = (c % 2) ? 30 : 0;
            bool busy = heThong->isTrungLich(rows[r]->getMaSan(),
                                             makeTime(h, m), makeTime(h, m + 30));

            auto* item = new QTableWidgetItem(busy ? "❌" : "✅");
            item->setTextAlignment(Qt::AlignCenter);
            if (busy) {
                item->setBackground(QColor(255, 200, 200));
                item->setForeground(Qt::red);
            } else {
                item->setBackground(QColor(200, 255, 200));
                item->setForeground(Qt::darkGreen);
            }
            tblStatus->setItem(r, c, item);
        }
    }
}

void MainWindow::refreshBookingSlots() {
    QLayoutItem* item;
    while ((item = slotLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int openHour = 6, closeHour = 22;
    int loai = getLoaiSanHienTai();
    const auto& ds = heThong->getDanhSachSan();

    QVector<const SanBong*> sans;
    for (int i = 0; i < ds.getKichThuoc(); ++i)
        if (ds[i].getLoaiSan() == loai && !ds[i].getBaoTri())
            sans.push_back(&ds[i]);

    if (sans.isEmpty()) {
        auto* lblEmpty = new QLabel("Không có sân nào thuộc loại này (hoặc sân đang bảo trì).");
        lblEmpty->setAlignment(Qt::AlignCenter);
        slotLayout->addWidget(lblEmpty, 0, 0);
        return;
    }

    QDate d = dateEdit->date();
    tm day{};
    day.tm_mday = d.day();
    day.tm_mon = d.month() - 1;
    day.tm_year = d.year() - 1900;

    auto makeTime = [&](int h, int m) {
        tm t = day;
        t.tm_hour = h;
        t.tm_min = m;
        t.tm_sec = 0;
        return mktime(&t);
    };

    QString gioFilter = cboGio->currentText();
    bool tatCaGio = (gioFilter == "Tất cả giờ");
    int h_filter = gioFilter.split(":")[0].toInt();

    int foundCount = 0;

    if (tatCaGio) {
        int row = 0;
        for (auto* san : sans) {
            auto* lblSan = new QLabel(QString("<b>%1</b><br>%2")
                                          .arg(QString::fromStdString(san->getMaSan()))
                                          .arg(QString::fromStdString(san->getTenSan())));
            lblSan->setAlignment(Qt::AlignCenter);
            lblSan->setMinimumHeight(60);
            lblSan->setStyleSheet("background-color: #E3F2FD; padding: 10px; border-radius: 5px;");
            slotLayout->addWidget(lblSan, row, 0);

            int col = 1;
            for (int h = openHour; h < closeHour; h++) {
                QTime start(h, 0);
                QTime end(h + 1, 0);

                bool avail = !heThong->isTrungLich(san->getMaSan(),
                                                   makeTime(h, 0), makeTime(h + 1, 0));

                auto* slot = new TimeSlotWidget(
                    QString::fromStdString(san->getMaSan()),
                    QString::fromStdString(san->getTenSan()),
                    start, end, avail, slotContainer
                    );

                connect(slot, &TimeSlotWidget::clicked, this, &MainWindow::onSlotClicked);
                slotLayout->addWidget(slot, row, col++);
                foundCount++;
            }
            row++;
        }
    } else {
        int r = 0, c = 0;
        const int MAX_COLS_PER_ROW = 8;

        QTime start(h_filter, 0);
        QTime end(h_filter + 1, 0);
        time_t tBD = makeTime(h_filter, 0);
        time_t tKT = makeTime(h_filter + 1, 0);

        for (auto* san : sans) {
            bool avail = !heThong->isTrungLich(san->getMaSan(), tBD, tKT);

            if (avail) {
                foundCount++;
                auto* slot = new TimeSlotWidget(
                    QString::fromStdString(san->getMaSan()),
                    QString::fromStdString(san->getTenSan()),
                    start, end, avail, slotContainer
                    );

                connect(slot, &TimeSlotWidget::clicked, this, &MainWindow::onSlotClicked);
                slotLayout->addWidget(slot, r, c);

                if (++c >= MAX_COLS_PER_ROW) {
                    c = 0;
                    r++;
                }
            }
        }
    }

    if (foundCount == 0) {
        QString msg = tatCaGio ? "Không có sân nào thuộc loại này."
                               : QString("Không có sân trống vào lúc %1.").arg(gioFilter);
        auto* lblEmpty = new QLabel(msg);
        lblEmpty->setAlignment(Qt::AlignCenter);
        lblEmpty->setStyleSheet("font-size: 16px; color: #888; padding: 20px;");
        slotLayout->addWidget(lblEmpty, 0, 0, 1, 4);
    }
}

int MainWindow::getLoaiSanHienTai() const {
    QString text = cboLoai->currentText();
    if (text.contains("5")) return 5;
    if (text.contains("7")) return 7;
    if (text.contains("11")) return 11;
    return 5;
}

void MainWindow::onSlotClicked(const QString& maSan, const QTime& start, const QTime& end) {
    const auto& ds = heThong->getDanhSachSan();
    QString tenSan;
    for (int i = 0; i < ds.getKichThuoc(); ++i) {
        if (ds[i].getMaSan() == maSan.toStdString()) {
            tenSan = QString::fromStdString(ds[i].getTenSan());
            break;
        }
    }

    // ✅ Mở BookingDialog thay vì hiện thông báo
    BookingDialog dlg(heThong, maSan, tenSan, dateEdit->date(), start, end, this);
    dlg.exec();

    refreshAll(); // Làm mới giao diện sau khi đặt sân xong
}


void MainWindow::showKhachHang() {
    KhachHangDialog dlg(heThong, this);
    dlg.exec();
}

void MainWindow::showLichSu() {
    LichSuDialog dlg(heThong, this);
    dlg.exec();
}

void MainWindow::showThanhToan() {
    ThanhToanDialog dlg(heThong, this);
    dlg.exec();
}

void MainWindow::showThongKe() {
    ThongKeDialog dlg(heThong, this);
    dlg.exec();
}

#include "main.moc"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWindow w;
    w.show();

    return app.exec();
}
