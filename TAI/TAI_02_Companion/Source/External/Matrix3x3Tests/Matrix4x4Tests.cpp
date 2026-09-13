#include "pch.h"
#include "CppUnitTest.h"
#include "../CommonUtilities/Matrix4x4.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonUtilities;

namespace Matrix4x4Tests
{
	TEST_CLASS(Matrix4x4Tests)
	{
	public:
		TEST_METHOD(Identity)
		{
			Matrix4x4<float> matrix;
			Assert::AreEqual(1.0f, matrix(1, 1), 0.00001f);
			Assert::AreEqual(1.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(1.0f, matrix(4, 4), 0.00001f);
			Assert::AreEqual(0.0f, matrix(1, 2), 0.00001f);
		}

		TEST_METHOD(IdentityDouble)
		{
			Matrix4x4<double> matrix;
			Assert::AreEqual(1.0, matrix(1, 1), 0.00001);
			Assert::AreEqual(0.0, matrix(1, 2), 0.00001);
		}

		TEST_METHOD(Constructor)
		{
			Matrix4x4<float> matrix(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);
			Assert::AreEqual(1.0f, matrix(1, 1), 0.00001f);
			Assert::AreEqual(6.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(11.0f, matrix(3, 3), 0.00001f);
			Assert::AreEqual(16.0f, matrix(4, 4), 0.00001f);
		}

		TEST_METHOD(Addition)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			Matrix4x4<float> result = m1 + m2;
			Assert::AreEqual(2.0f, result(1, 1), 0.00001f);
		}

		TEST_METHOD(AdditionEquals)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			m1 += m2;
			Assert::AreEqual(2.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Subtraction)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			Matrix4x4<float> result = m1 - m2;
			Assert::AreEqual(0.0f, result(1, 1), 0.00001f);
		}

		TEST_METHOD(SubtractionEquals)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			m1 -= m2;
			Assert::AreEqual(0.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Multiplication)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			Matrix4x4<float> result = m1 * m2;
			Assert::AreEqual(1.0f, result(1, 1), 0.00001f);
			Assert::AreEqual(0.0f, result(1, 2), 0.00001f);
		}

		TEST_METHOD(MultiplicationEquals)
		{
			Matrix4x4<float> m1;
			Matrix4x4<float> m2;
			m1 *= m2;
			Assert::AreEqual(1.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Transpose)
		{
			Matrix4x4<float> matrix(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);
			Matrix4x4<float> result = matrix.GetTranspose();
			Assert::AreEqual(1.0f, result(1, 1), 0.00001f);
			Assert::AreEqual(5.0f, result(1, 2), 0.00001f);
			Assert::AreEqual(2.0f, result(2, 1), 0.00001f);
		}

		TEST_METHOD(RotationX)
		{
			const float PI = 3.14159265f;
			Matrix4x4<float> rot = Matrix4x4<float>::CreateRotationAroundX(PI / 2.0f);
			Assert::AreEqual(1.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(0.0f, rot(2, 2), 0.001f);
			Assert::AreEqual(1.0f, rot(4, 4), 0.001f);
		}

		TEST_METHOD(RotationY)
		{
			const float PI = 3.14159265f;
			Matrix4x4<float> rot = Matrix4x4<float>::CreateRotationAroundY(PI / 2.0f);
			Assert::AreEqual(0.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(1.0f, rot(2, 2), 0.001f);
			Assert::AreEqual(1.0f, rot(4, 4), 0.001f);
		}

		TEST_METHOD(RotationZ)
		{
			const float PI = 3.14159265f;
			Matrix4x4<float> rot = Matrix4x4<float>::CreateRotationAroundZ(PI / 2.0f);
			Assert::AreEqual(0.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(1.0f, rot(3, 3), 0.001f);
			Assert::AreEqual(1.0f, rot(4, 4), 0.001f);
		}

		TEST_METHOD(BracketOperator)
		{
			Matrix4x4<float> matrix(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);
			Assert::AreEqual(1.0f, matrix[0], 0.00001f);
			Assert::AreEqual(6.0f, matrix[5], 0.00001f);
			Assert::AreEqual(16.0f, matrix[15], 0.00001f);
		}

		TEST_METHOD(FastInverse)
		{
			Matrix4x4<float> matrix2(
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				3.f, 4.f, 5.f, 1.f
			);

			Matrix4x4<float> inverseMatrix = Matrix4x4<float>::GetFastInverse(matrix2);

			Matrix4x4<float> matrix = inverseMatrix * matrix2;

			Assert::AreEqual(1.0f, matrix(1, 1), 0.00001f);
			Assert::AreEqual(1.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(0.0f, matrix(1, 3), 0.00001f);

			Assert::AreEqual(0.0f, matrix(2, 1), 0.00001f);
			Assert::AreEqual(1.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(0.0f, matrix(2, 3), 0.00001f);

			Assert::AreEqual(0.0f, matrix(3, 1), 0.00001f);
			Assert::AreEqual(0.0f, matrix(3, 2), 0.00001f);
			Assert::AreEqual(1.0f, matrix(3, 3), 0.00001f);

			Assert::AreEqual(1.0f, matrix(4, 4), 0.00001f);
			Assert::AreEqual(0.0f, matrix(4, 2), 0.00001f);
			Assert::AreEqual(0.0f, matrix(4, 3), 0.00001f);
		}
	};
}
