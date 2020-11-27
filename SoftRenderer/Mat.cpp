#include "Mat.h"
#include "Vec.h"
#include "Quat.h"

namespace SoftRenderer
{
	Mat4::Mat4(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33)
	{
		d[0][0] = a00; d[0][1] = a01; d[0][2] = a02; d[0][3] = a03;
		d[1][0] = a10; d[1][1] = a11; d[1][2] = a12; d[1][3] = a13;
		d[2][0] = a20; d[2][1] = a21; d[2][2] = a22; d[2][3] = a23;
		d[3][0] = a30; d[3][1] = a31; d[3][2] = a32; d[3][3] = a33;
	}

	Mat4 Mat4::transpose()
	{
		Mat4 res;
		res.d[0][0] = d[0][0];
		res.d[0][1] = d[1][0];
		res.d[0][2] = d[2][0];
		res.d[0][3] = d[3][0];
		res.d[1][0] = d[0][1];
		res.d[1][1] = d[1][1];
		res.d[1][2] = d[2][1];
		res.d[1][3] = d[3][1];
		res.d[2][0] = d[0][2];
		res.d[2][1] = d[1][2];
		res.d[2][2] = d[2][2];
		res.d[2][3] = d[3][2];
		res.d[3][0] = d[0][3];
		res.d[3][1] = d[1][3];
		res.d[3][2] = d[2][3];
		res.d[3][3] = d[3][3];
		return res;
	}

	Mat4 Mat4::inverse()
	{
		Mat4 res;
		res.d[0][0] = d[1][2] * d[2][3] * d[3][1] - d[1][3] * d[2][2] * d[3][1] + d[1][3] * d[2][1] * d[3][2] - d[1][1] * d[2][3] * d[3][2] - d[1][2] * d[2][1] * d[3][3] + d[1][1] * d[2][2] * d[3][3];
		res.d[0][1] = d[0][3] * d[2][2] * d[3][1] - d[0][2] * d[2][3] * d[3][1] - d[0][3] * d[2][1] * d[3][2] + d[0][1] * d[2][3] * d[3][2] + d[0][2] * d[2][1] * d[3][3] - d[0][1] * d[2][2] * d[3][3];
		res.d[0][2] = d[0][2] * d[1][3] * d[3][1] - d[0][3] * d[1][2] * d[3][1] + d[0][3] * d[1][1] * d[3][2] - d[0][1] * d[1][3] * d[3][2] - d[0][2] * d[1][1] * d[3][3] + d[0][1] * d[1][2] * d[3][3];
		res.d[0][3] = d[0][3] * d[1][2] * d[2][1] - d[0][2] * d[1][3] * d[2][1] - d[0][3] * d[1][1] * d[2][2] + d[0][1] * d[1][3] * d[2][2] + d[0][2] * d[1][1] * d[2][3] - d[0][1] * d[1][2] * d[2][3];
		res.d[1][0] = d[1][3] * d[2][2] * d[3][0] - d[1][2] * d[2][3] * d[3][0] - d[1][3] * d[2][0] * d[3][2] + d[1][0] * d[2][3] * d[3][2] + d[1][2] * d[2][0] * d[3][3] - d[1][0] * d[2][2] * d[3][3];
		res.d[1][1] = d[0][2] * d[2][3] * d[3][0] - d[0][3] * d[2][2] * d[3][0] + d[0][3] * d[2][0] * d[3][2] - d[0][0] * d[2][3] * d[3][2] - d[0][2] * d[2][0] * d[3][3] + d[0][0] * d[2][2] * d[3][3];
		res.d[1][2] = d[0][3] * d[1][2] * d[3][0] - d[0][2] * d[1][3] * d[3][0] - d[0][3] * d[1][0] * d[3][2] + d[0][0] * d[1][3] * d[3][2] + d[0][2] * d[1][0] * d[3][3] - d[0][0] * d[1][2] * d[3][3];
		res.d[1][3] = d[0][2] * d[1][3] * d[2][0] - d[0][3] * d[1][2] * d[2][0] + d[0][3] * d[1][0] * d[2][2] - d[0][0] * d[1][3] * d[2][2] - d[0][2] * d[1][0] * d[2][3] + d[0][0] * d[1][2] * d[2][3];
		res.d[2][0] = d[1][1] * d[2][3] * d[3][0] - d[1][3] * d[2][1] * d[3][0] + d[1][3] * d[2][0] * d[3][1] - d[1][0] * d[2][3] * d[3][1] - d[1][1] * d[2][0] * d[3][3] + d[1][0] * d[2][1] * d[3][3];
		res.d[2][1] = d[0][3] * d[2][1] * d[3][0] - d[0][1] * d[2][3] * d[3][0] - d[0][3] * d[2][0] * d[3][1] + d[0][0] * d[2][3] * d[3][1] + d[0][1] * d[2][0] * d[3][3] - d[0][0] * d[2][1] * d[3][3];
		res.d[2][2] = d[0][1] * d[1][3] * d[3][0] - d[0][3] * d[1][1] * d[3][0] + d[0][3] * d[1][0] * d[3][1] - d[0][0] * d[1][3] * d[3][1] - d[0][1] * d[1][0] * d[3][3] + d[0][0] * d[1][1] * d[3][3];
		res.d[2][3] = d[0][3] * d[1][1] * d[2][0] - d[0][1] * d[1][3] * d[2][0] - d[0][3] * d[1][0] * d[2][1] + d[0][0] * d[1][3] * d[2][1] + d[0][1] * d[1][0] * d[2][3] - d[0][0] * d[1][1] * d[2][3];
		res.d[3][0] = d[1][2] * d[2][1] * d[3][0] - d[1][1] * d[2][2] * d[3][0] - d[1][2] * d[2][0] * d[3][1] + d[1][0] * d[2][2] * d[3][1] + d[1][1] * d[2][0] * d[3][2] - d[1][0] * d[2][1] * d[3][2];
		res.d[3][1] = d[0][1] * d[2][2] * d[3][0] - d[0][2] * d[2][1] * d[3][0] + d[0][2] * d[2][0] * d[3][1] - d[0][0] * d[2][2] * d[3][1] - d[0][1] * d[2][0] * d[3][2] + d[0][0] * d[2][1] * d[3][2];
		res.d[3][2] = d[0][2] * d[1][1] * d[3][0] - d[0][1] * d[1][2] * d[3][0] - d[0][2] * d[1][0] * d[3][1] + d[0][0] * d[1][2] * d[3][1] + d[0][1] * d[1][0] * d[3][2] - d[0][0] * d[1][1] * d[3][2];
		res.d[3][3] = d[0][1] * d[1][2] * d[2][0] - d[0][2] * d[1][1] * d[2][0] + d[0][2] * d[1][0] * d[2][1] - d[0][0] * d[1][2] * d[2][1] - d[0][1] * d[1][0] * d[2][2] + d[0][0] * d[1][1] * d[2][2];

		float determinant =
			res.d[0][3] * res.d[1][2] * res.d[2][1] * res.d[3][0] - res.d[0][2] * res.d[1][3] * res.d[2][1] * res.d[3][0] - res.d[0][3] * res.d[1][1] * res.d[2][2] * res.d[3][0] + res.d[0][1] * res.d[1][3] * res.d[2][2] * res.d[3][0] +
			res.d[0][2] * res.d[1][1] * res.d[2][3] * res.d[3][0] - res.d[0][1] * res.d[1][2] * res.d[2][3] * res.d[3][0] - res.d[0][3] * res.d[1][2] * res.d[2][0] * res.d[3][1] + res.d[0][2] * res.d[1][3] * res.d[2][0] * res.d[3][1] +
			res.d[0][3] * res.d[1][0] * res.d[2][2] * res.d[3][1] - res.d[0][0] * res.d[1][3] * res.d[2][2] * res.d[3][1] - res.d[0][2] * res.d[1][0] * res.d[2][3] * res.d[3][1] + res.d[0][0] * res.d[1][2] * res.d[2][3] * res.d[3][1] +
			res.d[0][3] * res.d[1][1] * res.d[2][0] * res.d[3][2] - res.d[0][1] * res.d[1][3] * res.d[2][0] * res.d[3][2] - res.d[0][3] * res.d[1][0] * res.d[2][1] * res.d[3][2] + res.d[0][0] * res.d[1][3] * res.d[2][1] * res.d[3][2] +
			res.d[0][1] * res.d[1][0] * res.d[2][3] * res.d[3][2] - res.d[0][0] * res.d[1][1] * res.d[2][3] * res.d[3][2] - res.d[0][2] * res.d[1][1] * res.d[2][0] * res.d[3][3] + res.d[0][1] * res.d[1][2] * res.d[2][0] * res.d[3][3] +
			res.d[0][2] * res.d[1][0] * res.d[2][1] * res.d[3][3] - res.d[0][0] * res.d[1][2] * res.d[2][1] * res.d[3][3] - res.d[0][1] * res.d[1][0] * res.d[2][2] * res.d[3][3] + res.d[0][0] * res.d[1][1] * res.d[2][2] * res.d[3][3];

		float r = 1.0f / determinant;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.d[i][j] *= r;
			}
		}
		return res;
	}

	Mat4 Mat4::translate(Vec3 v)
	{
		return Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			v.x, v.y, v.z, 1);
	}

	Mat4 Mat4::rotate(Quat q)
	{
		// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/jay.htm
		// 这里找到的碉堡做法，暂未验证
		// let m1 = Mat4(
		//     q.w, q.z, -q.y, q.x,
		//     -q.z, q.w, q.x, q.y,
		//     q.y, -q.x, q.w, q.z,
		//     -q.x, -q.y, -q.z, q.w
		// );
		// let m2 = Mat4(
		//     q.w, q.z, -q.y, -q.x,
		//     -q.z, q.w, q.x, -q.y,
		//     q.y, -q.x, q.w, -q.z,
		//     q.x, q.y, q.z, q.w
		// );
		// return m1 * m2;

		Mat4 res;
		float xx = q.x * q.x * 2;
		float xy = q.x * q.y * 2;
		float xz = q.x * q.z * 2;
		float yy = q.y * q.y * 2;
		float yz = q.y * q.z * 2;
		float zz = q.z * q.z * 2;
		float wx = q.w * q.x * 2;
		float wy = q.w * q.y * 2;
		float wz = q.w * q.z * 2;
		res.d[0][0] = 1 - (yy + zz);
		res.d[0][1] = xy + wz;
		res.d[0][2] = xz - wy;
		res.d[0][3] = 0;
		res.d[1][0] = xy - wz;
		res.d[1][1] = 1 - (xx + zz);
		res.d[1][2] = yz + wx;
		res.d[1][3] = 0;
		res.d[2][0] = xz + wy;
		res.d[2][1] = yz - wx;
		res.d[2][2] = 1 - (xx + yy);
		res.d[2][3] = 0;
		res.d[3][0] = 0;
		res.d[3][1] = 0;
		res.d[3][2] = 0;
		res.d[3][3] = 1;
		return res;
	}

	Mat4 Mat4::scale(Vec3 v)
	{
		return Mat4(
			v.x, 0, 0, 0,
			0, v.y, 0, 0,
			0, 0, v.z, 0,
			0, 0, 0, 1);
	}

	Mat4 Mat4::lookAt(Vec3 eye, Vec3 center, Vec3 up)
	{
		Vec3 zAxis = (center - eye).normalize();
		Vec3 xAxis = up.cross(zAxis).normalize();
		Vec3 yAxis = zAxis.cross(xAxis).normalize();
		float tx = -xAxis.dot(eye);
		float ty = -yAxis.dot(eye);
		float tz = -zAxis.dot(eye);
		return Mat4(
			xAxis.x, yAxis.x, zAxis.x, 0,
			xAxis.y, yAxis.y, zAxis.y, 0,
			xAxis.z, yAxis.z, zAxis.z, 0,
			tx, ty, tz, 1);
	}

	Mat4 Mat4::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		float t = 1.0f / std::tan(fovy * 0.5f);
		Mat4 res;
		res.d[0][0] = t / aspect;
		res.d[1][1] = t;
		res.d[2][3] = 1.0f;
		res.d[3][3] = 0.0f;
		// [-1, 1]
		// res.d[2][2] = (zFar + zNear) / (zFar - zNear);
		// res.d[3][2] = (-2.0 * zFar * zNear) / (zFar - zNear);
		// [0, 1]
		res.d[2][2] = zFar / (zFar - zNear);
		res.d[3][2] = -(zFar * zNear) / (zFar - zNear);
		return res;
	}

	Mat4 operator*(Mat4 left, Mat4 right)
	{
		Mat4 res;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.d[i][j] =
					left.d[i][0] * right.d[0][j] +
					left.d[i][1] * right.d[1][j] +
					left.d[i][2] * right.d[2][j] +
					left.d[i][3] * right.d[3][j];
			}
		}
		return res;
	}
}
