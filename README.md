# FOOD
- Ro Neel - rowobeans
- Ethan Molder - wmolder
- Philip Vickery -PhilipVickery
- Robert Duardo - WanabeNecro
- Pablo Storch - Durza14

![FOOD logo](https://github.com/utk-cs340-fall25/FOOD/blob/main/food.png)

# Project Description
Our project, FOOD, allows the user to input the ingredients they own and then search a list of recipes based on those ingredients. This is the core part of the project. Additionally, the user can browse recipes they do not have the ingridients for and add items to a shopping list based on those recipes.

# Installation Instructions for Windows
- Download RELEASE.zip from FOOD/RELEASE.zip
- Extract the zip file
- Open the extracted folder RELEASE/executable/
- Run FOOD.exe

If your computer blocks FOOD.exe from running:
- Right click FOOD.exe
- Select "Properties"
- Under the "General" tab, there should be an "Unblock" setting with a checkbox at the bottom
- Check the "Unblock" checkbox
- Hit apply and try to run FOOD.exe again

# How to use FOOD
- The first tab is for inputting the ingredients owned. Simply click the checkbox next to an ingredient to add it to your list. For searching, input what you want to search, and click the search button. Clicking the search button with an empty field will reset back to showing all.
- The second tab allows you to check for the recipes you can make with your ingredient list. The search function works similarly, and you can filter by regional tags for recipes, too. Click on a recipe name to display it.
- The third tab allows you to see all recipes, regardless of if you own the ingredients. Click a recipe to display it.
- The fourth tab allows you to manage a personal shopping list. Click on a recipe to see it's ingredients, and select which ingredient you would like to add to your shopping list. You can also set the priority before doing so, or add all the ingreidents from the recipe. The bottom also lets you add a custom ingredient if you wish.

To add your own recipes and ingredients:
- Navigate to RELEASE/recipes and ingredients
- Add your recipe file in the same format as the rest of the files, and change your file's extenstion to ".rcp"
- You can open the .rcp files in any text editor to see how they're formatted
- Open "ingredients.txt" and add your ingredients to the list with the same format
- 0 for not had, and 1 for had, followed by the ingredient name
