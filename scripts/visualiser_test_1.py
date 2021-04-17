#!/usr/bin/python3
import pygame
import csv

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
    col=node_info[0]
    row=node_info[1]
    parent_col=node_info[2]
    parent_row=node_info[3]

    pygame.draw.circle(screen, (0, 0, 255, 1), (col*10, row*10), 40)
    pygame.display.update()

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
            print(node_info)
            # Show a dot at node location
            # showDot(node_info)
            col=int(node_info[0])
            row=int(node_info[1])
            parent_col=int(node_info[2])
            parent_row=int(node_info[3])

            pygame.draw.circle(screen, (250, 250, 250), (col*10, row*10), 5)

        except:
            pass
            
        pygame.display.flip()
