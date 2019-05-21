from tkinter import *
from tkinter import ttk
from tkinter import messagebox

root = Tk()
root.title("Checkers GUI")

board = [[0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10 ,[0]*10 , [0]*10, [0]*10]

#0 for empty, 1 for blues, 2 for whites, 3 for crowned blues, 4 for crowned whites

#filling blues
board[0][1] = board[0][3] = board[0][5] = board[0][7] = board[0][9] = 1
board[1][0] = board[1][2] = board[1][4] = board[1][6] = board[1][8] = 1
board[2][1] = board[2][3] = board[2][5] = board[2][7] = board[2][9] = 1
#filling whites
board[9][1] = board[9][3] = board[9][5] = board[9][7] = board[9][9] = 2
board[8][0] = board[8][2] = board[8][4] = board[8][6] = board[8][8] = 2
board[7][1] = board[7][3] = board[7][5] = board[7][7] = board[7][9] = 2


for i in range(0,10):
    print (board[i])
mainCanvas = Canvas(master = root, width= 700, height = 700)
color = True
shapeSize = 70
for i in range(0, 10):
    color = not color
    for j in range(0, 10):
        if color:
            paint = "blue"
        else:
            paint = "white"
        mainCanvas.create_rectangle(i*shapeSize, j*shapeSize, (i+1)*shapeSize, (j+1)*shapeSize, fill = paint)
        color = not color
mainCanvas.pack(fill = BOTH)
root.mainloop()

