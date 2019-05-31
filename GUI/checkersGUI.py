##This program implements the GUI for the checkers board and sends the required instructions 
##to be parsed to the interpreter.

from tkinter import *
from tkinter import ttk
from tkinter import messagebox
import os


###
#Receives a number and returns its asociated piece color
###
def getColor(color):
    if color == 1:
        return "white"
    if color == 2: 
        return "red"
    if color == 3:
        return "white"
    if color == 4: 
        return "red"
    else:
        return "blue"

###
##Receives two coordinates, a color string and a canvas name and it draw a circle of that color 
#onto the canvas in the specified square.
#Returns the reference of the created oval
###
def drawPiece(ySquare, xSquare, color, canvasName):
    x0 = xSquare * 70 + 10
    y0 = ySquare * 70 + 10
    x1 = x0 + 50
    y1 = y0 + 50
    return canvasName.create_oval(x0, y0 , x1, y1, width = 2.0, fill = color)

###
#Receives a matrix of oval references, a pair coordinates and a canvas name.
#Deletes the reference of the oval drawn on said coordinates from the reference matrix.
###
def erasePiece(references, i, j, canvasName):
    board[i][j] = 0
    canvasName.delete(references[i][j])
    

###
#Receives the matrix of pieces, the matrix of oval references, two pairs of coordinates, 
#a color number and a canvas name.
#Moves the piece in the matrix from the old position to the new and moves the oval on the board
#from the old position to the new.
###
def dropPiece(board, references, ogI, ogJ, newI, newJ, color, canvasName):
    board[newI][newJ] = color
    erasePiece(references, ogI, ogJ, canvasName)
    references[newI][newJ] = drawPiece(newI, newJ, getColor(color) ,canvasName)


###
#Receives a board of pieces in a matrix and draws each piece onto the graphical board.}
###
def drawBoard(board):
    for i in range (0, 10):
        for j in range(0, 10):
            if (board[i][j] == 1):
                references[i][j] = drawPiece(i, j, getColor(1), mainCanvas)
            elif (board[i][j] == 2):
                references[i][j] = drawPiece(i, j, getColor(2), mainCanvas)
            elif (board[i][j] == 3):
                references[i][j] = drawPiece(i, j, getColor(3), mainCanvas)
            elif (board[i][j] == 4):
                references[i][j] = drawPiece(i ,j , getColor(4), mainCanvas)

###
#Receives a matrix and a pair of coordinates and verifies whether it is a piece
###
def isPiece(board, iPos, jPos):
    if board[iPos][jPos] != 0:
        print(str(references[iPos][jPos]))
        return True
    else:
        print("No reference")
        return False

###
#This is the game's primary function it is called once the user clicks on the canvas
#One of two scenarios can occur:
#1. There is not a picked piece yet: It takes care of picking a piece if a valid one was clicked.
#2. There is already a picked piece: It takes care of dropping the piece if a valid destination was clicked.
###
def mouseClicked(event):
    global picked, pickedX, pickedY, board, references
    squareX = (event.x-2)//70
    squareY = (event.y-2)//70
    if squareX < 0:
        squareX = 0
    if squareY < 0:
        squareY = 0
    if not picked:
        if isPiece(board, squareY, squareX):
            print("Pick piece on " +  str(squareY) + "," + str(squareX))
            picked = True
            pickedY = squareY
            pickedX = squareX
        else:
            print("Not a piece")
        
    else:
        #hitting another piece or letting the piece go
        if isPiece(board, squareY, squareX): 
            if (squareY == pickedY and squareX == pickedX):
                print("DROP THE PIECE ON ITS OWN PLACE")
                picked = False
            else:
                print("Not a valid movement")


        else: ##movements
            if (board[pickedY][pickedX] == 1): 
                if (squareX == pickedX + 1 or squareX == pickedX -1) and squareY == pickedY + 1:
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 1, mainCanvas)
                    picked = False
                elif squareX == pickedX + 2 and squareY == pickedY + 2 and isPiece(board, pickedY +1, pickedX + 1):                    
                    picked = False
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 1, mainCanvas)
                    erasePiece(references, pickedY +1, pickedX +1, mainCanvas)
                elif squareX == pickedX - 2 and squareY == pickedY + 2 and isPiece(board, pickedY + 1, pickedX - 1):
                    picked = False
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 1, mainCanvas)
                    erasePiece(references, pickedY + 1, pickedX - 1, mainCanvas)
                else:
                    print("Not a valid movement")
            elif (board[pickedY][pickedX] == 2): 
                if (squareX == pickedX + 1 or squareX == pickedX -1) and squareY == pickedY - 1:
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 2, mainCanvas)
                    picked = False
                elif squareX == pickedX + 2 and squareY == pickedY - 2 and isPiece(board, pickedY - 1, pickedX + 1):                    
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 2, mainCanvas)
                    erasePiece(references, pickedY - 1, pickedX + 1, mainCanvas)
                    picked = False
                elif squareX == pickedX - 2 and squareY == pickedY - 2 and isPiece(board, pickedY - 1, pickedX - 1):
                    dropPiece(board, references, pickedY, pickedX, squareY, squareX, 2, mainCanvas)
                    erasePiece(references, pickedY - 1, pickedX - 1, mainCanvas)
                    picked = False
                else:
                    print("Not a valid movement")
            else: ##crowned
                if (squareX == pickedX + 1 or squareX == pickedX -1) and (squareY == pickedY + 1 or squareY == pickedY -1):
                    print("DROPE THE PIECE ON A NEW PLACE")
                    picked = False
                else:
                    print("Not a valid movement")


#Main window
root = Tk()
root.title("Checkers GUI")

#Game variables
picked = False #Defines whether there's a picked piece
pickedY = 0 #Y axis of the picked piece
pickedX = 0 #X axis of the picked piece
##Logical board of pieces
board = [[0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10 ,[0]*10 , [0]*10, [0]*10]
references = [[0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10, [0]*10 ,[0]*10 , [0]*10, [0]*10]
#The numbers are assigned as follow:
#0 for empty
#1 for whites
#2 for reds
#3 for crowned whites
#4 for crowned reds

#Filling Whites
board[0][1] = board[0][3] = board[0][5] = board[0][7] = board[0][9] = 1
board[1][0] = board[1][2] = board[1][4] = board[1][6] = board[1][8] = 1
board[2][1] = board[2][3] = board[2][5] = board[2][7] = board[2][9] = 1

#filling Reds
board[9][0] = board[9][2] = board[9][4] = board[9][6] = board[9][8] = 2
board[8][1] = board[8][3] = board[8][5] = board[8][7] = board[8][9] = 2
board[7][0] = board[7][2] = board[7][4] = board[7][6] = board[7][8] = 2

#Canvas that is going to be drawn onto the screen
mainCanvas = Canvas(master = root, width= 700, height = 700)
color = True
shapeSize = 70 #Each square has a site of 70x70 pixels

#Drawing the game board
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

#binding event for the click
mainCanvas.bind("<Button-1>", mouseClicked) 

#called the function to draw the pieces
drawBoard(board) 

#Main window's mainloop
root.mainloop()

