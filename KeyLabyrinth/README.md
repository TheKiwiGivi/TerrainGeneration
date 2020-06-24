# **Key Labyrinth

# libraries Used
- Glew 
- GLFW 
- Imgui (render text and windows)
- irrKlang 64 bit (sound)
- Assimp (model loading)
- glm (math library)
- stbi image

# Installation Guide

To set up the project in microsoft visual studio first create a new project. Then download the libarires that are used in the project.
the libraries that have to be installed are glew, glfw, imgui, irrKlang, assimp, glm and stbi image.

stbi image, glm and imGui just need to have their .cpp and .h files added in to the project but
the remaning libraires need to be linked inn to the c++ project.

So add glew, glfw, irrklang and assimp to the C++ additional include directories, then the project need
to link the .lib into the additional libraires settings. (make sure to use the same architecture for all your 
libraries i.e x86 or 64bit).

in the linker settings under additional dependencies add the following entries.
glfw3.lib
opengl32.lib
glew32s.lib
assimp.lib
irrKlang.lib

Then inn the C pre processor add the  text GLEW_STATIC

This should be all the steps to set up the project correctly

# Version 1.0.1

- Spikes
- jump
- run

# Objective:
The goal of the game is to escape the labyrinth. To escape, find and open all the chests. But be careful, I've heard that there are some monsters lurking around!

# Maps:
The game will include default maps, however the user can also create their own maps. In order to do this, create a .txt file named 'map_' followed by a number starting from 0. It is important to ensure that the maps are named in order starting from 0, and there to be no gaps in the counting.  
Inside the text file, start by writing the dimensions of the map (X and Y), for instance '10 15'.
Please note that the first value is the width, and the second would be the height of the text file.
On the next line specify for the map where you want walls. If your map is 10x15 you need to write a 10x15 block of values ranging from 0 to 9. The values will be explained down below.

Example of a map file: 

5 6
0 0 0 0 0
0 0 1 1 0
0 0 0 1 0
0 0 0 1 0
1 1 0 0 0
0 0 0 0 0

(where 0 is nothing and 1 is a wall)

You can also add torches to your map. Torches are always placed on walls, and they are placed on one of the fours sides of the wall.
To specify which side you want the torch to be placed, choose a number between 2 and 5.
The torches will bw placed on the side corresponding to the number.
Illustration:
 5
4 2
 3
 
Example of a file:

5 6
0 0 0 0 0
0 0 4 1 0
0 0 0 1 0
0 0 0 3 0
1 2 0 0 0
0 0 0 0 0

The walls will all be the same, but there will be torches placed on the ends pointing away.
2 = ->
3 = v
4 = <-
5 = ^

PLEASE NOTE!
Although there are walls around the entire map, the enemy can only detect walls created by the map file. Therefore in order to get the AI to work properly, please create a map with walls surrounding it (may also be torches)
Example:

7 8
1 1 1 1 1 1 1
1 0 0 0 0 0 1
1 0 0 4 1 0 1
1 0 0 0 1 0 1
1 0 0 0 3 0 1
1 1 2 0 0 0 1
1 0 0 0 0 0 1
1 1 1 1 1 1 1


# Spawn location:
If no spawn location is specified, the player will by default spawn in the middle of the map. To set a spawn location please use the map file and write '6' where you want the player to spawn.
Example:

7 8
1 1 1 1 1 1 1
1 0 0 0 0 0 1
1 0 0 4 1 0 1
1 0 0 0 1 0 1
1 0 0 0 3 0 1
1 1 2 0 0 0 1
1 0 0 0 0 6 1
1 1 1 1 1 1 1

(spawns in bottom right corner)

# Chests:
Chests are meant for the player to collect, and is the key to winning the game. All keys need to be collected from their chest which the player has to find while trying to stay alive. To create chests in the map, use the value '7'. 
Please remember to always include atleast one chest to give the game a purpose.

# Enemies:
Enemies are meant to chase the player, and is the most popular way to die in the game. To add an enemy use the value '8'.

# Spikes:
Spikes are dangerous objects lying around in the maze. Please avoid these at all cost as they will cost you your life if you step on one.
To add spikes to the map, use the value '9'.

# Example of finished product:

7 8
1 1 1 1 1 1 1
1 7 0 0 0 8 1
1 0 0 4 1 0 1
1 0 0 0 1 9 1
1 8 0 0 3 0 1
1 1 2 0 0 0 1
1 7 0 0 0 6 1
1 1 1 1 1 1 1


# Camera modes:
There are two camera modes. You will start in first person which works as expected. The second camera mode is top down and alters the viewing plane to cut off the roof. It also locks the cameras Y axis because it is not needed in this mode. You change camera mode by pressing TAB.

# Running:
You can run by holding down SHIFT while walking. It's fast!

# Mechanics:
W-A-S-D - Walk
Mouse - Look around
TAB - Change camera mode
SHIFT - Run

# Open chests:
To get keys you need to find a chest and get within close range of it, then press E.

# Win condiditon:
The only way to win the game is by collecting all the keys from the chests in the map. Only then can you be safe..

# Lose conditions:
You die if the enemies catches up to you. You will also die if you run into spikes (PROTIP: jump over it!). There are no weapons, you just have to avoid them.
Play at your own risk. c:

# Limits:
There are some hard-coded limits to map sizes and other things. They are all listed here:
* Max enemy count: 20
* Max wall count: 500
* Max chest count: 50
* Max spike count: 100
* Max torch count: 200


# Code
* [X]  1st person view
* [X]  Enemies
* [X]  3D Maze 
* [X]  Map rendering
* [X]  Objects
* [X]  Chest system
* [X]  Running
* [X]  Jumping
* [X]  Spikes
* [X]  Menu
* [X]  UI
* [ ]  Animation
* [ ]  Shadows

# If we get time
* [X]  Gravity
* [X]  Map loading
* [X]  Fake animation (chest rotation)
 