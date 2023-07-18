import random
import time

M,N,K=input("input three integer(512 ~2048):\n").split()

M=int(M)
N=int(N)
K=int(K)


x=[[100.0*random.random()
                for row in range(N)]
                for col in range(M)]

y=[[100.0*random.random()
                for row in range(K)]
                for col in range(N)]

z=[[0.0 for row in range(K)]
                for col in range(M)]



print("matrix_1:\n",x)
print("matrix_2\n",y)

start = time.clock()

for m in range(M):
    for k in range(K):
        for n in range(N):
            z[m][k]+=x[m][n]*y[n][k]

end = time.clock()

print("result:\n",z)

print("using time:",1000*(end-start),'ms\n' )
