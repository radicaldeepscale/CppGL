// ----------------------------------------------------------------------------
// GLgadget.h : a class dedicated exclusively to describe a gadget that is
//				expected to take part in an openGL driven visualization
//
//				the gadget as instance of this class is by no means meant to be
//				working independently but as a plugin for the GLcppmoth instance
//				or that of its descendants
//
//				note that gadget is not widget, it is particularly used in the
//				cases like you want a:
//				.orientation indicator when the rendering is in 3D space
//				.runtime prompt for interaction to be placed as a static textbox
//				.a button for navigation
//				...
//
// Creation : Feb. 25th 2011
//
// Revisions:
//	@Feb. 26th
//		.CGLAxis: add arrow beside axis to make it more verisimilar to the 
//		real one, simple text lable also added
//	@Feb. 27th
//		.Add CSphereColorMap, a class drawing a color map on a spheric surface
//	@Feb. 28th
//		.Add an anatomay axis class CAnatomyAxis which has six axes pointing to
//		all the six anatomical orientations respectively
//		.Tune the color map sphere, solving the "crack on XZ plane" problem,
//		 either patching up the points needed meant for padding the crack, or
//		 simply changing the primitive mode from GL_POINTS to GL_LINES. (by
//		 moving to GL_LINES, simply hand over the interpolation to GL). The
//		 latter approach can save lots of memory cost
//		.Tweak the gadget viewport to well adapt to different primary objects,
//		 while initialized as -1, the width will be set to 1/5 of the global
//		 viewport width and height adjusted as such the aspect ratio equal to
//		 the global one accordingly. Otherwise, use the custome ones and keep
//		 static even if the window will get resized.
//	@Mar. 1st
//		.fix the bug that the translation switch does work when it is on, which
//		is not tested before, discovered when gonna extend another subtype
//		CGLGadget : DWIViewer ( the matrix in openGL is always column-first
//		stored !! )
//	@Mar. 2nd
//		.add viewport ratio to ease further the setting of gadget viewport
//		.add fvoy customization considering the need of specific gadget
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#ifndef _GLGADGET_H_
#define _GLGADGET_H_

#include <stdint.h>

#include <iostream>

#include <GL/glut.h>
#include "glrand.h"
#include "point.h"

/*
 * this class describes what is needed for parameterizing a gadget and the
 * most essential operation used
 */
class CGLGadget {
public:
	CGLGadget();
	virtual ~CGLGadget();

	/*
	 * @brief tell parameters pertaining to gadget, simply update the 
	 *		parameters
	 * @param enabled a boolean giving if enable gadget drawing or not
	 * @param left,bottom (left,bottom) as the lower left corner of the mini
	 * viewport for the gadget
	 * @param width,height tell the size of the viewport holding the gadget, -1
	 *		says the value was not yet, will be replaced with certain defaults
	 * @param bTranslate boolean giving the gadget should be rotated along with
	 * the main object
	 * @param bRotate boolean giving the gadget should be translated along
	 * with the main object
	 * @return none
	 * @see draw
	 */
	void setParams(bool enabled=true, int left=0, int bottom=0, 
				int width=-1, int height=-1,
				bool bTranslate = false, bool bRotate=true);

	void setVertexCoordRange(GLdouble dx=1, GLdouble dy=1, GLdouble dz=1);

	/*
	 * @brief the function implementing the gadget drawing, left to be
	 * overloaded by subtype classes if special gadget needed, the default
	 * provided in this parent level of class is empty
	 * @return none
	 * @see setParams,display
	 */
	virtual void draw();

	/*
	 * @brief a binary switch flipping between on and off
	 * @return current state of the switch, true for being on and false for off
	 */
	GLboolean	switchTranslucent();

	/*
	 * @brief the trigger of gadget drawing for the caller, it will invoke
	 * draw() inside and is designed to save the repetitive code serving stuff
	 * like keeping matrices and set independent view and modelviewing
	 * parameters and then, after draw(), restoring all of thses.
	 * for flexibility, however, this is modified AS A VIRTUAL member so as to
	 * enable the overloading if necessary
	 * @return none
	 * @see draw
	 */
	virtual void display();

	/*
	 * @brief set a new viewport ratio
	 * - a viewport ratio only takes effect when the width and height of
	 *   viewport was left unset
	 * @param ratio an integer telling the new ratio of the size of the gadget 
	 *   viewport to the primary object viewport
	 * @return none
	 */
	void setViewportRatio(int ratio);

	/*
	 * @brief set a new fvoy for the gadget, instead the default one
	 * @param fvoy a floating point number telling the new fvoy for the
	 *	projectioon transformation
	 * @return none
	 */
	void setfvoy(GLfloat fvoy);

protected:
	// basically the switch
	bool	m_bEnabled;
	// the lower left corner of the viewport for the gadget
	int		m_left;
	int		m_bottom;
	// the width and height of viewport 
	int		m_width;
	int		m_height;
	bool	m_bUseStaticWidth;
	bool	m_bUseStaticHeight;

	// the symmetric range of cartesian coordinates of fitting vertices
	// , which should be in x:(-m_dx, m_dx), y:(-m_dy, m_dy), z:(-m_dz, m_dz)
	GLdouble m_dx, m_dy, m_dz;

	// if applied with the global translation from the parent modelview
	// transformation
	bool	m_bTranslate;
	// if applied with the global rotation from the parent modelview
	// transformation
	bool	m_bRotate;

	// if the gadget will be translucent
	GLboolean	m_bTranslucent;

	// sometimes we just want a down scaled viewport rathen than be able to know
	// exactly the size of the viewport desired. By setting view ratio, when the
	// width and height of the viewport left unset, a 1/ratio size of the
	// primary object viewport would be presumed
	int		m_nViewRatio;

	// specific gadget might need specific fvoy to be looked better
	int		m_fvoy;
};

/*
 * this class gives an axis gadget as an orientation indicator
 */
class CGLAxis : public CGLGadget {
public:
	CGLAxis();
	~CGLAxis();

	/*
	 * @brief set color to be used for drawing axis
	 * @param idx index of axis to set color for, 0 for X, 1 for Y and 2 for Z
	 *		  the special -1 is for setting them all
	 * @param r,g,b,a the three color channels and transparency alpha
	 * @return none
	 */
	void setColor(int idx, GLclampf r, GLclampf g, GLclampf b, GLclampf a = .6f);

	void draw();

protected:
	// colors for the axes
	GLfloat		m_colors[3][4];

	// draw an arrow at the endpoint of a vector
	void _drawArrow(const point_t& p1, const point_t& p2, GLdouble sz=1.0f);
};

/*
 * this class extends CGLAxis to implement an anatomical axis orientation
 * indicator which has six axes pointing to six anatomical orientations
 */
class CAnatomyAxis : public CGLAxis {
public:
	CAnatomyAxis();
	~CAnatomyAxis();

	void draw();

	// the axes are colored such as to reflect different color map
	void setColorScheme(int nScheme);

private:
	// pinpoint a coloring scheme
	int			m_colorschemeIdx;
};

/*
 * this class gives a color mapping scheme indicator on a sphere
 */
class CSphereColorMap : public CGLGadget {
public:
	// -1 indicates that the value is not yet set and will use the value
	// calculated according to a unit division of the sphere, both 
	// longitudinally and latitudinally
	CSphereColorMap(GLdouble radius=-1.0, GLint slices=-1, GLint stacks=-1);
	~CSphereColorMap();

	void setColorScheme(int nScheme);

	void updateSphereParams(GLdouble radius, GLint slices, GLint stacks);

	void draw();

private:
	/* interleaved array holding vertices and colors for the sphere in a format
	 * of :
	 * r1 g1 b1 x1 y1 z1 
	 * r2 g2 b2 x2 y2 z2 
	 * ...
	 * rn gn bn xn yn zn 
	 */
	GLfloat*	m_pData;
	// size of the linear list storage
	GLsizei		m_sz;

	// sphere parameterization
	GLdouble	m_radius;
	GLint		m_slices; // number of divisions around the Z (along the X) axis as
							// sphere longitude
	GLint		m_stacks; // number of divisions along the Z axis as the sphere latitude

	// pinpoint a coloring scheme
	int			m_colorschemeIdx;

	// compute vertices and, colors according to the color scheme indicated
	int	_calcShpereGeometry();

	// a subroutine just used for _calcShpereGeometry internally
	int _place_a_vertex(int vertexIndex, GLfloat x, GLfloat y, GLfloat z);
};

#endif // _GLGADGET_H_

/* set ts=4 sts=4 tw=80 sw=4 */

