#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstring>

#include "prime.h"

namespace
{

GapInfo find_max_gap_range(Int start, Int end, Int delta,
                           const PrimeList &prime_list)
{
    assert(end - start > delta);
    GapInfo res;
    Int base = start;

    GapInfo prev_info;
    {
        Sieve sieve = compute_sieve(base, delta, prime_list);
        prev_info = find_max_gap(sieve);
        prev_info.shift(base);
        res.max_gap = prev_info.max_gap;
        res.max_p = prev_info.max_p;
        res.lower_p = prev_info.lower_p;
    }

    GapInfo cur_info;
    cur_info.upper_p = prev_info.upper_p;
    for (base += delta; base < end; base += delta)
    {
        Int size = std::min(delta, end - base);
        Sieve sieve = compute_sieve(base, size, prime_list);
        cur_info = find_max_gap(sieve);
        cur_info.shift(base);

        if (cur_info.max_gap > res.max_gap)
        {
            res.max_gap = cur_info.max_gap;
            res.max_p = cur_info.max_p;
        }

        Int delta_gap = cur_info.lower_p - prev_info.upper_p;
        if (delta_gap > res.max_gap)
        {
            res.max_gap = delta_gap;
            res.max_p = prev_info.upper_p;
        }

        prev_info = cur_info;
    }

    res.upper_p = cur_info.upper_p;

    return res;
}

void worker_thread(Int start, Int end, Int delta, const PrimeList &prime_list,
                   GapInfo &res)
{
    res = find_max_gap_range(start, end, delta, prime_list);
}

std::tuple<Int, Int> prime_gap_finder(Int limit, unsigned nb_threads,
                                      unsigned delta_multiplier)
{
    if (limit == 5)
    {
        return {2, 3};
    }

    auto delta = static_cast<Int>(sqrt(limit));

    // We assume the biggest gap won't be in the first sqrt(limit)
    const PrimeList prime_list = get_primes(delta);
    Int start = delta + 1;
    Int end = limit + 1; // end is not inclusive

    GapInfo res;
    if (nb_threads > 1 && (end - start) >= nb_threads * 2 * delta)
    {
        std::vector<std::thread> threads;
        threads.reserve(nb_threads);
        std::vector<GapInfo> gap_infos(nb_threads);

        // Threads compute x chunks of delta numbers
        Int nb_chunks = (end - start) / delta / nb_threads;
        Int thread_delta = nb_chunks * delta;
        if (delta_multiplier > nb_chunks) delta_multiplier = nb_chunks;

        for (unsigned i = 0; i < nb_threads; ++i)
        {
            threads.emplace_back(worker_thread,
                                 start + i * thread_delta,
                                 std::min(start + (i + 1) * thread_delta, end),
                                 delta * delta_multiplier,
                                 std::cref(prime_list),
                                 std::ref(gap_infos[i]));
        }

        // Similar gap info merging as in find_max_gap_range()
        unsigned i = 0;
        GapInfo prev_info;
        threads[i].join();
        prev_info = gap_infos[i];
        res.max_gap = prev_info.max_gap;
        res.max_p = prev_info.max_p;

        GapInfo cur_info;
        for (++i; i < nb_threads; ++i)
        {
            threads[i].join();
            cur_info = gap_infos[i];

            if (cur_info.max_gap > res.max_gap)
            {
                res.max_gap = cur_info.max_gap;
                res.max_p = cur_info.max_p;
            }

            Int delta_gap = cur_info.lower_p - prev_info.upper_p;
            if (delta_gap > res.max_gap)
            {
                res.max_gap = delta_gap;
                res.max_p = prev_info.upper_p;
            }

            prev_info = cur_info;
        }
    }
    else
    {
        res = find_max_gap_range(start, end, delta, prime_list);
    }

    return {res.max_gap, res.max_p};
}

void usage(const char *program_name)
{
    std::cout 
        << "Usage: " << program_name 
        << " [-n <nb_thread>] [-d <delta_multiplier>] [limit [count]]"
        << std::endl;
    exit(1);
}

}

int main(int argc, char **argv)
{
    const char *program_name = argv[0];
    Int limit = 100*1000*1000u;
    unsigned count = 1;
    unsigned nb_threads = std::thread::hardware_concurrency();
    unsigned delta_multiplier = 1;

    try
    {
        while (argc > 2 && strlen(argv[1]) == 2 && argv[1][0] == '-')
        {
            switch (argv[1][1])
            {
                case 'd':
                    delta_multiplier = std::stoul(argv[2]); break;
                case 't':
                    nb_threads = std::stoul(argv[2]); break;
                default:
                    throw std::exception{};
            }
            argc -= 2; argv += 2;
        }

        if (argc > 1) limit = std::stoul(argv[1]);
        if (argc > 2) count = std::stoul(argv[2]);
    }
    catch (std::exception&)
    {
        usage(program_name);
    }

    if (limit < 5 || count == 0) usage(program_name);

    Int max_gap, max_p;
    for (unsigned i = 0; i < count; ++i)
    {
        std::tie(max_gap, max_p) = prime_gap_finder(limit, nb_threads,
                                                    delta_multiplier);
    }

    std::cout << "Max gap is " << max_gap << ", between "
              << max_p << " and " << max_p + max_gap << std::endl;
    std::cout << "Max seq is " << max_gap - 1 << ", between "
              << max_p + 1 << " and " << max_p + max_gap - 1 << std::endl;

    return 0;
}
