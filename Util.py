class KMP(object):

    def __init__(self, P, T):
        self.N = len(T)
        self.M = len(P)
        self.P = P
        self.T = T
        self.B = [0] * (self.N + self.M + 10)

    def search(self):
        self.kmpPreprocess()
        return self.kmpSearch()

    def kmpSearch(self):    
        i = 0; j = 0

        while (i < self.N):
            while (j >= 0 and self.T[i] != self.P[j]): j = self.B[j]
            i += 1; j += 1
            if (j == self.M):
                return (i - j)

        return -1

    def kmpPreprocess(self):
        i = 0; j = -1; self.B[0] = -1
        while (i < self.M):
            while (j >= 0 and self.P[i] != self.P[j]): j = self.B[j]
            i += 1; j +=1
            self.B[i] = j

class EditDistance(object):

    def __init__(self, A, B):
        self.A = A
        self.B = B
        self.N = len(A)
        self.M = len(B)

    def cost(self, a, b):
        if (a == b): return 2
        return -1 

    def run(self):
        pass
