// ----------------------------------------------------------------------------
// itubeRender.h: load geometry in the format of "tgdata" and then wrap each
//					each streamline with rings resembling tubes. 
//
//					Note that the tubes produced are not of any anatomical
//					meanings but simply a geometrical artifacts. So they are not
//					genuine streamtubes
//
// Creation : Feb. 6th 2011
//
// Revision:
//	@Feb. 21th 
//		1.Geometry-based direction color encoding
//		2.add interaction option for using vertex coordinate as vertex normal 
//	@Feb. 24th
//		1. correct the problem of negative colors in the orientation color
//		encoding. By take fabs over the negative ones, this coloring becomes
//		a pure orientation color encoding scheme
//	@Feb. 26th
//		1. add a continuous and anatomically symmetric coloring encoding by
//			scaling the gamut of [-1,1] to [0,2] and then to [0,1]
//		2. add multiple coloring scheme support for coloring comparative
//		analysis
//	@Feb. 27th
//		1. extract coloring scheme stuff into an independent component as a
//			template class CColorMapper in colorscheme.h
//	@Feb. 28th
//		1. add an anatomical coordinate system, together with the spheric color
//		map gadget
//	@Mar. 2nd
//		1. add the DWI image embedding, held by one more gadget : CDcmExplorer
//		2. in order to ease implementation of mituberRender, this single model
//		render, originally held in a single cpp, is mutated into an ordinary
//		class design and then instantiated in a boosting main-like cpp. Here
//		thus comes this header file
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#ifndef  _ITUBERENDER_H_

#include "DCMviewer.h"
#include "colorscheme.h"
#include "GLgadget.h"
#include "GLoader.h"
#include "GLiboxmoth.h"
#include "glrand.h"

using std::cout;
using std::cerr;
using std::vector;
using std::string;

class CTubeRender: public CGLIBoxApp {
public:
	CTubeRender(int argc, char **argv);

	void genColors();
	void findMaxMinSegLen();

	/*
	 * MAKING artificial tubes by wrapping rings around a single streamline
	 * given by the index of the streamline storage, as is a linear structure
	 * created by loading the input geometry of streamlines
	 */
	void buildTubefromLine(unsigned long lineIdx);

	/*
	 * serialize the streamtube geometry generated into a file in the format
	 * of WaveFront obj
	 */
	int serializeTubes(const string& fnobj);

	/* 
	 * read a file in the format of "tgdata", a .data ouput from tubegen into a
	 * series of linear array as vectors, each holding a streamline including
	 * its vertices and colors, then all these vectors are indexed in a headlist
	 * in which elements are pointers to these vectors respectively
	 *
	 * After the streamline geometry loaded, streamtube geometry was calculated
	 * by wrapping around each streamline a slew of rings that is expected to
	 * simulate a shape of tube only in the geometrical sense, not pertinent to
	 * visually encoding tensor attributes like the eigenvectors and eigenvalue
	 * in each voxel in the tensor field
	 */
	int loadGeometry();

	bool isTubeInSelbox(unsigned long lineIdx);

	void glInit(void);

	int handleOptions(int optv);

	void keyResponse(unsigned char key, int x, int y);

	int mainstay();

	void draw();

private:
	GLfloat m_colors[3];
	GLboolean m_bUseOrgColor;
	GLboolean m_bUseDirectionColor;
	GLboolean m_bVnormal;
	CTgdataLoader m_loader;

	string m_strfnsrc;
	string m_strfnobj;
	string m_strdwidir;

	// Level of Detail, the granularity of interpolation
	GLubyte				m_lod;
	// number of selection box
	GLubyte				m_nselbox;

	// if use variant radius for tubes
	GLboolean			m_bVradius;
	GLboolean			m_bRemovalbased;

	// fantastic factor tunning the streamtube generation
	GLfloat m_fAdd;

	// tube radius and radius of the bounding box of the streamline model
	GLfloat m_fRadius, m_fbdRadius;

	// the maximal and minimal length over all streamline segments
	GLfloat m_fMaxSegLen, m_fMinSegLen;

	// stash of streamtube geometry for all streamlines 
	vector< vector<GLfloat> >		m_alltubevertices;
	vector< vector<GLfloat>	>		m_alltubenormals;
	vector< vector<GLfloat>	>		m_alltubecolors;
	vector< vector<GLfloat>	>		m_encodedcolors;
	vector< vector<GLuint> >		m_alltubefaceIdxs;
	//
	// get the maximal X,Y and Z coordinate among all vertices
	GLdouble m_maxCoord[3];	
	// get the minimal X,Y and Z coordinate among all vertices
	GLdouble m_minCoord[3];	

	// color mapper responsible for color encoding
	CColorMapper<GLfloat> m_colormapper;
	// currently we might want to compare multiple coloring encoding scheme
	int m_colorschemeIdx;
	// a spherical color map gadget
	CGLGadget* m_pcmGadget;
	// anatomical axes gadget
	CGLGadget* m_paxesGagdet;

	// A DICOM DWI IMAGE explorer
	CDcmExplorer m_dcmexplorer;

	bool m_bShowDWIImage;
};

#endif // _ITUBERENDER_H_

/* set ts=4 sts=4 tw=80 sw=4 */

