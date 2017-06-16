#ifndef BIG_INT_H
#define BIG_INT_H

#include <string>
#include <stdexcept>
#include "shared_buffer.h"

namespace apa {
    class wrong_format : public std::exception {
    public:
        virtual const char *what() const throw() {
            return "Wrong format";
        }
    };

    class big_int {
    public:
        explicit big_int(uint32_t);

        explicit big_int(const std::string &);

        big_int(shared_buffer &);

        big_int(const big_int &);

        big_int(big_int &&);

        big_int &operator=(big_int);

        big_int &operator+=(const big_int &);

        big_int &operator*=(const big_int &);

        big_int &operator-=(const big_int &);

        big_int &operator/=(const big_int &);

        big_int &operator/=(const uint32_t);

        bool operator==(const big_int &) const;

        bool operator!=(const big_int &) const;

        bool operator<(const big_int &) const;

        bool operator<=(const big_int &) const;

        bool operator>(const big_int &) const;

        bool operator>=(const big_int &) const;

        friend std::ostream &operator<<(std::ostream &, const big_int &);

        friend std::istream &operator>>(std::istream &, big_int &);

        std::string str() const;

    private:
        int32_t cmp(const big_int &) const;

        shared_buffer number;

        const uint32_t base = 1000 * 1000 * 1000;
    };

    big_int operator*(const big_int &, const big_int &);

    big_int operator-(const big_int &, const big_int &);

    big_int operator+(const big_int &, const big_int &);

    big_int operator/(const big_int &, const big_int &);

    big_int operator/(const big_int &, const uint32_t);
}
#endif //BIG_INT_H
