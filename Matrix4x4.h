#pragma once

#include <memory>
#include "Vectors.h"

class Matrix4x4 {

public:
    Matrix4x4() = default;
    ~Matrix4x4() = default;

public:
    void setIdentity()
    {
        ::memset(mMatrix, 0, sizeof(float) * 16); // Fill matrix with 0
        mMatrix[0][0] = 1;
        mMatrix[1][1] = 1;
        mMatrix[2][2] = 1;
        mMatrix[3][3] = 1;
    }

    void setTranslation(const Vec3& translation)
    {
        mMatrix[3][0] = translation.m_x;
        mMatrix[3][1] = translation.m_y;
        mMatrix[3][2] = translation.m_z;
    }

    Vec3 getTranslation() {
        return Vec3(mMatrix[3][0], mMatrix[3][1], mMatrix[3][2]);
    }

    void setScale(const Vec3& scale)
    {
        mMatrix[0][0] = scale.m_x;
        mMatrix[1][1] = scale.m_y;
        mMatrix[2][2] = scale.m_z;
    }

    Vec3 getScale()
    {
        return Vec3(
            mMatrix[0][0],
            mMatrix[1][1],
            mMatrix[2][2]
        );
    }

    void setRotationX(float x)
    {
        mMatrix[1][1] = cosf(x);
        mMatrix[1][2] = sinf(x);
        mMatrix[2][1] = -sinf(x);
        mMatrix[2][2] = cosf(x);
    }

    void setRotationY(float y)
    {
        mMatrix[0][0] = cosf(y);
        mMatrix[0][2] = -sinf(y);
        mMatrix[2][0] = sinf(y);
        mMatrix[2][2] = cosf(y);
    }

    void setRotationZ(float z)
    {
        mMatrix[0][0] = cosf(z);
        mMatrix[0][1] = sinf(z);
        mMatrix[1][0] = -sinf(z);
        mMatrix[1][1] = cosf(z);
    }

    float getDeterminant()
    {
        Vec4 minor, v1, v2, v3;
        float det;

        v1 = Vec4(this->mMatrix[0][0], this->mMatrix[1][0], this->mMatrix[2][0], this->mMatrix[3][0]);
        v2 = Vec4(this->mMatrix[0][1], this->mMatrix[1][1], this->mMatrix[2][1], this->mMatrix[3][1]);
        v3 = Vec4(this->mMatrix[0][2], this->mMatrix[1][2], this->mMatrix[2][2], this->mMatrix[3][2]);

        minor.cross(v1, v2, v3);
        det = -(this->mMatrix[0][3] * minor.m_x + this->mMatrix[1][3] * minor.m_y + this->mMatrix[2][3] * minor.m_z +
            this->mMatrix[3][3] * minor.m_w);

        return det;
    }

    void inverse()
    {
        int a, i, j;
        Matrix4x4 out;
        Vec4 v, vec[3];
        float det = 0.0f;

        det = Matrix4x4::getDeterminant();
        if (det == 0) return;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (j != i) {
                    a = j;
                    if (j > i) a = a - 1;
                    vec[a].m_x = (this->mMatrix[j][0]);
                    vec[a].m_y = (this->mMatrix[j][1]);
                    vec[a].m_z = (this->mMatrix[j][2]);
                    vec[a].m_w = (this->mMatrix[j][3]);
                }
            }
            v.cross(vec[0], vec[1], vec[2]);

            out.mMatrix[0][i] = (float)pow(-1.0f, i) * v.m_x / det;
            out.mMatrix[1][i] = (float)pow(-1.0f, i) * v.m_y / det;
            out.mMatrix[2][i] = (float)pow(-1.0f, i) * v.m_z / det;
            out.mMatrix[3][i] = (float)pow(-1.0f, i) * v.m_w / det;
        }
        this->setMatrix(out);
    }

    void operator*=(const Matrix4x4 matrix)
    {
        Matrix4x4 out;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                out.mMatrix[i][j] =
                    mMatrix[i][0] * matrix.mMatrix[0][j] + mMatrix[i][1] * matrix.mMatrix[1][j] +
                    mMatrix[i][2] * matrix.mMatrix[2][j] + mMatrix[i][3] * matrix.mMatrix[3][j];
            }
        }
        setMatrix(out);
    }

    void operator*=(const float x) {
        Matrix4x4 out;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                out.mMatrix[i][j] = mMatrix[i][j] * x;
            }
        }
        setMatrix(out);
    }

    void setMatrix(const Matrix4x4& matrix) {
        ::memcpy(mMatrix, matrix.mMatrix, sizeof(float) * 16);
    }

    Vec3 getZDirection() {
        return Vec3(mMatrix[2][0], mMatrix[2][1], mMatrix[2][2]);
    }

    void setZDirection(const Vec3& dir) {
        mMatrix[2][0] = dir.m_x;
        mMatrix[2][1] = dir.m_y;
        mMatrix[2][2] = dir.m_z;
    }

    Vec3 getYDirection() {
        return Vec3(mMatrix[1][0], mMatrix[1][1], mMatrix[1][2]);
    }

    void setYDirection(const Vec3& dir) {
        mMatrix[1][0] = dir.m_x;
        mMatrix[1][1] = dir.m_y;
        mMatrix[1][2] = dir.m_z;
    }

    Vec3 getXDirection() {
        return Vec3(mMatrix[0][0], mMatrix[0][1], mMatrix[0][2]);
    }

    void setXDirection(const Vec3& dir) {
        mMatrix[0][0] = dir.m_x;
        mMatrix[0][1] = dir.m_y;
        mMatrix[0][2] = dir.m_z;
    }

    /// <summary>
    /// Perspective projection matrix. FOV = field of view angle
    /// </summary>
    /// <param name="fov">The angle that gives the field of view</param>
    /// <param name="aspect"></param>
    /// <param name="znear">Distance between the origin and the camera and the near plane along the view direction</param>
    /// <param name="zfar">Distance between the origin and the camera and the far plane along the view direction</param>
    void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
    {
        float yScale = 1.0f / tanf(fov / 2.0f);
        float xScale = yScale / aspect;
        mMatrix[0][0] = xScale;
        mMatrix[1][1] = yScale;
        mMatrix[2][2] = zfar / (zfar - znear);
        mMatrix[2][3] = 1.0f;
        mMatrix[3][2] = (-znear * zfar) / (zfar - znear);
    }

    // For the orthogonal projection matrix (information about what we see on the screen)
    void setOrthoLH(float width, float height, float near_plane, float far_plane)
    {
        this->setIdentity();
        mMatrix[0][0] = 2.0f / width;
        mMatrix[1][1] = 2.0f / height;
        mMatrix[2][2] = 1.0f / (far_plane - near_plane);
        mMatrix[3][2] = -(near_plane / (far_plane - near_plane));
    }

//private:
    float mMatrix[4][4] = {};

};
