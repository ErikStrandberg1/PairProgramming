#pragma once
#include <cmath>
#include <limits>
#include <type_traits>
#include <cassert>

namespace Tga
{
	template <typename T>
	class Vector3;
}

namespace CommonUtilities
{
	template <typename T>
	class Vector3
	{
	public:
		T x; //Note: this variable is explicitly public.
		T y; //Note: this variable is explicitly public.
		T z; //Note: this variable is explicitly public.
		//Creates a null-vector
		Vector3();
		//Creates a vector (aX, aY, aZ)
		Vector3(const T& aX, const T& aY, const T& aZ);
		//Copy constructor (compiler generated)
		Vector3(const Vector3<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;
		//Destructor (compiler generated)
		~Vector3() = default;
		// Returns a CommonUtilities Vector3 copy with another datatype as elements,
		// ex: converts from Vector3<int> to Vector3<float>
		// useful to explicitly do operations, such as addition, for an explicit type
		template <class TargetType>
		Vector3<TargetType> ToType() const;
		//Returns a copy of the vector as a Tga vector, use to interface with TGE
		Tga::Vector3<T> ToTga() const;
		//Returns a negated copy of the vector
		Vector3<T> operator-() const;
		//Returns the squared length of the vector, optimization compared to length
		T LengthSqr() const;
		//Returns the length of the vector, int vector is not required to work
		T Length() const;
		//Returns a normalized copy of this vector. Need not function for int vectors
		//Handle normalization of zero-vector by returning the zero vector
		Vector3<T> GetNormalized() const;
		//Normalizes the vector. Need not function for int vectors
		//Handle normalization of zero-vector by not modifying the vector.
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;
		//Returns the cross product of this and aVector. Only for Vector3
		Vector3<T> Cross(const Vector3<T>& aVector) const;

		bool IsSafeDivisor(const T& s);
	};

	//Returns the vector sum of aVector0 and aVector1
	template <typename T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1);
	//Returns the vector difference of aVector0 and aVector1
	template <typename T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1);
	//Returns the vector aVector0 component-multiplied by aVector1
	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector0, const Vector3<T>& aVector1);
	//Returns the vector aVector multiplied by the scalar aScalar. Vector * Scalar
	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar);
	//Returns the vector aVector multiplied by the scalar aScalar. Scalar * Vector
	template <typename T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector);
	//Returns the vector aVector divided by the scalar aScalar
	template <typename T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar);
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <typename T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1);
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <typename T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1);
	//Equivalent to setting aVector to (aVector * aScalar)
	template <typename T>
	void operator*=(Vector3<T>& aVector, const T& aScalar);
	//Equivalent to setting aVector to (aVector / aScalar)
	template <typename T>
	void operator/=(Vector3<T>& aVector, const T& aScalar);
	//Implementations below this line------------


	template <typename T>
	Vector3<T>::Vector3() : x(0), y(0), z(0)
	{
	}

	template <typename T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template <typename T>
	template <class TargetType>
	Vector3<TargetType> Vector3<T>::ToType() const
	{
		return {static_cast<TargetType>(x), static_cast<TargetType>(y), static_cast<TargetType>(z)};
	}

	template <typename T>
	Tga::Vector3<T> Vector3<T>::ToTga() const
	{
		return Tga::Vector3<T>{x, y, z};
	}

	template <typename T>
	inline Vector3<T> Vector3<T>::operator-() const
	{
		return {-x, -y, -z};
	}

	template <typename T>
	inline T Vector3<T>::LengthSqr() const
	{
		long double sqr =
			static_cast<long double>(x) * static_cast<long double>(x) +
			static_cast<long double>(y) * static_cast<long double>(y) +
			static_cast<long double>(z) * static_cast<long double>(z);

		return static_cast<T>(sqr);
	}

	template <typename T>
	inline T Vector3<T>::Length() const
	{
		long double sqr =
			static_cast<long double>(x) * static_cast<long double>(x) +
			static_cast<long double>(y) * static_cast<long double>(y) +
			static_cast<long double>(z) * static_cast<long double>(z);

		return static_cast<T>(std::sqrt(sqr));
	}

	template <typename T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			T len = Length();
			if (len <= std::numeric_limits<T>::epsilon())
			{
				return {T(0), T(0), T(0)};
			}

			const T inv = T(1) / len;
			return {x * inv, y * inv, z * inv};
		}
		else
		{
			return {T(0), T(0), T(0)};
		}
	}

	template <typename T>
	void Vector3<T>::Normalize()
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			T len = Length();
			if (len <= std::numeric_limits<T>::epsilon())
			{
				return;
			}
			const T inv = T(1) / len;
			x *= inv;
			y *= inv;
			z *= inv;
		}
		else
		{
		}
	}

	template <typename T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + aVector.z * z;
	}

	template <typename T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return {
			y * aVector.z - z * aVector.y,
			z * aVector.x - x * aVector.z,
			x * aVector.y - y * aVector.x
		};
	}

	template <typename T>
	bool Vector3<T>::IsSafeDivisor(const T& s)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return std::fabs(s) > std::numeric_limits<T>::epsilon();
		}
		else
		{
			return s != 0;
		}
	}

	//OPERATORS
	template <typename T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return {aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z};
	}

	template <typename T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return {aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z};
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return {aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z};
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return {aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar};
	}

	template <typename T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return {aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar};
	}

	template <typename T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		assert(IsSafeDivisor(aScalar) && "Vector3: divide by ~Zero");
		return {aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar};
	}

	template <typename T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	template <typename T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
	}

	template <typename T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}

	template <typename T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		assert(IsSafeDivisor(aScalar) && "Vector3: divide by ~Zero");
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
	}
}
