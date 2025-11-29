Recipe Search — owned-ingredients filtering

What changed

- RRSearchWindow now supports an "Only show recipes I can make" checkbox.
- The window exposes setOwnedIngredients(const QSet<QString>&) so the app can push the set of ingredients the user has on-hand into the search page.
- When the checkbox is enabled, recipes are filtered so that only those where every ingredient name appears in the owned set (case-insensitive) are shown.

How it's wired in the app

- The Input tab in main.cpp maintains a local map of ingredients -> owned (bool). When an item checkbox changes, main.cpp now builds a QSet of owned ingredient names and calls rsearchPage->setOwnedIngredients(ownedSet).
- The Search page will automatically refresh upon receiving the owned-ingredients update (and when the checkbox toggle changes).

Notes & future work

- Ingredient name matching is exact (case-insensitive); for better UX you may want to normalize singular/plural, synonyms, and fraction/units parsing.
- Because this is a UI-level feature, unit testing requires a Qt test harness — you can add a QtTest QTest case in this project to exercise RRSearchWindow::updateFilter.

If you want, I can also:
- Add a small QtTest unit to automatically verify filter behavior.
- Make matching tolerant to quantities/units (e.g. ignore numeric prefixes and units when matching).
- Persist the input selections across runs.
