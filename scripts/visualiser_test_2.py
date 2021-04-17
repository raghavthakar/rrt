#!/usr/bin/python3
import pygame
import csv
import time

pygame.init()

#---------------------SCREEN SETUP----------------------------------------------
# Creating the screen.
screen=pygame.display.set_mode((1000, 1000))
#Setting the background colour
screen.fill((200, 40, 40))
# Title and icon
pygame.display.set_caption("RRT")

# Function to print dot on screen
# -------------------------SHOW DOT---------------------------------------------
def showDot(node_info):
    col=int(node_info[0])
    row=int(node_info[1])
    parent_col=int(node_info[2])
    parent_row=int(node_info[3])

    pygame.draw.circle(screen, (250, 250, 250), (col*10, row*10), 5)

# ------------------------SHOW EDGE---------------------------------------------
def showEdge(node_info):
    col=int(node_info[0])
    row=int(node_info[1])
    parent_col=int(node_info[2])
    parent_row=int(node_info[3])

    pygame.draw.line(screen, (100, 200, 150), (col*10, row*10), (parent_col*10, parent_row*10), 2)

# ----------------------MAIN SEQUENCE-------------------------------------------
print("Hello World!")
running = True

#-----------------------csv file reading-------------------
#Execute the main loop, reading one row of csv every iteration
with open('../src/tree.csv', 'r') as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running=False

        try:
            node_info = next(reader)
            # Show a dot at node location
            showDot(node_info)
            #show an edge from node to parent node
            showEdge(node_info)

        except:
            pass

        pygame.display.flip()
        time.sleep(0.1)
