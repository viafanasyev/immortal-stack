# Immortal stack


!!! THIS IS NOT A WORKING COPY OF THE PROJECT. TO SEE THE REAL PROJECT CHANGE BRANCH TO 'MASTER' !!!


## Project description

This program is developed as a part of ISP RAS course.  

This program contains a generic stack with auto logging on errors.  
Stack performs additional corruption checking: silent verification, canary guards, hash checking.  
Errors with stack are logged into `stack-dump.txt` file.

### Structure

* src/ : Main project
    * main.cpp : Entry point for the program.
    * stack.h : Definition and implementation of error-secure generic stack.
    * logger.h : Definition and implementation of logging functions and macros.
    * environment.h : Helper macros that are environment-dependent (OS, bitness, etc).

* test/ : Tests and testing library
    * testlib.h, testlib.cpp : Library for testing with assertions and helper macros.
    * main.cpp : Entry point for tests. Just runs all tests.
    * stack_tests.cpp : Tests for stack struct.

* doc/ : doxygen documentation

* Doxyfile : doxygen config file

### Usage

To use stack use the following code (or the same):
```C++
#define STACK_TYPE int // Type of the stack
#include "stack.h"
#undef STACK_TYPE

...

    // Stack_int - is a stack of ints. If you define STACK_TYPE as double, then the struct name will be Stack_double
    Stack_int s; 
    
    // Call a constructor of the stack to allocate dynamic memory
    constructStack(&s);

...

    // Put some values to the stack
    push(&s, 1);
    push(&s, 2);

    // Take some values from the stack
    pop(&s);
    int x = top(&s);

...

    // Call a destructor of the stack to free dynamic memory
    destructStack(&s);

...
```

Also you can use multiple typed stacks:

```C++

#define STACK_TYPE int
#include "stack.h" // Includes Stack_int
#undef STACK_TYPE

#define STACK_TYPE double
#include "stack.h" // Includes Stack_double
#undef STACK_TYPE

...

    // Int and double stacks can be accessed
    Stack_int s1;
    Stack_double s2;

...

```

To enable corruption checking use the STACK_SECURITY_LEVEL macro (define it before including stack.h):

```C++

#define STACK_SECURITY_LEVEL 0 // No checks performed
#define STACK_SECURITY_LEVEL 1 // Silent verification, logging
#define STACK_SECURITY_LEVEL 2 // Silent verification, logging, canary guards
#define STACK_SECURITY_LEVEL 3 // Silent verification, logging, canary guards, hash checking

```

### Run

#### Immortal stack

To run main program execute next commands in terminal:
```
cmake . && make
./stack
```

Main program just shows the possible incorrect behaviour.  
See the resulting `stack-dump.txt` file to see the example stack dump.

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