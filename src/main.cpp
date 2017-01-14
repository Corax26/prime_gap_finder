#include <iostream>
#include <thread>
#include <tuple>

#include <cmath>

#include "prime.h"

namespace
{

GapInfo find_max_gap_range(Int start, Int end, Int delta,
                           const PrimeList &prime_list)
{
    GapInfo res;
    Int base = start;

    GapInfo prev_info;
    {
        Sieve sieve = compute_sieve(base, delta, prime_list);
        prev_info = find_max_gap(sieve);
        res.max_gap = prev_info.max_gap;
        res.max_p = base + prev_info.max_p;
        res.lower_p = base + prev_info.lower_p;
    }

    GapInfo cur_info;
    for (base += delta; base < end; base += delta)
    {
        Int size = std::min(delta, end - base);
        Sieve sieve = compute_sieve(base, size, prime_list);
        cur_info = find_max_gap(sieve);

        if (cur_info.max_gap > res.max_gap)
        {
            res.max_gap = cur_info.max_gap;
            res.max_p = base + cur_info.max_p;
        }

        Int delta_gap = delta + cur_info.lower_p - prev_info.upper_p;
        if (delta_gap > res.max_gap)
        {
            res.max_gap = delta_gap;
            res.max_p = base - delta + prev_info.upper_p;
        }

        prev_info = cur_info;
    }

    res.upper_p = cur_info.upper_p;

    return res;
}

std::tuple<Int, Int> prime_gap_finder(Int limit)
{
    if (limit == 5)
    {
        return {2, 3};
    }

    auto delta = static_cast<Int>(sqrt(limit));

    // We assume the biggest gap won't be in the first sqrt(limit)
    PrimeList prime_list = get_primes(delta);

    GapInfo gap_info = find_max_gap_range(delta + 1, limit, delta, prime_list);

    return {gap_info.max_gap, gap_info.max_p};
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
