// ProjectEulerCppTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#define MAX_DIGITS 100

void d_rotate(int8_t* digits, int32_t len, int8_t* dest)
{
    if (len <= 1) return;
    for (int32_t i = 0; i < len; ++i)
    {
		int32_t dest_ix = (i == 0) ? len-1 : i - 1;
		dest[dest_ix] = digits[i];
	}
}

void d_setarray(int64_t number, uint32_t len, int8_t* dest)
{
    if (len == 0) return;
    for (uint32_t i = 0; i < len; ++i)
    {
        dest[i] = number % 10;
        number /= 10;
	}
}

bool d_check_division(int8_t* divisor, int8_t* dividend, uint32_t len, uint8_t quotient)
{
    uint32_t carry = 0;
    uint32_t digit_ix = 0;
    while (digit_ix < len)
    {
        uint8_t d1 = divisor[digit_ix];
        uint8_t d2 = dividend[digit_ix];
        uint32_t calc = d1 * quotient + carry;
        uint8_t calc_mod = calc % 10;
        if (calc_mod != d2)
            return false;
        carry = calc / 10;
        digit_ix += 1;
    }
    if (carry)
        return false;

    std::cout << "qotient: " << (int)quotient << std::endl;
    return true;
}

bool d_check_divisor(int8_t* divisor, int8_t* dividend, int32_t len)
{
    if (dividend[len - 1] <= divisor[len - 1])
        return false;
    uint8_t d01 = divisor[0];
    uint8_t d02 = dividend[0];

    for (uint8_t test_q = 2; test_q < 10; test_q++)
    {
        if (
            ((d01 * test_q) % 10) == d02
            )
        {
        // check if divisor * i = dividend
            bool chk = d_check_division(divisor, dividend, len, test_q);
            if (chk)
                return true;
        }
    }
    return false;
}

void test()
{

    int8_t digits[MAX_DIGITS];
    int8_t digits_rotated[MAX_DIGITS];

    int32_t len = 6;
	d_setarray(142857, len, digits);
	d_rotate(digits, len, digits_rotated);
	d_check_divisor(digits, digits_rotated, len);

}

int64_t solve()
{
    test();
    
    int8_t digits[MAX_DIGITS];
    int8_t digits_rotated[MAX_DIGITS];

    int64_t solution_acc = 0;
    int32_t test_len = 2;
    int32_t prev_decimal_scale = 10;
    int32_t decimal_scale = prev_decimal_scale * 10;
    while (test_len < 7)
    {
        std::cout << "test_len: " << test_len << " decimal_scale: " << decimal_scale << std::endl;

        for (int32_t test_number = prev_decimal_scale; test_number < decimal_scale; ++test_number)
        {
            if (test_number == 3216547)
            {
                std::cout << "checkpoint: " << test_number << std::endl;
            }
            d_setarray(test_number, test_len, digits);
            d_rotate(digits, test_len, digits_rotated);
            bool chk = d_check_divisor(digits, digits_rotated, test_len);
            if (chk)
            {
                solution_acc += test_number;
                solution_acc %= 100'000;
                std::cout << "test number: " << test_number << " len: " << test_len << std::endl;
            }

            if (test_number % 10'000'000 == 0)
            {
                std::cout << "test number: " << test_number << " len: " << test_len << std::endl;
            }
        }
        test_len += 1;
        prev_decimal_scale = decimal_scale;
        decimal_scale *= 10;
    }
    return solution_acc;
}

int main()
{

    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int64_t solution = solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "solution: " << solution << std::endl << "duration: " << microSec << " micro seconds (" << ms << " ms)" << std::endl;
    if (microSec > 300'000'000)
    {
        std::cout << "(" << ((float)microSec) / 60'000'000 << " minutes )" << std::endl;
    }
}
