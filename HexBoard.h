#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QList>
#include "Hex.h"

/**
 * @brief Класс, хранящий в себе игровую доску.
 * @details Класс хранит в себе список экземпляров класса Hex, описывающий всю игровую доску в приложении.
 */
class HexBoard {
public:
    HexBoard();
    QList<Hex*> getHexes();
    int get_len();
    void placeHexes(int x, int y);
    void createHexColumn(int x, int y, int numOfRows);

    int cur_id;
    QList<Hex*> hexes;
};

#endif // HEXBOARD_H