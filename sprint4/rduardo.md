# Sprint 4

Robert Duardo
WanabeNecro
FOOD

### What you planned to do
- Implement recipe search functionality with tagging support
- Fix bugs in the ingredient and recipe system
- Integrate Qt UI components for the search and inventory features
- Debug and resolve issues with ingredient file persistence

### What you did not do
- Complete full production-ready recipe tags implementation (ongoing testing)
- Comprehensive error handling for edge cases in file I/O

### What problems you encountered
- Ingredients file adding unwanted spaces between quantity "1" and ingredient names on each program run
- Qt build cache issues causing clangd index regeneration
- Initial difficulties getting recipe search window functionality to work correctly

### Issues you worked on
- Recipe search window UI implementation and functionality
- Ingredient file parsing and storage issues
- Recipe tagging system
- Qt inventory model integration
- Main window and dialog component fixes

### Files you worked on
- `main project/mainproject/Rrsearchwindow.cpp` (Recipe search window implementation)
- `main project/mainproject/Rrsearchwindow.h` (Recipe search header)
- `main project/mainproject/food.cpp` (Core food class logic)
- `main project/mainproject/food.h` (Food class definitions)
- `main project/mainproject/main.cpp` (Main entry point)
- `main project/mainproject/Radditemdialog.cpp` (Add item dialog)
- `main project/mainproject/Rinventorymodel.cpp` (Inventory model)
- `main project/mainproject/Rmainwindow.cpp` (Main window)
- `main project/mainproject/recipes and ingredients/ingredients.txt` (Ingredient data)

### Use of AI and/or 3rd party software
- Didnt use any for these last 2 weeks

### What you accomplished
- Successfully implemented recipe search with tagging functionality (working on local testing)
- Created dynamic recipe loading from .rcp files
- Fixed multiple Qt compilation and linking issues
- Implemented ingredient file persistence (with ongoing bug fix for spacing issue)
- Integrated Matcha Banana Smoothie and Zucchini Chips recipes into test data
- Resolved merge conflicts and maintained codebase stability across 10+ commits in the sprint period
