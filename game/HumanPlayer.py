

class HumanPlayer:

    def __init__(self, color):
        self.human = True
        self.color = color

    def startGame(self, columns, rows, c, p, go_first):
        pass

    def finished(self, status):
        pass

    def done(self):
        return True
