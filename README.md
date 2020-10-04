# Immortal stack

## Project description

This program is developed as a part of ISP RAS course.  

### Structure

* src/ : Main project
    * main.cpp : Entry point for the program.

* test/ : Tests and testing library
    * testlib.h, testlib.cpp : Library for testing with assertions and helper macros.
    * main.cpp : Entry point for tests. Just runs all tests.

* doc/ : doxygen documentation

* Doxyfile : doxygen config file

### Run

#### Immortal stack

To run main program execute next commands in terminal:
```
cmake . && make
./stack
```

#### Tests

To run tests execute next commands in terminal:
```
cmake . && make
./tests
```

### Documentation

Doxygen is used to create documentation. You can watch it by opening `doc/html/index.html` in browser.  

### OS

Program is developed under Ubuntu 20.04.1 LTS.