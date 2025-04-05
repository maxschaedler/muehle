#include "board.h"

Board::Board() {
    _adjacent[0] = std::vector<int>{1,7,8};
    _adjacent[1] = std::vector<int>{0,2};
    _adjacent[2] = std::vector<int>{1,3,10};
    _adjacent[3] = std::vector<int>{2,4};
    _adjacent[4] = std::vector<int>{3,5,12};
    _adjacent[5] = std::vector<int>{4,6};
    _adjacent[6] = std::vector<int>{5,7,14};
    _adjacent[7] = std::vector<int>{0,6};

    _adjacent[8] = std::vector<int>{0,9,15,16};
    _adjacent[9] = std::vector<int>{8,10};
    _adjacent[10] = std::vector<int>{2,9,11,18};
    _adjacent[11] = std::vector<int>{10,12};
    _adjacent[12] = std::vector<int>{4,11,13,20};
    _adjacent[13] = std::vector<int>{12,14};
    _adjacent[14] = std::vector<int>{6,13,15,22};
    _adjacent[15] = std::vector<int>{8,14};

    _adjacent[16] = std::vector<int>{8,17,23};
    _adjacent[17] = std::vector<int>{16,18};
    _adjacent[18] = std::vector<int>{10,17,19};
    _adjacent[19] = std::vector<int>{18,20};
    _adjacent[20] = std::vector<int>{12,19,21};
    _adjacent[21] = std::vector<int>{20,22};
    _adjacent[22] = std::vector<int>{14,21,23};
    _adjacent[23] = std::vector<int>{16,22};

    _mills = {{
        {7,0,1}, {1,2,3}, {3,4,5}, {5,6,7},                 // outer mills
        {15,8,9}, {9,10,11}, {11,12,13}, {13,14,15},        // middle mills
        {23,16,17}, {17,18,19}, {19,20,21}, {21,22,23},     // inner mills
        {0,8,16}, {2,10,18}, {4,12,20}, {6,14,22}           // vertical mills
    }};
}

void Board::create() {
    for (int i = 0; i < 24; ++i) {
        _state[i] = Chip::NONE;
    }
}

bool Board::isMill(int field) const {

    Chip color = _state[field];                      // get the color of the placed chip
    if (color == Chip::NONE) return false;           // no chip, no mill

    for (const auto& mill : _mills) {
        if (_state[mill[0]] == color && _state[mill[1]] == color && _state[mill[2]] == color) {
            if (mill[0] == field || mill[1] == field || mill[2] == field) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isOccupied(int field) const {
    return _state[field] != Chip::NONE;
}

std::vector<int> Board::getNeighbors(int field) const {
    return _adjacent[field];
}

Chip Board::getState(int field) const {
    return _state[field];
}

void Board::setState(int field, Chip color) {
    _state[field] = color;
}
