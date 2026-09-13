#include "pch.h"
#include "CppUnitTest.h"
#include "../CommonUtilities/Matrix3x3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonUtilities;

namespace Matrix3x3Tests
{
	TEST_CLASS(Matrix3x3Tests)
	{
	public:
		TEST_METHOD(Identity)
		{
			Matrix3x3<float> matrix;
			Assert::AreEqual(1.0f, matrix(1, 1), 0.00001f);
			Assert::AreEqual(1.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(0.0f, matrix(1, 2), 0.00001f);
		}

		TEST_METHOD(IdentityDouble)
		{
			Matrix3x3<double> matrix;
			Assert::AreEqual(1.0, matrix(1, 1), 0.00001);
			Assert::AreEqual(0.0, matrix(1, 2), 0.00001);
		}

		TEST_METHOD(Constructor)
		{
			Matrix3x3<float> matrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			Assert::AreEqual(1.0f, matrix(1, 1), 0.00001f);
			Assert::AreEqual(5.0f, matrix(2, 2), 0.00001f);
			Assert::AreEqual(9.0f, matrix(3, 3), 0.00001f);
		}

		TEST_METHOD(Addition)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			Matrix3x3<float> result = m1 + m2;
			Assert::AreEqual(2.0f, result(1, 1), 0.00001f);
		}

		TEST_METHOD(AdditionEquals)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			m1 += m2;
			Assert::AreEqual(2.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Subtraction)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			Matrix3x3<float> result = m1 - m2;
			Assert::AreEqual(0.0f, result(1, 1), 0.00001f);
		}

		TEST_METHOD(SubtractionEquals)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			m1 -= m2;
			Assert::AreEqual(0.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Multiplication)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			Matrix3x3<float> result = m1 * m2;
			Assert::AreEqual(1.0f, result(1, 1), 0.00001f);
			Assert::AreEqual(0.0f, result(1, 2), 0.00001f);
		}

		TEST_METHOD(MultiplicationEquals)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			m1 *= m2;
			Assert::AreEqual(1.0f, m1(1, 1), 0.00001f);
		}

		TEST_METHOD(Transpose)
		{
			Matrix3x3<float> matrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			Matrix3x3<float> result = matrix.GetTranspose();
			Assert::AreEqual(1.0f, result(1, 1), 0.00001f);
			Assert::AreEqual(4.0f, result(1, 2), 0.00001f);
			Assert::AreEqual(2.0f, result(2, 1), 0.00001f);
		}

		TEST_METHOD(RotationX)
		{
			const float PI = 3.14159265f;
			Matrix3x3<float> rot = Matrix3x3<float>::CreateRotationAroundX(PI / 2.0f);
			Assert::AreEqual(1.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(0.0f, rot(2, 2), 0.001f);
		}

		TEST_METHOD(RotationY)
		{
			const float PI = 3.14159265f;
			Matrix3x3<float> rot = Matrix3x3<float>::CreateRotationAroundY(PI / 2.0f);
			Assert::AreEqual(0.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(1.0f, rot(2, 2), 0.001f);
		}

		TEST_METHOD(RotationZ)
		{
			const float PI = 3.14159265f;
			Matrix3x3<float> rot = Matrix3x3<float>::CreateRotationAroundZ(PI / 2.0f);
			Assert::AreEqual(0.0f, rot(1, 1), 0.001f);
			Assert::AreEqual(1.0f, rot(3, 3), 0.001f);
		}

		TEST_METHOD(Equality)
		{
			Matrix3x3<float> m1;
			Matrix3x3<float> m2;
			Assert::IsTrue(m1 == m2);
		}

		TEST_METHOD(BracketOperator)
		{
			Matrix3x3<float> matrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			Assert::AreEqual(1.0f, matrix[0], 0.00001f);
			Assert::AreEqual(5.0f, matrix[4], 0.00001f);
			Assert::AreEqual(9.0f, matrix[8], 0.00001f);
		}
	};
}
