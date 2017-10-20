from constants import *


class LogicalBoard:

    def __init__(self, columns, rows, c, p):
        self.c = c
        self.p = p
        self.columns = columns
        self.rows = rows
        self.reset()

    def __getitem__(self, index):
        return self.board[index]

    def getLowestEmptySpace(self, column):
        # Return the row number of the lowest empty row in the given column.
        for y in range(self.rows - 1, -1, -1):
            if self.board[column][y] == EMPTY:
                return y
        return -1

    def isValidMove(self, column):
        # if there is no more pieces to play, is not a valid move
        if not self.availableMoves():
            return False

        # Returns True if there is an empty space in the given column.
        # Otherwise returns False.
        if column < 0 or column >= (self.columns) or self.board[column][0] != EMPTY:
            return False
        return True

    def makeMove(self, player, column):
        lowest = self.getLowestEmptySpace(column)
        if lowest != -1:
            self.board[column][lowest] = player
            self.madeMoves += 1

    def undoMove(self, column):
        lowest = self.getLowestEmptySpace(column) + 1
        if lowest < self.rows:
            self.board[column][lowest] = EMPTY
            self.madeMoves -= 1

    def isBoardFull(self):
        # Returns True if there are no empty spaces anywhere on the board.
        for x in range(self.columns):
            for y in range(self.rows):
                if self.board[x][y] == EMPTY:
                    return False
        return True

    def availableMoves(self):
        return self.madeMoves < 2 * self.p

    # TODO: mirar que sea correcto el self.c - 1 en todos los ranges
    def isWinner(self, tile):
        # check horizontal spaces
        for x in range(self.columns - self.c + 1):
            for y in range(self.rows):
                if all([self.board[x + i][y] == tile for i in range(self.c)]):
                    return True
        # check vertical spaces
        for x in range(self.columns):
            for y in range(self.rows - self.c + 1):
                if all([self.board[x][y + i] == tile for i in range(self.c)]):
                    return True
        # check / diagonal spaces
        for x in range(self.columns - self.c + 1):
            for y in range(self.c - 1, self.rows):
                if all([self.board[x + i][y - i] == tile for i in range(self.c)]):
                    return True
        # check \ diagonal spaces
        for x in range(self.columns - self.c + 1):
            for y in range(self.rows - self.c + 1):
                if all([self.board[x + i][y + i] == tile for i in range(self.c)]):
                    return True
        return False

    def reset(self):
        self.madeMoves = 0
        self.board = []
        for x in range(self.columns):
            self.board.append([EMPTY] * self.rows)
