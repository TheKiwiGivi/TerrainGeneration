# TerrainGeneration
This project has two main functionalities, it renders heightmaps (PNGs) as terrain, and it can generate a random terrain if no hegihtmap it specified. It's written in C++ and uses OpenGL.
![Pic of mountain terrain](https://github.com/TheKiwiGivi/TerrainGeneration/blob/master/Pics/generated_mountain.PNG)

Here is a video showing what can be done: https://www.youtube.com/watch?v=2mlzpxDp3zI

# Installation
To run this program, please download it from 'https://github.com/TheKiwiGivi/TerrainGeneration'
Then, run the project 'KeyLabyrinth.sln' (from Visual Studios).
NB! 'TerrainGeneration.sln' does not work.

# Loading heightmaps
To load a heightmap, please select a map in the main menu and start. The heightmap will be read by the program and will be drawed to the screen. To add your own heightmap, please name is 'map_x' where x starts at 0 and corresponds to the amount of maps in your directory minus 1. This means that if you have 3 PNG files for loading, you will have to name the 'map_0', 'map_1' and 'map_2'. Please make sure all files are .PNG and is no more than 100x100 in size.

# Creating heightmaps
To create a heightmap, please click the 'Generate terrain' button. You can modify the generation by choosing biome and size (limited to 25x25 to 100x100).

# Color
When you load a terrain, you will be presented with the heightmap visualized in 3d. Depending on the height, the color will adjust to go from green (lowest) to brown(highest). There are exceptions to this color format. The exceptions occur between certain low and high ranges to create a greater contrast and a different feel. The exceptions include water (lowest range) beach (connecting to lowest), and mountain tops (highest).

# UI
The UI provides a way to modify the world. These features are listed by theme. The themes are 'Lighting', 'Weather', 'Camera' and mutliple toggleable features. 
To toggle cursor visibility, press 'E'. You can alternatively navigate the GUI with your keyboard.
In the top right corner, some extra information is provided (mainly fps). 

* Lighting
The first value in this category controls the power of the light that follows the player. Please note that this light it only visible if you enable player light.
The three next sliders are position values to a light you can set by the button following below. 
The next button creates a light at the current position.

* Camera
As for movement, you can change the position in world space by either adjusting the sliders in the GUI, or you can move around freely with WASD and mouse.

* Weather
In the weather tab the first value 'Rain' sets the density of the rain calling from the clouds. All clouds share the same density.
The three next input fields are position values for manually placing and creating clouds.
The two sliders are dimension sizes for the clouds created. 
The 'create cloud on me' button creates a cloud with the current position of the player.

* Toggles
You can toggle cloud rendering, rain rendering, sun rendering with the checkboxes.
The 'Walk mode' toggles if the player is moving freely on the Y axis or if it walks along the terrain (not perfectly).

# Consts
Please keep in mind that there are certain consts, that without warning will crash the program if capped (especially true for map sizes and raindrops).
* MAX LIGHTS = 100
NUMBER OF CUBES RENDERED = 27 000
MAX MAP SIZE = 100x100

# Perlin noise
I use perlin noise to generate the terrain. The code for perlin noise has been extracted and integrated into my project. The source code: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PerlinNoise.cpp

# LearnOpenGL
Some of the code (shaderclass, camera, models mainly), has been integrated into the project. Some parts are modified and some are identical. 
Source code: https://github.com/JoeyDeVries/LearnOpenGL

# Sound
Sound is played and repeated.

# Images
The program can load PNG images of up to 100 x 100 in size. More than that and I cannot guarantee success. Therefore the recommended size to create images is 100x100 (as I've mostly been using this size, there also might be some logic in the code that accidently just work on that size).

# Biomes
When generating terrain, a biome can be specified (in code) to get certain types of terrain. Current list it ocean(not very interesting), mountains and islands.

# Zoom
To zoom in or out, scroll the mouse wheel.

# Waves
Although not extremely interesting, I added a stage between water and beach to simulate waves.

# Deer
There is a slider where you can select the amount of deer (AI) to spawn in the map. The deer stalks the player around walking around the terrain. There is a current bug that messes with the texture of the deer model.
