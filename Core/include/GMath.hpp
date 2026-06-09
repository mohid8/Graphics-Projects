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

        Vec4 asVec4(float w) const
        {
            return Vec4(x,y,z,w);
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
            return x*x + y*y + z*z + w*w;
        }

        Vec3 asVec3() const
        {
            return Vec3(x,y,z);
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

        Mat4(
            float m0, float m4, float m8,  float m12,
            float m1, float m5, float m9,  float m13,
            float m2, float m6, float m10, float m14,
            float m3, float m7, float m11, float m15
        ):m{m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15}{}

        float& operator()(int row, int column)
        {
            assert(row >= 0 && row < 4 && column >= 0 && column < 4 && "Matrix Index out of bounds!");
            return m[column][row];
        }

        const float& operator()(int row, int column) const
        {
            assert(row >= 0 && row < 4 && column >= 0 && column < 4 && "Matrix Index out of bounds!");
            return m[column][row];
        }
    };

    struct Quat
    {
        union
        {
            struct{float x, y, z, w;};
            float e[4];
        };

        Quat(): x(0.0f), y(0.0f), z(0.0f), w(1.0f){}
        Quat(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w){}

        float length() const
        {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const
        {
            return x*x + y*y + z*z + w*w;
        }

        Quat& conjugate()
        {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }
    };

    inline std::ostream& operator<<(std::ostream& out, const Vec3& u)
    {
        return out << u.x << ' ' << u.y << ' ' << u.z;
    }

    inline std::ostream& operator<<(std::ostream& out, const Vec4& u)
    {
        return out << u.x << ' ' << u.y << ' ' << u.z << ' ' << u.w;
    }

    inline std::ostream& operator<<(std::ostream& out, const Quat& q)
    {
        return out << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w;
    }

    /*TODO: Mat printout*/

    inline Vec3 operator+(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    inline Vec4 operator+(const Vec4& u, const Vec4& v)
    {
        return Vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
    }

    inline Quat operator+(const Quat& q, const Quat& r)
    {
        return Quat(q.x + r.x, q.y + r.y, q.z + r.z, q.w + r.w);
    }

    inline Vec3 operator-(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    inline Vec4 operator-(const Vec4& u, const Vec4& v)
    {
        return Vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
    }

    inline Quat operator-(const Quat& q, const Quat& r)
    {
        return Quat(q.x - r.x, q.y - r.y, q.z - r.z, q.w - r.w);
    }

    inline Vec3 operator-(const Vec3& u)
    {
        return Vec3(u.x * - 1, u.y * -1, u.z * -1);
    }

    inline Vec4 operator-(const Vec4& u)
    {
        return Vec4(u.x * - 1, u.y * -1, u.z * -1, u.w * -1);
    }

    inline Quat operator-(const Quat& q)
    {
        return Quat(q.x * - 1, q.y * -1, q.z * -1, q.w * -1);
    }

    inline Vec3 operator*(const Vec3& u, float t)
    {
        return Vec3(u.x * t, u.y * t, u.z * t);
    }

    inline Vec4 operator*(const Vec4& u, float t)
    {
        return Vec4(u.x * t, u.y * t, u.z * t, u.w * t);
    }

    inline Quat operator*(const Quat& q, float t)
    {
        return Quat(q.x * t, q.y * t, q.z * t, q.w * t);
    }

    inline Mat4 operator*(const Mat4& m, float t)
    {
        return Mat4(
            m(0,0)*t, m(0,1)*t, m(0,2)*t, m(0,3)*t,
            m(1,0)*t, m(1,1)*t, m(1,2)*t, m(1,3)*t,
            m(2,0)*t, m(2,1)*t, m(2,2)*t, m(2,3)*t,
            m(3,0)*t, m(3,1)*t, m(3,2)*t, m(3,3)*t
        );
    }

    inline Vec3 operator*(float t, const Vec3& u)
    {
        return u * t;
    }

    inline Vec4 operator*(float t, const Vec4& u)
    {
        return u * t;
    }

    inline Quat operator*(float t, const Quat& q)
    {
        return q * t;
    }

    inline Mat4 operator*(float t, const Mat4& m)
    {
        return m * t;
    }

    inline Vec3 operator/(const Vec3& u, float t)
    {
        return u * 1/t;
    }

    inline Vec4 operator/(const Vec4& u, float t)
    {
        return u * 1/t;
    }

    inline Quat operator/(const Quat& q, float t)
    {
        return q * 1/t;
    }

    inline Mat4 operator/(const Mat4& m, float t)
    {
        return m * 1/t;
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

    inline Quat normalize(const Quat& q)
    {
        return q/q.length();
    }

    inline Vec3 power(const Vec3& u, float t)
    {
        return Vec3(std::pow(u.x, t), std::pow(u.y, t), std::pow(u.z, t));
    }

    inline Mat4 operator*(const Mat4& a, const Mat4& b)
    {
        return Mat4(
            a(0,0)*b(0,0) + a(0,1)*b(1,0) + a(0,2)*b(2,0) + a(0,3)*b(3,0),
            a(0,0)*b(0,1) + a(0,1)*b(1,1) + a(0,2)*b(2,1) + a(0,3)*b(3,1),
            a(0,0)*b(0,2) + a(0,1)*b(1,2) + a(0,2)*b(2,2) + a(0,3)*b(3,2),
            a(0,0)*b(0,3) + a(0,1)*b(1,3) + a(0,2)*b(2,3) + a(0,3)*b(3,3),

            a(1,0)*b(0,0) + a(1,1)*b(1,0) + a(1,2)*b(2,0) + a(1,3)*b(3,0),
            a(1,0)*b(0,1) + a(1,1)*b(1,1) + a(1,2)*b(2,1) + a(1,3)*b(3,1),
            a(1,0)*b(0,2) + a(1,1)*b(1,2) + a(1,2)*b(2,2) + a(1,3)*b(3,2),
            a(1,0)*b(0,3) + a(1,1)*b(1,3) + a(1,2)*b(2,3) + a(1,3)*b(3,3),

            a(2,0)*b(0,0) + a(2,1)*b(1,0) + a(2,2)*b(2,0) + a(2,3)*b(3,0),
            a(2,0)*b(0,1) + a(2,1)*b(1,1) + a(2,2)*b(2,1) + a(2,3)*b(3,1),
            a(2,0)*b(0,2) + a(2,1)*b(1,2) + a(2,2)*b(2,2) + a(2,3)*b(3,2),
            a(2,0)*b(0,3) + a(2,1)*b(1,3) + a(2,2)*b(2,3) + a(2,3)*b(3,3),

            a(3,0)*b(0,0) + a(3,1)*b(1,0) + a(3,2)*b(2,0) + a(3,3)*b(3,0),
            a(3,0)*b(0,1) + a(3,1)*b(1,1) + a(3,2)*b(2,1) + a(3,3)*b(3,1),
            a(3,0)*b(0,2) + a(3,1)*b(1,2) + a(3,2)*b(2,2) + a(3,3)*b(3,2),
            a(3,0)*b(0,3) + a(3,1)*b(1,3) + a(3,2)*b(2,3) + a(3,3)*b(3,3)
        );
    }

    inline Vec4 operator*(const Mat4& a, const Vec4& u)
    {
        return Vec4(
            a(0,0)*u.x + a(0,1)*u.y + a(0,2)*u.z + a(0,3)*u.w,
            a(1,0)*u.x + a(1,1)*u.y + a(1,2)*u.z + a(1,3)*u.w,
            a(2,0)*u.x + a(2,1)*u.y + a(2,2)*u.z + a(2,3)*u.w,
            a(3,0)*u.x + a(3,1)*u.y + a(3,2)*u.z + a(3,3)*u.w
        );
    }

    inline Quat operator*(const Quat& q, const Quat& r)
    {
        return Quat(
            q.y*r.z - q.z*r.y + r.w*q.x + q.w*r.x,
            q.z*r.x - q.x*r.z + r.w*q.y + q.w*r.y,
            q.x*r.y - q.y*r.x + r.w*q.z + q.w*r.z,
            q.w*r.w - q.x*r.x - q.y*r.y - q.z*r.z
        );
    }

    inline Quat conjugate(const Quat& q)
    {
        return Quat(-q.x, -q.y, -q.z, q.w);
    }

    inline Quat inverse(const Quat& q)
    {
        return conjugate(q)/q.lengthSquared();
        /*TODO: Maybe a check if the Quat is already a unit Quat
        Also need to check if length isn't 0*/
    }

    inline Mat4 transpose(const Mat4& m)
    {
        return Mat4(
            m(0,0), m(1,0), m(2,0), m(3,0),
            m(0,1), m(1,1), m(2,1), m(3,1),
            m(0,2), m(1,2), m(2,2), m(3,2),
            m(0,3), m(1,3), m(2,3), m(3,3)
        );
    }

    /*TODO: Make this more efficient*/
    inline Mat4 inverse(const Mat4& m)
    {
        /*Determinants of Minors*/
        float det00 =
            m(1,1)*m(2,2)*m(3,3) + m(1,2)*m(2,3)*m(3,1) + m(1,3)*m(2,1)*m(3,2)
           -m(1,3)*m(2,2)*m(3,1) - m(1,2)*m(2,1)*m(3,3) - m(1,1)*m(2,3)*m(3,2)
        ;
        float det01 =
            m(1,0)*m(2,2)*m(3,3) + m(1,2)*m(2,3)*m(3,0) + m(1,3)*m(2,0)*m(3,2)
           -m(1,3)*m(2,2)*m(3,0) - m(1,2)*m(2,0)*m(3,3) - m(1,0)*m(2,3)*m(3,2)
        ;
        float det02 =
            m(1,0)*m(2,1)*m(3,3) + m(1,1)*m(2,3)*m(3,0) + m(1,3)*m(2,0)*m(3,1)
           -m(1,3)*m(2,1)*m(3,0) - m(1,1)*m(2,0)*m(3,3) - m(1,0)*m(2,3)*m(3,1)
        ;
        float det03 =
            m(1,0)*m(2,1)*m(3,2) + m(1,1)*m(2,2)*m(3,0) + m(1,2)*m(2,0)*m(3,1)
           -m(1,2)*m(2,1)*m(3,0) - m(1,1)*m(2,0)*m(3,2) - m(1,0)*m(2,2)*m(3,1)
        ;

        float det10 =
            m(0,1)*m(2,2)*m(3,3) + m(0,2)*m(2,3)*m(3,1) + m(0,3)*m(2,1)*m(3,2)
           -m(0,3)*m(2,2)*m(3,1) - m(0,2)*m(2,1)*m(3,3) - m(0,1)*m(2,3)*m(3,2)
        ;
        float det11 =
            m(0,0)*m(2,2)*m(3,3) + m(0,2)*m(2,3)*m(3,0) + m(0,3)*m(2,0)*m(3,2)
           -m(0,3)*m(2,2)*m(3,0) - m(0,2)*m(2,0)*m(3,3) - m(0,0)*m(2,3)*m(3,2)
        ;
        float det12 =
            m(0,0)*m(2,1)*m(3,3) + m(0,1)*m(2,3)*m(3,0) + m(0,3)*m(2,0)*m(3,1)
           -m(0,3)*m(2,1)*m(3,0) - m(0,1)*m(2,0)*m(3,3) - m(0,0)*m(2,3)*m(3,1)
        ;
        float det13 =
            m(0,0)*m(2,1)*m(3,2) + m(0,1)*m(2,2)*m(3,0) + m(0,2)*m(2,0)*m(3,1)
           -m(0,2)*m(2,1)*m(3,0) - m(0,1)*m(2,0)*m(3,2) - m(0,0)*m(2,2)*m(3,1)
        ;

        float det20 =
            m(0,1)*m(1,2)*m(3,3) + m(0,2)*m(1,3)*m(3,1) + m(0,3)*m(1,1)*m(3,2)
           -m(0,3)*m(1,2)*m(3,1) - m(0,2)*m(1,1)*m(3,3) - m(0,1)*m(1,3)*m(3,2)
        ;
        float det21 =
            m(0,0)*m(1,2)*m(3,3) + m(0,2)*m(1,3)*m(3,0) + m(0,3)*m(1,0)*m(3,2)
           -m(0,3)*m(1,2)*m(3,0) - m(0,2)*m(1,0)*m(3,3) - m(0,0)*m(1,3)*m(3,2)
        ;
        float det22 =
            m(0,0)*m(1,1)*m(3,3) + m(0,1)*m(1,3)*m(3,0) + m(0,3)*m(1,0)*m(3,1)
           -m(0,3)*m(1,1)*m(3,0) - m(0,1)*m(1,0)*m(3,3) - m(0,0)*m(1,3)*m(3,1)
        ;
        float det23 =
            m(0,0)*m(1,1)*m(3,2) + m(0,1)*m(1,2)*m(3,0) + m(0,2)*m(1,0)*m(3,1)
           -m(0,2)*m(1,1)*m(3,0) - m(0,1)*m(1,0)*m(3,2) - m(0,0)*m(1,2)*m(3,1)
        ;

        float det30 =
            m(0,1)*m(1,2)*m(2,3) + m(0,2)*m(1,3)*m(2,1) + m(0,3)*m(1,1)*m(2,2)
           -m(0,3)*m(1,2)*m(2,1) - m(0,2)*m(1,1)*m(2,3) - m(0,1)*m(1,3)*m(2,2)
        ;
        float det31 =
            m(0,0)*m(1,2)*m(2,3) + m(0,2)*m(1,3)*m(2,0) + m(0,3)*m(1,0)*m(2,2)
           -m(0,3)*m(1,2)*m(2,0) - m(0,2)*m(1,0)*m(2,3) - m(0,0)*m(1,3)*m(2,2)
        ;
        float det32 =
            m(0,0)*m(1,1)*m(2,3) + m(0,1)*m(1,3)*m(2,0) + m(0,3)*m(1,0)*m(2,1)
           -m(0,3)*m(1,1)*m(2,0) - m(0,1)*m(1,0)*m(2,3) - m(0,0)*m(1,3)*m(2,1)
        ;
        float det33 =
            m(0,0)*m(1,1)*m(2,2) + m(0,1)*m(1,2)*m(2,0) + m(0,2)*m(1,0)*m(2,1)
           -m(0,2)*m(1,1)*m(2,0) - m(0,1)*m(1,0)*m(2,2) - m(0,0)*m(1,2)*m(2,1)
        ;

        float invDet = 1.0f / (m(0,0)*det00 - m(1,0)*det10 + m(2,0)*det20 - m(3,0)*det30);

        return Mat4(
            det00, -det10, det20, -det30,
            -det01, det11, -det21, det31,
            det02, -det12, det22, -det32,
            -det03, det13, -det23, det33
        ) * invDet;
    }

    inline Mat4 makeTranslation(float tx, float ty, float tz)
    {
        return Mat4(
            0.0f, 0.0f, 0.0f, tx,
            0.0f, 0.0f, 0.0f, ty,
            0.0f, 0.0f, 0.0f, tz,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 makeScale(float sx, float sy, float sz)
    {
        return Mat4(
            sx,   0.0f, 0.0f, 0.0f,
            0.0f, sy,   0.0f, 0.0f,
            0.0f, 0.0f, sz,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 makeRotationX(float angleRad)
    {
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        return Mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c,    -s,   0.0f,
            0.0f, s,     c,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 makeRotationY(float angleRad)
    {
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        return Mat4(
             c,   0.0f, s,    0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s,   0.0f, c,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 makeRotationZ(float angleRad)
    {
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        return Mat4(
            c,    -s,   0.0f, 0.0f,
            s,     c,   0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
}