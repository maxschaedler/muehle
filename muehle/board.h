#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>

/**
 * @brief The Chip enum represents the state of a field on the board
 */
enum class Chip {NONE, BLACK, WHITE};

/**
 * @brief The Board class manages board positions and their states
 */
class Board
{
public:
    Board();

    /**
     * @brief create initialzes the board with empty fields
     */
    void create();

    /**
     * @brief isMill checks if a field is in a mill
     * @param field the index of the field to check (0-23)
     * @return true if the field is in a mill, otherwise false
     */
    bool isMill(int field) const;

    /**
     * @brief isOccupied checks if a field is occupied
     * @param field the index of the field to check (0-23)
     * @return true if the field is occupied, otherwise false
     */
    bool isOccupied(int field) const;

    /**
     * @brief getNeighbors gets adjacent positions
     * @param field the index of the field to check (0-23)
     * @return a vector containing the neighboring positions
     */
    std::vector<int> getNeighbors(int field) const;

    /**
     * @brief getState gets the state of a field
     * @param field the index of the field (0-23)
     * @return the chip state: NONE, BLACK, or WHITE
     */
    Chip getState(int field) const;

    /**
     * @brief setState sets state of a field
     * @param field the index of the field (0-23)
     * @param the new chip state: NONE, BLACK, or WHITE
     */
    void setState(int field, Chip state);

private:
    std::array<Chip, 24> _state;                            ///< stores the state of all 24 positions
    std::array<std::vector<int>, 24> _adjacent;             ///< adjacency list defining neighboring positions
    std::array<std::array<int,3>, 16> _mills;               ///< all possible mill combinations
};

#endif // BOARD_H
