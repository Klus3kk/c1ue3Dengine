# 3D-Engine-in-C
Here we go, let's make 3D Engine in C. Why won't I do it in the easier programming languages like Python? Because I like challenges lol (this will be a long ride, help).
Every day I will write about my progress.


(1) So, I've begun watching some tutorials and read documentation about computer graphics. And for your record I've never done anything related to 3D engine etc. so it was really interesting. And what I've found out:
- C is literally one of the worst languages I could write a 3D engine,
- I will need SDL.h library for providing low level access to graphics hardware via OpenGL (which I will probably choose because Vulcan might be problematic) and Direct3D and also creating and controlling a window, (I will use the 2.0 version, I will write how to install it later),
- 
Tomorrow I will start making the first steps, so stay tuned! 

Sources from which I learned a lot:


Videos:
- https://www.youtube.com/watch?v=nvWDgBGcAIM&ab_channel=GraverDev
- https://www.youtube.com/watch?v=Pj0_O94ES_o&ab_channel=CarlthePerson
- https://www.youtube.com/playlist?list=PLYokS5qr7lSsvgemrTwMSrQsdk4BRqJU6 (It's a pretty good series on how to start making a game engine, definitely recommended!)

  
Books:
- 3D Graphics Rendering Cookbook by Sergey Kosarevsky
- Physically Based Rendering: From Theory To Implementation by Matt Pharr, Wenzel Jakob, and Greg Humphreys

  
(2) Alright, I've did some more reading and let me tell you, it's gonna take a long time to get to generate some advanced 3D models. Anyways, during my search for finding how can I install the SDL library (and no, you can't just use the cmd for downloading it, of course it had to be difficult x_x) I found out about GLEW. It's basically a loader, that helps manage OpenGL in an "easy way" and it adds more modern options (we'll see in practice if that's true). So, I think that it will help me with the development. So, we're adding GLEW to the list of "Important Libraries".


UPDATE: IT TOOK A LOT OF TIME BUT FINALLY, I'VE PREPARED EVERYTHING. Even the cursed SDL library (installing it took probably 1-2h cuz of so little tutorials of how to do it, but I managed to do it). Unfortunately I had to change the environment from VS Code to VS because somehow, it just didn't work on VS Code. 


So now, I've started preparations and basically, I just want to print a window with a single pixel on the center, that's all I care about right now.


And here's my final result for today:


<p align="center">
  <img width="460" height="300" src="https://github.com/Klus3kk/3D-Engine-in-C/assets/93116510/fe524b90-3329-4ecc-be93-171d9f14abff">
</p>


(3) Today I've read about how where should I begin development of my 3D engine. And let me tell you, there's a lot of math concepts to comprehend, like 3-dimensional Cartesian spaces, points and vectors, tuples, scalars etc.
Here are some links that I found useful:
- https://code.tutsplus.com/lets-build-a-3d-graphics-engine-points-vectors-and-basic-concepts--gamedev-8143t
- (here's a cool project that someone made, it's pretty similar of what I want to make) https://www.codeproject.com/Articles/1168552/Build-My-Own-3D-Graphics-Engine-Step-By-Step
- (that's also a pretty great tutorial) https://www.sitepoint.com/write-3d-soft-engine-scratch-part-1/



