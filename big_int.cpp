#include "big_int.h"
#include "rational.h"
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace apa;

big_int::big_int(uint32_t param)
        : number(1) {
    if (param >= base) {
        number.push();
        number.get_data()[0] = param % base;
        number.get_data()[1] = param / base;
    } else {
        number.get_data()[0] = param;
    }
}

big_int::big_int(const std::string &param)
        : number((param.length() + 9 - 1) / 9) {
    if (std::any_of(param.begin(), param.end(), isalpha))
        throw wrong_format();
    size_t position = 0;
    int64_t *tmp = number.get_data();
    for (int32_t i = (int32_t) param.length(); i > 0; i -= 9)
        if (i < 9) {
            std::string data = param.substr(0, (uint32_t) i);
            tmp[position++] = (int64_t) atoi(data.c_str());
        } else {
            std::string data = param.substr((uint32_t) (i - 9), 9);
            tmp[position++] = static_cast<int32_t>(atoi(data.c_str()));
        }
}

big_int::big_int(const big_int &param)
        : number(param.number) {

}

big_int::big_int(big_int &&param)
        : number(param.number) {

}

big_int &big_int::operator=(big_int param) {
    std::swap(this->number, param.number);
    return *this;
}


big_int &big_int::operator+=(const big_int &param) {
    size_t max_size_num = std::max(number.get_size(), param.number.get_size());
    int32_t carry = 0;
    shared_buffer tmp_num(max_size_num);
    memcpy(tmp_num.get_data(), number.get_data(), sizeof(int64_t) * number.get_size());
    for (size_t i = 0; i < max_size_num; ++i) {
        tmp_num.get_data()[i] += carry + (i < param.number.get_size() ? param.number.get_data()[i] : 0);
        carry = tmp_num.get_data()[i] >= static_cast<int32_t >(base);
        if (carry) tmp_num.get_data()[i] -= base;
    }
    if (carry) {
        tmp_num.push();
        tmp_num.get_data()[tmp_num.get_size() - 1] = 1;
    }
    number = tmp_num;
    return *this;
}

big_int &big_int::operator*=(const big_int &param) {
    int64_t carry = 0;
    shared_buffer temp(this->number.get_size() + param.number.get_size());
    for (size_t i = 0; i < this->number.get_size(); ++i) {
        carry = 0;
        for (int32_t j = 0; j < (int32_t) param.number.get_size() || carry; ++j) {
            int64_t cur = temp.get_data()[i + j] + number.get_data()[i] * 1ll *
                                                   (j < (int32_t) param.number.get_size() ? param.number.get_data()[j]
                                                                                          : 0) + carry;
            temp.get_data()[i + j] = (int64_t) cur % base;
            carry = (int64_t) cur / base;
        }
    }
    for (int32_t i = (int32_t) temp.get_size() - 1; i > 0; --i)
        if (temp.get_data()[i] == 0)
            temp.pop();
        else break;
    number = temp;
    return *this;
}


big_int &big_int::operator/=(const uint32_t param) {
    big_int param_2(param);
    if (param_2 == big_int(0))
        throw std::invalid_argument("Division by zero");
    int64_t carry = 0;
    shared_buffer temp(this->number.get_size());
    for (int32_t i = (int32_t) number.get_size() - 1; i >= 0; --i) {
        int64_t cur = number.get_data()[i] + carry * 1ll * base;
        temp.get_data()[i] = cur / param_2.number.get_data()[0];
        carry = cur % param_2.number.get_data()[0];
    }
    for (int32_t i = (int32_t) temp.get_size() - 1; i > 0; --i)
        if (temp.get_data()[i] == 0)
            temp.pop();
        else break;
    number = temp;
    return *this;
}

big_int &big_int::operator/=(const big_int &param) {
    if (param == big_int(0)) {
        throw std::invalid_argument("Division by zero");
    }

    big_int left(0);
    big_int right(*this);

    while (left < right) {
        big_int m = (left + right) / 2;
        if ((m * param) > *this) {
            right = m;
        } else if ((m * param) < *this) {
            left = m + big_int(1);
        } else {
            std::swap(number, m.number);
            return *this;
        }
    }

    if (right * param == *this) {
        std::swap(number, right.number);
        return *this;
    }

    if (left * param > *this) {
        left -= big_int(1);
    }

    std::swap(number, left.number);
    return *this;

}

big_int &big_int::operator-=(const big_int &param) {
    size_t size_num = param.number.get_size();
    int64_t carry = 0;
    shared_buffer tmp_num(number.get_size());
    memcpy(tmp_num.get_data(), number.get_data(), sizeof(int64_t) * number.get_size());
    for (size_t i = 0; i < size_num || carry; ++i) {
        tmp_num.get_data()[i] -= carry + (i < param.number.get_size() ? param.number.get_data()[i] : 0);
        carry = tmp_num.get_data()[i] < 0;
        if (carry) tmp_num.get_data()[i] += base;
    }
    for (int32_t i = (int32_t) tmp_num.get_size() - 1; i > 0; --i)
        if (tmp_num.get_data()[i] == 0)
            tmp_num.pop();
        else break;
    number = tmp_num;
    return *this;
}


big_int::big_int(shared_buffer &param)
        : number(param) {

}

bool big_int::operator==(const big_int &param) const {
    int32_t tmp = cmp(param);
    return tmp == 0;
}

bool big_int::operator!=(const big_int &param) const {
    return !this->operator==(param);
}

bool big_int::operator<(const big_int &param) const {
    int32_t tmp = cmp(param);
    return tmp == -1;
}

bool big_int::operator<=(const big_int &param) const {
    int32_t tmp = cmp(param);
    return tmp <= 0;
}

bool big_int::operator>(const big_int &param) const {
    int32_t tmp = cmp(param);
    return tmp == 1;
}

bool big_int::operator>=(const big_int &param) const {
    int32_t tmp = cmp(param);
    return tmp >=1;
}

int32_t big_int::cmp(const big_int &param) const {
    if (this->number.get_size() > param.number.get_size())
        return 1;
    else if (this->number.get_size() < param.number.get_size())
        return -1;
    else {
        for (int32_t i = static_cast<int>(this->number.get_size()) - 1; i >= 0; --i) {
            if (this->number.get_data()[i] > param.number.get_data()[i])
                return 1;
            else if (this->number.get_data()[i] < param.number.get_data()[i])
                return -1;
        }
    }
    return 0;
}

std::string big_int::str() const {
    std::string out;
    const int64_t *tmp_num = number.get_data();
    out += std::to_string(tmp_num[number.get_size() - 1]);
    for (int32_t i = (int32_t) number.get_size() - 2; i >= 0; --i) {
        for (size_t j = 0; j < 9 - std::to_string(tmp_num[i]).length(); ++j)
            out += "0";
        out += std::to_string(tmp_num[i]);
    }
    return out;
}

namespace apa {
    std::istream &operator>>(std::istream &in, big_int &param) {
        std::string tmp;
        in >> tmp;
        big_int temp_int(tmp);
        param = temp_int;
        return in;
    }

    std::ostream &operator<<(std::ostream &os, const big_int &param) {
        const int64_t *tmp_num = param.number.get_data();
        os << tmp_num[param.number.get_size() - 1];
        for (int32_t i = (int32_t) param.number.get_size() - 2; i >= 0; --i)
            os << std::setw(9) << std::setfill('0') << tmp_num[i];
        return os;
    }

    big_int operator*(const big_int &param_1, const big_int &param_2) {
        big_int tmp(param_1);
        return tmp *= param_2;
    }

    big_int operator-(const big_int &param_1, const big_int &param_2) {
        big_int tmp(param_1);
        return tmp -= param_2;
    }

    big_int operator+(const big_int &param_1, const big_int &param_2) {
        big_int tmp(param_1);
        return tmp += param_2;
    }

    big_int operator/(const big_int &param_1, const big_int &param_2) {
        big_int tmp(param_1);
        return tmp /= param_2;
    }

    big_int operator/(const big_int &param_1, const uint32_t param_2) {
        big_int tmp(param_1);
        return tmp /= param_2;
    }
}
