#ifndef PRIME_H_
#define PRIME_H_

#include <tuple>
#include <vector>

#include <cstddef>
#include <cstdint>

#include "sieve.h"

using PrimeList = std::vector<Int>;

struct GapInfo
{
    Int max_gap;
    Int max_p;
    Int lower_p;
    Int upper_p;

    void shift(Int base) { max_p += base; lower_p += base; upper_p += base; }
};
std::ostream& operator<<(std::ostream &o, const GapInfo &gap_info);

PrimeList get_primes(Int n);

Sieve compute_sieve(Int start, Int n, const PrimeList &primes);

GapInfo find_max_gap(const Sieve &sieve);

#endif // PRIME_H_
