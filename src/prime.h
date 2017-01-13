#include <tuple>
#include <vector>

#include <cstddef>
#include <cstdint>

using Int = uint32_t;
using PrimeList = std::vector<Int>;

// false = prime, true = non-prime (not very intuitive but this allows to
// zero-init the array)
static constexpr bool Prime = false;

class Sieve : private std::vector<bool>
{
    using V = std::vector<bool>;
public:
    Sieve(size_t n)
        : V(n, Prime) {}

    using V::operator[];
    using V::size;
};

struct GapInfo
{
    Int max_gap;
    Int max_p;
    Int lower_p;
    Int upper_p;
};

PrimeList get_primes(Int n);

Sieve compute_sieve(Int start, Int n, const PrimeList &primes);

GapInfo find_max_gap(const Sieve &sieve);
