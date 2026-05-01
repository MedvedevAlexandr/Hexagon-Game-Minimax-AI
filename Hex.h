#ifndef HEX_H
#define HEX_H

#include <QGraphicsPolygonItem>
#include <QList>
#include <QGraphicsTextItem>

/**
 * @brief Класс хранящий в себе информацию об одном гексе игрового поля.
 */
class Hex: public QGraphicsPolygonItem {
public:
    Hex(int new_id);
    QString get_owner();
    void set_owner(QString new_owner);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void create_lines();
    void find_neighbors();
    QList<Hex*> get_neighbors();
    int get_id();
    void set_id(int new_id);

    QString owner;
    int id;
    QGraphicsTextItem* hex_id;
    QList<Hex*> neighbors;
    QList<QGraphicsLineItem*> lines;
};

#endif // HEX_H