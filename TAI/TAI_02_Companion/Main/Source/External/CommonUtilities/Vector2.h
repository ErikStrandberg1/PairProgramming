#pragma once
#include <cmath>
#include <limits>
#include <type_traits>
#include <cassert>

namespace Tga
{
	template <typename T>
	class Vector2;
}

namespace CommonUtilities
{
	template <typename T>
	class Vector2
	{
	public:
		T x;
		T y;
		//Creates a null-vector
		Vector2();
		//Creates a vector (aX, aY)
		Vector2(const T& aX, const T& aY);
		//Copy constructor (compiler generated)
		Vector2(const Vector2<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
		//Destructor (compiler generated)
		~Vector2() = default;
		// Returns a CommonUtilities Vector2 copy with another datatype as elements,
		// ex: converts from Vector2<int> to Vector2<float>
		// useful to explicitly do operations, such as addition, for an explicit type
		template <class TargetType>
		Vector2<TargetType> ToType() const;
		//Returns a copy of the vector as a Tga vector, use to interface with TGE
		Tga::Vector2<T> ToTga() const;
		//Returns a negated copy of the vector
		Vector2<T> operator-() const;
		//Returns the squared length of the vector, optimization compared to length
		T LengthSqr() const;
		//Returns the length of the vector, int vector is not required to work
		T Length() const;
		//Returns a normalized copy of this vector. Need not function for int vectors
		//Handle normalization of zero-vector by returning the zero vector
		Vector2<T> GetNormalized() const;
		//Normalizes the vector. Need not function for int vectors
		//Handle normalization of zero-vector by not modifying the vector.
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;

		bool IsSafeDivisor(const T& s);
	};

	//Returns the vector sum of aVector0 and aVector1
	template <typename T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1);
	//Returns the vector difference of aVector0 and aVector1
	template <typename T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1);
	//Returns the vector aVector0 component-multiplied by aVector1
	template <typename T>
	Vector2<T> operator*(const Vector2<T>& aVector0, const Vector2<T>& aVector1);
	//Returns the vector aVector multiplied by the scalar aScalar. Vector * Scalar
	template <typename T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar);
	//Returns the vector aVector multiplied by the scalar aScalar. Scalar * Vector
	template <typename T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector);
	//Returns the vector aVector divided by the scalar aScalar
	template <typename T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar);
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <typename T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1);
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <typename T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1);
	//Equivalent to setting aVector to (aVector * aScalar)
	template <typename T>
	void operator*=(Vector2<T>& aVector, const T& aScalar);
	//Equivalent to setting aVector to (aVector / aScalar)
	template <typename T>
	void operator/=(Vector2<T>& aVector, const T& aScalar);
	//Implementations below this line------------

	template <typename T>
	Vector2<T>::Vector2() : x(0), y(0)
	{
	}

	template <typename T>
	Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template <typename T>
	template <class TargetType>
	Vector2<TargetType> Vector2<T>::ToType() const
	{
		return {static_cast<TargetType>(x), static_cast<TargetType>(y)};
	}

	template <typename T>
	Tga::Vector2<T> Vector2<T>::ToTga() const
	{
		return Tga::Vector2<T>{x, y};
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator-() const
	{
		return {-x, -y};
	}

	template <typename T>
	inline T Vector2<T>::LengthSqr() const
	{
		long double s =
			static_cast<long double>(x) * static_cast<long double>(x) +
			static_cast<long double>(y) * static_cast<long double>(y);

		return static_cast<T>(s);
	}

	template <typename T>
	inline T Vector2<T>::Length() const
	{
		long double s =
			static_cast<long double>(x) * static_cast<long double>(x) +
			static_cast<long double>(y) * static_cast<long double>(y);

		return {static_cast<T>(std::sqrt(s))};
	}

	template <typename T>
	Vector2<T> Vector2<T>::GetNormalized() const
	{
		using R = std::conditional_t<std::is_floating_point_v<T>, T, float>;
		const R length = static_cast<R>(Length());

		if (length <= std::numeric_limits<R>::epsilon())
		{
			return Vector2<R>{x, y};
		}
		return Vector2<R>{static_cast<R>(x) / length, static_cast<R>(y) / length};
	}

	template <typename T>
	void Vector2<T>::Normalize()
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
		}
		else
		{
		}
	}

	template <typename T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template <typename T>
	bool Vector2<T>::IsSafeDivisor(const T& s)
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

	template <typename T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return {aVector0.x + aVector1.x, aVector0.y + aVector1.y};
	}

	template <typename T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return {aVector0.x - aVector1.x, aVector0.y - aVector1.y};
	}

	template <typename T>
	Vector2<T> operator*(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return {aVector0.x * aVector1.x, aVector0.y * aVector1.y};
	}

	template <typename T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return {aVector.x * aScalar, aVector.y * aScalar};
	}

	template <typename T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return {aVector.x * aScalar, aVector.y * aScalar};
	}

	template <typename T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(IsSafeDivisor(aScalar) && "Vector2: divide by ~Zero");
		return {aVector.x / aScalar, aVector.y / aScalar};
	}

	template <typename T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	template <typename T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	template <typename T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	template <typename T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		assert(IsSafeDivisor(aScalar) && "Vector2: divide by ~Zero");
		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}
}
