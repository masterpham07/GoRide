#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <memory>
#include <vector>

#include "models/User.h"
#include "models/Trip.h"
#include "models/Place.h"
#include "ui/qt/MapWidget.h"

enum class TripPhase {
    IDLE,
    PICKUP_PHASE,
    DROPOFF_PHASE,
    COMPLETED
};

enum class MapSelectionMode {
    None,
    Pickup,
    Destination
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onSidebarNavClicked();
    void onLocationOrVehicleChanged();
    void onPickupComboChanged(int index);
    void onDestinationComboChanged(int index);
    void onFindDriverClicked();
    void onConfirmBookingClicked();
    void onStartTripClicked();
    void onMapCellClicked(int x, int y);
    void onAnimationFinished();
    void onTopUpClicked();

private:
    // Core Data Models & Services
    Rider rider;
    std::vector<std::unique_ptr<Driver>> driverStorage;
    std::vector<Driver*> drivers;
    std::vector<Place> places;

    Trip currentTrip;
    Driver* currentDriver;
    int tripCounter;
    TripPhase currentPhase;
    MapSelectionMode mapSelectionMode;
    Location customPickup;
    Location customDropoff;
    bool hasCustomPickup;
    bool hasCustomDropoff;

    // GUI Components
    QLabel* logoLabel;
    QLabel* greetingLabel;
    QLabel* walletLabel;
    QLabel* statusLabel;

    QFrame* headerFrame;
    QFrame* sidebarFrame;
    QStackedWidget* contentStack;

    QPushButton* btnHome;
    QPushButton* btnBooking;
    QPushButton* btnHistory;
    QPushButton* btnDrivers;
    QPushButton* btnWallet;
    QPushButton* btnExit;

    // Pages
    QWidget* pageHome;
    QWidget* pageBooking;
    QWidget* pageHistory;
    QWidget* pageDrivers;
    QWidget* pageWallet;

    // Booking Page Components
    QComboBox* pickupComboBox;
    QComboBox* destinationComboBox;
    QComboBox* vehicleComboBox;
    QComboBox* driverComboBox;
    QComboBox* paymentMethodComboBox;
    QLabel* distanceLabel;
    QLabel* estimatedFareLabel;
    QLabel* selectedDriverLabel;
    QPushButton* findDriverButton;
    QPushButton* confirmBookingButton;
    QPushButton* startTripButton;
    MapWidget* mapWidget;

    // History & Drivers Tables
    QTableWidget* historyTable;
    QTableWidget* driversTable;

    // Wallet Components
    QLabel* walletCurrentBalanceLabel;
    QSpinBox* walletAmountSpinBox;
    QPushButton* topUpButton;

    // Helper Setup Functions
    void initData();
    void setupUi();
    void createHeader();
    void createSidebar();
    void createContentStack();

    void createHomePage();
    void createBookingPage();
    void createHistoryPage();
    void createDriversPage();
    void createWalletPage();

    void updateWalletDisplay();
    void updateHistoryTable();
    void updateDriversTable();
    void updateEligibleDriversList();
    void updatePickupComboForCustomLocation(int x, int y);
    void updateDestinationComboForCustomLocation(int x, int y);
    Location getSelectedLocation(QComboBox* combo, bool isPickup);
    VehicleType getSelectedVehicleType();
    void setNavActive(QPushButton* activeButton);
};
