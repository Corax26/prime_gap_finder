#include <iostream>

#include <cmath>
#include <cassert>

#include "prime.h"

PrimeList get_primes(Int n)
{
    Sieve sieve{n+1};

    // Mark composite numbers in the sieve
    Int p = 2;
    auto bound = static_cast<Int>(sqrt(n));
    while (p <= bound)
    {
        for (auto i = p * p; i <= n; i += p)
            sieve[i] = !Prime;

        while (sieve[++p] == !Prime);
    }

    // Find prime numbers in the sieve
    PrimeList primes;
    // As per the prime number theorem, the number of prime numbers below n
    // is approximated by n/ln(n)
    primes.reserve(static_cast<size_t>(n / log(n)));

    for (Int i = 2; i <= n; ++i)
    {
        if (sieve[i] == Prime)
            primes.push_back(i);
    }

    return primes;
}

Sieve compute_sieve(Int start, Int n, const PrimeList &primes)
{
    Sieve sieve{n};

    // Mark composite numbers in the sieve
    for (auto p : primes)
    {
        for (auto i = (start - 1 + p) / p * p - start; i < n; i += p)
            sieve[i] = !Prime;
    }

    return sieve;
}

GapInfo find_max_gap(const Sieve &sieve)
{
    Int max_p = 0, max_gap = 0;
    Int i, p, n = sieve.size();
    Int lower_p, upper_p;

    for (i = 0; sieve[i] == !Prime && i < n; ++i);
    lower_p = i;
    assert(i != n);

    for (;;)
    {
        p = i;
        for (++i; sieve[i] == !Prime && i < n; ++i);
        if (i == n) break;

        Int gap = i - p;
        if (gap > max_gap)
        {
            max_p = p;
            max_gap = gap;
        }
    }
    upper_p = p;

    return {max_gap, max_p, lower_p, upper_p};
}
