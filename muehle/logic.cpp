#include "logic.h"

#include <QDebug>
#include <algorithm>

Logic::Logic() {start();}

void Logic::start() {
    qDebug() << "start";
    _board.create();                                        // resets board positions to empty
    _phase = Phase::place;                                  // starts with placing phase again
    _turnColor = Chip::WHITE;                               // white starts by convention
    _whitePlaced = 0;
    _blackPlaced = 0;
}

bool Logic::place(int field, Chip playColor) {

    qDebug() << "place" << field;
    infoText = "Nicht möglich!";

    if (_turnColor != playColor) return false;              // checks color of chip
    if (_phase != Phase::place) return false;               // checks phase
    if (_board.isOccupied(field)) return false;             // checks if field is empty

    if (_turnColor == Chip::BLACK) {                        // tracks number of placed chips
        _blackPlaced += 1;
    } else {
        _whitePlaced += 1;
    }

    if (_blackPlaced == 9 && _whitePlaced == 9) {           // advances to next phase
        _phase = Phase::move;
    }

    _board.setState(field, _turnColor);                     // place chip

    if (_board.isMill(field)) {                             // checks for new mill
        qDebug() << "mill detected";
        infoText = "";
        _phase = Phase::remove;                             // no endTurn if muehle detected
    }else {
        infoText = "";
        endTurn();
    }

    return true;
}

bool Logic::move(int from, int to, Chip playColor) {

    qDebug() << "move" << from << "to" << to;
    infoText = "Nicht möglich!";

    if (_turnColor != playColor) return false;                  // checks color of chip
    if (_phase != Phase::move) return false;                    // checks phase
    if (_board.getState(from) != _turnColor) return false;      // checks if chip is right color
    if (_board.isOccupied(to)) return false;                    // checks if field is empty

    std::vector<int> neighbors = _board.getNeighbors(from);
    if (countChips(_turnColor) > 3) {                           // adjacency only when not jumping
        if (std::find(neighbors.begin(), neighbors.end(), to) == neighbors.end()) return false;
    }

    _board.setState(from, Chip::NONE);                          // move chip
    _board.setState(to, _turnColor);

    if (_board.isMill(to)) {                                    // checks for new mill
        qDebug() << "mill detected";
        infoText = "";
        _phase = Phase::remove;                                 // no endTurn if muehle detected
    }else {
        infoText = "";
        endTurn();
    }

    return true;
}

bool Logic::remove(int choice, Chip playColor) {

    qDebug() << "remove" << choice;
    infoText = "Nicht möglich!";

    std::vector<int> removable;
    Chip opponent = _turnColor == Chip::WHITE ? Chip::BLACK : Chip::WHITE;

    if (opponent != playColor) return false;
    if (_phase != Phase::remove) return false;

    // collect all of the opponents chips that are not in mills
    for (int i = 0; i < 24; ++i) {
        if (_board.getState(i) == opponent && !_board.isMill(i)) {
            removable.push_back(i);
        }
    }

    // if all of the opponents chips are in a mill, allow all chips to be removed
    if (removable.empty()) {
        for (int i = 0; i < 24; ++i) {
            if (_board.getState(i) == opponent && _board.isMill(i)) {
                removable.push_back(i);
            }
        }
    }

    if (std::find(removable.begin(), removable.end(), choice) != removable.end()) {
        _board.setState(choice, Chip::NONE);

        if (_blackPlaced == 9 && _whitePlaced == 9) {
            _phase = Phase::move;
        }
        else
        {
           _phase = Phase::place;
        }
        infoText = "";
        endTurn();
        return true;
    }

    return false;
}

void Logic::endTurn() {

    if (gameOver()) return;

    _turnColor = _turnColor == Chip::WHITE ? Chip::BLACK : Chip::WHITE;
}

int Logic::countChips(Chip color) const {
    int count = 0;
    for (int i = 0; i < 24; ++i) {
        if (_board.getState(i) == color) {
            count++;
        }
    }
    return count;
}

bool Logic::gameOver() const {

    if (_phase != Phase::move) return false;

    Chip opponent = _turnColor == Chip::WHITE ? Chip::BLACK : Chip::WHITE;

    if (countChips(opponent) < 3) return true;

    // check if opponent has at least one valid move
    for (int i = 0; i < 24; ++i) {
        if (_board.getState(i) == opponent) {
            std::vector<int> neighbors = _board.getNeighbors(i);
            for (int n : neighbors) {
                if (_board.getState(n) == Chip::NONE) {
                    return false;
                }
            }
        }
    }

    return true;
}

Chip Logic::getTurnColor() const {
    return _turnColor;
}

QString Logic::getinfo() {
    if(! gameOver()) {
        switch (_phase) {
            case Phase::place:
                actionText = "Bitte Chip platzieren"; break;
            case Phase::move:
                actionText = "Bitte Chip bewegen"; break;
            case Phase::remove:
                actionText = "Mühle! Bitte Chip entfernen"; break;
            default:
                actionText = "Unmöglich"; break;
        }

        if (_turnColor == Chip::WHITE) {
            return "Am Zug: Weiß\n" + actionText + "\n" + infoText;
        } else {
            return "Am Zug: Schwarz\n" + actionText + "\n" + infoText;
        }
    }
    if (_turnColor == Chip::WHITE) {
        return "Weiß gewinnt!\nDanke fürs Spielen.";
    } else {
        return "Schwarz gewinnt!\nDanke fürs Spielen.";
    }
}
