# DWA
Distributed Worklist Algorithm for Monotone Framework

## The idea

Parallelize the existing intraprocedural montone data flow solver in [PhASAR](https://github.com/secure-software-engineering/phasar).

Compiling my files as a plugin did not work (phasar was not recognizing my .so file, although I followed the instructions from `--help`)

## Prerequisites

* C++ 17 and Boost v1.65+
* LLVM v12
* PhASAR

Install LLVM and PhASAR manually or use the Docker container (*coming soon*):

```
docker pull ivan23kor/dwa
```

_(c) Ivan Korostelev, 2020_
