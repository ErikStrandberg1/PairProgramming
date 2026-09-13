#pragma once
#include "Vector3.h"
#include "Ray.h"

namespace CommonUtilities
{
	template <typename T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);
		// Returns the center of the sphere.
		const Vector3<T>& GetCenter() const;
		// Returns the radius of the sphere.
		T GetRadius() const;

		T IntersectRay(const Ray<T>& aRay) const;

	private:
		Vector3<T> myCenter;
		T myRadius;
	};

	template <typename T>
	Sphere<T>::Sphere() : myCenter(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	                      , myRadius(static_cast<T>(0))
	{
	}

	template <typename T>
	Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
		: myCenter(aCenter)
		  , myRadius(aRadius)
	{
	}

	template <typename T>
	void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
	}

	template <typename T>
	const Vector3<T>& Sphere<T>::GetCenter() const
	{
		return myCenter;
	}

	template <typename T>
	T Sphere<T>::GetRadius() const
	{
		return myRadius;
	}

	template <typename T>
	T Sphere<T>::IntersectRay(const Ray<T>& aRay) const
	{
		Vector3<T> toCenter = myCenter - aRay.GetOrigin();
		T distanceToCenter = toCenter.Length();

		if (distanceToCenter < myRadius)
		{
			return static_cast<T>(0);
		}

		T projectionLength = toCenter.Dot(aRay.GetDirection());

		// Sphere is behind the ray
		if (projectionLength < 0)
		{
			return static_cast<T>(-1);
		}

		// closest point on ray to sphere center
		T distanceSquared = toCenter.LengthSqr() - (projectionLength * projectionLength);
		T radiusSquared = myRadius * myRadius;

		// Ray misses the sphere
		if (distanceSquared > radiusSquared)
		{
			return static_cast<T>(-1);
		}

		// intersection distance
		T offset = std::sqrt(radiusSquared - distanceSquared);
		return projectionLength - offset;
	}
}
