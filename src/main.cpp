#include <iostream>
#include <thread>
#include <tuple>

#include <cmath>

#include "prime.h"

namespace
{

std::tuple<Int, Int> prime_gap_finder(Int limit)
{
    if (limit == 5)
    {
        return {2, 3};
    }

    auto delta = static_cast<Int>(sqrt(limit));

    // We assume the biggest gap won't be in the first sqrt(limit)
    PrimeList prime_list = get_primes(delta);

    Int base = delta + 1;
    GapInfo prev_gap_info;
    Int max_gap, max_p;
    {
        Sieve sieve = compute_sieve(base, delta, prime_list);
        prev_gap_info = find_max_gap(sieve);
        max_gap = prev_gap_info.max_gap;
        max_p = prev_gap_info.max_p;
    }

    for (base += delta; base < limit; base += delta)
    {
        Int size = std::min(delta, limit - base);
        Sieve sieve = compute_sieve(base, size, prime_list);
        GapInfo gap_info = find_max_gap(sieve);

        if (gap_info.max_gap > max_gap)
        {
            max_gap = gap_info.max_gap;
            max_p = base + gap_info.max_p;
        }

        Int delta_gap = delta + gap_info.lower_p - prev_gap_info.upper_p;
        if (delta_gap > max_gap)
        {
            max_gap = delta_gap;
            max_p = base - delta + prev_gap_info.upper_p;
        }

        prev_gap_info = gap_info;
    }
    
    return {max_gap, max_p};
}

}

int main(int argc, char **argv)
{
    Int limit = 100*1000*1000u;
    unsigned count = 1;

    try
    {
        if (argc >= 2) limit = std::stoul(argv[1]);
        if (argc >= 3) count = std::stoul(argv[2]);
    }
    catch (std::exception&)
    {
        return 1;
    }

    if (limit < 5 || count == 0) return 1;

    Int max_gap, max_p;
    for (unsigned i = 0; i < count; ++i)
    {
        std::tie(max_gap, max_p) = prime_gap_finder(limit);
    }

    std::cout << "Max gap is " << max_gap << ", between " 
              << max_p << " and " << max_p + max_gap << std::endl;
    std::cout << "Max seq is " << max_gap - 1 << ", between " 
              << max_p + 1 << " and " << max_p + max_gap - 1 << std::endl;

    return 0;
}
