import pygame
from pygame.locals import *
from LogicalBoard import *
from constants import *

clock = pygame.time.Clock()


class GraphicalBoard:

    def __init__(self, logicalBoard):

        pygame.init()
        pygame.display.set_caption('Algoritmos III - cuatro en linea')

        self.logicalBoard = logicalBoard

        self.bg_color = BLACK
        self.stage_width = WINDOWWIDTH + self.logicalBoard.columns * SPACESIZE
        self.stage_height = WINDOWHEIGHT + self.logicalBoard.rows * SPACESIZE
        self.canvas = pygame.display.set_mode((self.stage_width, self.stage_height))
        self.margin = {
            'x': int((self.stage_width - logicalBoard.columns * SPACESIZE) / 2),
            'y': int((self.stage_height - logicalBoard.rows * SPACESIZE) / 2)
        }

        # img sources TODO: do not scale images
        self.red_token = pygame.image.load('img/tokens/red.png')
        self.red_token = pygame.transform.smoothscale(self.red_token, (SPACESIZE, SPACESIZE))
        self.blue_token = pygame.image.load('img/tokens/blue.png')
        self.blue_token = pygame.transform.smoothscale(self.blue_token, (SPACESIZE, SPACESIZE))
        self.cell_img = pygame.image.load('img/cell.png')
        self.cell_img = pygame.transform.smoothscale(self.cell_img, (SPACESIZE, SPACESIZE))

        self.red_won_img = pygame.image.load('img/results/red_won.png')
        self.red_won_rect = self.red_won_img.get_rect()
        self.red_won_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

        self.blue_won_img = pygame.image.load('img/results/blue_won.png')
        self.blue_won_rect = self.blue_won_img.get_rect()
        self.blue_won_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

        self.tie_img = pygame.image.load('img/results/tie.png')
        w, h = self.tie_img.get_size()
        scale = float(self.logicalBoard.columns * SPACESIZE) / float(w)
        self.tie_img = pygame.transform.smoothscale(self.tie_img, (int(scale * w), int(scale * h)))
        self.tie_rect = self.tie_img.get_rect()
        self.tie_rect.center = (int(self.stage_width / 2), int(self.stage_height / 2))

        self.red_pile = pygame.Rect(int(SPACESIZE / 2), self.stage_height - int(3 * SPACESIZE / 2), SPACESIZE, SPACESIZE)
        self.blue_pile = pygame.Rect(self.stage_width - int(3 * SPACESIZE / 2), self.stage_height - int(3 * SPACESIZE / 2), SPACESIZE, SPACESIZE)

    def draw(self, extraToken=None, update=True):
        self.canvas.fill(self.bg_color)

        # draw tokens
        spaceRect = pygame.Rect(0, 0, SPACESIZE, SPACESIZE)
        for x in range(self.logicalBoard.columns):
            for y in range(self.logicalBoard.rows):
                spaceRect.topleft = (self.margin['x'] + (x * SPACESIZE), self.margin['y'] + (y * SPACESIZE))
                if self.logicalBoard[x][y] == RED:
                    self.canvas.blit(self.red_token, spaceRect)
                elif self.logicalBoard[x][y] == BLUE:
                    self.canvas.blit(self.blue_token, spaceRect)

        # draw the extra token
        if extraToken is not None:
            if extraToken['color'] == RED:
                self.canvas.blit(self.red_token, (extraToken['x'], extraToken['y'], SPACESIZE, SPACESIZE))
            elif extraToken['color'] == BLUE:
                self.canvas.blit(self.blue_token, (extraToken['x'], extraToken['y'], SPACESIZE, SPACESIZE))

        # draw board over the tokens
        for x in range(self.logicalBoard.columns):
            for y in range(self.logicalBoard.rows):
                spaceRect.topleft = (self.margin['x'] + (x * SPACESIZE), self.margin['y'] + (y * SPACESIZE))
                self.canvas.blit(self.cell_img, spaceRect)

        # draw the red and blue tokens off to the side
        if self.logicalBoard.availableMoves():
            self.canvas.blit(self.red_token, self.red_pile)  # red on the left
            self.canvas.blit(self.blue_token, self.blue_pile)  # blue on the right

        if update:
            pygame.display.update()

    def dropToken(self, column, color):
        x = self.margin['x'] + column * SPACESIZE
        y = self.margin['y'] - SPACESIZE
        dropSpeed = 1.0  # TODO parametrize this value

        lowestEmptySpace = self.logicalBoard.getLowestEmptySpace(column)

        while True:
            y += int(dropSpeed)
            dropSpeed += 0.5
            if int((y - self.margin['y']) / SPACESIZE) >= lowestEmptySpace:
                break
            self.draw({'x': x, 'y': y, 'color': color})
            clock.tick()

        self.logicalBoard.makeMove(color, column)
        self.draw()

    def animateComputerMove(self, column, color):

        x = self.blue_pile.left if color == BLUE else self.red_pile.left
        y = self.blue_pile.top if color == BLUE else self.red_pile.top
        speed = 1.0  # TODO parametrize this

        # moving the token up
        while y > (self.margin['y'] - SPACESIZE):
            y -= int(speed)
            speed += 0.5
            self.draw({'x': x, 'y': y, 'color': color})
            clock.tick()

        # moving the token over
        y = self.margin['y'] - SPACESIZE
        speed = 1.0

        if x > (self.margin['x'] + column * SPACESIZE):
            while x > (self.margin['x'] + column * SPACESIZE):
                x -= int(speed)
                speed += 0.5
                self.draw({'x': x, 'y': y, 'color': color})
                clock.tick()
        else:
            while x < (self.margin['x'] + column * SPACESIZE):
                x += int(speed)
                speed += 0.5
                self.draw({'x': x, 'y': y, 'color': color})
                clock.tick()

        # dropping the blue
        self.dropToken(column, color)

    def getHumanMove(self, color):
        if not self.logicalBoard.availableMoves():
            return

        draggingToken = False
        tokenx, tokeny = None, None

        pile = self.red_pile if color == RED else self.blue_pile

        while True:
            for event in pygame.event.get():  # event handling loop
                if event.type == QUIT or (event.type == KEYUP and event.key == K_ESCAPE):
                    return self.exit()
                elif event.type == MOUSEBUTTONDOWN and not draggingToken and pile.collidepoint(event.pos):
                    # start of dragging on the token pile.
                    draggingToken = True
                    tokenx, tokeny = event.pos
                elif event.type == MOUSEMOTION and draggingToken:
                    # update the position of the dragged token
                    tokenx, tokeny = event.pos
                elif event.type == MOUSEBUTTONUP and draggingToken:
                    # drop the dragged token
                    if tokeny < self.margin['y'] and tokenx > self.margin['x'] and tokenx < self.stage_width - self.margin['x']:
                        # let go at the top of the screen.
                        column = int((tokenx - self.margin['x']) / SPACESIZE)
                        if self.logicalBoard.isValidMove(column):
                            self.dropToken(column, color)
                            return column
                    tokenx, tokeny = None, None
                    draggingToken = False
            if tokenx is not None and tokeny is not None:
                self.draw({
                    'x': tokenx - int(SPACESIZE / 2),
                    'y': tokeny - int(SPACESIZE / 2),
                    'color': color
                })
            else:
                self.draw()

            clock.tick()

    def finished(self, status):
        if status == RED:
            winner_img = self.red_won_img
            rect = self.red_won_rect
        elif status == BLUE:
            winner_img = self.blue_won_img
            rect = self.blue_won_rect
        else:
            winner_img = self.tie_img
            rect = self.tie_rect

        while True:
            # Keep looping until player clicks the mouse or quits.
            self.draw(update=False)
            self.canvas.blit(winner_img, rect)
            pygame.display.update()
            clock.tick()
            for event in pygame.event.get():  # event handling loop
                if event.type == QUIT or (event.type == KEYUP and event.key == K_ESCAPE):
                    return self.exit()
                elif event.type == MOUSEBUTTONUP:
                    return None

    def check_user_exits(self):
        for event in pygame.event.get():  # event handling loop
            if event.type == QUIT or (event.type == KEYUP and event.key == K_ESCAPE):
                return self.exit()

    def exit(self):
        pygame.quit()
        return EXIT
