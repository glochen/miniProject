slots = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
slots[0] =  {3:1, 5:2}
slots[1] =  {6:3, 8:4}
slots[2] =  {7:4, 9:5}
slots[3] =  {0:1, 5:4, 12:7, 10:6}
slots[4] =  {11:7, 13:8}
slots[5] =  {3:4, 0:2, 12:8, 14:9}
slots[6] =  {1:3, 8:7}
slots[7] =  {2:4, 9:8}
slots[8] =  {1:4, 6:7}
slots[9] =  {2:5, 7:8}
slots[10] = {3:6, 12:11}
slots[11] = {4:7, 13:12}
slots[12] = {10:11, 3:7, 5:8, 14:13}
slots[13] = {11:12, 4:8}
slots[14] = {12:13, 5:9}

def initialboard():
    return [False] + [True]*14

def bfromlist(l):
    b = [False]*15
    for i in l:
        b[i] = True
    return b

def bfrominvlist(l):
    b = [True]*15
    for i in l:
        b[i] = False
    return b

def moves(b):
    pegs = [i for i in range(0, len(b)) if b[i]]
    nopegs = [i for i in range(0, len(b)) if not b[i]]

    moves = []
    for s in pegs:
        for e in nopegs:
            if e in slots[s].keys() and b[slots[s][e]]:
                moves.append((s,e))
    return moves

def makemove(b, m):
    jumped = slots[m[0]][m[1]]
    b[jumped] = False
    b[m[0]] = False
    b[m[1]] = True
    return b

def unmove(b, m):
    jumped = slots[m[0]][m[1]]
    b[jumped] = True
    b[m[0]] = True
    b[m[1]] = False
    return b

def gameover(b):
    if moves(b) == []:
        return len([i for i in range(0, len(b)) if b[i]])
    else:
        return False

def minimize(b, best=((-1,-1),99), k=1):
    if gameover(b):
        return (-1,-1),gameover(b)
    for m in moves(b):
        score = minimize(makemove(b,m), best, k-1)
        if score[1] < best[1]:
            best = (m, score[1])
        unmove(b, m)
        if score[1] == 1:
            return best
    return best

def optimize(b,k=14):
    return minimize(b, ((-1,-1), 99), k)[0]

def lookahead(b,k=1):
    if gameover(b):
        return int(gameover(b) == 1)
    else:
        total = 0
        if k == 0:
            for m in moves(b):
                makemove(b, m)
                val = minimize(b, ((-1,-1), 99), 14)[1]
                unmove(b, m)
                total += int(val == 1)
        else:
            for m in moves(b):
                makemove(b, m)
                val = lookahead(b, k-1)
                unmove(b, m)
                total += val
        avg = total/len(moves(b))
        if not isinstance(avg, float):
            raise TypeError('avg in lookahead should be float')
        return avg

def lookaheadoptions(b, k=1):    
    options = moves(b)
    value = {}
    for m in options:
        makemove(b, m)
        value[m] = lookahead(b, k-1)
        unmove(b, m)
    return value

def showlookahead(b,k=1):
    showboard(b)
    for m,v in lookaheadoptions(b,k).items():
        print(f'{m}\t {v:.2f}')
    print('~~~~~~~~~~~~~')

def doOpt(b):
    showboard(b)
    m = optimize(b)
    print(m)
    b = makemove(b, m)
    showboard(b)
    print('~~~~~~~~~')

def c(c):
    if c:
        return 'X'
    else:
        return 'O'

def showboard(b):
    print(f'    {c(b[0])}')
    print(f'   {c(b[1])} {c(b[2])}')
    print(f'  {c(b[3])} {c(b[4])} {c(b[5])}')
    print(f' {c(b[6])} {c(b[7])} {c(b[8])} {c(b[9])}')
    print(f'{c(b[10])} {c(b[11])} {c(b[12])} {c(b[13])} {c(b[14])}')
