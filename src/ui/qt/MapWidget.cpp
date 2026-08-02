#include "ui/qt/MapWidget.h"
#include <QPainterPath>
#include <algorithm>
#include <cmath>

using namespace std;

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent),
      columns(16),
      rows(16),
      hasDriver(false),
      hasPassenger(false),
      hasPickup(false),
      hasDestination(false),
      currentPathIndex(0),
      carRenderer(QStringLiteral(":/icons/car.svg")),
      driverRenderer(QStringLiteral(":/icons/driver.svg")),
      passengerRenderer(QStringLiteral(":/icons/passenger.svg")),
      pickupRenderer(QStringLiteral(":/icons/pickup.svg")),
      destinationRenderer(QStringLiteral(":/icons/destination.svg")),
      buildingRenderer(QStringLiteral(":/icons/building.svg")),
      schoolRenderer(QStringLiteral(":/icons/school.svg")),
      airportRenderer(QStringLiteral(":/icons/airport.svg")),
      harborRenderer(QStringLiteral(":/icons/harbor.svg")),
      parkRenderer(QStringLiteral(":/icons/park.svg")),
      busStationRenderer(QStringLiteral(":/icons/busStation.svg")),
      mallRenderer(QStringLiteral(":/icons/mall.svg")),
      obstacleRenderer(QStringLiteral(":/icons/obstacle.svg"))
{
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MapWidget::advanceAnimation);
    setMinimumSize(320, 320);
}

void MapWidget::setMapSize(int cols, int rws) {
    columns = cols > 0 ? cols : 16;
    rows = rws > 0 ? rws : 16;
    update();
}

void MapWidget::setObstacles(const vector<Location>& obs) {
    obstacles = obs;
    update();
}

void MapWidget::setPlaces(const vector<Place>& plcs) {
    places = plcs;
    update();
}

void MapWidget::setDriverPosition(const Location& pos) {
    driverPosition = pos;
    hasDriver = true;
    update();
}

void MapWidget::setPassengerPosition(const Location& pos) {
    passengerPosition = pos;
    hasPassenger = true;
    update();
}

void MapWidget::setPickupLocation(const Location& pos) {
    pickupLocation = pos;
    hasPickup = true;
    update();
}

void MapWidget::setDestinationLocation(const Location& pos) {
    destinationLocation = pos;
    hasDestination = true;
    update();
}

void MapWidget::clearDriver() {
    hasDriver = false;
    update();
}

void MapWidget::clearPassenger() {
    hasPassenger = false;
    update();
}

void MapWidget::clearPickup() {
    hasPickup = false;
    update();
}

void MapWidget::clearDestination() {
    hasDestination = false;
    update();
}

void MapWidget::setPath(const vector<Location>& p) {
    path = p;
    currentPathIndex = 0;
    update();
}

void MapWidget::clearPath() {
    path.clear();
    currentPathIndex = 0;
    stopAnimation();
    update();
}

void MapWidget::startAnimation(int intervalMilliseconds) {
    if (path.empty()) {
        emit animationFinished();
        return;
    }
    currentPathIndex = 0;
    animationTimer->start(intervalMilliseconds);
}

void MapWidget::stopAnimation() {
    if (animationTimer->isActive()) {
        animationTimer->stop();
    }
}

void MapWidget::resetAnimation() {
    stopAnimation();
    currentPathIndex = 0;
    if (!path.empty()) {
        driverPosition = path[0];
        hasDriver = true;
    }
    update();
}

void MapWidget::advanceAnimation() {
    if (currentPathIndex >= static_cast<int>(path.size())) {
        animationTimer->stop();
        emit animationFinished();
        return;
    }

    driverPosition = path[currentPathIndex];
    hasDriver = true;
    ++currentPathIndex;

    update();
}

QSize MapWidget::minimumSizeHint() const {
    return QSize(300, 300);
}

QSize MapWidget::sizeHint() const {
    return QSize(600, 600);
}

QRectF MapWidget::calculateMapRect() const {
    double availW = width() - 20.0;
    double availH = height() - 20.0;
    double cellSize = min(availW / columns, availH / rows);
    cellSize = max(10.0, cellSize);

    double mapW = cellSize * columns;
    double mapH = cellSize * rows;

    double left = (width() - mapW) / 2.0;
    double top = (height() - mapH) / 2.0;

    return QRectF(left, top, mapW, mapH);
}

QRectF MapWidget::cellRect(int x, int y) const {
    QRectF mapRect = calculateMapRect();
    double cellSize = mapRect.width() / columns;
    return QRectF(mapRect.left() + x * cellSize, mapRect.top() + y * cellSize, cellSize, cellSize);
}

QPoint MapWidget::cellFromMousePosition(const QPoint& pos) const {
    QRectF mapRect = calculateMapRect();
    if (!mapRect.contains(pos)) return QPoint(-1, -1);

    double cellSize = mapRect.width() / columns;
    int col = static_cast<int>(floor((pos.x() - mapRect.left()) / cellSize));
    int row = static_cast<int>(floor((pos.y() - mapRect.top()) / cellSize));

    if (col >= 0 && col < columns && row >= 0 && row < rows) {
        return QPoint(col, row);
    }
    return QPoint(-1, -1);
}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint cell = cellFromMousePosition(event->pos());
        if (cell.x() >= 0 && cell.y() >= 0) {
            emit cellClicked(cell.x(), cell.y());
        }
    }
    QWidget::mousePressEvent(event);
}

void MapWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    drawBackground(painter);
    drawGrid(painter);
    drawObstacles(painter);
    drawPath(painter);
    drawPlaces(painter);
    drawPickup(painter);
    drawDestination(painter);
    drawPassenger(painter);
    drawDriver(painter);
}

void MapWidget::drawBackground(QPainter& painter) {
    painter.fillRect(rect(), QColor("#F8FAFC"));

    QRectF mapRect = calculateMapRect();
    painter.fillRect(mapRect, QColor("#FFFFFF"));
    painter.setPen(QPen(QColor("#CBD5E1"), 2));
    painter.drawRect(mapRect);
}

void MapWidget::drawGrid(QPainter& painter) {
    QRectF mapRect = calculateMapRect();
    double cellSize = mapRect.width() / columns;

    painter.setPen(QPen(QColor("#E2E8F0"), 1, Qt::SolidLine));
    for (int c = 0; c <= columns; ++c) {
        double x = mapRect.left() + c * cellSize;
        painter.drawLine(QPointF(x, mapRect.top()), QPointF(x, mapRect.bottom()));
    }
    for (int r = 0; r <= rows; ++r) {
        double y = mapRect.top() + r * cellSize;
        painter.drawLine(QPointF(mapRect.left(), y), QPointF(mapRect.right(), y));
    }
}

void MapWidget::drawObstacles(QPainter& painter) {
    for (const auto& obs : obstacles) {
        QRectF rect = cellRect(obs.x, obs.y);
        painter.fillRect(rect, QColor("#E2E8F0"));
        if (obstacleRenderer.isValid()) {
            drawSvgInCell(painter, obstacleRenderer, obs, 0.65);
        } else {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#64748B"));
            painter.drawRoundedRect(rect.adjusted(2, 2, -2, -2), 4, 4);
        }
    }
}

void MapWidget::drawPath(QPainter& painter) {
    if (path.empty()) return;

    // Fill path cells with blue tint
    for (const auto& loc : path) {
        QRectF rect = cellRect(loc.x, loc.y);
        painter.fillRect(rect, QColor(59, 130, 246, 50));
    }

    // Polyline through cell centers
    QPainterPath polyline;
    for (size_t i = 0; i < path.size(); ++i) {
        QPointF center = cellRect(path[i].x, path[i].y).center();
        if (i == 0) {
            polyline.moveTo(center);
        } else {
            polyline.lineTo(center);
        }
    }

    painter.setPen(QPen(QColor("#2563EB"), 3, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(polyline);
}

void MapWidget::drawPlaces(QPainter& painter) {
    for (const auto& place : places) {
        const Location& loc = place.getLocation();
        QRectF targetRect = cellRect(loc.x, loc.y);

        QSvgRenderer* renderer = nullptr;
        switch (place.getType()) {
            case PlaceType::Building: renderer = &buildingRenderer; break;
            case PlaceType::School:   renderer = &schoolRenderer; break;
            case PlaceType::Airport: renderer = &airportRenderer; break;
            case PlaceType::Park: renderer = &parkRenderer; break;
            case PlaceType::Harbor: renderer = &harborRenderer; break;
            case PlaceType::BusStation: renderer = &busStationRenderer; break;
            case PlaceType::Mall:     renderer = &mallRenderer; break;
            case PlaceType::Pickup:   renderer = &pickupRenderer; break;
            case PlaceType::Destination: renderer = &destinationRenderer; break;
        }

        if (renderer && renderer->isValid()) {
            drawSvgInCell(painter, *renderer, loc, 0.65);
        }

        // Draw label if cell is large enough
        if (targetRect.width() >= 24) {
            painter.setPen(QColor("#334155"));
            QFont font = painter.font();
            font.setPixelSize(max(8, static_cast<int>(targetRect.width() * 0.22)));
            font.setBold(true);
            painter.setFont(font);

            QString label = QString::fromUtf8(place.getName().c_str());
            painter.drawText(targetRect.adjusted(1, targetRect.height() * 0.6, -1, 0), Qt::AlignHCenter | Qt::AlignTop, label);
        }
    }
}

void MapWidget::drawPickup(QPainter& painter) {
    if (!hasPickup) return;
    drawSvgInCell(painter, pickupRenderer, pickupLocation, 0.85);
}

void MapWidget::drawDestination(QPainter& painter) {
    if (!hasDestination) return;
    drawSvgInCell(painter, destinationRenderer, destinationLocation, 0.85);
}

void MapWidget::drawPassenger(QPainter& painter) {
    if (!hasPassenger) return;
    drawSvgInCell(painter, passengerRenderer, passengerPosition, 0.75);
}

void MapWidget::drawDriver(QPainter& painter) {
    if (!hasDriver) return;
    drawSvgInCell(painter, carRenderer, driverPosition, 0.85);
}

void MapWidget::drawSvgInCell(QPainter& painter, QSvgRenderer& renderer, const Location& loc, double scale) {
    if (!renderer.isValid()) return;
    QRectF cell = cellRect(loc.x, loc.y);
    QSizeF svgSize(cell.width() * scale, cell.height() * scale);
    QRectF targetRect(QPointF(0, 0), svgSize);
    targetRect.moveCenter(cell.center());
    renderer.render(&painter, targetRect);
}
