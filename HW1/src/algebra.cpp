// #include "algebra.h"

#include <stdexcept>
#include <random>
#include <format>
#include <iostream>
#include <cmath>

namespace algebra {
    // Function template for matrix initialization
    template<typename T>
    MATRIX<T> create_matrix(std::size_t rows, std::size_t columns, std::optional<MatrixType> type,
                            std::optional<T> lowerBound, std::optional<T> upperBound) {
        if (rows == 0 || columns == 0)
            throw std::invalid_argument("Invalid matrix size");

        MATRIX<T> matrix(rows, std::vector<T>(columns, T{0}));
        switch (type.value()) {
            case MatrixType::Ones : {
                return MATRIX<T>(rows, std::vector<T>(columns, T{1}));
            }
            case MatrixType::Identity : {
                if (rows != columns)
                    throw std::invalid_argument("The number of rows must be equal to the number of columns.");
                for (std::size_t i = 0; i < rows; ++i)
                    matrix[i][i] = T{1};
                return matrix;
            }
            case MatrixType::Random : {
                if (!lowerBound.has_value() || !upperBound.has_value() || lowerBound.value() >= upperBound.value())
                    throw std::invalid_argument("The lower bound and upper bound cannot be nullopt.");

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(lowerBound.value(), upperBound.value());

                for (auto& row : matrix)
                    for (auto &elem: row)
                        elem = distrib(gen);
                return matrix;
            }
            case MatrixType::Zeros: {
                return matrix;
            }
            default: {
                throw std::invalid_argument("Invalid matrix type.");
            }
        }
    };

    template<typename T>
    void display(const MATRIX<T>& matrix) {
        for (const auto& row : matrix) {
            std::string fmt = "|";
            for (const auto& elem : row) {
                fmt += std::format("{:<7}|", elem);
            }
            std::cout << fmt << std::endl;
        }
    };

    template<typename T>
    MATRIX<T> sum_sub(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB, std::optional<std::string> operation ) {
        if (matrixA.size() == 0 && matrixB.size() == 0)
            return MATRIX<T>{};

        if (matrixA.size() != matrixB.size() || matrixA[0].size() != matrixB[0].size())
            throw std::invalid_argument("The number of rows and columns must be equal.");

        MATRIX<T> matrix(matrixA.size(), std::vector<T>(matrixA[0].size(), T{0}));
        if (operation.value() == "sub") {
            for (std::size_t i = 0; i < matrixA.size(); ++i)
                for (std::size_t j = 0; j < matrixA[0].size(); ++j)
                    matrix[i][j] = matrixA[i][j] - matrixB[i][j];
            return matrix;
        }
        for (std::size_t i = 0; i < matrixA.size(); ++i)
            for (std::size_t j = 0; j < matrixA[0].size(); ++j)
                matrix[i][j] = matrixA[i][j] + matrixB[i][j];
        return matrix;
    };

    template<typename T>
    MATRIX<T> multiply(const MATRIX<T>& matrix, const T scalar) {
        if (matrix.size() == 0)
            return MATRIX<T>{};

        MATRIX<T> result(matrix.size(), std::vector<T>(matrix[0].size(), T{0}));
        for (std::size_t i = 0; i < matrix.size(); ++i)
            for (std::size_t j = 0; j < matrix[0].size(); ++j)
                result[i][j] = matrix[i][j] * scalar;

        return result;
    };

    template<typename T>
    MATRIX<T> multiply(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB) {
        if (matrixA.size() == 0 || matrixB.size() == 0 || matrixA[0].size() != matrixB.size())
            throw std::invalid_argument("The number of A's columns and B's rows must be equal.");

        MATRIX<T> result(matrixA.size(), std::vector<T>(matrixB[0].size(), T{0}));
        for (std::size_t i = 0; i < matrixA.size(); ++i)
            for (std::size_t j = 0; j < matrixB[0].size(); ++j)
                for (std::size_t k = 0; k < matrixA[0].size(); ++k)
                    result[i][j] += matrixA[i][k] * matrixB[k][j];

        return result;
    };

    template<typename T>
    MATRIX<T> hadamard_product(const MATRIX<T>& matrixA, const MATRIX<T>& matrixB) {
        if (matrixA.size() == 0 && matrixB.size() == 0)
            return MATRIX<T>{};

        if (matrixA.size() != matrixB.size() || matrixA[0].size() != matrixB[0].size())
            throw std::invalid_argument("The number of rows and columns must be equal.");

        MATRIX<T> matrix(matrixA.size(), std::vector<T>(matrixA[0].size(), T{0}));
        for (std::size_t i = 0; i < matrix.size(); ++i)
            for (std::size_t j = 0; j < matrix[0].size(); ++j)
                matrix[i][j] = matrixA[i][j] * matrixB[i][j];

        return matrix;
    };

    template<typename T>
    MATRIX<T> transpose(const MATRIX<T>& matrix) {
        if (matrix.size() == 0)
            return MATRIX<T>{};

        MATRIX<T> result(matrix[0].size(), std::vector<T>(matrix.size(), T{0}));

        for (std::size_t i = 0; i < matrix.size(); ++i)
            for (std::size_t j = 0; j < matrix[0].size(); ++j)
                result[j][i] = matrix[i][j];

        return result;
    };

    template<typename T>
    T trace(const MATRIX<T>& matrix) {
        if (matrix.size() == 0 || matrix.size() != matrix[0].size())
            throw std::invalid_argument("The number of rows and columns must be equal.");

        T result{};
        for (std::size_t i = 0; i < matrix.size(); ++i)
            result += matrix[i][i];

        return result;
    };

    template<typename T>
    double determinant(const MATRIX<T>& matrix) {
        if (matrix.size() == 0 || matrix.size() != matrix[0].size())
            throw std::invalid_argument("The number of rows and columns must be equal.");

        if (matrix.size() == 1)
            return static_cast<double>(matrix[0][0]);

        if (matrix.size() == 2)
            return static_cast<double>(matrix[0][0]) * static_cast<double>(matrix[1][1]) - static_cast<double>(matrix[0][1]) * static_cast<double>(matrix[1][0]);

        double result = 0.0;
        int flag = 1;
        for (std::size_t i = 0; i < matrix.size(); ++i) {
            MATRIX<T> t(matrix.size() - 1, std::vector<T>(matrix[0].size() - 1, T{0}));
            for (std::size_t k = 1; k < matrix.size(); ++k)
                for (std::size_t j = 0, v = 0; j < matrix.size(); ++j) {
                    if (j == i)
                        continue;

                    t[k - 1][v] = matrix[k][j];
                    ++v;
                }

            result += flag * static_cast<double>(matrix[0][i]) * determinant(t);
            flag *= -1;
        }

        return result;
    };

    template<typename T>
    MATRIX<double> inverse(const MATRIX<T>& matrix) {
        if (matrix.size() == 0 || matrix.size() != matrix[0].size())
            throw std::invalid_argument("The number of rows and columns must be equal.");

        double eps = 1e-5;
        double det = determinant(matrix);
        if (fabs(det) < eps)
            throw std::invalid_argument("The matrix is not invertible.");

        MATRIX<double> result(matrix.size(), std::vector<T>(matrix.size(), T{0}));
        MATRIX<double> t(matrix.size() - 1, std::vector<T>(matrix.size() - 1, T{0}));
        for (std::size_t i = 0; i < matrix.size(); ++i) {
            for (std::size_t j = 0; j < matrix[0].size(); ++j) {

                for (std::size_t ii = 0, row = 0; ii < matrix.size(); ++ii) {
                    if (ii == i) continue;
                    for (std::size_t jj = 0, col = 0; jj < matrix.size(); ++jj) {
                        if (jj == j) continue;

                        t[row][col] = matrix[ii][jj];
                        ++col;
                    }
                    ++row;
                }

                result[j][i] = pow(-1, i + j) * determinant(t) / det;
            }
        }

        return result;
    };
};



