from subprocess import Popen, PIPE
from constants import *
import os


class PlayerCommunicator:

    def __init__(self, player, params, color, oponent_color):
        self.human = False
        self.color = color
        self.oponent_color = oponent_color
        self.player_process = Popen([player] + params, shell=False, stdout=PIPE, stdin=PIPE)

        if not os.path.isdir('log'):
            os.mkdir('log')
        self.log_file = open('log/player_communicator_' + self.color + '.log', 'a')

    def startGame(self, columns, rows, c, p, go_first):
        self.log('\n/*************** Nueva partida *****************/' + 
                 '\ncolumnas: ' + str(columns) +
                 '\nfilas: ' + str(rows) +
                 '\nc: ' + str(c) +
                 '\np: ' + str(p) +
                 '\ncolor: ' + str(self.color) +
                 '\ncolor del oponente: ' + str(self.oponent_color) +
                 '\nvas primero: ' + str(go_first) +
                 '\n/**********************************************/')
        self.send(self.color + ' ' + self.oponent_color)
        self.send(str(columns) + ' ' + str(rows) + ' ' + str(c) + ' ' + str(p))
        self.send(YOU_FIRST if go_first else HE_FIRST)

    def send(self, msg):
        self.log('envio ' + str(type(msg)) + ': ' + str(msg))
        self.player_process.stdin.write(str(msg) + '\n')
        self.player_process.stdin.flush()

    def receive(self, cast):
        msg = self.player_process.stdout.readline().strip()
        self.log('recibo ' + str(type(msg)) + ': ' + str(msg))
        return cast(msg)

    def move(self, oponent_move):
        if oponent_move is not None:
            self.send(oponent_move)
        return self.receive(int)

    def finished(self, status):
        self.send(status)

    def done(self):
        self.send(EXIT)
        self.receive(str)
        return True

    def log(self, msg):
        self.log_file.write(str(msg) + "\n")
        self.log_file.flush()
