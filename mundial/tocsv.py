import pandas as pd
import numpy as np


NN = [4,5,6,7,8,9]
CC = [3,4,5,6]

output = open('mundial.csv', 'w')

for N in NN:
    for M in [N-1,N,N+1]:
        for C in CC:
            if C > N or C > M:
                continue
            path = str(N)+'x'+str(M)+'x'+str(C)+'.npy'
            genoma = np.load(path)
            output.write(str(N)+','+str(M)+','+str(C)+',')
            for n in genoma:
                output.write(str(n)+' ')
            output.write('\n')
