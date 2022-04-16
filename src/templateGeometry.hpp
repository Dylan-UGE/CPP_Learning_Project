#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

template <typename type, int dimension = 1> struct PointND
{
    static_assert(dimension > 0, "dimension must be strictly positive");

    std::array<type, dimension> values {};

    /*template <typename... Ts> PointND(Ts... params) : values { static_cast<type>(params)... }
    {
        static_assert(dimension == sizeof...(Ts), "dimension must be strictly equel to number of elements");
    }*/
    
    PointND(type x, type y, type z) : values { x, y, z } {}
    PointND(type x, type y) : values { x, y } {}

    type get_n_dim(int dim) const
    {
        static_assert(dim < dimension, "dim must be strictly lower than dimension");
        return values[dim];
    }
    type& get_n_dim(int dim)
    {
        static_assert(dim < dimension, "dim must be strictly lower than dimension");
        return values[dim];
    }

    type x() const { return values[0]; }
    type& x() { return values[0]; }

    type y() const
    {
        static_assert(dimension > 1, "dimension must be strictly greater than 1");
        return values[1];
    }
    type& y()
    {
        static_assert(dimension > 1, "dimension must be strictly greater than 1");
        return values[1];
    }

    type z() const
    {
        static_assert(dimension > 2, "dimension must be strictly greater than 2");
        return values[2];
    }
    type& z()
    {
        static_assert(dimension > 2, "dimension must be strictly greater than 2");
        return values[2];
    }

    PointND& operator+=(const PointND& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](float value, float other_value) { return value + other_value; });
        return *this;
    }

    PointND& operator-=(const PointND& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](float value, float other_value) { return value - other_value; });
        return *this;
    }

    PointND& operator*=(const PointND& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](float value, float other_value) { return value * other_value; });
        return *this;
    }

    PointND& operator*=(const float scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](float value) { return value * scalar; });
        return *this;
    }

    PointND operator+(const PointND& other) const
    {
        PointND result = *this;
        result += other;
        return result;
    }

    PointND operator-(const PointND& other) const
    {
        PointND result = *this;
        result -= other;
        return result;
    }

    PointND operator*(const PointND& other) const
    {
        PointND result = *this;
        result *= other;
        return result;
    }

    PointND operator*(const float scalar) const
    {
        PointND result = *this;
        result *= scalar;
        return result;
    }

    PointND operator-() const
    {
        PointND result = *this;
        std::for_each(result.values.begin(), result.values.end(), [](type a) { return -a; });
        return result;
    }

    bool operator!=(PointND other) const
    {
        return !std::equal(values.begin(), values.end(), other.values.begin(), other.values.end());
    }

    float length() const
    {
        return std::sqrt(
            std::accumulate(values.begin(), values.end(), 0.f, [](float a, float b) { return a + b * b; }));
    }

    float distance_to(const PointND& other) const { return (*this - other).length(); }

    PointND& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    PointND& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    std::string get_point()
    {
        return "(" +
               std::accumulate(values.begin(), values.end(), std::string {},
                               [](std::string a, float b) { return a + ", " + std::to_string(b); }) +
               ")";
    }
};

inline void test_generic_points()
{
    PointND<int, 3> p1 { 3, 5, 6 };
    PointND<int, 3> p2 { 4, 5, 6 };
    auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point

    std::cout << p3.get_point() << std::endl;
}