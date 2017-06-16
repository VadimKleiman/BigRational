#include "rational.h"
#include <iomanip>
#include <algorithm>
#include <cstring>

using namespace apa;

rational::rational()
        : numerator(0), denumerator(1) {

}

rational::rational(rational &&param)
        : numerator(param.numerator), denumerator(param.denumerator) {

}

rational::rational(const big_int &param_1, const big_int &param_2)
        : numerator(param_1), denumerator(param_2) {
    if (denumerator == big_int(0))
        throw std::invalid_argument("Division by zero");
    big_int tmp = gcd(numerator, denumerator);
    numerator /= tmp;
    denumerator /= tmp;
}

rational::rational(const rational &param)
        : numerator(param.numerator), denumerator(param.denumerator) {
    if (denumerator == big_int(0))
        throw std::invalid_argument("Division by zero");
}

rational::rational(const std::string &p_numerator, const std::string &p_denumerator)
        : numerator(p_numerator), denumerator(p_denumerator) {
    if (denumerator == big_int(0))
        throw std::invalid_argument("Division by zero");
    big_int tmp = gcd(numerator, denumerator);
    numerator /= tmp;
    denumerator /= tmp;
}

rational::rational(const uint32_t p_numerator, const uint32_t p_denumerator)
        : numerator(p_numerator), denumerator(p_denumerator) {
    if (denumerator == big_int(0))
        throw std::invalid_argument("Division by zero");
    big_int tmp = gcd(numerator, denumerator);
    numerator /= tmp;
    denumerator /= tmp;
}

rational::~rational() {

}

rational &rational::operator=(rational param) {
    std::swap(numerator, param.numerator);
    std::swap(denumerator, param.denumerator);
    return *this;
}

rational &rational::operator+=(const rational &param) {
    rational res = rational(numerator * param.denumerator + param.numerator * denumerator,
                            denumerator * param.denumerator);
    big_int tmp = gcd(res.numerator, res.denumerator);
    res.numerator /= tmp;
    res.denumerator /= tmp;
    numerator = res.numerator;
    denumerator = res.denumerator;
    return *this;
}

rational &rational::operator*=(const rational &param) {
    big_int num = numerator * param.numerator;
    big_int den = denumerator * param.denumerator;
    big_int tmp = gcd(num, den);
    num /= tmp;
    den /= tmp;
    numerator = num;
    denumerator = den;
    return *this;
}

rational &rational::operator/=(const rational &param) {
    if (param.numerator == big_int(0)) {
        throw std::invalid_argument("Division by zero");
    }

    big_int num = numerator * param.denumerator;
    big_int den = denumerator * param.numerator;
    big_int tmp = gcd(num, den);
    num /= tmp;
    den /= tmp;
    numerator = num;
    denumerator = den;
    return *this;
}

rational::operator bool() const {
    return !(numerator == big_int(0));
}

std::string rational::str() const {
    return numerator.str() + " / " + denumerator.str();
}

big_int rational::gcd(const big_int &param_1, const big_int &param_2) {
    big_int left(param_1);
    big_int right(param_2);
    while (left != right) {
        if (left == big_int(0)) {
            return right;
        }
        if (right == big_int(0)) {
            return left;
        }

        if (left > right) {
            big_int removal = left - (left / right) * right;
            left = removal;
        } else if (left < right) {
            big_int removal = right - (right / left) * left;
            right = removal;
        } else {
            return left;
        }
    }
    return left;
}

namespace apa {
    std::ostream &operator<<(std::ostream &os, const rational &param) {
        os << param.numerator << " / " << param.denumerator;
        return os;
    }

    std::istream &operator>>(std::istream &is, rational &param) {
        std::string temp;
        std::string first;
        std::string second;
        is >> first >> temp >> second;
        if (temp != "/")
            throw wrong_format();
        param = rational(first, second);
        return is;
    }

    rational operator*(const rational &param_1, const rational &param_2) {
        rational tmp(param_1);
        return tmp *= param_2;
    }

    rational operator/(const rational &param_1, const rational &param_2) {
        rational tmp(param_1);
        return tmp /= param_2;
    }

    rational operator+(const rational &param_1, const rational &param_2) {
        rational tmp(param_1);
        return tmp += param_2;
    }

    bool operator==(const rational &l, const rational &r) {
        return (l.numerator == r.numerator) && (l.denumerator == r.denumerator);
    }

    bool operator!=(const rational &l, const rational &r) {
        return !(l == r);
    }

    bool operator>(const rational &l, const rational &r) {
        rational left;
        left.numerator = l.numerator * r.denumerator;
        left.denumerator = l.denumerator * r.denumerator;

        rational right;
        right.numerator = r.numerator * l.denumerator;
        right.denumerator = r.denumerator * l.denumerator;

        return (left.numerator > right.numerator);
    }

    bool operator<(const rational &l, const rational &r) {
        return r > l;
    }

    bool operator>=(const rational &l, const rational &r) {
        return (l > r) || (l == r);
    }

    bool operator<=(const rational &l, const rational &r) {
        return (l < r) || (l == r);
    }
}
// TODO implementation
