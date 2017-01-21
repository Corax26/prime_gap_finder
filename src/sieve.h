#ifndef SIEVE_H_
#define SIEVE_H_

#include <vector>

#include <cstddef>
#include <cstdint>

using Int = uint32_t;

// false = prime, true = non-prime (not very intuitive but this allows to
// zero-init the array)
static constexpr bool Prime = false;

#if !defined(SIEVE_IMPL) || SIEVE_IMPL == 1
class Sieve : private std::vector<uint8_t>
{
    using V = std::vector<uint8_t>;
public:
    Sieve(size_t n)
        : V(n, Prime) {}

    void mark_composite(Int i) { operator[](i) = !Prime; }

    Int next_prime(Int i) const
    {
        while (i < size() && operator[](i) != Prime) ++i;
        return i;
    }

    using V::size;
};
#elif SIEVE_IMPL == 2
class Sieve : private std::vector<bool>
{
    using V = std::vector<bool>;
public:
    Sieve(size_t n)
        : V(n, Prime) {}

    void mark_composite(Int i) { operator[](i) = !Prime; }

    Int next_prime(Int i) const
    {
        while (i < size() && operator[](i) != Prime) ++i;
        return i;
    }

    using V::size;
};
#elif SIEVE_IMPL == 3
class Sieve
{
public:
    Sieve(Int n)
        : data_{new bool[n]()}, size_{n} {}

    Sieve(const Sieve &) = delete;
    Sieve(Sieve &&other)
        : data_{other.data_}, size_{other.size_}
    { other.data_ = nullptr; }
    ~Sieve()
    { delete [] data_; }

    void mark_composite(Int i) { data_[i] = !Prime; }

    Int next_prime(Int i) const
    {
        while (i < size_ && data_[i] != Prime) ++i;
        return i;
    }

    Int size() const { return size_; }

private:
    bool *data_;
    Int size_;
};
#else
class Sieve
{
public:
    Sieve(Int n)
        : data_{new uint8_t[n / 8]()}, size_{n} {}

    Sieve(const Sieve &) = delete;
    Sieve(Sieve &&other)
        : data_{other.data_}, size_{other.size_}
    { other.data_ = nullptr; }
    ~Sieve()
    { delete [] data_; }

    void mark_composite(Int i) { data_[cell_idx(i)] |= (1u << cell_offset(i)); }

    Int next_prime(Int i) const
    {
        Int idx = cell_idx(i);
        if (idx >= size_ / 8) return i;

        Int b;
        uint8_t mask = ~((1u << cell_offset(i)) - 1);
        if ((data_[idx] & mask) != mask)
        {
            b = cell_offset(i);
        }
        else
        {
            do
            {
                ++idx;
                if (idx == size_ / 8) return i;
            }
            while (data_[idx] == 0xFFu);
            b = 0;
        }

        while ((data_[idx] & (1u << b)) != 0) ++b;
        return idx * 8 + b;
    }

    Int size() const { return size_; }

private:
    static constexpr Int cell_idx(Int i) { return i / 8; }
    static constexpr Int cell_offset(Int i) { return i % 8; }

    uint8_t *data_;
    Int size_;
};
#endif

#endif // SIEVE_H_
