#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>

template <typename T>
class Polynomial {
private:
    std::vector<T> polynomial_;

    void cutZeros() {
        while (!polynomial_.empty() && polynomial_.back() == static_cast<T>(0)) {
            polynomial_.pop_back();
        }
    }

public:
    using Iterator = typename std::vector<T>::const_iterator;

    explicit Polynomial(std::vector<T>& odds) :  polynomial_(std::move(odds)) {
        cutZeros();
    }

    explicit Polynomial(const T& coefficient = T()) {
        polynomial_.push_back(coefficient);
        cutZeros();
    }

    template <typename Iterator>
    Polynomial(Iterator begin, Iterator end) {
        while (begin != end) {
            polynomial_.push_back(*begin);
            ++begin;
        }
        cutZeros();
    }

    Iterator begin() const {
        return polynomial_.begin();
    }

    Iterator end() const {
        return polynomial_.end();
    }

    bool operator == (const Polynomial& otherPolynomial) const {
        auto firstSize = polynomial_.size();
        auto secondSize = otherPolynomial.polynomial_.size();
        auto maximum = std::max(firstSize, secondSize);
        Polynomial first = *this;
        for (size_t i = 0; i < maximum; ++i) {
            if (first[i] != otherPolynomial[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator != (const Polynomial& otherPolynomial) const {
        return !(*this == otherPolynomial);
    }

    bool operator == (const T& otherScalar) const {
        return Polynomial(otherScalar) == *this;
    }

    bool operator != (const T& otherScalar) const {
        return !(*this == otherScalar);
    }

    Polynomial& operator += (const Polynomial& otherPolynomial) {
        auto firstSize = polynomial_.size();
        auto secondSize = otherPolynomial.polynomial_.size();
        for (size_t i = 0; i < firstSize; ++i) {
            polynomial_[i] += otherPolynomial[i];
        }
        if (firstSize < secondSize) {
            for (size_t i = firstSize; i < secondSize; ++i) {
                polynomial_.push_back(otherPolynomial[i]);
            }
        }
        cutZeros();
        return *this;
    }

    Polynomial& operator += (const T& otherScalar) {
        polynomial_[0] += otherScalar;
        cutZeros();
        return *this;
    }

    Polynomial& operator -= (const Polynomial& otherPolynomial) {
        auto firstSize = polynomial_.size();
        auto secondSize = otherPolynomial.polynomial_.size();
        for (size_t i = 0; i < firstSize; ++i) {
            polynomial_[i] -= otherPolynomial[i];
        }
        if (firstSize < secondSize) {
            for (size_t i = firstSize; i < secondSize; ++i) {
                polynomial_.push_back(-otherPolynomial[i]);
            }
        }
        cutZeros();
        return *this;
    }

    Polynomial& operator -= (const T& otherScalar) {
        polynomial_[0] -= otherScalar;
        cutZeros();
        return *this;
    }

    Polynomial& operator *= (const Polynomial& otherPolynomial) {
        *this = *this * otherPolynomial;
        cutZeros();
        return *this;
    }

    Polynomial& operator *= (const T& otherScalar) {
        for (size_t i = 0; i < polynomial_.size(); ++i) {
            polynomial_[i] *= otherScalar;
        }
        cutZeros();
        return *this;
    }

    Polynomial operator + (const Polynomial& otherPolynomial) const {
        Polynomial result = *this;
        result += otherPolynomial;
        result.cutZeros();
        return result;
    }

    Polynomial operator + (const T& otherScalar) const {
        std::vector<T> result = polynomial_;
        result[0] += otherScalar;
        return Polynomial(result);
    }

    Polynomial operator - (const Polynomial& otherPolynomial) const {
        Polynomial result = *this;
        result -= otherPolynomial;
        result.cutZeros();
        return result;
    }

    Polynomial operator - (const T& otherScalar) const {
        std::vector<T> result = polynomial_;
        result[0] -= otherScalar;
        return Polynomial(result);
    }

    Polynomial operator * (const Polynomial& otherPolynomial) const {
        auto firstSize = polynomial_.size();
        auto secondSize = otherPolynomial.polynomial_.size();
        std::vector<T> result(firstSize + secondSize, 0);
        for (size_t i = 0; i < firstSize; ++i) {
            for (size_t j = 0; j < secondSize; ++j) {
                result[i + j] += polynomial_[i] * otherPolynomial[j];
            }
        }
        return Polynomial(result);
    }

    Polynomial operator * (const T& otherScalar) const {
        Polynomial result = *this;
        result *= otherScalar;
        return Polynomial(result);
    }

    T operator[] (const size_t i) const {
        auto size = polynomial_.size();
        if (i < size) {
            return polynomial_[i];
        } else {
            return static_cast<T>(0);
        }
    }

    T operator() (const T& scalar) const {
        if (polynomial_.empty()) {
            return static_cast<T>(0);
        }
        auto size = polynomial_.size();
        T result = polynomial_[size - 1];
        for (int i = size - 2; i >= 0; --i) {
            result = result * scalar + polynomial_[i];
        }
        return result;
    }

    int Degree() const {
        return static_cast<int>(polynomial_.size()) - 1;
    }

    Polynomial operator & (const Polynomial& otherPolynomial) const {
        if (polynomial_.empty()) {
            return Polynomial(static_cast<T>(0));
        }
        auto size = polynomial_.size() - 1;
        Polynomial result(polynomial_[size]);
        for (int i = size - 1; i >= 0; --i) {
            result = result * otherPolynomial + polynomial_[i];
        }
        return result;
    }
};

template <typename T>
Polynomial<T> operator + (const T& first, const Polynomial<T>& second) {
    return second + first;
}

template <typename T>
Polynomial<T> operator - (const T& first, const Polynomial<T>& second) {
    Polynomial r(first);
    return r - second;
}

template <typename T>
Polynomial<T> operator * (const T& first, const Polynomial<T>& second) {
    return second * first;
}

template <typename T>
bool operator == (const T& first, const Polynomial<T>& second) {
    return second == first;
}

template <typename T>
bool operator != (const T& first, const Polynomial<T>& second) {
    return !(second == first);
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T> polynomial) {
    if (polynomial.Degree() < 1) {
        out << polynomial[0];
        return out;
    }
    bool check = true;
    for (int i = polynomial.Degree(); i >= 0; --i) {
        if (polynomial[i] != static_cast<T>(0)) {
            if (polynomial[i] > static_cast<T>(0) && !check) {
                out << "+";
            }
            if (i == 0) {
                out << polynomial[i];
            }
            if (i != 0 && polynomial[i] != static_cast<T>(1) &&
            polynomial[i] != static_cast<T>(-1)) {
                out << polynomial[i] << "*";
            } else if (i != 0 && polynomial[i] == static_cast<T>(-1)) {
                out << "-";
            }
            if (i != 0) {
                out << "x";
            }
            if (i > 1) {
                out << "^" << i;
            }
            check = false;
        }
    }
    return out;
}
