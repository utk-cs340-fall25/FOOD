# Sprint 2
- Ro Neel
- rowobeans
- FOOD

### What you planned to do
- Make the checklist dynamically load from a map instead of hardcoding
- Fix the search function issue where searching multiple times in a row would eventually make all items disappear
- Fix the memory leaks in the input system
- Create and explore the tab layout system that we will use to stitch the project together

Trello link: [FOOD Trello](https://trello.com/b/7gyxaQrc/food)

### What you did not do
- Everything was accomplished.

### What problems you encountered
- There were more difficulties with learning the quirks of Qt with it's heavy reliance on specialty classes and functions unqiue to Qt. This sometimes made it feel like I was learning a new language than just C++ and may be why it seems we have "less" code.
- I'm not entirely sure how to extract out my code into a header implementation with said quirks of Qt. This is more about neatness than functionality, though.

### Issues you worked on
- [Make the checklist dynamically load from a map instead of hardcoding](https://trello.com/c/k7kVIwrA/27-making-the-checklist-dynamically-load-from-a-qstringbool-map-sprint-2)
- [Fix the search function issue where searching multiple times in a row would eventually make all items disappear](https://trello.com/c/5ddvVqZG/34-fix-search-issue-with-searching-multiple-times-in-a-row-not-showing-correct-results-sprint-2)
- [Fix the memory leaks in the input system](https://trello.com/c/lvxf4a7k/33-fix-memory-leaks-in-inputtest-sprint-2)
- [Create and explore the tab layout system that we will use to stitch the project together](https://trello.com/c/0UlE2VnH/29-tab-page-system-so-that-each-part-can-be-displayed-on-its-own-and-not-conflict-with-each-other-sprint-2)

### Files you worked on
- More on FOOD/inputtest
- All of FOOD/tab layout test

### Use of AI and/or 3rd party software
- Once again, no AI or 3rd party software was used. [Qt's documentation pages](https://doc.qt.io/) were used, and Stack Overflow was used for finding the correct classes and function names. Example code was not referenced this time.

### What you accomplished
- I finished up the loose ends of the input functionality by making the checklist dynamically load and fixing issues left in sprint 1. The main two issues were memory leaks and the search function would end up breaking if multiple searches without clearing were done in a row. Then, I moved onto figureing out how we would stitch the project together using a tab layout system. I explored the tab layout and the various quirks of it. I also explained to the other members how the tab system works so that they can tweak their code to mesh together seamlessly.
