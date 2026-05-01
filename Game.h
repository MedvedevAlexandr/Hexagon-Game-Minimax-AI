#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "HexBoard.h"

/**
 * @brief Класс осуществляющий управление всей игрой и событиями в приложении. Также он отвечает за
 * расположение всех элементов в графической сцене приложения.
 */
class Game: public QGraphicsView {
public:
    Game();
    void start();
    QString get_current_turn();
    void set_current_turn(QString cur);
    void next_turn();
    void update_count();
    void make_comp();
    void make_comp_smart();
    void make_turn(int hex_id, QString player);
    int count_turn(int hex_id, QString player);
    std::pair<int, std::vector<int>> minimax(const std::vector<int>& state, int depth, int alpha, int beta, bool maximizingPlayer);

    // public attributes
    QGraphicsScene* scene;
    HexBoard* hexBoard;
    QString Turn;
    QGraphicsTextItem* whoisTurnText;
    QGraphicsTextItem* RedCount;
    QGraphicsTextItem* BlueCount;
    int red_count;
    int blue_count;
    int end_game_flag;
    int ai_turn_count;
    QString player_side = "red";
    QString computer_side = "blue";
};

#endif // GAME_H