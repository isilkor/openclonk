/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de/
 * Copyright (c) 2011-2015, The OpenClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

#ifndef INC_StdMeshMath
#define INC_StdMeshMath

#include <emmintrin.h>

// OGRE mesh

namespace math { namespace detail {
	inline __m128 insert_float(__m128 dest, float f, int pos) {
		__m128 src = _mm_set_ps1(f);
		__m128 mask = _mm_castsi128_ps(_mm_set_epi32(-(pos==3), -(pos==2), -(pos==1), -(pos==0)));
		return _mm_or_ps(_mm_andnot_ps(mask, dest), _mm_and_ps(mask, src));
	}
	inline __m128 haddps_full(__m128 v)
	{
		v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 3, 0, 1)));
		v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2)));
		return v;
	}
}}


class StdMeshVector
{
	// v = w z y x     where w is unused
	__m128 v;

public:
	StdMeshVector(const StdMeshVector &r) : v(r.v) {}
	StdMeshVector &operator=(const StdMeshVector &r) { v = r.v; return *this; }
	StdMeshVector(float x, float y, float z) : v(_mm_set_ps(0, z, y, x)) {}

	StdMeshVector &set(float x, float y, float z) { v = _mm_set_ps(0, z, y, x); return *this; }
	StdMeshVector &extract(float *x, float *y, float *z) { *x = this->x(); *y = this->y(); *z = this->z(); return *this; }
	const StdMeshVector &extract(float *x, float *y, float *z) const { return const_cast<StdMeshVector*>(this)->extract(x, y, z); }

	float x() const { return _mm_cvtss_f32(v); }
	float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 1))); }
	float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 2))); }

	StdMeshVector &setX(float x) { v = ::math::detail::insert_float(v, x, 0); return *this; }
	StdMeshVector &setY(float y) { v = ::math::detail::insert_float(v, y, 1); return *this; }
	StdMeshVector &setZ(float z) { v = ::math::detail::insert_float(v, z, 2); return *this; }

	StdMeshVector &operator+=(const StdMeshVector &r) { v = _mm_add_ps(v, r); return *this; }
	StdMeshVector &operator+=(float r) { v = _mm_add_ps(v, _mm_set_ps1(r)); return *this; }
	StdMeshVector &operator-=(const StdMeshVector &r) { v = _mm_sub_ps(v, r); return *this; }
	StdMeshVector &operator-=(float r) { v = _mm_sub_ps(v, _mm_set_ps1(r)); return *this; }
	StdMeshVector &operator*=(const StdMeshVector &r) { v = _mm_mul_ps(v, r); return *this; }
	StdMeshVector &operator*=(float r) { v = _mm_mul_ps(v, _mm_set_ps1(r)); return *this; }
	StdMeshVector &operator/=(const StdMeshVector &r) { v = _mm_div_ps(v, r); return *this; }
	StdMeshVector &operator/=(float r) { v = _mm_div_ps(v, _mm_set_ps1(r)); return *this; }

	StdMeshVector operator-() const { return _mm_sub_ps(_mm_setzero_ps(), v); }

	friend StdMeshVector operator+(const StdMeshVector &l, const StdMeshVector &r);
	friend StdMeshVector operator-(const StdMeshVector &l, const StdMeshVector &r);
	friend StdMeshVector operator*(const StdMeshVector &l, const StdMeshVector &r);
	friend StdMeshVector operator/(const StdMeshVector &l, const StdMeshVector &r);

	friend StdMeshVector operator+(const StdMeshVector &l, float r);
	friend StdMeshVector operator-(const StdMeshVector &l, float r);
	friend StdMeshVector operator*(const StdMeshVector &l, float r);
	friend StdMeshVector operator/(const StdMeshVector &l, float r);

	friend StdMeshVector operator+(float l, const StdMeshVector &r);
	friend StdMeshVector operator-(float l, const StdMeshVector &r);
	friend StdMeshVector operator*(float l, const StdMeshVector &r);
	friend StdMeshVector operator/(float l, const StdMeshVector &r);

	friend bool operator==(const StdMeshVector &l, const StdMeshVector &r);
	friend bool operator!=(const StdMeshVector &l, const StdMeshVector &r);

	StdMeshVector abs() const { return _mm_max_ps(v, -*this); }
	StdMeshVector normalized() const
	{
		static const __m128 wmask = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
		__m128 t = _mm_and_ps(v, wmask);
		t = ::math::detail::haddps_full(_mm_mul_ps(t, t));
		// don't use RSQRTSS, it's imprecise
		t = _mm_div_ss(_mm_set_ss(1.0f), _mm_sqrt_ss(t));
		t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 0, 0, 0));
		return _mm_mul_ps(v, t);
	}

	// Static constructors
	static StdMeshVector Zero() { return _mm_setzero_ps(); }
	static StdMeshVector UnitScale() { return _mm_set_ps(0, 1.0f, 1.0f, 1.0f); }
	static StdMeshVector Translate(float dx, float dy, float dz) { return StdMeshVector(dx, dy, dz); }
	static StdMeshVector Cross(const StdMeshVector& lhs, const StdMeshVector& rhs)
	{
		// f0 = lw*rw ; lz*rx ; ly*rz ; lx*ry
		__m128 f0 = _mm_mul_ps(lhs, _mm_shuffle_ps(rhs, rhs, _MM_SHUFFLE(3, 0, 2, 1)));
		// f1 = rw*lw ; rz*lx ; ry*lz ; rx*ly
		__m128 f1 = _mm_mul_ps(rhs, _mm_shuffle_ps(lhs, lhs, _MM_SHUFFLE(3, 0, 2, 1)));
		// f2 = lw*rw-rw*lw ; lz*rx-rz*lx ; ly*rz-ry*lz ; lx*ry-rx*ly
		__m128 f2 = _mm_sub_ps(f0, f1);
		return _mm_shuffle_ps(f2, f2, _MM_SHUFFLE(3, 0, 2, 1));
	}

private:
	StdMeshVector() {}
	StdMeshVector(__m128 v) : v(v) {}
	operator __m128() const { return v; }

	friend struct StdMeshTransformation;
};

inline StdMeshVector operator+(const StdMeshVector &l, const StdMeshVector &r) { return _mm_add_ps(l, r); }
inline StdMeshVector operator-(const StdMeshVector &l, const StdMeshVector &r) { return _mm_sub_ps(l, r); }
inline StdMeshVector operator*(const StdMeshVector &l, const StdMeshVector &r) { return _mm_mul_ps(l, r); }
inline StdMeshVector operator/(const StdMeshVector &l, const StdMeshVector &r) { return _mm_div_ps(l, r); }

inline StdMeshVector operator+(const StdMeshVector &l, float r) { return _mm_add_ps(l, _mm_set_ps1(r)); }
inline StdMeshVector operator-(const StdMeshVector &l, float r) { return _mm_sub_ps(l, _mm_set_ps1(r)); }
inline StdMeshVector operator*(const StdMeshVector &l, float r) { return _mm_mul_ps(l, _mm_set_ps1(r)); }
inline StdMeshVector operator/(const StdMeshVector &l, float r) { return _mm_div_ps(l, _mm_set_ps1(r)); }

inline StdMeshVector operator+(float l, const StdMeshVector &r) { return _mm_add_ps(_mm_set_ps1(l), r); }
inline StdMeshVector operator-(float l, const StdMeshVector &r) { return _mm_sub_ps(_mm_set_ps1(l), r); }
inline StdMeshVector operator*(float l, const StdMeshVector &r) { return _mm_mul_ps(_mm_set_ps1(l), r); }
inline StdMeshVector operator/(float l, const StdMeshVector &r) { return _mm_div_ps(_mm_set_ps1(l), r); }

// MOVMSKPS & 7 clears the result of the w component comparison
inline bool operator==(const StdMeshVector &l, const StdMeshVector &r) { return (_mm_movemask_ps(_mm_cmpneq_ps(l, r)) & 7) == 0; }
inline bool operator!=(const StdMeshVector &l, const StdMeshVector &r) { return (_mm_movemask_ps(_mm_cmpeq_ps(l, r)) & 7) == 0; }

struct StdMeshVertex
{
	static const size_t MaxBoneWeightCount = 8;

	// Match GL_T2F_N3F_V3F
	float u, v;
	float nx, ny, nz;
	float x, y, z;

	float bone_weight[MaxBoneWeightCount];
	uint16_t bone_index[MaxBoneWeightCount];

	char _padding[16];

	StdMeshVertex() : u(0), v(0), nx(0), ny(0), nz(0), x(0), y(0), z(0) 
	{
		std::uninitialized_fill(std::begin(bone_weight), std::end(bone_weight), 0);
		std::uninitialized_fill(std::begin(bone_index), std::end(bone_index), 0);
		std::uninitialized_fill(std::begin(_padding), std::end(_padding), 0);
	}
	//void Normalize();
};
static_assert((sizeof(StdMeshVertex) & 31) == 0, "StdMeshVertex should be a multiple of 32 bytes");

struct StdMeshQuaternion
{
	float w;
	// union
	// {
	//   struct { float x, y, z; };
	//   StdMeshVector v;
	// };
	float x, y, z;

	static StdMeshQuaternion Zero();
	static StdMeshQuaternion AngleAxis(float theta, const StdMeshVector& axis);

	float LenSqr() const { return w*w+x*x+y*y+z*z; }
	void Normalize();

	static StdMeshQuaternion Nlerp(const StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs, float w);
	//static StdMeshQuaternion Slerp(const StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs, float w);
};

struct StdMeshTransformation
{
	StdMeshVector scale;
	StdMeshQuaternion rotate;
	StdMeshVector translate;

	static StdMeshTransformation Zero();
	static StdMeshTransformation Identity();
	static StdMeshTransformation Inverse(const StdMeshTransformation& transform);
	static StdMeshTransformation Translate(float dx, float dy, float dz);
	static StdMeshTransformation Scale(float sx, float sy, float sz);
	static StdMeshTransformation Rotate(float angle, float rx, float ry, float rz);

	// TODO: Might add path parameter if necessary
	static StdMeshTransformation Nlerp(const StdMeshTransformation& lhs, const StdMeshTransformation& rhs, float w);
	//static  StdMeshQuaternion Slerp(const StdMeshTransformation& lhs, const StdMeshTransformation& rhs, float w);
};

class StdMeshMatrix
{
public:
	static StdMeshMatrix Zero();
	static StdMeshMatrix Identity();
	static StdMeshMatrix Inverse(const StdMeshMatrix& mat);
	static StdMeshMatrix Translate(float dx, float dy, float dz);
	static StdMeshMatrix Scale(float sx, float sy, float sz);
	static StdMeshMatrix Rotate(float angle, float rx, float ry, float rz);
	static StdMeshMatrix Transform(const StdMeshTransformation& transform);
	static StdMeshMatrix TransformInverse(const StdMeshTransformation& transform);

	float& operator()(int i, int j) { return a[i][j]; }
	float operator()(int i, int j) const { return a[i][j]; }

	float Determinant() const;
	StdMeshTransformation Decompose() const;

private:
	// 3x3 orthogonal + translation in last column
	float a[3][4];
};

StdMeshMatrix operator*(const StdMeshMatrix& lhs, const StdMeshMatrix& rhs);
StdMeshMatrix operator*(float lhs, const StdMeshMatrix& rhs);
StdMeshMatrix operator*(const StdMeshMatrix& lhs, float rhs);
StdMeshMatrix& operator*=(StdMeshMatrix& lhs, const StdMeshMatrix& rhs);
StdMeshMatrix operator+(const StdMeshMatrix& lhs, const StdMeshMatrix& rhs);
StdMeshQuaternion operator-(const StdMeshQuaternion& rhs);
StdMeshQuaternion operator*(const StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs);
StdMeshQuaternion& operator*=(StdMeshQuaternion& lhs, float rhs);
StdMeshQuaternion operator*(const StdMeshQuaternion& lhs, float rhs);
StdMeshQuaternion operator*(float lhs, const StdMeshQuaternion& rhs);
StdMeshQuaternion& operator+=(StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs);
StdMeshQuaternion operator+(const StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs);
StdMeshQuaternion operator-(const StdMeshQuaternion& lhs, const StdMeshQuaternion& rhs);
StdMeshTransformation operator*(const StdMeshTransformation& lhs, const StdMeshTransformation& rhs);

StdMeshVector operator*(const StdMeshMatrix& lhs, const StdMeshVector& rhs); // does not apply translation part
StdMeshVector operator*(const StdMeshQuaternion& lhs, const StdMeshVector& rhs);

StdMeshVertex& operator+=(StdMeshVertex& lhs, const StdMeshVertex& rhs);
StdMeshVertex operator+(const StdMeshVertex& lhs, const StdMeshVertex& rhs);
StdMeshVertex operator*(float lhs, const StdMeshVertex& rhs);
StdMeshVertex operator*(const StdMeshVertex& lhs, float rhs);
StdMeshVertex operator*(const StdMeshMatrix& lhs, const StdMeshVertex& rhs);

#endif
