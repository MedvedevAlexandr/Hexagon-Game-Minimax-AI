#include "HexBoard.h"
#include "Hex.h"
#include <QGraphicsTextItem>

HexBoard::HexBoard() {
    cur_id = 0;
}

QList<Hex*> HexBoard::getHexes() {
    return hexes;
}

void HexBoard::placeHexes(int x, int y) {
    int X_SHIFT = 82;
    int Y_SHIFT = 41;
    createHexColumn(x + X_SHIFT * 1, y + Y_SHIFT * 5, 5);
    createHexColumn(x + X_SHIFT * 2, y + Y_SHIFT * 4, 6);
    createHexColumn(x + X_SHIFT * 3, y + Y_SHIFT * 3, 7);
    createHexColumn(x + X_SHIFT * 4, y + Y_SHIFT * 2, 8);
    createHexColumn(x + X_SHIFT * 5, y + Y_SHIFT,     9);
    createHexColumn(x + X_SHIFT * 6, y + Y_SHIFT * 2, 8);
    createHexColumn(x + X_SHIFT * 7, y + Y_SHIFT * 3, 7);
    createHexColumn(x + X_SHIFT * 8, y + Y_SHIFT * 4, 6);
    createHexColumn(x + X_SHIFT * 9, y + Y_SHIFT * 5, 5);
    hexes[2]->set_owner(QString("red"));
    hexes[58]->set_owner(QString("blue"));
}

int HexBoard::get_len() {
    return hexes.length();
}

void HexBoard::createHexColumn(int x, int y, int numOfRows) {
    for (size_t i = 0, n = numOfRows; i < n; i++) {
        Hex* hex = new Hex(cur_id);
        QGraphicsTextItem* hex_id = new QGraphicsTextItem();
        hex_id->setPlainText(QString::number(cur_id));
        hex->setPos(x, y + i * 80);
        hex->set_id(cur_id);
        hex->hex_id = hex_id;
        hex_id->setPos(x + 60, y + i * 80 + 30);
        game->scene->addItem(hex);
        game->scene->addItem(hex_id);
        hexes.append(hex);
        cur_id++;
    }
}