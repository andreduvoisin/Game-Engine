// FastMath.cpp defines statics for the fast math library
#include "fastmath.h"

namespace ITP485
{
float _ident[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f};
const FastMatrix4 FastMatrix4::Identity(_ident);

const FastVector3 FastVector3::Zero(0.0f, 0.0f, 0.0f);
const FastVector3 FastVector3::UnitX(1.0f, 0.0f, 0.0f);
const FastVector3 FastVector3::UnitY(0.0f, 1.0f, 0.0f);
const FastVector3 FastVector3::UnitZ(0.0f, 0.0f, 1.0f);
const FastVector3 FastVector3::NegativeUnitX(-1.0f, 0.0f, 0.0f);
const FastVector3 FastVector3::NegativeUnitY(0.0f, -1.0f, 0.0f);
const FastVector3 FastVector3::NegativeUnitZ(0.0f, 0.0f, -1.0f);

const FastQuaternion FastQuaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

void FastMatrix4::CreateTranslation(const FastVector3& translation)
{
	// 1 0 0 temp.x
	_rows[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, translation.GetX());

	// 0 1 0 temp.y
	_rows[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, translation.GetY());

	// 0 0 1 temp.z
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, translation.GetZ());

	// 0 0 0 1
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

void FastMatrix4::CreateFromQuaternion(const FastQuaternion& q)
{
	DirectX::XMMATRIX result = DirectX::XMMatrixRotationQuaternion(q._data);
	_rows[0] = result.r[0];
	_rows[1] = result.r[1];
	_rows[2] = result.r[2];
	_rows[3] = result.r[3];
	// Have to transpose b/c DirectXMath assumes row vectors
	_MM_TRANSPOSE4_PS(_rows[0], _rows[1], _rows[2], _rows[3]);
}

// Constructs a Look-At matrix
// vUp MUST be normalized or bad things will happen
void FastMatrix4::CreateLookAt( const FastVector3& vEye, const FastVector3& vAt, const FastVector3& vUp )
{
	DirectX::XMMATRIX result = DirectX::XMMatrixLookAtLH(vEye._data, vAt._data, vUp._data);
	_rows[0] = result.r[0];
	_rows[1] = result.r[1];
	_rows[2] = result.r[2];
	_rows[3] = result.r[3];

	// Have to transpose b/c DirectXMath assumes row vectors
	_MM_TRANSPOSE4_PS(_rows[0], _rows[1], _rows[2], _rows[3]);
}

void FastMatrix4::CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar)
{
	float fYScale = tanf(1.57079633f - (fFOVy/2)); // cot(x) is the same as tan(pi/2 - x)
	float fXScale = fYScale / fAspectRatio;

	_rows[0] = _mm_setr_ps(fXScale, 0.0f, 0.0f, 0.0f);
	_rows[1] = _mm_setr_ps(0.0f, fYScale, 0.0f, 0.0f);
	_rows[2] = _mm_setr_ps(0.0f, 0.0f, fFar/(fFar-fNear), -fNear*fFar/(fFar-fNear));
	_rows[3] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
}

void FastMatrix4::Invert()
{
	// Vectorizing this was a huge pain, so just use scalars :(
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float dst[16]; /* storage */
	float det; /* determinant */

	// row 1 to col 1
	src[0] = _rows[0].m128_f32[0];
	src[4] = _rows[0].m128_f32[1];
	src[8] = _rows[0].m128_f32[2];
	src[12] = _rows[0].m128_f32[3];

	// row 2 to col 2
	src[1] = _rows[1].m128_f32[0];
	src[5] = _rows[1].m128_f32[1];
	src[9] = _rows[1].m128_f32[2];
	src[13] = _rows[1].m128_f32[3];

	// row 3 to col 3
	src[2] = _rows[2].m128_f32[0];
	src[6] = _rows[2].m128_f32[1];
	src[10] = _rows[2].m128_f32[2];
	src[14] = _rows[2].m128_f32[3];

	// row 4 to col 4
	src[3] = _rows[3].m128_f32[0];
	src[7] = _rows[3].m128_f32[1];
	src[11] = _rows[3].m128_f32[2];
	src[15] = _rows[3].m128_f32[3];

// 	for (int i = 0; i < 4; i++) {
// 		src[i] = mat[i*4];
// 		src[i + 4] = mat[i*4 + 1];
// 		src[i + 8] = mat[i*4 + 2];
// 		src[i + 12] = mat[i*4 + 3];
// 	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];
	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	/* calculate determinant */
	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	/* calculate matrix inverse */
	det = 1/det;
	for (int j = 0; j < 16; j++)
		dst[j] *= det;

	// Set it back
	_rows[0] = _mm_setr_ps(dst[0], dst[1], dst[2], dst[3]);
	_rows[1] = _mm_setr_ps(dst[4], dst[5], dst[6], dst[7]);
	_rows[2] = _mm_setr_ps(dst[8], dst[9], dst[10], dst[11]);
	_rows[3] = _mm_setr_ps(dst[12], dst[13], dst[14], dst[15]);
}

void FastVector3::Rotate(const FastQuaternion& q)
{
	_data = DirectX::XMVector3Rotate(_data, q._data);
}

} // namespace ITP485
