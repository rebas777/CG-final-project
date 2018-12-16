
# Firefly
- Implementing an interactive particle system with **OpenGL** and **C++**.
- The particle system simulate a swarm of fireflies **attracted by a point light**, while every individual of them is doing some kind of **random motion**.
- Using modern Core-Profile mode of OpenGL.
- Online Video Demo can be find at: [YouTube Link](https://youtu.be/5jFhPgfzuoM)

## Details 
- The player will control a flying camera in a 3D simple scene(mainly composed of a skybox and a mountain-like mesh), and all the fireflies will be attracted by the point where the camera is located.
- Two scenes are provided, one low poly and another realistic.
- At most 400 fireflies are supported and each of them can emit 200 second-level particles.
- Every single firefly act as a light source, which will illuminate the mountain-like mesh. Some optimizations have been done for better performance.
- Bloom effect is implemented to make fireflies shiny.

## Screenshot & Video Record
![Screenshot1](https://raw.githubusercontent.com/wiki/rebas777/CG-final-project/Screenshot1.jpg)
![Screenshot2](https://raw.githubusercontent.com/wiki/rebas777/CG-final-project/Screenshot2.jpg)
![Screenshot3](https://raw.githubusercontent.com/wiki/rebas777/CG-final-project/Screenshot3.jpg)

- Video record can be found at [release](https://github.com/rebas777/CG-final-project/releases)

More details and ideas can be found [Here(zh_CN)](https://www.jianshu.com/p/f633ae9904c7)

