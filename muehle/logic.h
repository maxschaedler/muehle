#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"
#include <QString>

class Logic
{
public:
    Logic(); ///< calls start initially

    /**
     * @brief start resets the game back to starting conditions
     */
    void start();

    /**
     * @brief places a chip on the board
     * @param field the index of the field where the chip is placed (0-23)
     * @param playColor the color of the chip being placed (BLACK or WHITE)
     * @return true if the placement is successful, otherwise false
     */
    bool place(int field, Chip playColor);

    /**
     * @brief moves a chip from one position to another (includes jumping)
     * @param from the index of the starting position (0-23)
     * @param to the index of the destination position (0-23)
     * @param playColor the color of the chip being moved
     * @return true if the move is valid, otherwise false
     */
    bool move(int from, int to, Chip playColor);

    /**
     * @brief removes an opponent's chip from the board
     * @param choice the index of the chip to remove (0-23)
     * @param playColor the color of the player performing the removal
     * @return true if the removal is valid, otherwise false
     */
    bool remove(int choice, Chip playColor);

    /**
     * @brief switches to the opponents color
     */
    void endTurn();

    /**
     * @brief counts the number of chips of a given color on the board
     * @param color the chip color to count
     * @return the number of chips of the specified color
     */
    int countChips(Chip color) const;

    /**
     * @brief checks if the game is over based on chip count and the opponent being able to move
     * @return true if the game is over, otherwise false
     */
    bool gameOver() const;

    /**
     * @brief gets the color of the player whose turn it is
     * @return the current player's color (BLACK or WHITE)
     */
    Chip getTurnColor() const;

    /**
     * @brief retrieves information about the color in play, necessary action, move validity
     * @return a QString containing the game information
     */
    QString getinfo();


private:
    enum class Phase {place, move, remove};                     ///< enum for tracking the game phase

    Phase _phase;                                               ///< stores the current game phase
    Chip _turnColor = Chip::WHITE;                              ///< stores the current turn color
    Board _board;                                               ///< represents the game board

    int _whitePlaced = 0;                                       ///< number of white chips placed on the board
    int _blackPlaced = 0;                                       ///< number of black chips placed on the board

    QString actionText;                                         ///< stores information about the necessary action based on phase
    QString infoText;                                           ///< stores information about move validity

};

#endif // LOGIC_H
