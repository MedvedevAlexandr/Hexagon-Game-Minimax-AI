#include "Hex.h"
#include <QPointF>
#include <QPolygonF>
#include <QVector>
#include <QBrush>
#include <QGraphicsScene>
#include "Game.h"

extern Game* game;

Hex::Hex(int new_id) {
    QVector<QPointF> hexPoints;
    hexPoints << QPointF(1,0) << QPointF(2,0) << QPointF(3,1) << QPointF(2,2) << QPointF(1,2) << QPointF(0,1);
    int SCALE_BY = 40;
    for (size_t i = 0, n = hexPoints.size(); i < n; ++i) {
        hexPoints[i] = hexPoints[i] * SCALE_BY;
    }
    QPolygonF hexagon(hexPoints);
    setPolygon(hexagon);
    owner = "neutral";
    create_lines();
    id = new_id;
}

void Hex::set_owner(QString new_owner) {
    owner = new_owner;
    if(new_owner == "red") {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        setBrush(brush);
    }
    if(new_owner == "blue") {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::blue);
        setBrush(brush);
    }
}

void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(game->end_game_flag != 1) {
        if(game->get_current_turn() == game->player_side) {
            int flag = 0;
            if(get_owner() == QString("neutral")) {
                for(auto neighbor: neighbors) {
                    if(neighbor->get_owner() == game->get_current_turn()) {
                        set_owner(game->get_current_turn());
                        for(auto neighbor2: neighbors) {
                            if(neighbor2->get_owner() != QString("neutral")) {
                                neighbor2->set_owner(game->get_current_turn());
                            }
                        }
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0) return;
                game->update_count();
                game->next_turn();
                game->make_comp_smart();
            }
        }
    }
}

QString Hex::get_owner() {
    return owner;
}

void Hex::create_lines() {
    QPointF hexCenter(x() + 60, y() + 40);
    QPointF finalPt(hexCenter.x(), hexCenter.y() - 65);
    QLineF ln(hexCenter, finalPt);
    for (size_t i = 0, n = 6; i < n; ++i) {
        QLineF lnCopy(ln);
        lnCopy.setAngle(90 + 60 * i);
        QGraphicsLineItem* line = new QGraphicsLineItem(lnCopy, this);
        lines.append(line);
        line->setVisible(false);
    }
}

void Hex::find_neighbors() {
    for (size_t i = 0, n = lines.size(); i < n; i++) {
        QList<QGraphicsItem*> colliding = lines[i]->collidingItems();
        for (auto item : colliding) {
            Hex* hex = dynamic_cast<Hex*>(item);
            if(hex && hex != this && !neighbors.contains(hex)) {
                neighbors.append(hex);
            }
        }
    }
}

QList<Hex*> Hex::get_neighbors() {
    return neighbors;
}

int Hex::get_id() {
    return id;
}

void Hex::set_id(int new_id) {
    id = new_id;
}