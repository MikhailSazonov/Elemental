#pragma once

#include <array>

namespace Math {

template<size_t H, size_t W>
class Matrix {
    public:
        Matrix() {
            for (size_t i = 0; i < H; ++i) {
                for (size_t j = 0; j < W; ++j) {
                    data[i][j] = 0;
                }
            }
        }

        std::array<double, W>& operator[](size_t idx) {
            return data[idx];
        }

        Matrix<H, W>& operator+=(Matrix<H, W>& other) {
            for (size_t i = 0; i < H; ++i) {
                for (size_t j = 0 ; j < W; ++j) {
                    data[i][j] += other[i][j];
                }
            }
            return *this;
        }

        Matrix<H, W> operator+(Matrix<H, W>& other) {
            Matrix<H, W> left_op = *this;
            left_op += other;
            return left_op;
        }

        Matrix<H, W>& operator-=(Matrix<H, W>& other) {
            for (size_t i = 0; i < H; ++i) {
                for (size_t j = 0 ; j < W; ++j) {
                    data[i][j] -= other[i][j];
                }
            }
            return *this;
        }

        Matrix<H, W> operator-(Matrix<H, W>& other) {
            Matrix<H, W> left_op = *this;
            left_op -= other;
            return left_op;
        }

        template<size_t U>
        Matrix<H, U> operator*(Matrix<W, U>& other) {
            Matrix<H, U> result;
            for (size_t i = 0; i < H; ++i) {
                for (size_t j = 0 ; j < U; ++j) {
                    for (size_t k = 0; k < W; ++k) {
                        result[i][j] += (*this)[i][k] * other[k][j];
                    }
                }
            }
            return result;
        }

        Matrix<H, W> operator*=(Matrix<W, W>& other) {
            Matrix<H, W> result;
            result = (*this) * other;
            *this = result;
            return *this;
        }

    private:
        std::array<std::array<double, W>, H> data;
};

}
