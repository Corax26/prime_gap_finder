prime_gap_finder
================

This is a simple C++ program that finds the largest number of consecutive non-prime numbers (in other words, the maximum prime gap) below a certain limit. It uses a rather simple algorithm (segmented sieve) to find all the primes within the limit and the gaps between them. Thanks to the sieve segments/deltas/chunks being computable in parallel, I have parallelised the algorithm, and by default a thread per logical core will be used.

For perfomance reasons primes are assumed to fit in a 32-bit unsigned integer and thefore the limit is up to 2^32 - 1 by default. It can be changed simply by redefining `Int` in `sieve.h`.

Building
--------

Simply use `make` to build the release version. No particular library is used, you only need a C++11 compiler. By default clang is used if available as it apparently produces better code, you can choose another compiler with `CXX` (e.g. `make CXX=g++`).

Running
-------

To find the largest gap up to 2 billions:
`./prime_gap_finder $((2*10**9))`
