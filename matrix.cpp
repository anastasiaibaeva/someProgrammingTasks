#include <iostream>
#include <vector>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix_;

public:
    Matrix(std::vector<std::vector<T>> matrix) {
        matrix_ = matrix;
    }
    std::pair<size_t, size_t> size() const {
        return std::make_pair(matrix_.size(), matrix_[0].size());
    }
    const std::vector<T> operator[](size_t index) const {
        return matrix_[index];
    }
    const Matrix& operator += (const Matrix& otherMatrix) {
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < matrix_[0].size(); ++j) {
                matrix_[i][j] += otherMatrix[i][j];
            }
        }
        return *this;
    }
    Matrix operator + (const Matrix& otherMatrix) const {
        Matrix newMatrix = matrix_;
        newMatrix += otherMatrix;
        return newMatrix;
    }
    const Matrix& operator *= (const T& number) {
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < matrix_[0].size(); ++j) {
                matrix_[i][j] = matrix_[i][j] * number;
            }
        }
        return *this;
    }
    Matrix operator * (const T number) const {
        Matrix newMatrix = matrix_;
        newMatrix *= number;
        return newMatrix;
    }
    Matrix& transpose() {
        std::vector<std::vector<T>> tempVector(matrix_[0].size(), std::vector<T>(matrix_.size()));
        for (size_t i = 0; i < matrix_[0].size(); ++i) {
            for (size_t j = 0; j < matrix_.size(); ++j) {
                tempVector[i][j] = matrix_[j][i];
            }
        }
        *this = Matrix(tempVector);
        return *this;
    }
    Matrix transposed() const {
        std::vector<std::vector<T>> tempVector(matrix_[0].size(), std::vector<T>(matrix_.size()));
        for (size_t i = 0; i < matrix_[0].size(); ++i) {
            for (size_t j = 0; j < matrix_.size(); ++j) {
                tempVector[i][j] = matrix_[j][i];
            }
        }
        return Matrix(tempVector);
    }
    Matrix& operator *= (const Matrix& other) {
        assert(matrix_[0].size() == other.size().first);
        std::vector<std::vector<T>> tempVector(matrix_.size(), std::vector<T>(other[0].size()));
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < other[0].size(); ++j) {
                for (size_t k = 0; k < matrix_[0].size(); ++k) {
                    tempVector[i][j] += matrix_[i][k] * other[k][j];
                }
            }
        }
        *this = Matrix(tempVector);
        return *this;
    }
    Matrix operator * (const Matrix& other) const {
        assert(matrix_[0].size() == other.size().first);
        std::vector<std::vector<T>> tempVector(matrix_.size(), std::vector<T>(other[0].size()));
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < other[0].size(); ++j) {
                for (size_t k = 0; k < matrix_[0].size(); ++k) {
                    tempVector[i][j] += matrix_[i][k] * other[k][j];
                }
            }
        }
        return Matrix(tempVector);
    }
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.size().first; ++i) {
        for (size_t j = 0; j + 1 < m.size().second; ++j) {
            out << m[i][j] << "\t";
        }
        out << m[i][m.size().second - 1];
        if (i + 1 != m.size().first) {
            out << "\n";
        }
    }
    return out;
}
