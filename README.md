# GRASP Algorithm implementation

## Author

Airam Rafael Luque León (alu0101335158@ull.edu.es)

## Description:

In this repository, we implement the GRASP algorithm for the traveling salesman problem.

## Organization of the repository:

```
├── Makefile
├── README.md
├── bin
│   └── main.exe
├── src
│   ├── algorithm.cc
│   ├── algorithm.h
│   ├── local_search.cc
│   ├── local_search.h
│   ├── main.cc
│   ├── problem.cc
│   ├── problem.h
│   ├── route.h
│   └── solution.h
└── test
    ├── I40j_2m_S1_1.txt
    ├── I40j_4m_S1_1.txt
    ├── I40j_6m_S1_1.txt
    └── I40j_8m_S1_1.txt
```

## Usage:

```Bash
$ make
$ ./bin/main.exe <input_file>
```

### Example:

```Bash
$ ./bin/main.exe test/I40j_2m_S1_1.txt
```

## Bibligraphy:

[GREEDY RANDOMIZED ADAPTIVE SEARCH PROCEDURES: ADVANCES AND APPLICATIONS](http://www.optimization-online.org/DB_FILE/2008/07/2038.pdf)
