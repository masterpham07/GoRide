#pragma once

#include <QWidget>
#include <QTimer>
#include <QSvgRenderer>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>

#include "models/Location.h"
#include "models/Place.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr);
    ~MapWidget() override = default;

    void setMapSize(int columns, int rows);

    void setObstacles(const std::vector<Location>& obstacles);
    void setPlaces(const std::vector<Place>& places);

    void setDriverPosition(const Location& position);
    void setPassengerPosition(const Location& position);
    void setPickupLocation(const Location& position);
    void setDestinationLocation(const Location& position);

    void clearDriver();
    void clearPassenger();
    void clearPickup();
    void clearDestination();

    void setPath(const std::vector<Location>& path);
    void clearPath();

    void startAnimation(int intervalMilliseconds = 200);
    void stopAnimation();
    void resetAnimation();

signals:
    void cellClicked(int x, int y);
    void animationFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private slots:
    void advanceAnimation();

private:
    int columns;
    int rows;

    std::vector<Location> obstacles;
    std::vector<Place> places;
    std::vector<Location> path;

    Location driverPosition;
    Location passengerPosition;
    Location pickupLocation;
    Location destinationLocation;

    bool hasDriver;
    bool hasPassenger;
    bool hasPickup;
    bool hasDestination;

    int currentPathIndex;

    QTimer* animationTimer;

    QSvgRenderer carRenderer;
    QSvgRenderer driverRenderer;
    QSvgRenderer passengerRenderer;
    QSvgRenderer pickupRenderer;
    QSvgRenderer destinationRenderer;
    QSvgRenderer buildingRenderer;
    QSvgRenderer schoolRenderer;
    QSvgRenderer airportRenderer;
    QSvgRenderer harborRenderer;
    QSvgRenderer busStationRenderer;
    QSvgRenderer parkRenderer;
    QSvgRenderer mallRenderer;
    QSvgRenderer obstacleRenderer;

    QRectF calculateMapRect() const;
    QRectF cellRect(int x, int y) const;
    QPoint cellFromMousePosition(const QPoint& position) const;

    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawObstacles(QPainter& painter);
    void drawPath(QPainter& painter);
    void drawPlaces(QPainter& painter);
    void drawPickup(QPainter& painter);
    void drawDestination(QPainter& painter);
    void drawPassenger(QPainter& painter);
    void drawDriver(QPainter& painter);

    void drawSvgInCell(
        QPainter& painter,
        QSvgRenderer& renderer,
        const Location& location,
        double scale = 0.7
    );
};
