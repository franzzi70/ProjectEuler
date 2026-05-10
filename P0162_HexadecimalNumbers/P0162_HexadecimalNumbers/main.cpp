
#include <iostream>
#include <chrono>

uint64_t fac(uint64_t n)
{
    if (n <= 1)
        return 1;
    return n * fac(n - 1);
}

uint64_t n_over_k(int n, int k)
{
    int k_less;
    if (k > (n / 2))
    {
        k_less = n - k;
    }
    else
    {
        k_less = k;
    }
    uint64_t num = 1;
    for (int i = 0; i < k_less; i++)
    {
        num *= (n - i);
    }
    uint64_t denom = fac(k_less);
    return num / denom;
}

uint64_t power(int64_t base, int exp)
{
    if (exp == 0) return 1;
    int64_t result = 1;
    for (int i = 0; i < exp; ++i)
    {
        result *= base;
    }
    return result;
}

std::string create_hex(uint64_t n)
{
    std::string hex;
    while (n > 0)
    {
        int digit = n % 16;
        if (digit < 10)
        {
            hex = char('0' + digit) + hex;
        }
        else
        {
            hex = char('A' + (digit - 10)) + hex;
        }
        n /= 16;
    }
    return hex;
}

std::string solve()
{
    // 16 over 3
	// = 16*15*14/(3*2*1)
    const uint64_t _16_over_3 = n_over_k(16, 3); // 16*15*14 / 6;
	const uint64_t _15_over_2 = n_over_k(15, 2); // 15 * 14 / 2;
	std::cout << "_16_over_3: " << _16_over_3 << std::endl;
    uint64_t t1 = _16_over_3 * 6 * power(3, 13);
    uint64_t t2 = _15_over_2 * 2 * power(3, 12);
    uint64_t solution =
        t1
        // minus numbers with leading zero
        - t2;

    return create_hex(solution);
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    std::string solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
