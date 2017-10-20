import sys
import json
import os
from RandomPlayer import *
from constants import * 


def read(type):
    msg = raw_input()
    if msg == EXIT:
        send(LISTO)
        sys.exit()
    return type(msg)


def send(msg):
    sys.stdout.write(str(msg) + '\n')
    sys.stdout.flush()


def main():
    
    while True:
        colors = read(str)
        colors = colors.split(' ')
        log = open('log/player_controller_' + colors[0] + '.log', "a")
        log.write("Yo soy el " + colors[0] + " y el es el " + colors[1] + "\n")

        board = read(str)
        board = board.split(' ')
        board = map(lambda x: int(x), board)
        log.write("Dimensiones: " + str(board[0]) + " x " + str(board[1]) + '\n')
        log.write("c: " + str(board[2]) + " p: " + str(board[3]) + '\n')

        player = RandomPlayer(colors[0], colors[1], board[0], board[1], board[2], board[3])

        go_first = read(str)
        if go_first == YOU_FIRST:
            log.write("empezas vos\n")
            move = player.move()
            send(move)
            log.write("movi: " + str(move))
        else:
            log.write("empieza el\n")

        while True:
            msg = read(str)
            if msg in [LOSE, WIN, TIE]:
                log.write(msg + '\n')
                sys.stdout.flush()
                break
            log.write("el movio: " + msg + '\n')
            move = player.move(int(msg))
            send(move)
            log.write("movi: " + str(move) + '\n')
            log.flush()


if __name__ == '__main__':

    if not os.path.isdir('log'):
        os.mkdir('log')

    main()
