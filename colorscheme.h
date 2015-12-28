// ----------------------------------------------------------------------------
// colorscheme.h : class declaration and definition of coloring schemes
// supported as yet
//
// this is the template class definition describing implementations of
// different coloring scheme, it is to describe, in actuality, a set of mappings
// that convert a tuple symbolizing a geometric point to another tuple that will
// tell the color corresponding to that point
//
// Creation : Feb. 27th 2011
// revision : 
//	@Feb. 28th 
//		1.the CLSCHM_ANATOMY_SYMMETRIC coloring scheme is improved to discern the
//		color encoded on the Y axis and that on the Z axis
//		2.CLSCHM_ANATOMY_SYMMETRIC mapping but in Lab color space is added as
//		another choice of coloring
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#ifndef _COLORSCHEME_H_
#define _COLORSCHEME_H_

#include "glrand.h"

#include <iostream>
#include <map>

using std::ostream;
using std::map;

#define NUM_COLOR_SCHEME 5
typedef enum _colorscheme_t {
	CLSCHM_COORDINATE_MAPPING = 0,
	CLSCHM_YZ_WATERSHED,
	CLSCHM_ALL_SYMMETRIC,
	CLSCHM_ANATOMY_SYMMETRIC,
	CLSCHM_ANATOMY_SYMMETRIC_LAB,
	CLSCHM_CUSTOM
}colorscheme_t;

const char* const g_colorschemes[] = {
	"direct coordinate mapping",
	"YZ watershed",
	"all symmetric",
	"anatomy symmetric",
	"anatomy symmetric in Lab space",
	"custom" // vacuous scheme
};

const GLfloat g_rgb2xyz_cie_mat[16] = {
	0.4887180, 0.3106803,  0.2006017, 0.0,
	0.1762044, 0.8129847,  0.0108109, 0.0,
	0.0000000, 0.0102048,  0.9897952, 0.0,
	0.0000000, 0.0000000,  0.0000000, 0.0
};

const GLfloat g_xyz2lab_epsilon = 0.008856;
const GLfloat g_xyz2lab_k = 903.3;

template <class _T = double>
class CColorMapper {
public:

public:
	CColorMapper(int nScheme = CLSCHM_CUSTOM);
	~CColorMapper();

	int setColorScheme(int nScheme);
	const char* getName() const;

	// do the mapping
	// @return 0 for success and other values for nothing done
	int getColor(const _T& x, const _T& y, const _T& z,
				_T& r, _T& g, _T& b) const;

private:
	// pinpoint a coloring scheme
	int			m_colorschemeIdx;
};

// just to be used as factory functions
inline int getNumberOfColorSchemes()
{
	return ARRAY_SIZE(g_colorschemes);
}

inline int isColorSchemeSupported(int nScheme)
{
	return nScheme >= CLSCHM_COORDINATE_MAPPING && 
		nScheme <= CLSCHM_CUSTOM;
}

inline const char* getColorSchemeName(int nScheme)
{
	if ( !isColorSchemeSupported(nScheme) ) {
		return "~~N/A~~";
	}

	return g_colorschemes[nScheme];
}

template <typename _T>
void rgb2xyz(const _T& r, const _T& g, const _T& b,
			_T& x, _T& y, _T& z)
{
	_T w = 0.0;
	x = r, y = g, z = b;
	transpoint(g_rgb2xyz_cie_mat, x, y, z, w);
}

template <typename _T>
void xyz2lab(const _T& x, const _T& y, const _T& z,
			_T& l, _T& a, _T& b)
{
	_T fx, fy, fz;
	fx = x > g_xyz2lab_epsilon ? (_T)powf( (float)x, 1.0/3.0 ) :
	   (g_xyz2lab_k*x + 16)/116.0;
	fy = y > g_xyz2lab_epsilon ? (_T)powf( (float)y, 1.0/3.0 ) :
	   (g_xyz2lab_k*y + 16)/116.0;
	fz = z > g_xyz2lab_epsilon ? (_T)powf( (float)z, 1.0/3.0 ) :
	   (g_xyz2lab_k*z + 16)/116.0;

	l = 116*fy - 16;
	a = 500 * (fx - fy);
	b = 200 * (fy - fz);
}

template <typename _T>
void rgb2lab(const _T& r, const _T& g, const _T& b,
			_T& l, _T& a, _T& b1)
{
	_T x = 0, y = 0, z = 0;
	rgb2xyz(r,g,b,x,y,z);
	xyz2lab(x,y,z,l,a,b1);
}

//////////////////////////////////////////////////////////////////////////////
//
// in-place template class implementation
//
template <class _T>
CColorMapper<_T>::CColorMapper(int nScheme) :
	m_colorschemeIdx(nScheme) 
{
}

template <class _T>
CColorMapper<_T>::~CColorMapper()
{
}

template <class _T>
int CColorMapper<_T>::setColorScheme(int nScheme)
{
	if ( !isColorSchemeSupported(nScheme) ) {
		return -1;
	}

	m_colorschemeIdx = nScheme;
	return 0;
}

template <class _T>
const char* CColorMapper<_T>::getName() const
{
	return getColorSchemeName(m_colorschemeIdx);
}

template <class _T>
int CColorMapper<_T>::getColor(const _T& x, const _T& y, const _T& z,
				_T& r, _T& g, _T& b) const
{
	// these are intuitive mapping from cartesian space to RGB color space
	// from x-axis to the r-axis, y to g and z to b.
	r = x, g = y, b = z;
	normalize(r, g, b);
	
	switch (m_colorschemeIdx) {
		case CLSCHM_COORDINATE_MAPPING:
			/* coloring scheme 0.*/
			break;
		case CLSCHM_YZ_WATERSHED:
			{
				/* coloring scheme 1.*/
				r = fabs(r);
				g = g>0?g:(1.0+g);
				b = b>0?b:(1.0+b);
			}
			break;
		case CLSCHM_ALL_SYMMETRIC:
			{
				/* coloring scheme 2.*/
				r = fabs(r);
				g = fabs(g);
				b = fabs(b);
			}
			break;
		case CLSCHM_ANATOMY_SYMMETRIC:
			{
				/* coloring scheme 3.*/
				r = fabs(r);
				g = (g+1.0)/2.0;
				//b = (b+1.0)/2.0;
				b = 1- (b+1.0)/2.0;
			}
			break;
		case CLSCHM_ANATOMY_SYMMETRIC_LAB:
			{
				/* coloring scheme 4.*/
				r = fabs(r);
				g = (g+1.0)/2.0;
				b = 1- (b+1.0)/2.0;

				_T l,a,b1;
				rgb2lab(r,g,b,l,a,b1);
				r = l, g = a, b = b1;
				normalize(r, g, b);
				r = fabs(r);
				g = (g+1.0)/2.0;
				b = 1- (b+1.0)/2.0;
			}
			break;

		case CLSCHM_CUSTOM:
		default:
			// the caller will implement a custom scheme itself
			return 1;
	}
	return 0;
}

#endif // _COLORSCHEME_H_

/* set ts=4 sts=4 tw=80 sw=4 */

