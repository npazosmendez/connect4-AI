from LogicalBoard import *
from random import randint
import os

class RandomPlayer:

    def __init__(self, color, oponent_color, columns, rows, c, p):
        self.color = color
        self.oponent_color = oponent_color
        self.logicalBoard = LogicalBoard(columns, rows, c, p)

        if not os.path.isdir('log'):
            os.mkdir('log')
        self.log_file = open('log/random_player_' + self.color + '.log', 'a')

    def move(self, last_move=None):
        if last_move is not None:
            self.log("He moved: " + str(last_move))
            self.logicalBoard.makeMove(self.oponent_color, last_move)

        while True:
            move = randint(0, self.logicalBoard.columns - 1)
            if self.logicalBoard.isValidMove(move):
                break
        self.log("I moved: " + str(move))
        self.logicalBoard.makeMove(self.color, move)
        return move

    def log(self, msg):
        self.log_file.write(str(msg) + "\n")
        self.log_file.flush()
