#ifndef _MYODEMATH_H
#define _MYODEMATH_H

#include <iostream>
#include <cmath>
#include <ode/ode.h>

#define DETERMINANT_THRESHOLD 0.0005

// Reimplementations of certain useful internal ODE functions

void myDVector3Normalize(dReal* v) {

	dReal length = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] = v[0]/length; v[1] = v[1]/length; v[2] = v[2]/length;
}

void myDVector3Subtract(const dReal* v1, const dReal* v2, dReal* dest) {

	dest[0] = v1[0] - v2[0]; dest[1] = v1[1] - v2[1]; dest[2] = v1[2] - v2[2];
}

void myDMatrix3LastColumnToZeros(dReal* m) {

	m[3] = 0.; m[7] = 0.; m[11] = .0;
}

void myDMultiplyMat3Vec3(const dReal* m, const dVector3& v, dVector3& r) {

	r[0] = m[0]*v[0] + m[1]*v[1] + m[2]*v[2];
	r[1] = m[4]*v[0] + m[5]*v[1] + m[6]*v[2];
	r[2] = m[8]*v[0] + m[9]*v[1] + m[10]*v[2];
	r[3] = 0.;
}

dReal myDMatrix3Det(const dReal* mat) {

	dReal det;
	det = mat[0] * ( mat[5]*mat[10] - mat[9]*mat[6] )
	    - mat[1] * ( mat[4]*mat[10] - mat[8]*mat[6] )
	    + mat[2] * ( mat[4]*mat[9]  - mat[8]*mat[5] );

	return det;
}

void myDMatrix3Inv(const dReal* ma, dReal* dst) {

	dReal det = myDMatrix3Det( ma );

	if ( fabs(det) < DETERMINANT_THRESHOLD ) {

		dst[0]  = 1.; dst[1]  = 0.; dst[2]  = 0.; dst[3]  = 0.;
		dst[4]  = 0.; dst[5]  = 1.; dst[6]  = 0.; dst[7]  = 0.;
		dst[8]  = 0.; dst[9]  = 0.; dst[10] = 1.; dst[11] = 0.;

		return;
	}

	dst[0]  =    ma[5]*ma[10] - ma[6]*ma[9]   / det;
	dst[1]  = -( ma[1]*ma[10] - ma[9]*ma[2] ) / det;
	dst[2]  =    ma[1]*ma[6]  - ma[5]*ma[2]   / det;

	dst[4]  = -( ma[4]*ma[10] - ma[6]*ma[8] ) / det;
	dst[5]  =    ma[0]*ma[10] - ma[8]*ma[2]   / det;
	dst[6]  = -( ma[0]*ma[6] - ma[4]*ma[2] )  / det;

	dst[8]  =    ma[4]*ma[9] - ma[8]*ma[5]   / det;
	dst[9]  = -( ma[0]*ma[9] - ma[8]*ma[1] ) / det;
	dst[10] =    ma[0]*ma[5] - ma[1]*ma[4]   / det;

	myDMatrix3LastColumnToZeros(dst);
}

void myDMatrix3Transpose(dReal* m) {

	/// Transposes the matrix in place
	dReal t[3]; // t is for temporary
	t[0] = m[4]; t[1] = m[8]; t[2] = m[9];
	m[4] = m[1]; m[8] = m[2]; m[9] = m[6];
	m[1] = t[0]; m[2] = t[1]; m[6] = t[2];

	myDMatrix3LastColumnToZeros(m);
}

void myDMatrix3Subtract(const dReal* m1, const dReal* m2, dReal* dst) {

	for(unsigned i=0; i<12; i++)
		dst[i] = m1[i] - m2[i];
}

void myDMatrix3Copy(const dReal* src, dReal* dst) {

	for(unsigned i=0; i<12; i++)
		dst[i] = src[i];
}

// IO for ODE types

void myPrintDMatrix3(std::ostream& stream, const dReal* m) {

	stream << m[0] << ' ' << m[1] << ' ' << m[2] << ' ' << m[3] << '\n'
	       << m[4] << ' ' << m[5] << ' ' << m[6] << ' ' << m[7] << '\n'
	       << m[8] << ' ' << m[9] << ' ' << m[10]<< ' ' << m[11]<< '\n';
}

void myPrintDVector3(std::ostream& stream, const dVector3& v) {

	stream << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3];
}

#endif // _MYODEMATH_H
