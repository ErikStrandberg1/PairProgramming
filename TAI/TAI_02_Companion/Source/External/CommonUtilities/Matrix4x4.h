#pragma once
#include <cassert>
#include <cmath>
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix3x3.h"

namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		// Copies the 3x3 matrix into the top left area of the 4x4 matrix.
		Matrix4x4<T>(const Matrix3x3<T>& aMatrix);
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix) = default;

		Matrix4x4(const T a00, const T a01, const T a02, const T a03,
		          const T a10, const T a11, const T a12, const T a13,
		          const T a20, const T a21, const T a22, const T a23,
		          const T a30, const T a31, const T a32, const T a33);

		//Assignment operator (compiler generated)
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix) = default;

		Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator+=(const Matrix4x4<T>& aMatrix);

		Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& aMatrix);

		Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& aMatrix);

		Vector4<T> operator*(const Vector4<T>& aVector);

		bool operator==(const Matrix4x4<T>& aMatrix) const;

		// () operator for accessing element (row, column) for read/write or read,
		//respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		T& operator[](const int aIndex);
		// Creates a transposed copy of the matrix.
		Matrix4x4<T> GetTranspose() const;
		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T aAngleInRadians);

		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aMatrix);

	private:
		T myMatrix[4][4];
	};

	template <typename T>
	Matrix4x4<T>::Matrix4x4()
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
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
	Matrix4x4<T>::Matrix4x4(const Matrix3x3<T>& aMatrix)
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				if (row < 3 && col < 3)
				{
					myMatrix[row][col] = aMatrix(row + 1, col + 1);
				}
				else
				{
					myMatrix[row][col] = 0;
				}
			}
		}
		myMatrix[3][3] = 1;
	}

	template <typename T>
	Matrix4x4<T>::Matrix4x4(const T a00, const T a01, const T a02, const T a03,
	                        const T a10, const T a11, const T a12, const T a13,
	                        const T a20, const T a21, const T a22, const T a23,
	                        const T a30, const T a31, const T a32, const T a33)
	{
		myMatrix[0][0] = a00;
		myMatrix[0][1] = a01;
		myMatrix[0][2] = a02;
		myMatrix[0][3] = a03;

		myMatrix[1][0] = a10;
		myMatrix[1][1] = a11;
		myMatrix[1][2] = a12;
		myMatrix[1][3] = a13;

		myMatrix[2][0] = a20;
		myMatrix[2][1] = a21;
		myMatrix[2][2] = a22;
		myMatrix[2][3] = a23;

		myMatrix[3][0] = a30;
		myMatrix[3][1] = a31;
		myMatrix[3][2] = a32;
		myMatrix[3][3] = a33;
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4 result;

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result.myMatrix[row][col] = myMatrix[row][col] + aMatrix(row + 1, col + 1);
			}
		}
		return result;
	}

	template <typename T>
	Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this + aMatrix;
		return *this;
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4 result;

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result.myMatrix[row][col] = myMatrix[row][col] - aMatrix(row + 1, col + 1);
			}
		}
		return result;
	}

	template <typename T>
	Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this - aMatrix;
		return *this;
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix)
	{
		return Matrix4x4<T>(
			myMatrix[0][0] * aMatrix(1, 1) +
			myMatrix[0][1] * aMatrix(2, 1) +
			myMatrix[0][2] * aMatrix(3, 1) +
			myMatrix[0][3] * aMatrix(4, 1),

			myMatrix[0][0] * aMatrix(1, 2) +
			myMatrix[0][1] * aMatrix(2, 2) +
			myMatrix[0][2] * aMatrix(3, 2) +
			myMatrix[0][3] * aMatrix(4, 2),

			myMatrix[0][0] * aMatrix(1, 3) +
			myMatrix[0][1] * aMatrix(2, 3) +
			myMatrix[0][2] * aMatrix(3, 3) +
			myMatrix[0][3] * aMatrix(4, 3),

			myMatrix[0][0] * aMatrix(1, 4) +
			myMatrix[0][1] * aMatrix(2, 4) +
			myMatrix[0][2] * aMatrix(3, 4) +
			myMatrix[0][3] * aMatrix(4, 4),

			myMatrix[1][0] * aMatrix(1, 1) +
			myMatrix[1][1] * aMatrix(2, 1) +
			myMatrix[1][2] * aMatrix(3, 1) +
			myMatrix[1][3] * aMatrix(4, 1),

			myMatrix[1][0] * aMatrix(1, 2) +
			myMatrix[1][1] * aMatrix(2, 2) +
			myMatrix[1][2] * aMatrix(3, 2) +
			myMatrix[1][3] * aMatrix(4, 2),

			myMatrix[1][0] * aMatrix(1, 3) +
			myMatrix[1][1] * aMatrix(2, 3) +
			myMatrix[1][2] * aMatrix(3, 3) +
			myMatrix[1][3] * aMatrix(4, 3),

			myMatrix[1][0] * aMatrix(1, 4) +
			myMatrix[1][1] * aMatrix(2, 4) +
			myMatrix[1][2] * aMatrix(3, 4) +
			myMatrix[1][3] * aMatrix(4, 4),

			myMatrix[2][0] * aMatrix(1, 1) +
			myMatrix[2][1] * aMatrix(2, 1) +
			myMatrix[2][2] * aMatrix(3, 1) +
			myMatrix[2][3] * aMatrix(4, 1),

			myMatrix[2][0] * aMatrix(1, 2) +
			myMatrix[2][1] * aMatrix(2, 2) +
			myMatrix[2][2] * aMatrix(3, 2) +
			myMatrix[2][3] * aMatrix(4, 2),

			myMatrix[2][0] * aMatrix(1, 3) +
			myMatrix[2][1] * aMatrix(2, 3) +
			myMatrix[2][2] * aMatrix(3, 3) +
			myMatrix[2][3] * aMatrix(4, 3),

			myMatrix[2][0] * aMatrix(1, 4) +
			myMatrix[2][1] * aMatrix(2, 4) +
			myMatrix[2][2] * aMatrix(3, 4) +
			myMatrix[2][3] * aMatrix(4, 4),

			myMatrix[3][0] * aMatrix(1, 1) +
			myMatrix[3][1] * aMatrix(2, 1) +
			myMatrix[3][2] * aMatrix(3, 1) +
			myMatrix[3][3] * aMatrix(4, 1),

			myMatrix[3][0] * aMatrix(1, 2) +
			myMatrix[3][1] * aMatrix(2, 2) +
			myMatrix[3][2] * aMatrix(3, 2) +
			myMatrix[3][3] * aMatrix(4, 2),

			myMatrix[3][0] * aMatrix(1, 3) +
			myMatrix[3][1] * aMatrix(2, 3) +
			myMatrix[3][2] * aMatrix(3, 3) +
			myMatrix[3][3] * aMatrix(4, 3),

			myMatrix[3][0] * aMatrix(1, 4) +
			myMatrix[3][1] * aMatrix(2, 4) +
			myMatrix[3][2] * aMatrix(3, 4) +
			myMatrix[3][3] * aMatrix(4, 4)
		);
	}

	template <typename T>
	Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this * aMatrix;
		return *this;
	}

	template <typename T>
	Vector4<T> Matrix4x4<T>::operator*(const Vector4<T>& aVector)
	{
		return Vector4<T>{
			aVector.x * myMatrix[0][0] + aVector.y * myMatrix[0][1] +
			aVector.z * myMatrix[0][2] + aVector.w * myMatrix[0][3],

			aVector.x * myMatrix[1][0] + aVector.y * myMatrix[1][1] +
			aVector.z * myMatrix[1][2] + aVector.w * myMatrix[1][3],

			aVector.x * myMatrix[2][0] + aVector.y * myMatrix[2][1] +
			aVector.z * myMatrix[2][2] + aVector.w * myMatrix[2][3],

			aVector.x * myMatrix[3][0] + aVector.y * myMatrix[3][1] +
			aVector.z * myMatrix[3][2] + aVector.w * myMatrix[3][3],
		};
	}

	template <typename T>
	bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aMatrix) const
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
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
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow >= 1 && aRow <= 4 && "Matrix4x4 error: row out of bounds ");
		assert(aColumn >= 1 && aColumn <= 4 && "Matrix4x4 error: col out of bounds ");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <typename T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow >= 1 && aRow <= 4 && "Matrix4x4 error: row out of bounds ");
		assert(aColumn >= 1 && aColumn <= 4 && "Matrix4x4 error: col out of bounds ");

		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <typename T>
	T& Matrix4x4<T>::operator[](const int aIndex)
	{
		assert(aIndex >= 0 && aIndex < 16 && "Matrix4x4 error: index out of bounds ");

		int row = aIndex / 4;
		int col = aIndex % 4;

		return myMatrix[row][col];
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		Matrix4x4 result;

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result.myMatrix[row][col] = myMatrix[col][row];
			}
		}
		return result;
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix4x4{
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix4x4{
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		T s = sinf(aAngleInRadians);
		T c = cosf(aAngleInRadians);

		return Matrix4x4{
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aMatrix)
	{
		Matrix3x3<T> transposed = aMatrix.GetTranspose();

		Vector3<T> translation(
			aMatrix(4, 1),
			aMatrix(4, 2),
			aMatrix(4, 3)
		);

		Vector3<T> newTranslation = -translation * transposed;

		Matrix4x4<T> inverse(transposed);

		inverse.myMatrix[3][0] = newTranslation.x;
		inverse.myMatrix[3][1] = newTranslation.y;
		inverse.myMatrix[3][2] = newTranslation.z;

		return inverse;
	}
}
