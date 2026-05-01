#include "Game.h"
#include "HexBoard.h"
#include "Hex.h"
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>

extern Game* game;

Game::Game() {
    scene = new QGraphicsScene();
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1600, 900);
    scene->setSceneRect(0, 0, 1600, 900);
    hexBoard = new HexBoard();
    hexBoard->placeHexes(0, 0);
    red_count = 0;
    blue_count = 0;
    end_game_flag = 0;
    ai_turn_count = 0;
    Turn = "red";
    for(auto hex: hexBoard->getHexes()) {
        hex->find_neighbors();
    }
}

void Game::start() {
    // отображение счёта и текста хода
    whoisTurnText = new QGraphicsTextItem();
    whoisTurnText->setPos(800, 0);
    whoisTurnText->setPlainText(QString("TURN: ") + get_current_turn());
    scene->addItem(whoisTurnText);

    RedCount = new QGraphicsTextItem();
    RedCount->setPos(800, 50);
    scene->addItem(RedCount);
    BlueCount = new QGraphicsTextItem();
    BlueCount->setPos(800, 100);
    scene->addItem(BlueCount);
    update_count();
}

QString Game::get_current_turn() {
    return Turn;
}

void Game::next_turn() {
    if(get_current_turn() == "red") {
        set_current_turn(QString("blue"));
        whoisTurnText->setPlainText(QString("TURN: ") + get_current_turn());
    } else {
        set_current_turn(QString("red"));
        whoisTurnText->setPlainText(QString("TURN: ") + get_current_turn());
    }

    QList<Hex*> available_computer_moves;
    QList<Hex*> available_player_moves;
    QList<Hex*> player_hex;
    QList<Hex*> computer_hex;

    for(auto hex: hexBoard->getHexes()) {
        if(hex->get_owner() == computer_side) computer_hex.append(hex);
        if(hex->get_owner() == player_side) player_hex.append(hex);
    }

    for(auto hex: player_hex) {
        for(auto nbr: hex->get_neighbors()) {
            if(nbr->get_owner() == QString("neutral")) available_player_moves.append(nbr);
        }
    }
    for(auto hex: computer_hex) {
        for(auto nbr: hex->get_neighbors()) {
            if(nbr->get_owner() == QString("neutral")) available_computer_moves.append(nbr);
        }
    }

    if(red_count + blue_count == 61 || red_count == 0 || blue_count == 0 ||
       available_computer_moves.length() == 0 || available_player_moves.length() == 0) {
        end_game_flag = 1;
        QGraphicsTextItem* GameResult = new QGraphicsTextItem();
        GameResult->setPos(550, 60);
        if(red_count > blue_count || blue_count == 0) {
            GameResult->setPlainText(QString("RED WIN"));
        } else if(blue_count > red_count || red_count == 0) {
            GameResult->setPlainText(QString("BLUE WIN"));
        } else {
            GameResult->setPlainText(QString("DRAW"));
        }
        scene->addItem(GameResult);
    }
}

void Game::set_current_turn(QString cur) {
    Turn = cur;
}

void Game::update_count() {
    QList<Hex*> board = hexBoard->getHexes();
    int new_red_count = 0;
    int new_blue_count = 0;
    for(auto hex: board) {
        if(hex->get_owner() == QString("red")) new_red_count++;
        if(hex->get_owner() == QString("blue")) new_blue_count++;
    }
    red_count = new_red_count;
    blue_count = new_blue_count;
    RedCount->setPlainText(QString("RED COUNT: ") + QString::number(red_count));
    BlueCount->setPlainText(QString("BLUE COUNT: ") + QString::number(blue_count));
}

void Game::make_comp() {
    if(end_game_flag != 1) {
        srand(time(NULL));
        QList<Hex*> available_n;
        QList<Hex*> available_o;
        for(auto hex: hexBoard->getHexes()) {
            if(hex->get_owner() == computer_side) {
                for(auto nbr: hex->get_neighbors()) {
                    if(nbr->get_owner() == QString("neutral") || nbr->get_owner() == player_side) {
                        available_n.append(nbr);
                    } else {
                        available_o.append(nbr);
                    }
                }
            }
        }
        if(available_n.length() > 0) {
            Hex* move = available_n[rand() % available_n.length()];
            QGraphicsTextItem* new_turn_text = new QGraphicsTextItem();
            new_turn_text->setPos(1100, 20 * ai_turn_count);
            new_turn_text->setPlainText(QString("AI TURN: ") + QString::number(move->get_id()) + QString(" HEX"));
            scene->addItem(new_turn_text);
            move->set_owner(computer_side);
            for(auto nei: move->get_neighbors()) {
                if(nei->get_owner() == player_side) nei->set_owner(computer_side);
            }
        } else {
            Hex* move = available_n[rand() % available_n.length()];
            QGraphicsTextItem* new_turn_text = new QGraphicsTextItem();
            new_turn_text->setPos(1100, 20 * ai_turn_count);
            new_turn_text->setPlainText(QString("AI TURN: ") + QString::number(move->get_id()) + QString(" HEX"));
            scene->addItem(new_turn_text);
            move->set_owner(computer_side);
            for(auto nei: move->get_neighbors()) {
                if(nei->get_owner() == player_side) nei->set_owner(computer_side);
            }
        }
        QGraphicsTextItem* new_turn_text = new QGraphicsTextItem();
        new_turn_text->setPos(1100, 20 * ai_turn_count);
        new_turn_text->setPlainText(QString("AI TURN"));
        scene->addItem(new_turn_text);
        ai_turn_count++;
        update_count();
        next_turn();
    }
}

void Game::make_turn(int hex_id, QString player) {
    for(auto hex: hexBoard->getHexes()) {
        if(hex->get_id() == hex_id) {
            hex->set_owner(player);
            hex->find_neighbors();
            for(auto nei: hex->get_neighbors()) {
                if(nei->get_owner() != player && nei->get_owner() != QString("neutral")) {
                    nei->set_owner(player);
                }
            }
            break;
        }
    }
}

int Game::count_turn(int hex_id, QString player) {
    int out_num = 0;
    for(auto hex: hexBoard->getHexes()) {
        if(hex->get_id() == hex_id) {
            out_num++;
            for(auto nei: hex->get_neighbors()) {
                if(nei->get_owner() != player && nei->get_owner() != QString("neutral")) {
                    out_num++;
                }
            }
            return out_num;
        }
    }
    return 0;
}

void Game::make_comp_smart() {
    QList<Hex*> available;
    for(auto hex: hexBoard->getHexes()) {
        if(hex->get_owner() == computer_side) {
            for(auto nbr: hex->get_neighbors()) {
                if(!available.contains(nbr) && nbr->get_owner() == QString("neutral")) {
                    available.append(nbr);
                }
            }
        }
    }
    int max_val = -1;
    int arr[available.length()];
    for(int ind = 0; ind < available.length(); ind++) {
        arr[ind] = count_turn(available[ind]->get_id(), computer_side);
        if(arr[ind] > max_val) max_val = arr[ind];
    }
    for(int i = 0; i < available.length(); i++) {
        if(arr[i] == max_val) {
            make_turn(available[i]->get_id(), computer_side);
            QGraphicsTextItem* new_turn_text = new QGraphicsTextItem();
            new_turn_text->setPos(1100, 20 * ai_turn_count);
            new_turn_text->setPlainText(QString("AI TURN: ") + QString::number(available[i]->get_id()) + QString(" HEX"));
            scene->addItem(new_turn_text);
            break;
        }
    }
    if(end_game_flag) {
        std::vector<int> board;
        minimax(board, 0, 0, 0, 1);
    }
    ai_turn_count++;
    update_count();
    next_turn();
}

std::pair<int, std::vector<int>> Game::minimax(const std::vector<int>& state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if(depth == 0) {
        return {count_turn(state[0], computer_side), state};
    }
    if(maximizingPlayer) {
        int maxEval = -1000000;
        std::vector<int> bestState = state;
        std::vector<int> moves = state;
        for(const auto& move : moves) {
            std::pair<int, std::vector<int>> result = minimax(state, depth - 1, alpha, beta, false);
            int eval = result.first;
            if(eval > maxEval) {
                maxEval = eval;
            }
            if(maxEval > alpha) alpha = maxEval;
            if(beta <= alpha) break;
        }
        return {maxEval, bestState};
    } else {
        int minEval = 1000000;
        std::vector<int> bestState = state;
        // ... (далее по аналогии, но в исходном коде обрывается – оставлено как есть)
        return {minEval, bestState};
    }
}