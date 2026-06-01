#pragma once

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace GMath
{
    struct Vec3
    {
        union
        {
            struct{float x, y, z;};
            struct{float r, g, b;};
            float e[3];
        };

        Vec3(): x(0.0f), y(0.0f), z(0.0f) {}
        Vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}


        Vec3& operator+=(const Vec3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3& operator*=(float t)
        {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        Vec3& operator/=(float t)
        {
            return *this *= 1/t;
        }

        float length() const
        {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const
        {
            return x * x + y * y + z * z;
        }

        Vec3& clamp(float min, float max)
        {
            x = std::clamp(x, min, max);
            y = std::clamp(y, min, max);
            z = std::clamp(z, min, max);
            return *this;
        }
    };

    struct Vec4
    {
        union
        {
            struct{float x, y, z, w;};
            struct{float r, g, b, a;};
            float e[4];
        };

        Vec4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f){}
        Vec4(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w){}

        Vec4& operator+=(const Vec4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Vec4& operator-=(const Vec4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Vec4& operator*=(float t)
        {
            x *= t;
            y *= t;
            z *= t;
            w *= t;
            return *this;
        }

        Vec4& operator/=(float t)
        {
            return *this *= 1/t;
        }

        float length() const
        {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const
        {
            return x * x + y * y + z * z + w * w;
        }

    };

    struct Mat4
    {
        union
        {
            float e[16];
            float m[4][4];
        };

        Mat4(): m{
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        } {}

        float& operator()(int row, int column)
        {
            assert(row >= 0 && row < 4 && column >= 0 && column < 4 && "Matrix Index out of bounds!");
            return m[column][row];
        }
    };

    struct Quat
    {

    };

    inline std::ostream& operator<<(std::ostream& out, const Vec3& u)
    {
        return out << u.x << ' ' << u.y << ' ' << u.z;
    }

    inline std::ostream& operator<<(std::ostream& out, const Vec4& u)
    {
        return out << u.x << ' ' << u.y << ' ' << u.z << ' ' << u.w;
    }

    inline Vec3 operator+(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    inline Vec4 operator+(const Vec4& u, const Vec4& v)
    {
        return Vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
    }

    inline Vec3 operator-(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    inline Vec4 operator-(const Vec4& u, const Vec4& v)
    {
        return Vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
    }

    inline Vec3 operator-(const Vec3& u)
    {
        return Vec3(u.x * - 1, u.y * -1, u.z * -1);
    }

    inline Vec4 operator-(const Vec4& u)
    {
        return Vec4(u.x * - 1, u.y * -1, u.z * -1, u.w * -1);
    }

    inline Vec3 operator*(const Vec3& u, float t)
    {
        return Vec3(u.x * t, u.y * t, u.z * t);
    }

    inline Vec4 operator*(const Vec4& u, float t)
    {
        return Vec4(u.x * t, u.y * t, u.z * t, u.w * t);
    }

    inline Vec3 operator*(float t, const Vec3& u)
    {
        return u * t;
    }

    inline Vec4 operator*(float t, const Vec4& u)
    {
        return u * t;
    }

    inline Vec3 operator/(const Vec3& u, float t)
    {
        return u * 1/t;
    }

    inline Vec4 operator/(const Vec4& u, float t)
    {
        return u * 1/t;
    }

    inline float dot(const Vec3& u, const Vec3& v)
    {
        return u.x*v.x + u.y*v.y + u.z*v.z;
    }

    inline float dot(const Vec4& u, const Vec4& v)
    {
        return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
    }

    inline Vec3 cross(const Vec3& u, const Vec3& v)
    {
        return Vec3(
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        );
    }

    inline Vec3 normalize(const Vec3& u)
    {
        return u/u.length();
    }

    inline Vec3 power(const Vec3& u, float t)
    {
        return Vec3(std::pow(u.x, t), std::pow(u.y, t), std::pow(u.z, t));
    }
}