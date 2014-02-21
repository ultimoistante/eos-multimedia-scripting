
#include "Vector3.h"
#include "Quaternion.h"

// -----------------------------------------------------------------------------

    const Vector3 Vector3::ZERO( 0, 0, 0 );

    const Vector3 Vector3::UNIT_X( 1, 0, 0 );
    const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
    const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
    const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
    const Vector3 Vector3::UNIT_SCALE(1, 1, 1);


        /** Generates a new random vector which deviates from this vector by a
            given angle in a random direction.
            @remarks
                This method assumes that the random number generator has already
                been seeded appropriately.
            @param
                angle The angle at which to deviate
            @param
                up Any vector perpendicular to this one (which could generated
                by cross-product of this vector and any other non-colinear
                vector). If you choose not to provide this the function will
                derive one on it's own, however if you provide one yourself the
                function will be faster (this allows you to reuse up vectors if
                you call this method more than once)
            @returns
                A random vector which deviates from this vector by angle. This
                vector will not be normalised, normalise it if you wish
                afterwards.
        */
        Vector3 Vector3::randomDeviant(const Radian& angle, const Vector3& up ) const
        {
            Vector3 newUp;

            if (up == Vector3::ZERO)
            {
                // Generate an up vector
                newUp = this->perpendicular();
            }
            else
            {
                newUp = up;
            }

            // Rotate up vector by random amount around this
            Quaternion q;
            q.FromAngleAxis( Radian(Math::UnitRandom() * Math::TWO_PI), *this );
            newUp = q * newUp;

            // Finally rotate this by given angle around randomised up
            q.FromAngleAxis( angle, newUp );
            return q * (*this);
        }
/*#ifndef OGRE_FORCE_ANGLE_TYPES
        Vector3 Vector3::randomDeviant(Real angle, const Vector3& up) const
        {
            return randomDeviant ( Radian(angle), up );
        }
#endif//OGRE_FORCE_ANGLE_TYPES*/




        /** Gets the shortest arc quaternion to rotate this vector to the destination
            vector.
        @remarks
            Don't call this if you think the dest vector can be close to the inverse
            of this vector, since then ANY axis of rotation is ok.
        */
        Quaternion Vector3::getRotationTo(const Vector3& dest) const
        {
            // Based on Stan Melax's article in Game Programming Gems
            Quaternion q;
            // Copy, since cannot modify local
            Vector3 v0 = *this;
            Vector3 v1 = dest;
            v0.normalise();
            v1.normalise();

            Vector3 c = v0.crossProduct(v1);

            // NB if the crossProduct approaches zero, we get unstable because ANY axis will do
            // when v0 == -v1
            Real d = v0.dotProduct(v1);
            // If dot == 1, vectors are the same
            if (d >= 1.0f)
            {
                return Quaternion::IDENTITY;
            }
            Real s = Math::Sqrt( (1+d)*2 );

            //assert (s != 0 && "Divide by zero!");
            // I REALLY don't like asserts! :@
            if(s == 0)
              s = 0.0000000000000001;

            Real invs = 1 / s;


            q.x = c.x * invs;
            q.y = c.y * invs;
            q.z = c.z * invs;
            q.w = s * 0.5;
            return q;
        }
