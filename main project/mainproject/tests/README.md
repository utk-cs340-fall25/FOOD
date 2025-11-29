How to run tests

This test uses Qt Test (QTest) to exercise RRSearchWindow filtering logic.

Steps to run (requires Qt/qmake):

```bash
cd "main project/mainproject"
# Build the tests (create a separate target or add this file to your build)
qmake "CONFIG+=debug" 
make -j4
# Or add tests/test_rrsearchwindow.cpp into a test pro file and build that target separately
```

This repository doesn't currently include an automated CI/test runner for Qt, so you'll need a Qt-enabled environment to compile and run the test executable.
