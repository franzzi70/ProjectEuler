#include "Board.h"

bool Board::groupsInitialized = false;
char Board::squares[9][9];
char Board::lines[9][9];
char Board::columns[9][9];
int Board::groupMembership[81][3];

