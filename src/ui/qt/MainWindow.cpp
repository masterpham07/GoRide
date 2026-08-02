#include "ui/qt/MainWindow.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QDateTime>
#include <QGroupBox>
#include <QFormLayout>
#include <QScrollArea>
#include <QSignalBlocker>
#include <iomanip>
#include <sstream>

#include "services/FareCalculator.h"
#include "services/MatchingEngine.h"
#include "services/MapService.h"
#include "services/PathFinder.h"
#include "services/PaymentService.h"
#include "data/FileHandler.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rider("R001", "Ms.Dung", "0909123456", "1234", 200000.0),
      currentDriver(nullptr),
      tripCounter(1000),
      currentPhase(TripPhase::IDLE),
      mapSelectionMode(MapSelectionMode::None),
      hasCustomPickup(false),
      hasCustomDropoff(false)
{
    initData();
    setupUi();
    updateWalletDisplay();
    onLocationOrVehicleChanged();
}

void MainWindow::initData()
{
    // Populate drivers
    driverStorage.push_back(make_unique<Driver>("D001", "Phạm Đan Trường", "0901111111", "1111", "6651071086", VehicleType::BIKE, Location(3, 5)));
    driverStorage.push_back(make_unique<Driver>("D002", "Phạm Ngọc Thắng", "0902222222", "2222", "6651071071", VehicleType::BIKE, Location(10, 10)));
    driverStorage.push_back(make_unique<Driver>("D003", "Phan Minh Tân", "0903333333", "3333", "6651071067", VehicleType::BIKE, Location(6, 2)));
    driverStorage.push_back(make_unique<Driver>("D004", "Nguyễn Phương Nam", "0904444444", "4444", "6651071047", VehicleType::CAR4, Location(0, 15)));
    driverStorage.push_back(make_unique<Driver>("D005", "Trần Minh Khôi", "0905555555", "7777", "6651071034", VehicleType::CAR4, Location(13, 5)));
    driverStorage.push_back(make_unique<Driver>("D007", "Cristiano Ronaldo", "0907777777", "5555", "7777777777", VehicleType::CAR7, Location(15, 0)));
    driverStorage.push_back(make_unique<Driver>("D010", "Lionel Messi", "0910101010", "6666", "1010101010", VehicleType::CAR7, Location(9, 9)));

    drivers.clear();
    for (auto &d : driverStorage)
    {
        drivers.push_back(d.get());
    }

    // Map existing locations to Place objects
    const auto &mapPlaces = MapService::places();
    places.clear();
    for (size_t i = 0; i < mapPlaces.size(); ++i)
    {
        const auto &loc = mapPlaces[i];
        PlaceType type = PlaceType::Building;
        string icon = ":/icons/building.svg";

        if (loc.label.find("Chợ") != string::npos)
        {
            type = PlaceType::Mall;
            icon = ":/icons/mall.svg";
        }
        else if (loc.label.find("Đại học") != string::npos || loc.label.find("Trường") != string::npos)
        {
            type = PlaceType::School;
            icon = ":/icons/school.svg";
        }
        else if (loc.label.find("Bến xe") != string::npos)
        {
            type = PlaceType::BusStation;
            icon = ":/icons/busStation.svg";
        }
        else if (loc.label.find("Bến Nhà Rồng") != string::npos)
        {
            type = PlaceType::Harbor;
            icon = ":/icons/harbor.svg";
        }
        else if (loc.label.find("Sân bay") != string::npos)
        {
            type = PlaceType::Airport;
            icon = ":/icons/airport.svg";
        }
        else if (loc.label.find("Công viên") != string::npos)
        {
            type = PlaceType::Park;
            icon = ":/icons/park.svg";
        }

        places.emplace_back(loc.label, loc, type, icon);
    }
}

void MainWindow::setupUi()
{
    setWindowTitle(QStringLiteral("GoRide - Ứng Dụng Đặt Xe Công Nghệ"));
    resize(1100, 720);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createHeader();
    mainLayout->addWidget(headerFrame);

    QHBoxLayout *bodyLayout = new QHBoxLayout();
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    createSidebar();
    bodyLayout->addWidget(sidebarFrame);

    createContentStack();
    bodyLayout->addWidget(contentStack, 1);

    mainLayout->addLayout(bodyLayout, 1);

    statusLabel = new QLabel(QStringLiteral("Sẵn sàng đặt xe cùng GoRide."), this);
    statusLabel->setObjectName(QStringLiteral("statusLabel"));
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);
}

void MainWindow::createHeader()
{
    headerFrame = new QFrame(this);
    headerFrame->setObjectName(QStringLiteral("headerFrame"));

    QHBoxLayout *layout = new QHBoxLayout(headerFrame);
    layout->setContentsMargins(20, 10, 20, 10);

    logoLabel = new QLabel(QStringLiteral("GoRide"), headerFrame);
    logoLabel->setObjectName(QStringLiteral("logoLabel"));

    greetingLabel = new QLabel(QString::fromUtf8(("Xin chào, " + rider.getName() + "!").c_str()), headerFrame);
    greetingLabel->setObjectName(QStringLiteral("greetingLabel"));

    walletLabel = new QLabel(headerFrame);
    walletLabel->setObjectName(QStringLiteral("walletLabel"));

    layout->addWidget(logoLabel);
    layout->addWidget(greetingLabel);
    layout->addStretch();
    layout->addWidget(walletLabel);
}

void MainWindow::createSidebar()
{
    sidebarFrame = new QFrame(this);
    sidebarFrame->setObjectName(QStringLiteral("sidebarFrame"));

    QVBoxLayout *layout = new QVBoxLayout(sidebarFrame);
    layout->setContentsMargins(10, 20, 10, 20);
    layout->setSpacing(8);

    btnHome = new QPushButton(QStringLiteral("Trang chủ"), sidebarFrame);
    btnBooking = new QPushButton(QStringLiteral("Đặt xe"), sidebarFrame);
    btnHistory = new QPushButton(QStringLiteral("Lịch sử chuyến đi"), sidebarFrame);
    btnDrivers = new QPushButton(QStringLiteral("Danh sách tài xế"), sidebarFrame);
    btnWallet = new QPushButton(QStringLiteral("Nạp tiền ví"), sidebarFrame);
    btnExit = new QPushButton(QStringLiteral("Thoát"), sidebarFrame);
    btnExit->setObjectName(QStringLiteral("exitButton"));

    layout->addWidget(btnHome);
    layout->addWidget(btnBooking);
    layout->addWidget(btnHistory);
    layout->addWidget(btnDrivers);
    layout->addWidget(btnWallet);
    layout->addStretch();
    layout->addWidget(btnExit);

    connect(btnHome, &QPushButton::clicked, this, &MainWindow::onSidebarNavClicked);
    connect(btnBooking, &QPushButton::clicked, this, &MainWindow::onSidebarNavClicked);
    connect(btnHistory, &QPushButton::clicked, this, &MainWindow::onSidebarNavClicked);
    connect(btnDrivers, &QPushButton::clicked, this, &MainWindow::onSidebarNavClicked);
    connect(btnWallet, &QPushButton::clicked, this, &MainWindow::onSidebarNavClicked);
    connect(btnExit, &QPushButton::clicked, qApp, &QApplication::quit);

    setNavActive(btnBooking);
}

void MainWindow::setNavActive(QPushButton *activeButton)
{
    btnHome->setProperty("active", false);
    btnBooking->setProperty("active", false);
    btnHistory->setProperty("active", false);
    btnDrivers->setProperty("active", false);
    btnWallet->setProperty("active", false);

    if (activeButton)
    {
        activeButton->setProperty("active", true);
    }

    btnHome->style()->unpolish(btnHome);
    btnHome->style()->polish(btnHome);
    btnBooking->style()->unpolish(btnBooking);
    btnBooking->style()->polish(btnBooking);
    btnHistory->style()->unpolish(btnHistory);
    btnHistory->style()->polish(btnHistory);
    btnDrivers->style()->unpolish(btnDrivers);
    btnDrivers->style()->polish(btnDrivers);
    btnWallet->style()->unpolish(btnWallet);
    btnWallet->style()->polish(btnWallet);
}

void MainWindow::createContentStack()
{
    contentStack = new QStackedWidget(this);

    createHomePage();
    createBookingPage();
    createHistoryPage();
    createDriversPage();
    createWalletPage();

    contentStack->addWidget(pageHome);
    contentStack->addWidget(pageBooking);
    contentStack->addWidget(pageHistory);
    contentStack->addWidget(pageDrivers);
    contentStack->addWidget(pageWallet);

    contentStack->setCurrentWidget(pageBooking);
}

void MainWindow::createHomePage()
{
    pageHome = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(pageHome);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *welcomeTitle = new QLabel(QStringLiteral("Chào mừng đến với GoRide!"), pageHome);
    welcomeTitle->setStyleSheet(QStringLiteral("font-size: 24px; font-weight: bold; color: #0F172A;"));

    QLabel *welcomeDesc = new QLabel(
        QStringLiteral("Hệ thống di chuyển thông minh hàng đầu. Đặt xe nhanh chóng, theo dõi lộ trình thời gian thực trên bản đồ lưới 16x16."),
        pageHome);
    welcomeDesc->setWordWrap(true);
    welcomeDesc->setStyleSheet(QStringLiteral("font-size: 14px; color: #475569; margin-bottom: 20px;"));

    QHBoxLayout *cardsLayout = new QHBoxLayout();

    // Quick booking card
    QFrame *bookingCard = new QFrame(pageHome);
    bookingCard->setObjectName(QStringLiteral("cardFrame"));
    QVBoxLayout *c1Layout = new QVBoxLayout(bookingCard);
    QLabel *c1Title = new QLabel(QStringLiteral("Đặt Xe Ngay"), bookingCard);
    c1Title->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: #2563EB;"));
    QLabel *c1Desc = new QLabel(QStringLiteral("Tìm kiếm tài xế gần bạn nhất và theo dõi di chuyển mượt mà trên bản đồ."), bookingCard);
    c1Desc->setWordWrap(true);
    QPushButton *btnGoBooking = new QPushButton(QStringLiteral("Bắt đầu đặt xe"), bookingCard);
    connect(btnGoBooking, &QPushButton::clicked, [this]()
            {
        contentStack->setCurrentWidget(pageBooking);
        setNavActive(btnBooking); });
    c1Layout->addWidget(c1Title);
    c1Layout->addWidget(c1Desc);
    c1Layout->addStretch();
    c1Layout->addWidget(btnGoBooking);

    // Quick wallet card
    QFrame *walletCard = new QFrame(pageHome);
    walletCard->setObjectName(QStringLiteral("cardFrame"));
    QVBoxLayout *c2Layout = new QVBoxLayout(walletCard);
    QLabel *c2Title = new QLabel(QStringLiteral("Ví Điện Tử"), walletCard);
    c2Title->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: #D97706;"));
    QLabel *c2Desc = new QLabel(QStringLiteral("Nạp tiền nhanh chóng vào tài khoản để sẵn sàng cho mọi chuyến đi."), walletCard);
    c2Desc->setWordWrap(true);
    QPushButton *btnGoWallet = new QPushButton(QStringLiteral("Nạp tiền vào ví"), walletCard);
    connect(btnGoWallet, &QPushButton::clicked, [this]()
            {
        contentStack->setCurrentWidget(pageWallet);
        setNavActive(btnWallet); });
    c2Layout->addWidget(c2Title);
    c2Layout->addWidget(c2Desc);
    c2Layout->addStretch();
    c2Layout->addWidget(btnGoWallet);

    cardsLayout->addWidget(bookingCard);
    cardsLayout->addWidget(walletCard);

    layout->addWidget(welcomeTitle);
    layout->addWidget(welcomeDesc);
    layout->addLayout(cardsLayout);
    layout->addStretch();
}

void MainWindow::createBookingPage()
{
    pageBooking = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(pageBooking);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(16);

    QFrame *bookingControlFrame = new QFrame(pageBooking);
    bookingControlFrame->setObjectName(QStringLiteral("bookingControlFrame"));
    bookingControlFrame->setMinimumWidth(0);
    bookingControlFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *ctrlLayout = new QVBoxLayout(bookingControlFrame);
    ctrlLayout->setContentsMargins(16, 16, 16, 16);
    ctrlLayout->setSpacing(12);

    QLabel *titleLabel = new QLabel(QStringLiteral("THÔNG TIN ĐẶT XE"), bookingControlFrame);
    titleLabel->setStyleSheet(QStringLiteral("font-size: 16px; font-weight: bold; color: #0F172A; margin-bottom: 6px;"));
    ctrlLayout->addWidget(titleLabel);

    // Pickup
    ctrlLayout->addWidget(new QLabel(QStringLiteral("Điểm đón:"), bookingControlFrame));
    pickupComboBox = new QComboBox(bookingControlFrame);
    ctrlLayout->addWidget(pickupComboBox);

    // Destination
    ctrlLayout->addWidget(new QLabel(QStringLiteral("Điểm đến:"), bookingControlFrame));
    destinationComboBox = new QComboBox(bookingControlFrame);
    ctrlLayout->addWidget(destinationComboBox);

    // Vehicle Type
    ctrlLayout->addWidget(new QLabel(QStringLiteral("Loại phương tiện:"), bookingControlFrame));
    vehicleComboBox = new QComboBox(bookingControlFrame);
    vehicleComboBox->addItem(QStringLiteral("Xe máy (GoBike)"), static_cast<int>(VehicleType::BIKE));
    vehicleComboBox->addItem(QStringLiteral("Ô tô 4 chỗ (GoCar 4)"), static_cast<int>(VehicleType::CAR4));
    vehicleComboBox->addItem(QStringLiteral("Ô tô 7 chỗ (GoCar 7)"), static_cast<int>(VehicleType::CAR7));
    ctrlLayout->addWidget(vehicleComboBox);

    // Info Labels
    distanceLabel = new QLabel(QStringLiteral("Khoảng cách: 0.00 km"), bookingControlFrame);
    distanceLabel->setObjectName(QStringLiteral("distanceLabel"));
    ctrlLayout->addWidget(distanceLabel);

    estimatedFareLabel = new QLabel(QStringLiteral("Giá dự kiến: 0 VNĐ"), bookingControlFrame);
    estimatedFareLabel->setObjectName(QStringLiteral("estimatedFareLabel"));
    ctrlLayout->addWidget(estimatedFareLabel);

    // Driver Selection Mode
    ctrlLayout->addWidget(new QLabel(QStringLiteral("Chọn tài xế mong muốn:"), bookingControlFrame));
    driverComboBox = new QComboBox(bookingControlFrame);
    ctrlLayout->addWidget(driverComboBox);

    findDriverButton = new QPushButton(QStringLiteral("Tìm tài xế phù hợp"), bookingControlFrame);
    ctrlLayout->addWidget(findDriverButton);

    selectedDriverLabel = new QLabel(QStringLiteral("Tài xế: Chưa chọn"), bookingControlFrame);
    selectedDriverLabel->setObjectName(QStringLiteral("selectedDriverLabel"));
    ctrlLayout->addWidget(selectedDriverLabel);

    // Payment Method
    ctrlLayout->addWidget(new QLabel(QStringLiteral("Phương thức thanh toán:"), bookingControlFrame));
    paymentMethodComboBox = new QComboBox(bookingControlFrame);
    paymentMethodComboBox->addItem(QStringLiteral("Ví điện tử"), QStringLiteral("wallet"));
    paymentMethodComboBox->addItem(QStringLiteral("Tiền mặt"), QStringLiteral("cash"));
    ctrlLayout->addWidget(paymentMethodComboBox);

    confirmBookingButton = new QPushButton(QStringLiteral("Xác nhận đặt xe"), bookingControlFrame);
    confirmBookingButton->setObjectName(QStringLiteral("primaryButton"));
    confirmBookingButton->setEnabled(false);
    ctrlLayout->addWidget(confirmBookingButton);

    startTripButton = new QPushButton(QStringLiteral("Bắt đầu chuyến đi"), bookingControlFrame);
    startTripButton->setObjectName(QStringLiteral("primaryButton"));
    startTripButton->setVisible(false);
    ctrlLayout->addWidget(startTripButton);

    ctrlLayout->addStretch();

    const auto configureComboBox = [](QComboBox *comboBox)
    {
        comboBox->setMinimumWidth(0);
        comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        comboBox->setMinimumContentsLength(0);
        QObject::connect(comboBox, &QComboBox::currentTextChanged, comboBox,
                         [comboBox](const QString &text)
                         {
                             comboBox->setToolTip(text);
                         });
    };

    configureComboBox(pickupComboBox);
    configureComboBox(destinationComboBox);
    configureComboBox(vehicleComboBox);
    configureComboBox(driverComboBox);
    configureComboBox(paymentMethodComboBox);

    const auto configureButton = [](QPushButton *button)
    {
        button->setMinimumWidth(0);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    };

    configureButton(findDriverButton);
    configureButton(confirmBookingButton);
    configureButton(startTripButton);

    distanceLabel->setMinimumWidth(0);
    distanceLabel->setWordWrap(true);
    distanceLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    estimatedFareLabel->setMinimumWidth(0);
    estimatedFareLabel->setWordWrap(true);
    estimatedFareLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    selectedDriverLabel->setMinimumWidth(0);
    selectedDriverLabel->setWordWrap(true);
    selectedDriverLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QScrollArea *bookingScrollArea = new QScrollArea(pageBooking);
    bookingScrollArea->setObjectName(QStringLiteral("bookingScrollArea"));
    bookingScrollArea->setWidgetResizable(true);
    bookingScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bookingScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bookingScrollArea->setFrameShape(QFrame::NoFrame);
    bookingScrollArea->setContentsMargins(0, 0, 0, 0);
    bookingScrollArea->setMinimumWidth(370);
    bookingScrollArea->setMaximumWidth(440);
    bookingScrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    bookingScrollArea->setWidget(bookingControlFrame);

    // Map Widget
    mapWidget = new MapWidget(pageBooking);
    mapWidget->setMapSize(16, 16);
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Convert obstacles to Location list
    const auto &obsPairs = MapService::obstacles();
    vector<Location> obsLocs;
    for (const auto &p : obsPairs)
    {
        obsLocs.emplace_back(p.first, p.second, "Vật cản");
    }
    mapWidget->setObstacles(obsLocs);
    mapWidget->setPlaces(places);

    layout->addWidget(bookingScrollArea, 0);
    layout->addWidget(mapWidget, 1);

    // Populate Comboboxes
    for (size_t i = 0; i < places.size(); ++i)
    {
        QString itemText = QString::fromUtf8(places[i].getName().c_str());
        pickupComboBox->addItem(itemText, static_cast<int>(i));
        destinationComboBox->addItem(itemText, static_cast<int>(i));
    }
    pickupComboBox->addItem(QStringLiteral("[Chọn trên bản đồ]"), -1);
    destinationComboBox->addItem(QStringLiteral("[Chọn trên bản đồ]"), -1);
    if (destinationComboBox->count() > 1)
    {
        destinationComboBox->setCurrentIndex(1);
    }

    // Signal connections
    connect(pickupComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onPickupComboChanged);
    connect(destinationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onDestinationComboChanged);
    connect(vehicleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLocationOrVehicleChanged);

    connect(findDriverButton, &QPushButton::clicked, this, &MainWindow::onFindDriverClicked);
    connect(confirmBookingButton, &QPushButton::clicked, this, &MainWindow::onConfirmBookingClicked);
    connect(startTripButton, &QPushButton::clicked, this, &MainWindow::onStartTripClicked);

    connect(mapWidget, &MapWidget::cellClicked, this, &MainWindow::onMapCellClicked);
    connect(mapWidget, &MapWidget::animationFinished, this, &MainWindow::onAnimationFinished);
}

void MainWindow::createHistoryPage()
{
    pageHistory = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(pageHistory);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *title = new QLabel(QStringLiteral("LỊCH SỬ CHUYẾN ĐỊ"), pageHistory);
    title->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold; color: #0F172A; margin-bottom: 10px;"));
    layout->addWidget(title);

    historyTable = new QTableWidget(pageHistory);
    historyTable->setColumnCount(9);
    QStringList headers;
    headers << QStringLiteral("Mã chuyến")
            << QStringLiteral("Điểm đón")
            << QStringLiteral("Điểm đến")
            << QStringLiteral("Tài xế")
            << QStringLiteral("Loại xe")
            << QStringLiteral("Quãng đường")
            << QStringLiteral("Cước phí")
            << QStringLiteral("Thanh toán")
            << QStringLiteral("Đánh giá");
    historyTable->setHorizontalHeaderLabels(headers);
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(historyTable);
}

void MainWindow::createDriversPage()
{
    pageDrivers = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(pageDrivers);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *title = new QLabel(QStringLiteral("DANH SÁCH TÀI XẾ ĐANG HOẠT ĐỘNG"), pageDrivers);
    title->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold; color: #0F172A; margin-bottom: 10px;"));
    layout->addWidget(title);

    driversTable = new QTableWidget(pageDrivers);
    driversTable->setColumnCount(7);
    QStringList headers;
    headers << QStringLiteral("Mã TX")
            << QStringLiteral("Tên tài xế")
            << QStringLiteral("Loại xe")
            << QStringLiteral("Biển số")
            << QStringLiteral("Đánh giá")
            << QStringLiteral("Trạng thái")
            << QStringLiteral("Vị trí");
    driversTable->setHorizontalHeaderLabels(headers);
    driversTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    driversTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    driversTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(driversTable);
}

void MainWindow::createWalletPage()
{
    pageWallet = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(pageWallet);
    layout->setContentsMargins(40, 40, 40, 40);

    QFrame *card = new QFrame(pageWallet);
    card->setObjectName(QStringLiteral("cardFrame"));
    card->setMaximumWidth(500);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(16);

    QLabel *title = new QLabel(QStringLiteral("NẠP TIỀN VÀO VÍ ĐIỆN TỬ"), card);
    title->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold; color: #0F172A;"));
    cardLayout->addWidget(title);

    walletCurrentBalanceLabel = new QLabel(card);
    walletCurrentBalanceLabel->setStyleSheet(QStringLiteral("font-size: 15px; font-weight: bold; color: #2563EB;"));
    cardLayout->addWidget(walletCurrentBalanceLabel);

    cardLayout->addWidget(new QLabel(QStringLiteral("Nhập số tiền nạp (VNĐ, tối thiểu 10.000 VNĐ):"), card));

    walletAmountSpinBox = new QSpinBox(card);
    walletAmountSpinBox->setRange(10000, 10000000);
    walletAmountSpinBox->setSingleStep(50000);
    walletAmountSpinBox->setValue(100000);
    cardLayout->addWidget(walletAmountSpinBox);

    topUpButton = new QPushButton(QStringLiteral("Xác nhận nạp tiền"), card);
    topUpButton->setObjectName(QStringLiteral("primaryButton"));
    connect(topUpButton, &QPushButton::clicked, this, &MainWindow::onTopUpClicked);
    cardLayout->addWidget(topUpButton);

    layout->addWidget(card, 0, Qt::AlignCenter);
    layout->addStretch();
}

void MainWindow::onSidebarNavClicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    if (!senderBtn)
        return;

    if (senderBtn == btnHome)
    {
        contentStack->setCurrentWidget(pageHome);
    }
    else if (senderBtn == btnBooking)
    {
        contentStack->setCurrentWidget(pageBooking);
    }
    else if (senderBtn == btnHistory)
    {
        updateHistoryTable();
        contentStack->setCurrentWidget(pageHistory);
    }
    else if (senderBtn == btnDrivers)
    {
        updateDriversTable();
        contentStack->setCurrentWidget(pageDrivers);
    }
    else if (senderBtn == btnWallet)
    {
        updateWalletDisplay();
        contentStack->setCurrentWidget(pageWallet);
    }

    setNavActive(senderBtn);
}

Location MainWindow::getSelectedLocation(QComboBox *combo, bool isPickup)
{
    int idxData = combo->currentData().toInt();
    if (idxData >= 0 && idxData < static_cast<int>(places.size()))
    {
        return places[idxData].getLocation();
    }
    // Custom location chosen on map
    if (isPickup && hasCustomPickup)
    {
        return customPickup;
    }
    if (!isPickup && hasCustomDropoff)
    {
        return customDropoff;
    }
    return Location(0, 0, "Mặc định");
}

VehicleType MainWindow::getSelectedVehicleType()
{
    return static_cast<VehicleType>(vehicleComboBox->currentData().toInt());
}

void MainWindow::onLocationOrVehicleChanged()
{
    Location pickup = getSelectedLocation(pickupComboBox, true);
    Location dropoff = getSelectedLocation(destinationComboBox, false);
    VehicleType vt = getSelectedVehicleType();

    mapWidget->setPickupLocation(pickup);
    mapWidget->setDestinationLocation(dropoff);

    double dist = pickup.distanceTo(dropoff);
    double fare = FareCalculator::calculate(dist, vt);

    stringstream ssDist, ssFare;
    ssDist << fixed << setprecision(2) << dist;
    distanceLabel->setText(QString::fromUtf8(("Khoảng cách: " + ssDist.str() + " km").c_str()));

    long lFare = static_cast<long>(fare);
    ssFare << lFare;
    estimatedFareLabel->setText(QString::fromUtf8(("Giá dự kiến: " + ssFare.str() + " VNĐ").c_str()));

    updateEligibleDriversList();
}

void MainWindow::updateEligibleDriversList()
{
    driverComboBox->clear();
    driverComboBox->addItem(QStringLiteral("--- Tự động chọn tài xế gần nhất ---"), -1);

    VehicleType vt = getSelectedVehicleType();
    vector<Driver *> candidates = MatchingEngine::findEligibleDrivers(drivers, vt);
    Location pickup = getSelectedLocation(pickupComboBox, true);

    for (size_t i = 0; i < candidates.size(); ++i)
    {
        Driver *d = candidates[i];
        double dist = d->location().distanceTo(pickup);

        stringstream ss;
        ss << d->getName() << " (" << d->getPlate() << ") - " << fixed << setprecision(1) << dist << " km";
        driverComboBox->addItem(QString::fromUtf8(ss.str().c_str()), static_cast<int>(i));
    }
}

void MainWindow::onPickupComboChanged(int index)
{
    int data = pickupComboBox->itemData(index).toInt();
    if (data == -1)
    {
        mapSelectionMode = MapSelectionMode::Pickup;
        statusLabel->setText(QStringLiteral("Hãy chọn điểm đón trên bản đồ."));
        return;
    }
    if (data >= 0)
    {
        hasCustomPickup = false;
        mapSelectionMode = MapSelectionMode::None;
    }
    onLocationOrVehicleChanged();
}

void MainWindow::onDestinationComboChanged(int index)
{
    int data = destinationComboBox->itemData(index).toInt();
    if (data == -1)
    {
        mapSelectionMode = MapSelectionMode::Destination;
        statusLabel->setText(QStringLiteral("Hãy chọn điểm đến trên bản đồ."));
        return;
    }
    if (data >= 0)
    {
        hasCustomDropoff = false;
        mapSelectionMode = MapSelectionMode::None;
    }
    onLocationOrVehicleChanged();
}

void MainWindow::updatePickupComboForCustomLocation(int x, int y)
{
    QSignalBlocker blocker(pickupComboBox);
    QString customText = QString::fromUtf8(("Vị trí tùy chọn (" + to_string(x) + ", " + to_string(y) + ")").c_str());
    int customIdx = pickupComboBox->findData(-2);
    if (customIdx == -1)
    {
        pickupComboBox->addItem(customText, -2);
        customIdx = pickupComboBox->count() - 1;
    }
    else
    {
        pickupComboBox->setItemText(customIdx, customText);
    }
    pickupComboBox->setCurrentIndex(customIdx);
}

void MainWindow::updateDestinationComboForCustomLocation(int x, int y)
{
    QSignalBlocker blocker(destinationComboBox);
    QString customText = QString::fromUtf8(("Vị trí tùy chọn (" + to_string(x) + ", " + to_string(y) + ")").c_str());
    int customIdx = destinationComboBox->findData(-2);
    if (customIdx == -1)
    {
        destinationComboBox->addItem(customText, -2);
        customIdx = destinationComboBox->count() - 1;
    }
    else
    {
        destinationComboBox->setItemText(customIdx, customText);
    }
    destinationComboBox->setCurrentIndex(customIdx);
}

void MainWindow::onFindDriverClicked()
{
    VehicleType vt = getSelectedVehicleType();
    vector<Driver *> candidates = MatchingEngine::findEligibleDrivers(drivers, vt);

    if (candidates.empty())
    {
        QMessageBox::warning(this, QStringLiteral("Không tìm thấy tài xế"),
                             QStringLiteral("Hiện không có tài xế phù hợp đang rảnh. Vui lòng chọn loại xe khác!"));
        confirmBookingButton->setEnabled(false);
        selectedDriverLabel->setText(QStringLiteral("Tài xế: Không có"));
        currentDriver = nullptr;
        return;
    }

    Location pickup = getSelectedLocation(pickupComboBox, true);
    int selectedIdx = driverComboBox->currentData().toInt();

    if (selectedIdx >= 0 && selectedIdx < static_cast<int>(candidates.size()))
    {
        currentDriver = candidates[selectedIdx];
    }
    else
    {
        currentDriver = MatchingEngine::findNearestDriver(pickup, candidates);
    }

    if (currentDriver)
    {
        stringstream ss;
        ss << "Tài xế: " << currentDriver->getName() << " | BS: " << currentDriver->getPlate()
           << " | Sao: " << fixed << setprecision(1) << currentDriver->getRating();
        selectedDriverLabel->setText(QString::fromUtf8(ss.str().c_str()));
        confirmBookingButton->setEnabled(true);

        mapWidget->setDriverPosition(currentDriver->location());
        statusLabel->setText(QStringLiteral("Đã tìm thấy tài xế. Bấm 'Xác nhận đặt xe' để bắt đầu."));
    }
}

void MainWindow::onConfirmBookingClicked()
{
    if (!currentDriver)
        return;

    Location pickup = getSelectedLocation(pickupComboBox, true);
    Location dropoff = getSelectedLocation(destinationComboBox, false);

    if (pickup.x == dropoff.x && pickup.y == dropoff.y)
    {
        QMessageBox::warning(this, QStringLiteral("Tọa độ trùng nhau"),
                             QStringLiteral("Điểm đón và điểm đến trùng nhau, vui lòng chọn lại!"));
        return;
    }

    VehicleType vt = getSelectedVehicleType();
    double dist = pickup.distanceTo(dropoff);
    double fare = FareCalculator::calculate(dist, vt);

    currentTrip.tripId = "TRIP" + to_string(tripCounter++);
    currentTrip.riderId = rider.getId();
    currentTrip.driverId = currentDriver->getId();
    currentTrip.pickup = pickup;
    currentTrip.dropoff = dropoff;
    currentTrip.distanceKm = dist;
    currentTrip.fare = fare;
    currentTrip.status = TripStatus::ACCEPTED;
    currentTrip.driverName = currentDriver->getName();
    currentTrip.driverPlate = currentDriver->getPlate();
    currentTrip.vehicleTypeStr = vehicleTypeToStr(vt);
    currentTrip.paymentMethod = paymentMethodComboBox->currentText().toStdString();

    currentDriver->setAvailable(false);

    // Find BFS path from Driver to Pickup (Phase 1)
    const Map &map = MapService::getMap();
    vector<Location> path1 = PathFinder::findPathBFS(map, currentDriver->location(), pickup);

    currentPhase = TripPhase::PICKUP_PHASE;
    confirmBookingButton->setEnabled(false);
    findDriverButton->setEnabled(false);
    pickupComboBox->setEnabled(false);
    destinationComboBox->setEnabled(false);
    vehicleComboBox->setEnabled(false);

    mapWidget->setPickupLocation(pickup);
    mapWidget->setDestinationLocation(dropoff);
    mapWidget->setDriverPosition(currentDriver->location());
    mapWidget->setPath(path1);
    mapWidget->startAnimation(200);

    statusLabel->setText(QStringLiteral("[GIAI ĐOẠN 1/2] Tài xế đang đến điểm đón..."));
}

void MainWindow::onAnimationFinished()
{
    if (currentPhase == TripPhase::PICKUP_PHASE)
    {
        Location pickup = getSelectedLocation(pickupComboBox, true);
        currentDriver->location() = pickup;
        mapWidget->setDriverPosition(pickup);

        statusLabel->setText(QStringLiteral("Tài xế đã đến điểm đón. Sẵn sàng bắt đầu chuyến đi!"));
        startTripButton->setVisible(true);
        startTripButton->setEnabled(true);
        startTripButton->updateGeometry();
        if (startTripButton->parentWidget())
        {
            startTripButton->parentWidget()->updateGeometry();
        }
    }
    else if (currentPhase == TripPhase::DROPOFF_PHASE)
    {
        Location dropoff = getSelectedLocation(destinationComboBox, false);
        currentDriver->location() = dropoff;
        mapWidget->setDriverPosition(dropoff);

        statusLabel->setText(QStringLiteral("Chuyến đi đã hoàn tất!"));

        // Process Payment
        string method = paymentMethodComboBox->currentData().toString().toStdString();
        bool paid = PaymentService::pay(rider, *currentDriver, currentTrip.fare, method);

        while (!paid)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, QStringLiteral("Số dư không đủ"),
                QString::fromUtf8(("Số dư ví không đủ! Cần thanh toán: " + to_string((long)currentTrip.fare) + " VNĐ. Bạn có muốn chuyển sang thanh toán tiền mặt không?").c_str()),
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                method = "cash";
                currentTrip.paymentMethod = "Tiền mặt";
                paid = PaymentService::pay(rider, *currentDriver, currentTrip.fare, "cash");
            }
            else
            {
                bool ok;
                double amount = QInputDialog::getDouble(
                    this, QStringLiteral("Nạp tiền vào ví"),
                    QStringLiteral("Nhập số tiền nạp thêm (VNĐ):"), 50000, 10000, 10000000, 0, &ok);
                if (ok)
                {
                    rider.addToWallet(amount);
                    updateWalletDisplay();
                    paid = PaymentService::pay(rider, *currentDriver, currentTrip.fare, "wallet");
                }
                else
                {
                    break;
                }
            }
        }

        currentTrip.status = TripStatus::COMPLETED;

        // Rating Dialog
        bool ok;
        int stars = QInputDialog::getInt(
            this, QStringLiteral("Đánh giá tài xế"),
            QString::fromUtf8(("Hãy đánh giá tài xế " + currentDriver->getName() + " (1 - 5 sao):").c_str()),
            5, 1, 5, 1, &ok);
        if (!ok)
            stars = 5;

        currentDriver->addRating(stars);
        currentTrip.ratingGiven = stars;

        rider.addTrip(currentTrip);

        FileHandler::ensureHeader();
        FileHandler::appendTripToFile(currentTrip);

        currentDriver->setAvailable(true);
        updateWalletDisplay();

        QMessageBox::information(
            this, QStringLiteral("Hoàn tất chuyến đi"),
            QString::fromUtf8(("Thanh toán thành công " + to_string((long)currentTrip.fare) + " VNĐ. Cảm ơn bạn đã sử dụng GoRide!").c_str()));

        // Reset Booking Page
        startTripButton->setVisible(false);
        confirmBookingButton->setEnabled(true);
        findDriverButton->setEnabled(true);
        pickupComboBox->setEnabled(true);
        destinationComboBox->setEnabled(true);
        vehicleComboBox->setEnabled(true);
        mapWidget->clearPath();

        currentPhase = TripPhase::COMPLETED;
    }
}

void MainWindow::onStartTripClicked()
{
    startTripButton->setEnabled(false);

    Location pickup = getSelectedLocation(pickupComboBox, true);
    Location dropoff = getSelectedLocation(destinationComboBox, false);

    const Map &map = MapService::getMap();
    vector<Location> path2 = PathFinder::findPathBFS(map, pickup, dropoff);

    currentPhase = TripPhase::DROPOFF_PHASE;
    mapWidget->setPath(path2);
    mapWidget->startAnimation(200);

    statusLabel->setText(QStringLiteral("[GIAI ĐOẠN 2/2] Đang trên đường đến điểm trả..."));
}

void MainWindow::onMapCellClicked(int x, int y)
{
    if (MapService::isBlocked(x, y))
    {
        QMessageBox::warning(this, QStringLiteral("Vị trí không hợp lệ"),
                             QStringLiteral("Không thể chọn vật cản làm địa điểm."));
        return;
    }

    if (mapSelectionMode == MapSelectionMode::None)
    {
        if (pickupComboBox->currentData().toInt() == -1 && destinationComboBox->currentData().toInt() != -1)
        {
            mapSelectionMode = MapSelectionMode::Pickup;
        }
        else if (destinationComboBox->currentData().toInt() == -1 && pickupComboBox->currentData().toInt() != -1)
        {
            mapSelectionMode = MapSelectionMode::Destination;
        }
        else
        {
            statusLabel->setText(QStringLiteral("Hãy chọn điểm đón hoặc điểm đến trước khi bấm lên bản đồ."));
            return;
        }
    }

    switch (mapSelectionMode)
    {
    case MapSelectionMode::Pickup:
    {
        customPickup = Location(x, y, "Tự chọn (" + to_string(x) + "," + to_string(y) + ")");
        hasCustomPickup = true;
        updatePickupComboForCustomLocation(x, y);
        statusLabel->setText(QStringLiteral("Đã chọn điểm đón trên bản đồ."));
        break;
    }
    case MapSelectionMode::Destination:
    {
        customDropoff = Location(x, y, "Tự chọn (" + to_string(x) + "," + to_string(y) + ")");
        hasCustomDropoff = true;
        updateDestinationComboForCustomLocation(x, y);
        statusLabel->setText(QStringLiteral("Đã chọn điểm đến trên bản đồ."));
        break;
    }
    case MapSelectionMode::None:
        return;
    }

    mapSelectionMode = MapSelectionMode::None;
    onLocationOrVehicleChanged();
}

void MainWindow::updateWalletDisplay()
{
    long wallet = static_cast<long>(rider.getWallet());
    stringstream ss;
    ss << "Ví: " << wallet << " VNĐ";
    walletLabel->setText(QString::fromUtf8(ss.str().c_str()));
    walletCurrentBalanceLabel->setText(QString::fromUtf8(("Số dư hiện tại: " + ss.str()).c_str()));
}

void MainWindow::onTopUpClicked()
{
    int amount = walletAmountSpinBox->value();
    rider.addToWallet(amount);
    updateWalletDisplay();

    QMessageBox::information(this, QStringLiteral("Nạp tiền thành công"),
                             QString::fromUtf8(("Đã nạp " + to_string(amount) + " VNĐ vào ví!").c_str()));
}

void MainWindow::updateHistoryTable()
{
    historyTable->setRowCount(0);
    const auto &history = rider.getHistory();

    for (size_t i = 0; i < history.size(); ++i)
    {
        const Trip &t = history[i];
        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        historyTable->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(t.tripId.c_str())));
        historyTable->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(t.pickup.label.c_str())));
        historyTable->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(t.dropoff.label.c_str())));
        historyTable->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(t.driverName.c_str())));
        historyTable->setItem(row, 4, new QTableWidgetItem(QString::fromUtf8(t.vehicleTypeStr.c_str())));

        stringstream ssDist, ssFare;
        ssDist << fixed << setprecision(2) << t.distanceKm << " km";
        ssFare << static_cast<long>(t.fare) << " VNĐ";

        historyTable->setItem(row, 5, new QTableWidgetItem(QString::fromUtf8(ssDist.str().c_str())));
        historyTable->setItem(row, 6, new QTableWidgetItem(QString::fromUtf8(ssFare.str().c_str())));
        historyTable->setItem(row, 7, new QTableWidgetItem(QString::fromUtf8(t.paymentMethod.c_str())));

        string stars(t.ratingGiven, '*');
        historyTable->setItem(row, 8, new QTableWidgetItem(QString::fromUtf8((stars + " (" + to_string(t.ratingGiven) + "/5)").c_str())));
    }
}

void MainWindow::updateDriversTable()
{
    driversTable->setRowCount(0);

    for (size_t i = 0; i < drivers.size(); ++i)
    {
        Driver *d = drivers[i];
        int row = driversTable->rowCount();
        driversTable->insertRow(row);

        driversTable->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(d->getId().c_str())));
        driversTable->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(d->getName().c_str())));
        driversTable->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(vehicleTypeToStr(d->getVehicleType()).c_str())));
        driversTable->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(d->getPlate().c_str())));

        stringstream ssRating, ssLoc;
        ssRating << fixed << setprecision(1) << d->getRating() << " / 5.0";
        ssLoc << "(" << d->location().x << ", " << d->location().y << ")";

        driversTable->setItem(row, 4, new QTableWidgetItem(QString::fromUtf8(ssRating.str().c_str())));

        QTableWidgetItem *statusItem = new QTableWidgetItem(d->isAvailable() ? QStringLiteral("Đang rảnh") : QStringLiteral("Đang bận"));
        statusItem->setForeground(d->isAvailable() ? QBrush(QColor("#10B981")) : QBrush(QColor("#EF4444")));
        driversTable->setItem(row, 5, statusItem);

        driversTable->setItem(row, 6, new QTableWidgetItem(QString::fromUtf8(ssLoc.str().c_str())));
    }
}
