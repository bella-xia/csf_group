
# Online Python - IDE, Editor, Compiler, Interpreter
total = 16384
setblock = 1024

def printa(a, b, c):
    print(f'./csim 1024 {1024//1024} 16 {a} {b} {c} < gcc.trace')
    print(f'./csim 128 {1024//128} 16 {a} {b} {c} < gcc.trace')
    print(f'./csim 32 {1024//32} 16 {a} {b} {c} < gcc.trace')
    print(f'./csim 2 {1024//2} 16 {a} {b} {c} < gcc.trace')
    print(f'./csim 1 {1024//1} 16 {a} {b} {c} < gcc.trace')

a = "write-allocate"
b = "write-back"
c = "fifo"

A = "no-write-allocate"
B = "write-through"
C = "lru"

printa(a,b,c)
printa(a,b,C)
printa(a,B,c)
printa(a,B,C)
printa(A,b,c)
printa(A,B,c)
printa(A,b,C)
printa(A,B,C)

