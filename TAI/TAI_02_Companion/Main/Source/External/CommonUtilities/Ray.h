#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	template <typename T>
	class Ray
	{
	public:
		// Default constructor: there is no ray, the origin and direction are the
		// zero vector.
		Ray();
		// Constructor that takes origin and direction to define the ray
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);
		// Init the ray with two points.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>&
		                                aDirection);
		// Returns the normalized direction of the ray.
		const Vector3<T>& GetDirection() const;
		// Returns the origin of the ray.
		const Vector3<T>& GetOrigin() const;

	private:
		Vector3<float> myOrigin;
		Vector3<float> myDirection;
	};

	template <typename T>
	Ray<T>::Ray()
	{
	}

	template <typename T>
	Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection) : myOrigin(aOrigin), myDirection(aDirection)
	{
	}

	template <typename T>
	void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = (aPoint - aOrigin).GetNormalized();
	}

	template <typename T>
	void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection.GetNormalized();
	}

	template <typename T>
	const Vector3<T>& Ray<T>::GetDirection() const
	{
		return myDirection;
	}

	template <typename T>
	const Vector3<T>& Ray<T>::GetOrigin() const
	{
		return myOrigin;
	}
}
