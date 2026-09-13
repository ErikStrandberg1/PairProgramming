#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	template <typename T>
	class AABB3D
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Returns the min point of the AABB.
		const Vector3<T>& GetMin() const;
		// Returns the max point of the AABB.
		const Vector3<T>& GetMax() const;

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template <typename T>
	AABB3D<T>::AABB3D() : myMin(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	                      , myMax(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	{
	}

	template <typename T>
	AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax) : myMin(aMin)
	                                                                    , myMax(aMax)
	{
	}

	template <typename T>
	void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
	}

	template <typename T>
	const Vector3<T>& AABB3D<T>::GetMin() const
	{
		return myMin;
	}

	template <typename T>
	const Vector3<T>& AABB3D<T>::GetMax() const
	{
		return myMax;
	}
}
