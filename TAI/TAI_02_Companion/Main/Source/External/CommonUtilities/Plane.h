#pragma once
#include "Vector3.h"

//template <typename T>
//using Vector3 = CommonUtilities::Vector3<T>;

namespace CommonUtilities
{
	template <typename T>
	class Plane
	{
	public:
		// Default constructor.
		Plane();
		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2-aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0,
		                     const Vector3<T>& aPoint1,
		                     const Vector3<T>& aPoint2);
		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T>& aPosition) const;
		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const { return myNormal; }

	private:
		Vector3<T> myNormal;
		Vector3<T> myPoint;
	};

	template <typename T>
	Plane<T>::Plane()
	{
		myNormal = Vector3{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};
		myPoint = Vector3{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};
	}

	// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2-aPoint0).
	template <typename T>
	Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		Vector3<T> edge1 = aPoint1 - aPoint0;
		Vector3<T> edge2 = aPoint2 - aPoint0;
		myNormal = edge1.Cross(edge2).GetNormalized();
	}

	// Constructor taking a point and a normal.
	template <typename T>
	Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal) : myPoint(aPoint0), myNormal(aNormal)
	{
	}

	template <typename T>
	void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		Vector3<T> edge1 = aPoint1 - aPoint0;
		Vector3<T> edge2 = aPoint2 - aPoint0;
		myNormal = edge1.Cross(edge2).GetNormalized();
	}

	// Init the plane with a point and a normal, the same as the constructor above.
	template <typename T>
	void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
	}

	// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
	template <typename T>
	bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> toPoint = aPosition - myPoint;
		T dot = toPoint.Dot(myNormal);
		return dot <= 0;
	}
}
