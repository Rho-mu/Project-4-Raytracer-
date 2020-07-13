# Introduction
CS 430 Project 3: Recursive Ray Tracing
This Project is supposed to ray trace basic shapes to a scene, along
with lights to create lighting and shadows on the objects, and reflection
and refraction.

# Usage
takes 4 arguments:

1: The width of the scene greater than 0

2: The Height of the scene greater than 0

3: The input file (Doesn't change anything)

4: The output file (I've been using .data instead of .ppm, because I haven't
                    been able to write to ppm)

# Authors
Riley McWilliams - rcm264@nau.edu

# Known Errors
This program does not work!!
I haven't fixed most of the problems I was having before. Refer to Known Errors for Project 3.
I feel like I implemented refraction and reflection correctly. I understood all of the math and how to put it into code.
I also understood the recursion and think I implemented that correctly. Basically, I'm pretty sure that I did
everything new for this project except the new parsing. I added new variables for parsing, but ended up hard-coding them.
Also, I was able to at least get an output.data file that was colored completely with the background color, but I must've
done something toward the end of the project that screwed it up, because I haven't gotten that result since.

# Author's Notes
I really do want to be able to get this program working eventually, but it's been tough so far. I'll probably come
back to it sometime in the future when I have some time and work on it again.


# Known Errors - Project 3 Illumination
I was able to get a lot further on this project than the previous one. I've been trying to get it down, but nothing clicked for me until now. I finally implemented vectors as type "v3". For some reason I thought v3 was a library function. I tried having separate files for stuff like vector math and intersection tests, but I couldn't get that to work, so my code is kind of all together and messy looking. I recommend collapsing all of the functions to make it easier to navigate.

Since I was so behind on the other projects, I was only able to get up to somewhat raycasting.
The program:
-Declares structs for vectors, objects, lights, and pixels.
-Writes to a .data file at least. I was able to see color come up when I opened it up.
-Shoots unit rays over a grid of pixels and colors them.
-Checks for not enough arguments and invalid width and height arguments.

This program does not:
-Do basically anything else.
-Raycast properly, especially with shading.
-Write specifically to ppm.
