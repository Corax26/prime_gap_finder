#include <iostream>

#include <cmath>
#include <cassert>

#include "prime.h"

std::ostream& operator<<(std::ostream &o, const GapInfo &gap_info)
{
    return o << "(" << gap_info.max_gap << ", " << gap_info.max_p << ") ("
             << gap_info.lower_p << ", " << gap_info.upper_p << ")";
}

PrimeList get_primes(Int n)
{
    Sieve sieve{n+1};

    // Mark composite numbers in the sieve
    auto bound = static_cast<Int>(sqrt(n));
    for (Int p = 2; p <= bound; p = sieve.next_prime(p + 1))
    {
        for (auto i = p * p; i <= n; i += p)
            sieve.mark_composite(i);
    }

    // Find prime numbers in the sieve
    PrimeList primes;
    // As per the prime number theorem, the number of prime numbers below n
    // is approximated by n/ln(n)
    primes.reserve(static_cast<size_t>(n / log(n)));

    for (Int p = 2; p <= n; p = sieve.next_prime(p + 1))
    {
        primes.push_back(p);
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
            sieve.mark_composite(i);
    }

    return sieve;
}

GapInfo find_max_gap(const Sieve &sieve)
{
    Int max_p = 0, max_gap = 0;
    Int i, p, n = sieve.size();
    Int lower_p, upper_p;

    i = sieve.next_prime(0);
    lower_p = i;
    assert(i != n);

    for (;;)
    {
        p = i;
        i = sieve.next_prime(i + 1);
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
