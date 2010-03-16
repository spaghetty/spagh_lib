from heapq import *
from math import ceil, floor, sqrt

def get_prime(n):
    c=[(9,3)]
    test = 3
    yield 2
    while test<=n:
        if c[0][0]!=test:
            yield test
            heappush(c, ((test*test),test))
        else:
            while c[0][0]==test:
                p = c[0][1]
                heapreplace(c,(test+p,p))            
        test+=2

    
if __name__=="__main__":
    for i in get_prime(10):
        print "prime is %d"%i

