class Board:
    def __init__(self,n,m,c):
        self.n = n
        self.m = m
        self.c = c
        self.p = n*m

    def __str__(self):
        return "n: " + str(self.n) + " " +\
               "m: " + str(self.m) + " " +\
               "c: " + str(self.c)

        

def getAllBoardConfigs():
    allBoards = []    
    Ns = [4,5,6,7,8,9]
    for n in Ns:
        Ms = [n-1, n, n+1]
        for m in Ms:
            for c in [3,4,5,6]:
                p = n*m
                if c <= min(n,m):
                    # tablero definido OK
                    # print "n: " ,n," m: ", m, " c: ",c, "\n"
                    allBoards.append(Board(n,m,c))
    return allBoards
                
def main():
    boards = getAllBoardConfigs()
    # implementar ACA ejecucion del genetico con los parametros, y parsear la salida?
    # Quizas con los logs se puede levantar a mano despues


if __name__ == "__main__":
    main()
    

