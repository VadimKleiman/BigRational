#pragma once

#include <string>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include "big_int.h"

namespace apa {
    class rational {
    public:
        rational();

        rational(const rational &);

        rational(rational &&);

        rational(const big_int &, const big_int &);

        explicit rational(const uint32_t, const u_int32_t = 1);

        explicit rational(const std::string &, const std::string & = "1");

        ~rational();

        rational &operator=(rational);

        rational &operator+=(const rational &);

        rational &operator*=(const rational &);

        rational &operator/=(const rational &);

        friend bool operator==(const rational &, const rational &);

        friend bool operator>(const rational &, const rational &);

        explicit operator bool() const;

        std::string str() const;

    private:
        big_int numerator;
        big_int denumerator;

        friend std::ostream &operator<<(std::ostream &, const rational &);

        friend std::istream &operator>>(std::istream &, rational &);

        big_int gcd(const big_int &, const big_int &);

    };

    rational operator*(const rational &, const rational &);

    rational operator/(const rational &, const rational &);

    rational operator+(const rational &, const rational &);

    bool operator==(const rational &, const rational &);

    bool operator!=(const rational &, const rational &);

    bool operator<(const rational &, const rational &);

    bool operator<=(const rational &, const rational &);

    bool operator>(const rational &, const rational &);

    bool operator>=(const rational &, const rational &);
    // TODO implementation

} // namespace apa
