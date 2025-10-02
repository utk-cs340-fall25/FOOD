# Sprint 1
- Ro Neel
- rowobeans
- FOOD

### What you planned to do
- Ingredients checklist that user can select if the item is had or not
- Taking the input data and updating the internal storage (QString:bool map)
- Displaying all the selected items for reference
- Search bar for the checklist to display only which checkboxes have the search terms

Trello link: [FOOD Trello](https://trello.com/b/7gyxaQrc/food)

### What you did not do
- Everything was accomplished.

### What problems you encountered
- Learning Qt was it's own challenge, and getting used to it was difficult.
- My current code probably has memory leaks.

### Issues you worked on
- [Ingredients checklist that user can select if the item is had or not](https://trello.com/c/XusMhaVt/1-ingredients-checklist-that-user-can-select-if-the-item-is-had-or-not-sprint-1)
- [Taking the input data and updating the internal storage (QString:bool map)](https://trello.com/c/PCmNRquQ/3-taking-the-input-data-and-updating-the-internal-storage-qstringbool-map-sprint-1)
- [Displaying all the selected items for reference](https://trello.com/c/NsXPTN0q/28-displaying-all-the-selected-items-for-reference-sprint-1)
- [Search bar for the checklist to display only which checkboxes have the search terms](https://trello.com/c/rrIoypM2/2-search-bar-for-the-checklist-to-display-only-which-checkboxes-have-the-search-terms-sprint-1)

### Files you worked on
- All of FOOD/inputtest/

### Use of AI and/or 3rd party software
- I did not use any AI or 3rd party software at all. I used [Qt's documentation pages](https://doc.qt.io/) and looked up examples of function/variable use on Stack Overflow. However, no code was copied.

### What you accomplished
- I made the main structure of how the user will input their data (the ingredients they own) into the system and store that data for later. This consists of a list of checkboxes and an additional search ability to quickly find items in the list to check or uncheck. The user can see what items they have selected at the bottom to know if they've already checked the box even if the item is not currently displayed in the list (such as for search reasons). The search also resets if the search button is pressed while the search field is empty.
