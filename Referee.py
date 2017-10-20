import sys

from LogicalBoard import *
from GraphicalBoard import *
from constants import *


class Referee:

    def __init__(self, columns, rows, c, p, player1, player2, show_ui=False):

        self.player1 = player1
        self.player2 = player2

        self.logicalBoard = LogicalBoard(columns, rows, c, p)

        if show_ui:
            self.graphicalBoard = GraphicalBoard(self.logicalBoard)
        else:
            self.graphicalBoard = None

        assert self.graphicalBoard is not None or (not player1.human and not player2.human)

    def runPlay(self, first_player):

        self.logicalBoard.reset()
        if self.graphicalBoard is not None:
            self.graphicalBoard.draw()

        self.last_move = None
        self.turn = first_player.color

        self.player1.startGame(self.logicalBoard.columns,
                               self.logicalBoard.rows,
                               self.logicalBoard.c,
                               self.logicalBoard.p,
                               self.turn == self.player1.color)
        self.player2.startGame(self.logicalBoard.columns,
                               self.logicalBoard.rows,
                               self.logicalBoard.c,
                               self.logicalBoard.p,
                               self.turn == self.player2.color)

        while True:  # main game loop

            # Checks if the human wants to exit
            if self.graphicalBoard is not None:
                self.check_exit(self.graphicalBoard.check_user_exits())

            # Select correct player to move
            if self.turn == self.player1.color:
                player = self.player1
                oponent = self.player2
            else:
                player = self.player2
                oponent = self.player1

            # make the player move
            if player.human:  # if human playing graphical board exists, previously checked by the __init__ assert
                self.last_move = self.graphicalBoard.getHumanMove(player.color)
                self.check_exit(self.last_move)
            else:
                column = player.move(self.last_move)
                if self.graphicalBoard is not None:
                    self.graphicalBoard.animateComputerMove(column, player.color)
                else:
                    self.logicalBoard.makeMove(player.color, column)
                self.last_move = column

            # check if the game has reached the end
            if self.logicalBoard.isWinner(player.color):
                winner = player.color
                player.finished(WIN)
                oponent.finished(LOSE)
                break
            elif self.logicalBoard.isWinner(oponent.color):
                winner = oponent.color
                player.finished(LOSE)
                oponent.finished(WIN)
                break
            elif self.logicalBoard.isBoardFull() or not self.logicalBoard.availableMoves():
                winner = None
                player.finished(TIE)
                oponent.finished(TIE)
                break

            # switch to other player's turn
            self.turn = oponent.color

        if self.graphicalBoard is not None:
            status = self.graphicalBoard.finished(winner)
            self.check_exit(status)

    def check_exit(self, status):
        if status == EXIT:
            self.exit()

    def exit(self):
        self.player1.done()
        self.player2.done()
        sys.exit()
