#include "Matrix4.h"

#include "Vector3.h"
#include "Matrix3.h"

    const Matrix4 Matrix4::ZERO(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0 );

    const Matrix4 Matrix4::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );

    const Matrix4 Matrix4::CLIPSPACE2DTOIMAGESPACE(
        0.5,    0,  0, 0.5, 
          0, -0.5,  0, 0.5, 
          0,    0,  1,   0,
          0,    0,  0,   1);

    //-----------------------------------------------------------------------
    inline static Real
        MINOR(const Matrix4& m, const size_t r0, const size_t r1, const size_t r2, 
								const size_t c0, const size_t c1, const size_t c2)
    {
        return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
            m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
            m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
    }
    //-----------------------------------------------------------------------
    Matrix4 Matrix4::adjoint() const
    {
        return Matrix4( MINOR(*this, 1, 2, 3, 1, 2, 3),
            -MINOR(*this, 0, 2, 3, 1, 2, 3),
            MINOR(*this, 0, 1, 3, 1, 2, 3),
            -MINOR(*this, 0, 1, 2, 1, 2, 3),

            -MINOR(*this, 1, 2, 3, 0, 2, 3),
            MINOR(*this, 0, 2, 3, 0, 2, 3),
            -MINOR(*this, 0, 1, 3, 0, 2, 3),
            MINOR(*this, 0, 1, 2, 0, 2, 3),

            MINOR(*this, 1, 2, 3, 0, 1, 3),
            -MINOR(*this, 0, 2, 3, 0, 1, 3),
            MINOR(*this, 0, 1, 3, 0, 1, 3),
            -MINOR(*this, 0, 1, 2, 0, 1, 3),

            -MINOR(*this, 1, 2, 3, 0, 1, 2),
            MINOR(*this, 0, 2, 3, 0, 1, 2),
            -MINOR(*this, 0, 1, 3, 0, 1, 2),
            MINOR(*this, 0, 1, 2, 0, 1, 2));
    }
    //-----------------------------------------------------------------------
    Real Matrix4::determinant() const
    {
        return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
            m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
            m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
            m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
    }
    //-----------------------------------------------------------------------
    Matrix4 Matrix4::inverse() const
    {
        return adjoint() * (1.0f / determinant());
    }
    //-----------------------------------------------------------------------
    void Matrix4::makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
    {
        // Ordering:
        //    1. Scale
        //    2. Rotate
        //    3. Translate

        Matrix3 rot3x3, scale3x3;
        orientation.ToRotationMatrix(rot3x3);
        scale3x3 = Matrix3::ZERO;
        scale3x3[0][0] = scale.x;
        scale3x3[1][1] = scale.y;
        scale3x3[2][2] = scale.z;

        // Set up final matrix with scale, rotation and translation
        *this = rot3x3 * scale3x3;
        this->setTrans(position);

        // No projection term
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }
    //-----------------------------------------------------------------------
    void Matrix4::makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
    {
        // Invert the parameters
        Vector3 invTranslate = -position;
        Vector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
        Quaternion invRot = orientation.Inverse();

        // Because we're inverting, order is translation, rotation, scale
        // So make translation relative to scale & rotation
        invTranslate *= invScale; // scale
        invTranslate = invRot * invTranslate; // rotate

        // Next, make a 3x3 rotation matrix and apply inverse scale
        Matrix3 rot3x3, scale3x3;
        invRot.ToRotationMatrix(rot3x3);
        scale3x3 = Matrix3::ZERO;
        scale3x3[0][0] = invScale.x;
        scale3x3[1][1] = invScale.y;
        scale3x3[2][2] = invScale.z;

        // Set up final matrix with scale, rotation and translation
        *this = scale3x3 * rot3x3;
        this->setTrans(invTranslate);

        // No projection term
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }

