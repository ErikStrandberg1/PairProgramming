#pragma once
#include "Vector2.h"

//template <typename T>
//using Vector2 = CommonUtilities::Vector2<T>;

namespace CommonUtilities
{
	template <typename T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();
		// Copy constructor.
		Line(const Line<T>& aLine);
		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>&
		                               aDirection);
		// Returns whether a point is inside the line: it is inside when the point is on the
		//line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T>& aPosition) const;
		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const { return myDirection; }
		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T>& GetNormal() const { return myNormal; }

		const Vector2<T>& GetPoint() const { return myPointOnLine; }

	private:
		Vector2<T> myPointOnLine;
		Vector2<T> myDirection;
		Vector2<T> myNormal;
	};

	template <typename T>
	Line<T>::Line()
	{
		myPointOnLine = Vector2<T>(static_cast<T>(0), static_cast<T>(0));
		myDirection = Vector2<T>(static_cast<T>(0), static_cast<T>(0));
		myNormal = Vector2<T>(static_cast<T>(0), static_cast<T>(0));
	}

	template <typename T>
	Line<T>::Line(const Line<T>& aLine)
	{
		myPointOnLine = aLine.myPointOnLine;
		myDirection = aLine.myDirection;
		myNormal = aLine.myNormal;
	}

	// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
	template <typename T>
	Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPointOnLine = aPoint0;
		myDirection = (aPoint1 - aPoint0).GetNormalized();
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	// Init the line with two points, the same as the constructor above.
	template <typename T>
	void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		myPointOnLine = aPoint0;
		myDirection = (aPoint1 - aPoint0).GetNormalized();
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	// Init the line with a point and a direction.
	template <typename T>
	void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPointOnLine = aPoint;
		myDirection = aDirection;
		myNormal = Vector2<T>(-myDirection.y, myDirection.x);
	}

	// Returns whether a point is inside the line: it is inside when the point is on the
	//line or on the side the normal is pointing away from.
	template <typename T>
	bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		Vector2<T> lineToPoint = aPosition - myPointOnLine;
		T dot = lineToPoint.Dot(myNormal);
		return dot <= 0;
	}
}
