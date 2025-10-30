# Sprint 3

(include your name, github id, and group name here)
Robert Duardo
WanabeNecro, 
FOOD

### What you planned to do

impliment my recipie search function as a widget inside of main.cpp under mainproject
i also wanted to get my code to run off of a seperate data system and not just the hard coded recipies.txt file ive been using this whole time 

### What you did not do

I wasnt able to get the data system change done in time for the presentation so thats still a big red TODO


### What problems you encountered

I had no idea how to turn my program into a widget and i created and had to work around a lot of issues

### Issues you worked on

 Implemented recipe file loading and error checking
 Added multi-ingredient filter system using QListWidget
 Adjusted layout to include filter, recipe list, and detail view
 Fixed recipies.txt copy process through CMake build step

### Files you worked on
ok so most of my changes would be the files that start with R inside of "mainproject" as i needed to get it to play well as a widget instead of a window i also needed to canibalize some code from my old main.cpp to work inside of Ro's code, then change her project file to include the headers, forms, and other files needed to work my code. 

### Use of AI and/or 3rd party software
mostly for debugging during my integration process since as you might expect it was not a smooth transition, so i used cursor to look through my github repository and help me whenever i got stuck on vauge issues or oversights in this massive repository.


### What you accomplished
I was able to get the code to run inside of the main window as a widget but it currently doesnt have any information to disolay due to the unifinished data transformation.