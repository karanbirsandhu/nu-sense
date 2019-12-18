# Nu-Sense
---
## Organization :
**Carleton University**

## Authors
---
*Karanbir Singh Sandhu*    karanbirsandhu@cmail.carleton.ca

*Akaljot Singh Dhillon*    akaljotsinghdhillon@cmail.carleton.ca

*Ripudaman Singh*          ripudamansidhu@cmail.carleton.ca

## Description

Nu-sense is a software that implements the data fusion algorithm  using principal component analysis by gathering data from multiple sensors and at first, a support degree matrix is established, Then we obtain each sensor’s integrated support degree score by PCA. According to their scores, we decide validity of sensor’s observation data. For abnormal observation data, they must be deleted before fusion. For the rest valid observation data, they are allocated corresponding weight coefficient in the light of their integrated support degree scores. Finally the fusion results are obtained. The proposed method objectively reflects the mutual support degree of sensors without knowing prior knowledge and assuming independent sensor noises.

By properly administrating multiple sensors and their measurements, it can achieve improved accuracy and more specific inferences than those using a single sensor alone

This implementation uses an algorithm discussed in this paper:

*"A Simple Multi-sensor Data Fusion Algorithm Based on Principal Component Analysis"* by GAO Hongyan

## Folder Structure of the repository

.
+--bin
│   +--main.exe
|
+--data
│   +--input_data
│   │       sample_input.csv
│   │
│   +--output_data
│   │       output.csv
|   |
│   +--sensor_history
│           sensor_history.csv
│
+--doc
│       Flow_Diagram.png
|       NU-Software Design Document.pdf
│       architecture.png
│       developer_manual.pdf
│       user_manual.pdf
│
+--include
│       data_process.h
│       externs.h
|       fileio.h
|       sensor_history.h
|       sensor_validation.h
│
+--lib
│       gsl-2.6
│
+--src
│       data_process.c
│       main.c
|       fileio.c
|       sensor_history.c
|       sensor_validation.c
│
+--test
|   +--bin
│   +--data
|   +--include
|   +--lib  
│   +--src
|  
|   .gitignore                 
│   LICENSE
|   Makefile
|   README.md

## Instructions for compilation/build:

1. Install MinGW and extract msys.
2. Add path in Environment variables.
3. Copy msys to MinGW.
4. Rename mingw-makefile.exe to make (File present in C->MinGW->bin)
5. Compile GSL library using ./configure command. (Navigate to lib gsl2.6 and use ./configure for compilation)
6. Place nu-sense project to lib folder (C:\MinGW\msys\1.0\home\user\nu-sense)
7. navigate to nu-sense.
8. use "make clean"
9. run "make" command".
10. navigate to bin folder in nu-sense
11. run main.exe

## User manual
Detailed end user documentation can be found at /doc/user_manual.pdf
## developer manual
Detailed developer documentation can be found at/doc/developer_manual.pdf
## NU-software design documentation
Detailed coding convention documentation can be found at /doc/NU-software design documentation.pdf
