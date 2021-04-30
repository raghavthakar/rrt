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

# Function to show obstacle on Screen
# -----------------------SHOW OBSTACLE------------------------------------------
def showObstacle(obstacle_info):
    top_left_col=int(obstacle_info[0])
    top_left_row=int(obstacle_info[1])
    bottom_right_col=int(obstacle_info[2])
    bottom_right_row=int(obstacle_info[3])

    obstacle_rectangle=pygame.Rect(top_left_col, top_left_row,
        bottom_right_col-top_left_col, bottom_right_row-top_left_row)

    pygame.draw.rect(screen, (0, 0, 0), obstacle_rectangle)

# Function to print dot on screen
# -------------------------SHOW DOT---------------------------------------------
def showDot(node_info):
    col=int(node_info[0])
    row=int(node_info[1])
    parent_col=int(node_info[2])
    parent_row=int(node_info[3])

    pygame.draw.circle(screen, (250, 250, 250), (col, row), 5)

# overloaded to Consider the special start and end points
def showSpecialDot(col, row):
    col=int(col)
    row=int(row)

    pygame.draw.circle(screen, (0, 0, 250), (col, row), 7)

# -----------------------SHOW TARGET BOX----------------------------------------
def showTargetBox(col, row, radius):
    col=int(col)
    row=int(row)
    radius=int(radius)

    target_rectangle=pygame.Rect(int(col-radius), int(row-radius),
        radius*2, radius*2)

    pygame.draw.rect(screen, (250, 250, 0), target_rectangle)

# ------------------------SHOW EDGE---------------------------------------------
def showEdge(node_info, colour, width):
    col=int(node_info[0])
    row=int(node_info[1])
    parent_col=int(node_info[2])
    parent_row=int(node_info[3])

    pygame.draw.line(screen, colour, (col, row), (parent_col, parent_row), width)

# ----------------------MAIN SEQUENCE-------------------------------------------
print("Hello World!")
running = True

#-----------------------get obstacle from csv-----------------------------------
with open('../src/map.csv', 'r') as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')

    starting_point_info=next(reader)
    showSpecialDot(starting_point_info[0], starting_point_info[1]);

    target_point_info=next(reader)

    target_radius_info=next(reader)
    showTargetBox(target_point_info[0], target_point_info[1], target_radius_info[0])

    # Target dot shown after radius so that it's over it
    showSpecialDot(target_point_info[0], target_point_info[1]);

    while(True):
        try:
            obstacle_info=next(reader)
            showObstacle(obstacle_info)
        except:
            break

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
            showEdge(node_info, (100, 200, 150), 2)

        except:
            break;

        pygame.display.flip()
        # time.sleep(0.1)

#--------------------path display--------------------------------
with open('../src/path.csv', 'r') as csv_file:
    reader = csv.reader(csv_file, delimiter = ',')

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running=False

        try:
            node_info = next(reader)
            #show an edge from node to parent node
            showEdge(node_info, (0, 50, 0), 5)

        except:
            pass

        pygame.display.flip()
        time.sleep(0.1)
