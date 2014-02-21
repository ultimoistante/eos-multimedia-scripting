#ifndef THREEDMATH_PLANE_H
#define THREEDMATH_PLANE_H

#include "Vector3.h"
#include <vector>


    /** Defines a plane in 3D space.
        @remarks
            A plane is defined in 3D space by the equation
            Ax + By + Cz + D = 0
        @par
            This equates to a vector (the normal of the plane, whose x, y
            and z components equate to the coefficients A, B and C
            respectively), and a constant (D) which is the distance along
            the normal you have to go to move the plane back to the origin.
     */
	class Plane
    {
    public:
        /** Default constructor - sets everything to 0.
        */
        Plane ();
        Plane (const Plane& rhs);
        /** Construct a plane through a normal, and a distance to move the plane along the normal.*/
        Plane (const Vector3& rkNormal, Real fConstant);
        Plane (const Vector3& rkNormal, const Vector3& rkPoint);
        Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
            const Vector3& rkPoint2);

        /** The "positive side" of the plane is the half space to which the
            plane normal points. The "negative side" is the other half
            space. The flag "no side" indicates the plane itself.
        */
        enum Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE
        };

        Side getSide (const Vector3& rkPoint) const;

        /** This is a pseudodistance. The sign of the return value is
            positive if the point is on the positive side of the plane,
            negative if the point is on the negative side, and zero if the
            point is on the plane.
            @par
            The absolute value of the return value is the true distance only
            when the plane normal is a unit length vector.
        */
        Real getDistance (const Vector3& rkPoint) const;

        /** Redefine this plane based on 3 points. */
        void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
            const Vector3& rkPoint2);

		/** Project a vector onto the plane. 
		@remarks This gives you the element of the input vector that is perpendicular 
			to the normal of the plane. You can get the element which is parallel
			to the normal of the plane by subtracting the result of this method
			from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		Vector3 projectVector(const Vector3& v);

		Vector3 normal;
        Real d;
        /// Comparison operator
        bool operator==(const Plane& rhs) const
        {
            return (rhs.d == d && rhs.normal == normal);
        }

        //_OgreExport friend std::ostream& operator<< (std::ostream& o, Plane& p);
    };

    typedef std::vector<Plane> PlaneList;


#endif
