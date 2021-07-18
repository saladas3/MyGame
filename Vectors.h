#pragma once

#include <math.h>

class Vec2 {

public:
    Vec2() : m_x(0), m_y(0) {}

    Vec2(float x, float y) : m_x(x), m_y(y) {}

    Vec2(const Vec2& vector) {
        m_x = vector.m_x;
        m_y = vector.m_y;
    }
    
    ~Vec2() = default;

public:
    Vec2 operator*(float num) const {
        return Vec2(m_x * num, m_y * num);
    }

    Vec2 operator+(const Vec2& vec) const {
        return Vec2(m_x + vec.m_x, m_y + vec.m_y);
    }

    Vec2 operator-(const Vec2& vec) const {
        return Vec2(m_x - vec.m_x, m_y - vec.m_y);
    }

public:
    float m_x, m_y;
};

class Vec3 {

public:
    Vec3() : m_x(0), m_y(0), m_z(0) {}

    Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

    Vec3(const Vec3& vector) {
        m_x = vector.m_x;
        m_y = vector.m_y;
        m_z = vector.m_z;
    }
    
    ~Vec3() = default;

public:
    static Vec3 lerp(const Vec3& start, const Vec3& end, float delta) {
        Vec3 v;
        v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
        v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
        v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);
        return v;
    }

    Vec3 operator*(float num) const {
        return Vec3(m_x * num, m_y * num, m_z * num);
    }

    Vec3 operator+(const Vec3& vec) const {
        return Vec3(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
    }

    Vec3 operator-(const Vec3& vec) const {
        return Vec3(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
    }

    bool operator!=(const Vec3& vec) const {
        return m_x != vec.m_x && m_y != vec.m_y && m_z != vec.m_z;
    }

    static Vec3 normalize(const Vec3& vec) {
        Vec3 res;
        float len = sqrtf((vec.m_x * vec.m_x) + (vec.m_y * vec.m_y) + (vec.m_z * vec.m_z));

        if (len == 0)
            return Vec3();

        res.m_x = vec.m_x / len;
        res.m_y = vec.m_y / len;
        res.m_z = vec.m_z / len;

        return res;
    }

    static Vec3 cross(const Vec3& v1, const Vec3& v2) {
        Vec3 res;
        res.m_x = (v1.m_y * v2.m_z) - (v1.m_z * v2.m_y);
        res.m_y = (v1.m_z * v2.m_x) - (v1.m_x * v2.m_z);
        res.m_z = (v1.m_x * v2.m_y) - (v1.m_y * v2.m_x);
        return res;
    }

    // Calculates the distance between two vectors
    //  by passing only the difference between them
    //  because sqrt is expensive
    static float calculateDistance(const Vec3& diff) {
        return sqrtf(diff.m_x * diff.m_x + diff.m_y * diff.m_y + diff.m_z * diff.m_z);
    }

    static Vec3 calculateMin(const Vec3& vec, float radius) {
        return Vec3(vec.m_x - radius, vec.m_y - radius, vec.m_z - radius);
    }

    static Vec3 calculateMax(const Vec3& vec, float radius) {
        return Vec3(vec.m_x + radius, vec.m_y + radius, vec.m_z + radius);
    }

public:
    float m_x, m_y, m_z;
};

class Vec4 {

public:
    Vec4() : m_x(0), m_y(0), m_z(0), m_w(0) {}

    Vec4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

    Vec4(const Vec4& vector) {
        m_x = vector.m_x;
        m_y = vector.m_y;
        m_z = vector.m_z;
        m_w = vector.m_w;
    }

    explicit Vec4(const Vec3& vector) : m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_w(1.0) {}

    ~Vec4() = default;
   
public:
    void cross(Vec4& v1, Vec4& v2, Vec4& v3) {
        this->m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) +
            v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
        this->m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) +
            v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
        this->m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) +
            v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
        this->m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) +
            v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
    }

public:
    float m_x, m_y, m_z, m_w;
};
