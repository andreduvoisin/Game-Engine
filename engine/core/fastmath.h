// FastMath.h defines SIMD vector, matrix, and quaternion libraries
// NOTE: This library uses SSE4.1 extensions, which is not supported
// by AMD.
#ifndef _FASTMATH_H_
#define _FASTMATH_H_

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>
#include <DirectXMath.h>

// Define D3D Matrix here so we don't have to replicate
#ifndef D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;
#define D3DMATRIX_DEFINED
#endif

namespace ITP485
{

#ifndef _MATH_PI_CONSTANTS_
#define _MATH_PI_CONSTANTS_
const float TwoPi = 3.1415926535f * 2.0f;
const float Pi = 3.1415926535f;
const float PiOver2 = 3.1415926535f / 2.0f;
const float PiOver4 = 3.1415926535f / 4.0f;
#endif

// 4x4 Matrix class using SIMD
__declspec(align(16)) class FastMatrix4
{
private:
	union 
	{
		__m128 _rows[4];
		D3DMATRIX _d3dm;
	};
public:
	friend class FastVector3;
	friend class FastQuaternion;

	// Default constructor does nothing
	__forceinline FastMatrix4() {}

	// Constructs the matrix based on the passed-in floating point array[rows][column]
	__forceinline FastMatrix4(float mat[4][4])
	{
		_rows[0] = _mm_setr_ps(mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
		_rows[1] = _mm_setr_ps(mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
		_rows[2] = _mm_setr_ps(mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
		_rows[3] = _mm_setr_ps(mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}

	__forceinline void Set(float mat[4][4])
	{
		_rows[0] = _mm_setr_ps(mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
		_rows[1] = _mm_setr_ps(mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
		_rows[2] = _mm_setr_ps(mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
		_rows[3] = _mm_setr_ps(mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}

	// Copy constructor
	__forceinline FastMatrix4(const FastMatrix4& rhs)
	{
		_rows[0] = rhs._rows[0];
		_rows[1] = rhs._rows[1];
		_rows[2] = rhs._rows[2];
		_rows[3] = rhs._rows[3];
	}

	// Assignment operator
	__forceinline FastMatrix4& operator=(const FastMatrix4& rhs)
	{
		_rows[0] = rhs._rows[0];
		_rows[1] = rhs._rows[1];
		_rows[2] = rhs._rows[2];
		_rows[3] = rhs._rows[3];
		return *this;
	}

	// Returns this FastMatrix4 as a D3DMATRIX*
	__forceinline D3DMATRIX* ToD3D()
	{
		return &_d3dm;
	}

	// Multiplies this matrix by the rhs matrix, and stores the result in this matrix.
	void Multiply(const FastMatrix4& rhs)
	{
		__m128 rhs_row0 = rhs._rows[0];
		__m128 rhs_row1 = rhs._rows[1];
		__m128 rhs_row2 = rhs._rows[2];
		__m128 rhs_row3 = rhs._rows[3];

		// transpose the rhs matrix
		_MM_TRANSPOSE4_PS(rhs_row0, rhs_row1, rhs_row2, rhs_row3);

		// TODO
	}

	// Adds the rhs matrix to this one, storing in this
	__forceinline void Add(FastMatrix4& rhs)
	{
		_rows[0] = _mm_add_ps(_rows[0], rhs._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], rhs._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], rhs._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], rhs._rows[3]);
	}

	// Subtracts this - rhs, storing in this
	__forceinline void Sub(FastMatrix4& rhs)
	{
		_rows[0] = _mm_sub_ps(_rows[0], rhs._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], rhs._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], rhs._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], rhs._rows[3]);
	}

	// Given the passed in scale, constructs a Scale matrix
	__forceinline void CreateScale(float scale)
	{
		// scale 0 0 0
		_rows[0] = _mm_set_ss(scale);
		_rows[0] = _mm_shuffle_ps(_rows[0], _rows[0], _MM_SHUFFLE(1, 1, 1, 0));

		// 0 scale 0 0
		_rows[1] = _mm_set_ss(scale);
		_rows[1] = _mm_shuffle_ps(_rows[1], _rows[1], _MM_SHUFFLE(1, 1, 0, 1));

		// 0 0 scale 0
		_rows[2] = _mm_set_ss(scale);
		_rows[2] = _mm_shuffle_ps(_rows[2], _rows[2], _MM_SHUFFLE(1, 0, 1, 1));

		// 0 0 0 1
		_rows[3] = _mm_set_ss(1.0f);
		_rows[3] = _mm_shuffle_ps(_rows[3], _rows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Given the angle (in radians), constructs a Rotation about the X axis
	__forceinline void CreateRotationX(float angle)
	{
		// 1 0 0 0
		_rows[0] = _mm_set_ss(1.0f);
		_rows[0] = _mm_shuffle_ps(_rows[0], _rows[0], _MM_SHUFFLE(1, 1, 1, 0));

		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		// 0 cos -sin 0
		_rows[1] = _mm_setr_ps(0.0f, cos_theta, sin_theta * -1.0f, 0.0f);

		// 0 sin cos 0
		_rows[2] = _mm_setr_ps(0.0f, sin_theta, cos_theta, 0.0f);
		
		// 0 0 0 1
		_rows[3] = _mm_set_ss(1.0f);
		_rows[3] = _mm_shuffle_ps(_rows[3], _rows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Given the angle (in radians), constructs a Rotation about the Y axis
	__forceinline void CreateRotationY(float angle)
	{
		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		// cos 0 sin 0
		_rows[0] = _mm_setr_ps(cos_theta, 0.0f, sin_theta, 0.0f);

		// 0 1 0 0
		_rows[1] = _mm_set_ss(1.0f);
		_rows[1] = _mm_shuffle_ps(_rows[1], _rows[1], _MM_SHUFFLE(1, 1, 0, 1));

		// -sin 0 cos 0
		_rows[2] = _mm_setr_ps(sin_theta * -1.0f, 0.0f, cos_theta, 0.0f);

		// 0 0 0 1
		_rows[3] = _mm_set_ss(1.0f);
		_rows[3] = _mm_shuffle_ps(_rows[3], _rows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	// Given the angle (in radians), constructs a Rotation about the Z axis
	__forceinline void CreateRotationZ(float angle)
	{
		float cos_theta = cosf(angle);
		float sin_theta = sinf(angle);

		// cos -sin 0 0
		_rows[0] = _mm_setr_ps(cos_theta, sin_theta * -1.0f, 0.0f, 0.0f);

		// sin cos 0 0
		_rows[1] = _mm_setr_ps(sin_theta, cos_theta, 0.0f, 0.0f);

		// 0 0 1 0
		_rows[2] = _mm_set_ss(1.0f);
		_rows[2] = _mm_shuffle_ps(_rows[2], _rows[2], _MM_SHUFFLE(1, 0, 1, 1));

		// 0 0 0 1
		_rows[3] = _mm_set_ss(1.0f);
		_rows[3] = _mm_shuffle_ps(_rows[3], _rows[3], _MM_SHUFFLE(0, 1, 1, 1));
	}

	__forceinline friend FastMatrix4 Lerp(const FastMatrix4& a, const FastMatrix4& b, float f)
	{

		FastMatrix4 retVal;
		
		// row 0
		__m128 pct = _mm_set_ps1(f);
		retVal._rows[0] = _mm_mul_ps(b._rows[0], pct);
		__m128 ones = _mm_set_ps1(1.0f);
		ones = _mm_sub_ps(ones, pct);
		retVal._rows[0] = _mm_add_ps(retVal._rows[0], _mm_mul_ps(a._rows[0], ones));

		// row 1
		retVal._rows[1] = _mm_mul_ps(b._rows[1], pct);
		retVal._rows[1] = _mm_add_ps(retVal._rows[1], _mm_mul_ps(a._rows[1], ones));

		// row 2
		retVal._rows[2] = _mm_mul_ps(b._rows[2], pct);
		retVal._rows[2] = _mm_add_ps(retVal._rows[2], _mm_mul_ps(a._rows[2], ones));

		// row 3
		retVal._rows[3] = _mm_mul_ps(b._rows[3], pct);
		retVal._rows[3] = _mm_add_ps(retVal._rows[3], _mm_mul_ps(a._rows[3], ones));

		return retVal;
	}
	// Has to be defined in fastmath.cpp because of circular dependency
	
	// Given the translation vector, constructs a translation matrix.
	void CreateTranslation(const FastVector3& translation);

	// Given the quaternion, constructs a rotation matrix
	void CreateFromQuaternion(const FastQuaternion& q);

	// Constructs a Look-At matrix
	// vUp MUST be normalized or bad things will happen
	void CreateLookAt(const FastVector3& vEye, const FastVector3& vAt, const FastVector3& vUp);

	// Constructs a Perspective FOV matrix
	void CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar);

	// Inverts the matrix
	void Invert();

	// Identity matrix
	static const FastMatrix4 Identity;
};

// 3D vector class using SIMD
__declspec(align(16)) class FastVector3
{
private:
	__m128 _data;
	// _data.m128_f32[0] = x
	// _data.m128_f32[1] = y
	// _data.m128_f32[2] = z
	// _data.m128_f32[3] = w
public:

	// Default constructor does nothing
	__forceinline FastVector3() {}

	// Constructs a vector given the passed x, y, and z components.
	// w is set to 1.0f
	__forceinline FastVector3(float x, float y, float z)
	{
		_data = _mm_setr_ps(x, y, z, 1.0f);
	}

	// Constructs a FastVector3 given an __m128
	__forceinline FastVector3(__m128 value)
	{
		_data = value;
	}

	// Copy constructor
	__forceinline FastVector3(const FastVector3& rhs)
	{
		_data = rhs._data;
	}

	// Assignment operator
	__forceinline FastVector3& operator=(const FastVector3& rhs)
	{
		_data = rhs._data;
		return *this;
	}

	// Returns the X component (index 0) - SLOW
	__forceinline float GetX() const
	{
		return _data.m128_f32[0];
	}
	
	// Returns the Y component (index 1) - SLOW
	__forceinline float GetY() const
	{
		return _data.m128_f32[1];
	}
	
	// Returns the Z component (index 2) - SLOW
	__forceinline float GetZ() const
	{
		return _data.m128_f32[2];
	}

	// Returns the W component (index 3) - SLOW
	__forceinline float GetW() const
	{
		return _data.m128_f32[3];
	}
	
	// Sets the x, y, and z components to passed values.
	// w is set to 1.0f
	__forceinline void Set(float x, float y, float z)
	{
		_data = _mm_setr_ps(x, y, z, 1.0f);
	}

	// Sets the X component
	__forceinline void SetX(float x)
	{
		// set index 0 of temp to x
		__m128 temp = _mm_set_ss(x);
		// insert the value into the X component
		_data = _mm_insert_ps(_data, temp, 0x00);
	}
	
	// Sets the Y component
	__forceinline void SetY(float y)
	{
		// set index 0 of temp to y
		__m128 temp = _mm_set_ss(y);
		// insert the value into the Y component
		_data = _mm_insert_ps(_data, temp, 0x10);
	}
	
	// Sets the Z component
	__forceinline void SetZ(float z)
	{
		// set index 0 of temp to z
		__m128 temp = _mm_set_ss(z);
		// insert the value into the Z component
		_data = _mm_insert_ps(_data, temp, 0x20);
	}

	// Computes the dot product between this vector and rhs.
	// Returns the float result.
	__forceinline float Dot(const FastVector3& rhs) const
	{
		return 0.0f; // TODO: Fix
	}

	// Adds this vector to rhs, storing in this
	__forceinline void Add(const FastVector3& rhs)
	{
		// TODO
	}

	// Subtracts this - rhs, storing in this
	__forceinline void Sub(const FastVector3& rhs)
	{
		// TODO
	}

	// Does a scalar multiply by scalar
	__forceinline void Multiply(float scalar)
	{
		__m128 temp = _mm_set_ps1(scalar);
		_data = _mm_mul_ps(_data, temp);
	}

	// Normalizes this vector
	__forceinline void Normalize()
	{
		// TODO
 	}

	// Returns the length squared of this vector
	__forceinline float LengthSquared() const
	{
		return 0.0f; // TODO: Fix
	}

	// Returns the length of this vector
	__forceinline float Length() const
	{
		return 0.0f; // TODO: Fix
	}

	// Does a cross product between lhs and rhs, returning the result vector by value
	__forceinline friend FastVector3 Cross(const FastVector3& lhs, const FastVector3& rhs)
	{
		return FastVector3(FastVector3::Zero); // TODO: Fix
	}

	// Interpolates between a and b, returning the result vector by value
	// result = a * (1.0f - f) + b * f
	__forceinline friend FastVector3 Lerp(const FastVector3 &a, const FastVector3 &b, float f)
	{
		return FastVector3(FastVector3::Zero); // TODO: Fix
	}

	// does a 4-way blend
	// result = a * fa + b * fb + c * fc + d * fd
	// fd = 1.0f - fa - fb - fc
	__forceinline friend FastVector3 Blend(const FastVector3& a, const FastVector3& b, const FastVector3& c, const FastVector3& d,
											float fa, float fb, float fc)
	{
		return FastVector3(FastVector3::Zero); // TODO: Fix
	}

	// Transforms this vector by the passed 4x4 matrix
	// w is set to 1.0f before the transform is done
	__forceinline void Transform(const FastMatrix4 &mat)
	{
		// TODO
 	}

	// Rotates this vector by the passed quaternion
	void Rotate(const FastQuaternion& q);

	// Transforms this vector by the passed 4x4 matrix
	// w is set to 0.0f before the transform is done
	__forceinline void TransformAsVector(const FastMatrix4 &mat)
	{
		// TODO
 	}

	friend class FastMatrix4;
	friend class FastQuaternion;

	static const FastVector3 Zero;
	static const FastVector3 UnitX;
	static const FastVector3 UnitY;
	static const FastVector3 UnitZ;
	static const FastVector3 NegativeUnitX;
	static const FastVector3 NegativeUnitY;
	static const FastVector3 NegativeUnitZ;
};

// Unit quaternion class using SIMD
__declspec(align(16)) class FastQuaternion
{
private:
	DirectX::XMVECTOR _data;
	// 3-1 is vector component
	// 0 is scalar component
public:
	// Default constructor does nothing
	FastQuaternion() {}

	// Constructs the quaternion given the normalized axis
	// and the angle (in radians).
	FastQuaternion(const FastVector3& axis, float angle)
	{
		_data = DirectX::XMQuaternionRotationNormal(axis._data, angle);
	}

	// Constructs the quaternion given an __m128
	__forceinline FastQuaternion(__m128 value)
	{
		_data = value;
	}

	// Constructs a quaternion given the qv and qs components.
	// Note this assumes you have already applied the correct formula.
	__forceinline FastQuaternion(float qv_x, float qv_y, float qv_z, float qs)
	{
		_data = _mm_setr_ps(qv_x, qv_y, qv_z, qs);
	}

	__forceinline void Set(float qv_x, float qv_y, float qv_z, float qs)
	{
		_data = _mm_set_ps(qv_x, qv_y, qv_z, qs);
	}

	// Copy constructor
	__forceinline FastQuaternion(const FastQuaternion& rhs)
	{
		_data = rhs._data;
	}

	// Assignment operator
	__forceinline FastQuaternion& operator=(const FastQuaternion& rhs)
	{
		_data = rhs._data;
		return *this;
	}

	// Returns the x component of the vector component (index 0)
	__forceinline float GetVectorX() const
	{
		return _data.m128_f32[0];
	}

	// Returns the y component of the vector component (index 1)
	__forceinline float GetVectorY() const
	{
		return _data.m128_f32[1];
	}

	// Returns the z component of the vector component (index 2)
	__forceinline float GetVectorZ() const
	{
		return _data.m128_f32[2];
	}

	// Returns the scalar component (index 3)
	__forceinline float GetScalar() const
	{
		return _data.m128_f32[3];
	}
	
	// Rotate by THIS quaternion, followed by rhs
	// Or in other words, p = rhs, q = this
	// Store result in this quaternion.
	void Multiply(const FastQuaternion& rhs)
	{
		_data = DirectX::XMQuaternionMultiply(_data, rhs._data);
	}

	// Calculates the conjugate of this quaternion
	// Remember, for unit quaternions, the inverse is the conjugate.
	__forceinline void Conjugate()
	{
		_data = DirectX::XMQuaternionConjugate(_data);
	}

	// Returns the length squared of this quaternion
	__forceinline float LengthSquared() const
	{
		__m128 result = DirectX::XMQuaternionLengthSq(_data);
		return result.m128_f32[0];
	}

	// Returns the length of this quaternion
	__forceinline float Length() const
	{
		__m128 result = DirectX::XMQuaternionLength(_data);
		return result.m128_f32[0];
	}

	// Normalizes this quaternion
	__forceinline void Normalize()
	{
		_data = DirectX::XMQuaternionNormalize(_data);
	}

	// Interpolates between quaternion a and b, returning the result by value.
	// Note that the result quaternion is also normalized before it is returned, to maintain
	// unit length.
	// result = normalize(a * (1.0f - f) + b * f)
	__forceinline friend FastQuaternion Lerp(const FastQuaternion& a, const FastQuaternion& b, float f)
	{
		__m128 pct = _mm_set_ps1(f);
		__m128 result = _mm_mul_ps(b._data, pct);
		__m128 ones = _mm_set_ps1(1.0f);
		pct = _mm_sub_ps(ones, pct);
		result = _mm_add_ps(result, _mm_mul_ps(a._data, pct));

		// now normalize the result
		result = DirectX::XMQuaternionNormalize(result);

		return FastQuaternion(result);
	}

	// does a 4-way blend, normalizing the result
	// result = normalize(a * fa + b * fb + c * fc + d * fd)
	// fd = 1.0f - fa - fb - fc
	__forceinline friend FastQuaternion Blend(const FastQuaternion& a, const FastQuaternion& b, 
											  const FastQuaternion& c, const FastQuaternion& d,
											  float fa, float fb, float fc)
	{
		__m128 dpct = _mm_set_ps1(1.0f);
		__m128 pct = _mm_set_ps1(fa);
		__m128 result = _mm_mul_ps(a._data, pct);
		dpct = _mm_sub_ps(dpct, pct);

		pct = _mm_set_ps1(fb);
		__m128 temp = _mm_mul_ps(b._data, pct);
		result = _mm_add_ps(result, temp);
		dpct = _mm_sub_ps(dpct, pct);

		pct = _mm_set_ps1(fc);
		temp = _mm_mul_ps(c._data, pct);
		result = _mm_add_ps(result, temp);
		dpct = _mm_sub_ps(dpct, pct);

		temp = _mm_mul_ps(d._data, pct);
		result = _mm_add_ps(result, temp);

		// now normalize the result
		// dot with self to get length squared
		result = DirectX::XMQuaternionNormalize(result);

		return FastQuaternion(result);
	}

	friend class FastVector3;
	friend class FastMatrix4;

	static const FastQuaternion Identity;
};

} // namespace ITP485
#endif // _FASTMATH_H_
