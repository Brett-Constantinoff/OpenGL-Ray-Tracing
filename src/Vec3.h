#pragma once

#include <cmath>
#include <iostream>
#include <stdint.h>

template <typename T>
struct Vec3
{
public:
    Vec3() :
        m_vals{0,0,0} 
    {

    }

    Vec3(T x, T y, T z) : 
        m_vals{ x, y, z } 
    {

    }

    Vec3(const Vec3& other)
    {
        m_vals[0] = other.m_vals[0];
        m_vals[1] = other.m_vals[1];
        m_vals[2] = other.m_vals[2];
    }

    T x() const 
    { 
        return m_vals[0];
    }
    T y() const 
    { 
        return m_vals[1];
    }
    T z() const 
    { 
        return m_vals[2];
    }

    T r() const
    {
        return m_vals[0];
    }
    T g() const
    {
        return m_vals[1];
    }
    T b() const
    {
        return m_vals[2];
    }

    Vec3 operator-() const 
    { 
        return Vec3(-m_vals[0], -m_vals[1], -m_vals[2]);
    }


    T& operator[](int32_t i) 
    { 
        return m_vals[i];
    }

    Vec3& operator+=(const Vec3& v) {
        m_vals[0] += v.m_vals[0];
        m_vals[1] += v.m_vals[1];
        m_vals[2] += v.m_vals[2];
        return *this;
    }

    Vec3& operator*=(const T t) {
        m_vals[0] *= t;
        m_vals[1] *= t;
        m_vals[2] *= t;
        return *this;
    }

    Vec3& operator/=(const T t) 
    {
        return *this *= 1 / t;
    }

    T length() const
    {
        return sqrt(length_squared());
    }

    T length_squared() const
    {
        return m_vals[0] * m_vals[0] + m_vals[1] * m_vals[1] + m_vals[2] * m_vals[2];
    }

public:
	T m_vals[3];
};

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec3<T>& v)
{
    return out << v.m_vals[0] << ' ' << v.m_vals[1] << ' ' << v.m_vals[2];
}

template <typename T>
inline Vec3<T> operator+(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3(u.m_vals[0] + v.m_vals[0], u.m_vals[1] + v.m_vals[1], u.m_vals[2] + v.m_vals[2]);
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3(u.m_vals[0] - v.m_vals[0], u.m_vals[1] - v.m_vals[1], u.m_vals[2] - v.m_vals[2]);
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3(u.m_vals[0] * v.m_vals[0], u.m_vals[1] * v.m_vals[1], u.m_vals[2] * v.m_vals[2]);
}

template <typename T>
inline Vec3<T> operator*(T t, const Vec3<T>& v) {
    return Vec3(t * v.m_vals[0], t * v.m_vals[1], t * v.m_vals[2]);
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T>& v, float t) {
    return t * v;
}

template <typename T>
inline Vec3<T> operator/(Vec3<T> v, float t) {
    return (1 / t) * v;
}

template <typename T>
inline T dot(const Vec3<T>& u, const Vec3<T>& v) {
    return u.m_vals[0] * v.m_vals[0]
        + u.m_vals[1] * v.m_vals[1]
        + u.m_vals[2] * v.m_vals[2];
}

template <typename T>
inline Vec3<T> cross(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3(u.m_vals[1] * v.m_vals[2] - u.m_vals[2] * v.m_vals[1],
        u.m_vals[2] * v.m_vals[0] - u.m_vals[0] * v.m_vals[2],
        u.m_vals[0] * v.m_vals[1] - u.m_vals[1] * v.m_vals[0]);
}

template <typename T>
inline Vec3<T> unit(Vec3<T> v)
{
    return v / v.length();
}