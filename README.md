# Parallels

Implementation of the Parallels project.

Tested on MacOS Ventura 13.6.4.

Made on December 8 2023 as part of my education in School 21 :)

# Information

This project introduces basic approaches to parallelism, and implementation of algorithms applying it.

**Synchronous programming** is a programming model where each thread has a set of tasks. All tasks within the thread are executed sequentially, and when one task is completed, it is possible to do another one. In this model you can’t stop the execution of a task in order to perform another one in between.

A special case of synchronous programming is *single-threading*. If there are several tasks to be executed, and the current system provides one thread that can handle all the tasks, then it takes one by one.

In cases where the order in which the tasks are executed does not affect the result of the program, *Multithreading* can be applied.

Multithreading is another case of the synchronous programming in which there are many threads that can take tasks and start working on them, i.e. we have thread pools and many tasks.

Creating new threads each time is unadvisable, because it requires using additional system resources such as CPU time and memory. So, the initial number of threads should be set in advance.

## Part 1. Ant colony optimization algorithm (ACO)

Implement an ACO algorithm to solve the traveling salesman problem from the [SimpleNavigator_1.0 project](https://github.com/Sanf1r/SimpleNavigator_v1.0) with and without parallel computing:
- The program have a console interface
- The user sets the matrix for the traveling salesman problem
- The user sets the number of executions *N*
- Display the results of each of the algorithms for the specified matrix
- Measure the time it takes to execute *N* times an ACO algorithm applying parallelism for a user-defined matrix
- Measure the time it takes to execute *N* times the usual ant algorithm for a user-defined matrix
- Display the obtained time

## Part 2. Winograd algorithm

Implement the Winograd algorithm of matrix multiplication without using parallelism, as well as using pipeline and classical methods of parallelism:

- There should be four stages of the pipeline work
- The program must have a console interface
- There should be two ways of entering:
    - The user sets both matrices for multiplication
    - The user sets dimensions of the matrices, which are then filled into the program randomly
- The user sets the number of executions *N*
- Display the results of each of the algorithms as well as the generated matrices
- Measure the time it takes to execute *N* times matrix multiplication without using parallelism
- Measure the time it takes to execute *N* times matrix multiplication using classical parallelism with the number of threads equal to 2, 4, 8, ..., 4 * (number of logical computer processors)
- Measure the time it takes to execute *N* times matrix multiplication using pipeline parallelism
- Display the obtained time

Both parts use Open Multi-Processing API.

# Usage

Build with **make** command and you got two executable files with console interface :)

