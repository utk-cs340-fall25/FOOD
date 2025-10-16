# Sprint n (1, 2, 3, or 4)

(include your name, github id, and group name here)
Robert Duardo
WanabeNecro, 
FOOD

### What you planned to do

Implement recipe loading system from recipies.txt 
Add ingredient-based filtering system using a QListWidget 
Improve UI layout to include recipe details and filter options 
Expand recipies.txt to 50 entries with tiered “filling” levels 

### What you did not do

Did not finish the visual alignment and layout of the ingredient filter
Tier-based “filling” filter not yet implemented
Recipe detail panel layout still unpolished


### What problems you encountered

recipies.txt not being found during runtime due to build directory path differences
Undefined reference errors caused by missing function declarations
Difficulty positioning new UI widgets within the existing layout
Layout corruption and blank UI caused by dynamic widget generation in code
UI spacing and widget scaling issues in Qt Designer


### Issues you worked on

 Implemented recipe file loading and error checking
 Added multi-ingredient filter system using QListWidget
 Adjusted layout to include filter, recipe list, and detail view
 Fixed recipies.txt copy process through CMake build step

### Files you worked on
just look in the RobsSearchFunc i honestly dont care to name them, its all me

### Use of AI and/or 3rd party software
Used ChatGPT (GPT-5) to assist with debugging Qt build errors, resolving path handling for recipies.txt, and designing new UI layout logic for filter panels. No 3rd-party code directly copied into the project.

### What you accomplished
Implemented full recipe loading from an external text file, connected filtering and UI components, and ensured the program could dynamically populate recipe data into a table. Added logic to handle missing files gracefully and to support future expansion for tier and region filtering.