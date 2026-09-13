#pragma once
#include <cassert>
#include <cmath>
#include "Vector3.h"


namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4;

	template <typename T>
	class Matrix3x3
	{
	public:
		friend class Matrix4x4<T>;
		// Creates the identity matrix.
		Matrix3x3<T>();
		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix) = default;

		Matrix3x3(const T a00, const T a01, const T a02,
		          const T a10, const T a11, const T a12,
		          const T a20, const T a21, const T a22);
		//Assignment operator (compiler generated)
		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix) = default;
		Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator+=(const Matrix3x3<T>& aMatrix);

		Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator-=(const Matrix3x3<T>& aMatrix);

		Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator*=(const Matrix3x3<T>& aMatrix);

		Vector3<T> operator*(const Vector3<T>& aVector);

		bool operator==(const Matrix3x3<T>& aMatrix) const;

		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read,
		//respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		T& operator[](const int aIndex);
		// Creates a transposed copy of the matrix.
		Matrix3x3<T> GetTranspose() const;
		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);

	private:
		T myMatrix[3][3];
	};

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		return Vector3<T>{
			aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(1, 2) + aVector.z * aMatrix(1, 3),
			aVector.x * aMatrix(2, 1) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(2, 3),
			aVector.x * aMatrix(3, 1) + aVector.y * aMatrix(3, 2) + aVector.z * aMatrix(3, 3)
		};
	}

	template <typename T>
	Matrix3x3<T>::Matrix3x3()
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				if (row == col)
				{
					myMatrix[row][col] = 1;
				}
				else
				{
					myMatrix[row][col] = 0;
				}
			}
		}
	}

	template <typename T>
	Matrix3x3<T>::Matrix3x3(const T a00, const T a01, const T a02,
	                        const T a10, const T a11, const T a12,
	                        const T a20, const T a21, const T a22)
	{
		myMatrix[0][0] = a00;
		myMatrix[0][1] = a01;
		myMatrix[0][2] = a02;
		myMatrix[1][0] = a10;
		myMatrix[1][1] = a11;
		myMatrix[1][2] = a12;
		myMatrix[2][0] = a20;
		myMatrix[2][1] = a21;
		myMatrix[2][2] = a22;
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3 result;

		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				result.myMatrix[row][col] = myMatrix[row][col] + aMatrix(row + 1, col + 1);
			}
		}
		return result;
	}

	template <typename T>
	Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
	{
		*this = *this + aMatrix;
		return *this;
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3 result;

		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				result.myMatrix[row][col] = myMatrix[row][col] - aMatrix(row + 1, col + 1);
			}
		}
		return result;
	}

	template <typename T>
	Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
	{
		*this = *this - aMatrix;
		return *this;
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& aMatrix)
	{
		return Matrix3x3<T>(
			myMatrix[0][0] * aMatrix(1, 1) + myMatrix[0][1] * aMatrix(2, 1) + myMatrix[0][2] * aMatrix(3, 1),
			myMatrix[0][0] * aMatrix(1, 2) + myMatrix[0][1] * aMatrix(2, 2) + myMatrix[0][2] * aMatrix(3, 2),
			myMatrix[0][0] * aMatrix(1, 3) + myMatrix[0][1] * aMatrix(2, 3) + myMatrix[0][2] * aMatrix(3, 3),

			myMatrix[1][0] * aMatrix(1, 1) + myMatrix[1][1] * aMatrix(2, 1) + myMatrix[1][2] * aMatrix(3, 1),
			myMatrix[1][0] * aMatrix(1, 2) + myMatrix[1][1] * aMatrix(2, 2) + myMatrix[1][2] * aMatrix(3, 2),
			myMatrix[1][0] * aMatrix(1, 3) + myMatrix[1][1] * aMatrix(2, 3) + myMatrix[1][2] * aMatrix(3, 3),

			myMatrix[2][0] * aMatrix(1, 1) + myMatrix[2][1] * aMatrix(2, 1) + myMatrix[2][2] * aMatrix(3, 1),
			myMatrix[2][0] * aMatrix(1, 2) + myMatrix[2][1] * aMatrix(2, 2) + myMatrix[2][2] * aMatrix(3, 2),
			myMatrix[2][0] * aMatrix(1, 3) + myMatrix[2][1] * aMatrix(2, 3) + myMatrix[2][2] * aMatrix(3, 3)
		);
	}

	template <typename T>
	Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		*this = *this * aMatrix;
		return *this;
	}

	template <typename T>
	Vector3<T> Matrix3x3<T>::operator*(const Vector3<T>& aVector)
	{
		return Vector3<T>{
			aVector.x * myMatrix[0][0] + aVector.y * myMatrix[0][1] + aVector.z * myMatrix[0][2],
			aVector.x * myMatrix[1][0] + aVector.y * myMatrix[1][1] + aVector.z * myMatrix[1][2],
			aVector.x * myMatrix[2][0] + aVector.y * myMatrix[2][1] + aVector.z * myMatrix[2][2],
		};
	}

	template <typename T>
	bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aMatrix) const
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				if (myMatrix[row][col] != aMatrix(row + 1, col + 1))
				{
					return false;
				}
			}
		}
		return true;
	}

	template <typename T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				myMatrix[row][col] = aMatrix(row + 1, col + 1);
			}
		}
	}

	template <typename T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow >= 1 && aRow <= 3 && "Matrix3x3 error: row out of bounds");
		assert(aColumn >= 1 && aColumn <= 3 && "Matrix3x3 error: column out of bounds");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <typename T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow >= 1 && aRow <= 3 && "Matrix3x3 error: row out of bounds");
		assert(aColumn >= 1 && aColumn <= 3 && "Matrix3x3 error: column out of bounds");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <typename T>
	T& Matrix3x3<T>::operator[](const int aIndex)
	{
		assert(aIndex >= 0 || aIndex < 9 && "Matrix3x3 error: operator[]");

		int row = aIndex / 3;
		int col = aIndex % 3;
		return myMatrix[row][col];
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::GetTranspose() const
	{
		Matrix3x3<T> result;
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				result.myMatrix[row][col] = myMatrix[col][row];
			}
		}
		return result;
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix3x3{
			1, 0, 0,
			0, c, -s,
			0, s, c
		};
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix3x3{
			c, 0, s,
			0, 1, 0,
			-s, 0, c
		};
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix3x3{
			c, -s, 0,
			s, c, 0,
			0, 0, 1
		};
	}
}
