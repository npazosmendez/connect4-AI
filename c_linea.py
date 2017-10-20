# -*- coding: utf-8 -*-

from random import choice
import argparse

from LogicalBoard import *
from GraphicalBoard import *
from Referee import *
from PlayerCommunicator import *
from HumanPlayer import *


def main(args):

    assert args.c <= args.columns and args.c <= args. rows

    print "----- Parámetros -----"
    print "columnas: \t", args.columns
    print "filas: \t\t", args.rows
    print "c: \t\t", args.c
    print "p: \t\t", args.p
    print "----------------------"

    if args.blue_player is None:
        blue_player = HumanPlayer(BLUE)
    else:
        executable = args.blue_player.pop(0)
        blue_player = PlayerCommunicator(executable, args.blue_player, BLUE, RED)

    if args.red_player is None:
        red_player = HumanPlayer(RED)
    else:
        executable = args.red_player.pop(0)
        red_player = PlayerCommunicator(executable, args.red_player, RED, BLUE)

    ref = Referee(args.columns, args.rows, args.c, args.p, blue_player, red_player, show_ui=args.ui)

    iteration = 0
    while args.iterations is None or args.iterations > iteration:
        iteration += 1
        if args.first == BLUE:
            ref.runPlay(blue_player)
        elif args.first == RED:
            ref.runPlay(red_player)
        else:
            ref.runPlay(choice([blue_player, red_player]))

    ref.exit()


def positive_integer(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s Valor inválido, tiene que ser un numero natural." % value)
    return ivalue


if __name__ == '__main__':

    description = 'Versión del laboratorio de AED III del juego cuatro en linea para el TP3.'
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--red_player',
                        type=str,
                        nargs='*',
                        help='Path al ejecutable del jugador rojo.')
    parser.add_argument('--blue_player',
                        type=str,
                        nargs='*',
                        help='Path al ejecutable del jugador azul.')
    parser.add_argument('--columns',
                        type=positive_integer,
                        default=7,
                        help="Cantidad de columnas. columns > 0.")
    parser.add_argument('--rows',
                        type=positive_integer,
                        default=6,
                        help="Cantidad de filas. rows > 0.")
    parser.add_argument('--c',
                        type=positive_integer,
                        default=4,
                        help="El tamaño de la linea a formar. c <= mínimo {columns, rows}.")
    parser.add_argument('--p',
                        type=int,
                        default=21,
                        help="Cantidad de fichas por jugador. p > 0.")
    parser.add_argument('--first',
                        type=str,
                        choices=[BLUE, RED],
                        help='Color del jugador que comienza, si no está especificado comienza'
                             ' siempre un jugador random con distribución uniforme.')
    parser.add_argument('--iterations',
                        type=positive_integer,
                        help="Cantidad de iteraciones. iteraciones > 0.")
    parser.add_argument('--ui',
                        type=bool,
                        default=False,
                        help="Si es True, la aplicacion muestra una interfaz grafica.")
    args = parser.parse_args()

    assert args.c <= min(args.columns, args.rows)

    main(args)
