/**************************************************************************************************
 *
 *	XCS Crowd Simulation Software
 *	Author: Scott Bevin
 *	Web: www.gameXcore.co.uk
 *	Email: scott@gameXcore.co.uk
 *	All code (c)2010 Scott Bevin, all rights reserved.
 *	
 *	This software is part of the disssertation work entitled "Real time crowd simulations incorporating individual
 *	agent personality models and group interactions" submitted to The University of Bolton by Scott Bevin in partial
 *	fulfilment of the requirements of The University of Bolton for the degree of Bachelor of Science in May 2010.
 *	
 **************************************************************************************************/

 /************************************************************************

 This file is part of "Real time crowd simulations incorporating individual agent personality models and group interactions".

 "Real time crowd simulations incorporating individual agent personality 
 models and group interactions" is free software: you can redistribute 
 it and/or modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 "Real time crowd simulations incorporating individual agent personality models and group interactions" is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with "Real time crowd simulations incorporating individual agent personality models and group interactions".  
 If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// All code (c)2010 Scott Bevin, all rights reserved.

#ifndef VECTOR2_H
#define VECTOR2_H

#include <cstdlib>
#include <cassert>
#include <math.h>

namespace XCS
{
	class Vector2f
	{
	public:
		float mX, mY;

	public:
		Vector2f()
		{
		}

		Vector2f(const float fX, const float fY )
			: mX( fX ), mY( fY )
		{
		}

		Vector2f( const float scaler )
			: mX( scaler), mY( scaler )
		{
		}

		float operator [] ( const size_t i ) const
		{
			assert( i < 2 );

			return *(&mX+i);
		}

		float& operator [] ( const size_t i )
		{
			assert( i < 2 );

			return *(&mX+i);
		}

		/// Pointer accessor for direct copying
		float* Ptr()
		{
			return &mX;
		}
		/// Pointer accessor for direct copying
		const float* Ptr() const
		{
			return &mX;
		}

		/** Assigns the value of the other vector.
		@param
		rkVector The other vector
		*/
		Vector2f& operator = ( const Vector2f& rkVector )
		{
			mX = rkVector.mX;
			mY = rkVector.mY;

			return *this;
		}

		Vector2f& operator = ( const float fScalar)
		{
			mX = fScalar;
			mY = fScalar;

			return *this;
		}

		bool operator == ( const Vector2f& rkVector ) const
		{
			return ( mX == rkVector.mX && mY == rkVector.mY );
		}

		bool operator != ( const Vector2f& rkVector ) const
		{
			return ( mX != rkVector.mX || mY != rkVector.mY  );
		}

		// arithmetic operations
		Vector2f operator + ( const Vector2f& rkVector ) const
		{
			return Vector2f(
				mX + rkVector.mX,
				mY + rkVector.mY);
		}

		Vector2f operator - ( const Vector2f& rkVector ) const
		{
			return Vector2f(
				mX - rkVector.mX,
				mY - rkVector.mY);
		}

		Vector2f operator * ( const float fScalar ) const
		{
			return Vector2f(
				mX * fScalar,
				mY * fScalar);
		}

		Vector2f operator * ( const Vector2f& rhs) const
		{
			return Vector2f(
				mX * rhs.mX,
				mY * rhs.mY);
		}

		Vector2f operator / ( const float fScalar ) const
		{
			assert( fScalar != 0.0 );

			float fInv = 1.0f / fScalar;

			return Vector2f(
				mX * fInv,
				mY * fInv);
		}

		Vector2f operator / ( const Vector2f& rhs) const
		{
			return Vector2f(
				mX / rhs.mX,
				mY / rhs.mY);
		}

		const Vector2f& operator + () const
		{
			return *this;
		}

		Vector2f operator - () const
		{
			return Vector2f(-mX, -mY);
		}

		// overloaded operators to help Vector2f
		friend Vector2f operator * ( const float fScalar, const Vector2f& rkVector )
		{
			return Vector2f(
				fScalar * rkVector.mX,
				fScalar * rkVector.mY);
		}

		friend Vector2f operator / ( const float fScalar, const Vector2f& rkVector )
		{
			return Vector2f(
				fScalar / rkVector.mX,
				fScalar / rkVector.mY);
		}

		friend Vector2f operator + (const Vector2f& lhs, const float rhs)
		{
			return Vector2f(
				lhs.mX + rhs,
				lhs.mY + rhs);
		}

		friend Vector2f operator + (const float lhs, const Vector2f& rhs)
		{
			return Vector2f(
				lhs + rhs.mX,
				lhs + rhs.mY);
		}

		friend Vector2f operator - (const Vector2f& lhs, const float rhs)
		{
			return Vector2f(
				lhs.mX - rhs,
				lhs.mY - rhs);
		}

		friend Vector2f operator - (const float lhs, const Vector2f& rhs)
		{
			return Vector2f(
				lhs - rhs.mX,
				lhs - rhs.mY);
		}
		// arithmetic updates
		Vector2f& operator += ( const Vector2f& rkVector )
		{
			mX += rkVector.mX;
			mY += rkVector.mY;

			return *this;
		}

		Vector2f& operator += ( const float fScaler )
		{
			mX += fScaler;
			mY += fScaler;

			return *this;
		}

		Vector2f& operator -= ( const Vector2f& rkVector )
		{
			mX -= rkVector.mX;
			mY -= rkVector.mY;

			return *this;
		}

		Vector2f& operator -= ( const float fScaler )
		{
			mX -= fScaler;
			mY -= fScaler;

			return *this;
		}

		Vector2f& operator *= ( const float fScalar )
		{
			mX *= fScalar;
			mY *= fScalar;

			return *this;
		}

		Vector2f& operator *= ( const Vector2f& rkVector )
		{
			mX *= rkVector.mX;
			mY *= rkVector.mY;

			return *this;
		}

		Vector2f& operator /= ( const float fScalar )
		{
			assert( fScalar != 0.0 );

			float fInv = 1.0 / fScalar;

			mX *= fInv;
			mY *= fInv;

			return *this;
		}

		Vector2f& operator /= ( const Vector2f& rkVector )
		{
			mX /= rkVector.mX;
			mY /= rkVector.mY;

			return *this;
		}

		/** Returns the length (magnitude) of the vector.
		@warning
		This operation requires a square root and is expensive in
		terms of CPU operations. If you don't need to know the exact
		length (e.g. for just comparing lengths) use squaredLength()
		instead.
		*/
		float Length () const
		{
			return sqrt( mX * mX + mY * mY );
		}

		/** Returns the square of the length(magnitude) of the vector.
		@remarks
		This  method is for efficiency - calculating the actual
		length of a vector requires a square root, which is expensive
		in terms of the operations required. This method returns the
		square of the length of the vector, i.e. the same as the
		length but before the square root is taken. Use this if you
		want to find the longest / shortest vector without incurring
		the square root.
		*/
		float SquaredLength () const
		{
			return mX * mX + mY * mY;
		}

		/** Calculates the dot (scalar) product of this vector with another.
		@remarks
		The dot product can be used to calculate the angle between 2
		vectors. If both are unit vectors, the dot product is the
		cosine of the angle; otherwise the dot product must be
		divided by the product of the lengths of both vectors to get
		the cosine of the angle. This result can further be used to
		calculate the distance of a point from a plane.
		@param
		vec Vector with which to calculate the dot product (together
		with this one).
		@returns
		A float representing the dot product value.
		*/
		float DotProduct(const Vector2f& vec) const
		{
			return mX * vec.mX + mY * vec.mY;
		}

		/** Normalises the vector.
		@remarks
		This method normalises the vector such that it's
		length / magnitude is 1. The result is called a unit vector.
		@note
		This function will not crash for zero-sized vectors, but there
		will be no changes made to their components.
		@returns The previous length of the vector.
		*/
		float Normalise()
		{
			float fLength = sqrt( mX * mX + mY * mY);

			// Will also work for zero-sized vectors, but will change nothing
			if ( fLength > 1e-08 )
			{
				float fInvLength = 1.0 / fLength;
				mX *= fInvLength;
				mY *= fInvLength;
			}

			return fLength;
		}



		/** Returns a vector at a point half way between this and the passed
		in vector.
		*/
		Vector2f MidPoint( const Vector2f& vec ) const
		{
			return Vector2f(
				( mX + vec.mX ) * 0.5f,
				( mY + vec.mY ) * 0.5f );
		}

		/** Returns true if the vector's scalar components are all greater
		that the ones of the vector it is compared against.
		*/
		bool operator < ( const Vector2f& rhs ) const
		{
			if( mX < rhs.mX && mY < rhs.mY )
				return true;
			return false;
		}

		/** Returns true if the vector's scalar components are all smaller
		that the ones of the vector it is compared against.
		*/
		bool operator > ( const Vector2f& rhs ) const
		{
			if( mX > rhs.mX && mY > rhs.mY )
				return true;
			return false;
		}

		/** Sets this vector's components to the minimum of its own and the
		ones of the passed in vector.
		@remarks
		'Minimum' in this case means the combination of the lowest
		value of mX, mY and z from both vectors. Lowest is taken just
		numerically, not magnitude, so -1 < 0.
		*/
		void MakeFloor( const Vector2f& cmp )
		{
			if( cmp.mX < mX ) mX = cmp.mX;
			if( cmp.mY < mY ) mY = cmp.mY;
		}

		/** Sets this vector's components to the maximum of its own and the
		ones of the passed in vector.
		@remarks
		'Maximum' in this case means the combination of the highest
		value of mX, mY and z from both vectors. Highest is taken just
		numerically, not magnitude, so 1 > -3.
		*/
		void MakeCeil( const Vector2f& cmp )
		{
			if( cmp.mX > mX ) mX = cmp.mX;
			if( cmp.mY > mY ) mY = cmp.mY;
		}

		/** Generates a vector perpendicular to this vector (eg an 'up' vector).
		@remarks
		This method will return a vector which is perpendicular to this
		vector. There are an infinite number of possibilities but this
		method will guarantee to generate one of them. If you need more
		control you should use the Quaternion class.
		*/
		Vector2f Perpendicular(void) const
		{
			return Vector2f (-mY, mX);
		}
		/** Calculates the 2 dimensional cross-product of 2 vectors, which results
		in a single floating point value which is 2 times the area of the triangle.
		*/
		float CrossProduct( const Vector2f& rkVector ) const
		{
			return mX * rkVector.mY - mY * rkVector.mX;
		}
		/** Generates a new random vector which deviates from this vector by a
		given angle in a random direction.
		@remarks
		This method assumes that the random number generator has already
		been seeded appropriately.
		@param
		angle The angle at which to deviate in radians
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
		Vector2f RandomDeviant(
			float angle) const
		{

			angle *=  ((float)rand()/((float)RAND_MAX + 1.0f)) * 3.14159265f;
			float cosa = cos(angle);
			float sina = sin(angle);
			return  Vector2f(cosa * mX - sina * mY,
				sina * mX + cosa * mY);
		}

		/** Returns true if this vector is zero length. */
		bool IsZeroLength(void) const
		{
			float sqlen = (mX * mX) + (mY * mY);
			return (sqlen < (1e-06 * 1e-06));

		}

		/** As normalise, except that this vector is unaffected and the
		normalised vector is returned as a copy. */
		Vector2f NormalisedCopy(void) const
		{
			Vector2f ret = *this;
			ret.Normalise();
			return ret;
		}

		/** Calculates a reflection vector to the plane with the given normal .
		@remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		Vector2f Reflect(const Vector2f& normal) const
		{
			return Vector2f( *this - ( 2 * this->DotProduct(normal) * normal ) );
		}

		// special points
		static Vector2f ZERO(){return Vector2f(0.0f);}
		static Vector2f UNIT_X(){return Vector2f(1.0f, 0.0f);}
		static Vector2f UNIT_Y(){return Vector2f(0.0f, 1.0f);}
		static Vector2f NEGATIVE_UNIT_X(){return Vector2f(-1.0f, 0.0f);}
		static Vector2f NEGATIVE_UNIT_Y(){return Vector2f(0.0f, -1.0f);}
		static Vector2f UNIT_SCALE(){return Vector2f(1.0f, 1.0f);}
	};
}

#endif