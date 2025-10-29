cmake_minimum_required(VERSION 3.16)

project(NEWGUI VERSION 2.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

# ==================== CORE - Entities ====================
set(CORE_ENTITIES_SOURCES
    core/entities/KhachHang.cpp
    core/entities/SanBong.cpp
    core/entities/LichDatSan.cpp
    core/entities/DichVu.cpp
    core/entities/DonDichVu.cpp
)

set(CORE_ENTITIES_HEADERS
    core/entities/KhachHang.h
    core/entities/SanBong.h
    core/entities/LichDatSan.h
    core/entities/DichVu.h
    core/entities/DonDichVu.h
)

# ==================== CORE - Utils ====================
set(CORE_UTILS_SOURCES
    core/utils/DateTimeUtils.cpp
)

set(CORE_UTILS_HEADERS
    core/utils/DateTimeUtils.h
    core/utils/ThuatToan.h
)

# ==================== CORE - Services ====================
set(CORE_SERVICES_SOURCES
    core/services/TinhTienService.cpp
    core/services/ValidationService.cpp
)

set(CORE_SERVICES_HEADERS
    core/services/TinhTienService.h
    core/services/ValidationService.h
)

# ==================== CORE - Managers ====================
set(CORE_MANAGERS_SOURCES
    core/managers/QuanLyKhachHang.cpp
    core/managers/QuanLySanBong.cpp
    core/managers/QuanLyLichDat.cpp
    core/managers/QuanLyThongKe.cpp
    core/managers/QuanLyDichVu.cpp
)

set(CORE_MANAGERS_HEADERS
    core/managers/QuanLyKhachHang.h
    core/managers/QuanLySanBong.h
    core/managers/QuanLyLichDat.h
    core/managers/QuanLyThongKe.h
    core/managers/QuanLyDichVu.h
)

# ==================== DATA ====================
set(DATA_SOURCES
    data/DataManager.cpp
)

set(DATA_HEADERS
    data/DataManager.h
)

# ==================== FACADE ====================
set(FACADE_SOURCES
    QuanLyThueSan.cpp
)

set(FACADE_HEADERS
    QuanLyThueSan.h
)

# ==================== UI (Dialogs) ====================
set(UI_DIALOGS_SOURCES
    ui/dialogs/KhachHangDialog.cpp
    ui/dialogs/BookingDialog.cpp
    ui/dialogs/AddCustomerDialog.cpp
    ui/dialogs/LichSuDialog.cpp
    ui/dialogs/ThanhToanDialog.cpp
    ui/dialogs/ThongKeDialog.cpp
    ui/dialogs/DichVuDialog.cpp
)

set(UI_DIALOGS_HEADERS
    ui/dialogs/KhachHangDialog.h
    ui/dialogs/BookingDialog.h
    ui/dialogs/AddCustomerDialog.h
    ui/dialogs/LichSuDialog.h
    ui/dialogs/ThanhToanDialog.h
    ui/dialogs/ThongKeDialog.h
    ui/dialogs/DichVuDialog.h
)

set(UI_SOURCES
    main.cpp
)

# ==================== TẤT CẢ FILES ====================
set(PROJECT_SOURCES
    ${CORE_ENTITIES_SOURCES}
    ${CORE_UTILS_SOURCES}
    ${CORE_SERVICES_SOURCES}
    ${CORE_MANAGERS_SOURCES}
    ${DATA_SOURCES}
    ${FACADE_SOURCES}
    ${UI_DIALOGS_SOURCES}
    ${UI_SOURCES}
)

set(PROJECT_HEADERS
    ${CORE_ENTITIES_HEADERS}
    ${CORE_UTILS_HEADERS}
    ${CORE_SERVICES_HEADERS}
    ${CORE_MANAGERS_HEADERS}
    ${DATA_HEADERS}
    ${FACADE_HEADERS}
    ${UI_DIALOGS_HEADERS}
)

# ==================== BUILD ====================
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NEWGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    add_executable(NEWGUI
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
endif()

target_link_libraries(NEWGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

# Include directories
target_include_directories(NEWGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(NEWGUI PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NEWGUI)
endif()

# ==================== INSTALL ====================
install(TARGETS NEWGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
