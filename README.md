# RED DOT FOREVER

------

Jordan Yu  
For version 1.04.1  (*25-April-2015*)

## Introduction

Red Dot Forever is a very small program used to record/save MIDI input from a device.  
The original software was created by Matthijs Hollemans.  
The program, as well as the source, can be on [sourceforge](http://sourceforge.net/projects/reddot/).  

## Goals

I found this program because I wanted to record midi files from my digial keyboard.
When I found that the source-code was also "freely" avaiable I felt like it was
a good project for me to pick up.  

There are several reasons I felt this was a good project:  

* I wanted to learn more about Windows application development. It is a very small
code base (~10 cpp files) so the project is a good size for me to sink my teeth in.
* Project hasn't had any development since 2007. One of my professors says that 
to be a better programmer you should try to pick-up an abandoned project and 
read through the code and try to 'hack' it to do things you want. He believes it
is helpful in developing your system/program reverse engineering skills... so here 
I am trying it out.


## Target Features  
-   Unicode/UTF-8 support
-   Remove registry dependency
-   Add Tests
-   Create a new simpler GUI
-   Cross-platform (Windows/Linux) (stretch)


## Build

I'm currently using Visual Studio 2013 for this project.  
I don't have specific tips on how to build this project with other compilers,
but I will try to get to that later (date 29-apr-2015).

## Development Notes:
_28-april-2015_  
-   Converted application for mostly unicode/utf-8 support  
-   Got rid of using the Windows registry. Use a "settings.ini" file

_25-april-2015_  
-   Added Unicode/UTF-8 support?  
-   Started writing tests. Using ("Catch")[https://github.com/philsquared/Catch] framework  