// ----------------------------------------------------------------------------
// mitubeRender.cpp : a multiple-window itubeRender capsule, loading multiple
//					DTI voxel model each assigned with a separate window using
//					multiple process run-time modality
//
// Creation : Mar. 2nd 2011
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#include "mitubeRender.h"

using std::cout;
using std::cerr;
using std::ostringstream;

///////////////////////////////////////////////////////////////////////////////
//
//	Implementation of the Class CMitubeRender
//
CMitubeRender* CMitubeRender::m_pmitInstance = NULL;

CMitubeRender::CMitubeRender(int argc, char **argv) : 
	CGLIBoxApp(argc, argv),
	m_nAliveChildren(0)	
{
	setVerinfo("mitubeRender");

	addOption('f', true, "input-file-name", "the name of source file"
			" containing geometry and in the format of tgdata");
	addOption('g', true, "output-file-name", "the name of target file"
			" to store the geometry of streamtubes produced");
	addOption('r', true, "tube-radius", "fixed radius of the tubes"
			" to generate");
	addOption('l', true, "lod", "level ot detail controlling the tube"
			" generation, it is expected to impact the smoothness of tubes");
	addOption('b', true, "box-num", "number of selection box"
			" which is 1 by default");
	addOption('d', true, "dwi-b0-dir", "directory holding DWI b0 DICOM images");
}

CMitubeRender::~CMitubeRender()
{
	killallchildren();
	for (size_t i = 0; i < m_allargvs.size(); ++i) {
		delete [] m_allargvs[i];
		m_allargvs[i] = NULL;
	}

	for (size_t i = 0; i < m_renders.size(); ++i) {
		delete m_renders[i];
		m_renders[i] = NULL;
	}
	m_pmitInstance = NULL;
}

int CMitubeRender::handleOptions(int optv) 
{
	switch( optv ) {
		case 'f':
			m_strfnsrcs.push_back(optarg);
			return 0;
		case 'd':
			m_strdwidirs.push_back( optarg );
			return 0;
		case 'b':
		case 'l':
		case 'r':
			return 0;
		default:
			return CGLIBoxApp::handleOptions( optv );
	}
	return 1;
}

void CMitubeRender::keyResponse(unsigned char key, int x, int y) 
{
	switch (key) {
		case 27:
			killallchildren();
			cleanup();
			exit (EXIT_SUCCESS);
		default:
			return CGLIBoxApp::keyResponse(key, x, y);
	}
	glutPostRedisplay();
}

int CMitubeRender::mainstay() 
{
	if ( 0 != _createProcesses() ) {
		cerr << "Failed to create multiple processes.\n";
		return -1;
	}

	/* make this process as the group leader so as to ease the signal
	 * broadcasting afterwards
	 */
	if ( 0 != setpgid(0, 0) ) {
		m_cout << "Failed to create process group as the assumed group leader.\n";
		return -1;
	}

	_setWindowLayout();

	if ( 0 != startchildren() ) {
		return -1;
	}

	m_pmitInstance = this;

	m_nAliveChildren = (int)m_renders.size();

	/*
	signal(SIGCHLD, _on_child_exit);
	return CGLIBoxApp::mainstay();
	*/
	signal(SIGUSR2, _on_all_exit);
	waitforallchildren();
	return 0;
}

int CMitubeRender::startchildren() {
	// start all processes, each for a single model render
	
	pid_t parent_pid = getpid();
	for (size_t i = 0; i < m_renders.size(); ++i) {
		m_allchildpids[i] = fork();
		if ( m_allchildpids[i] < 0 ) {
			cerr << "Failed to start process for render No." << i << "\n";
			killallchildren();
			return -1;
		}

		if ( 0 == m_allchildpids[i] ) {
			m_renders[i]->setNumberOfSiblings( (int)m_renders.size() - 1 );
			m_renders[i]->run();
			return 0;	
		}
		else {
			m_cout << "one more process started, pid = " << m_allchildpids[i] << ". \n";
			// each child process should join the same group as the 
			// parent process to be easily managed afterwards
			if (0 != setpgid( m_allchildpids[i], parent_pid) ) {
				cerr << "Failed to enlist the child process whose pid=" << 
					m_allchildpids[i] << " into process group of leader pid=" 
					<< parent_pid << "\n";
				killallchildren();
				return -1;
			}
		}
	}

	return 0;
}

int CMitubeRender::killallchildren()
{
	int ret = 0;
	for (size_t i = 0; i < m_allchildpids.size(); ++i) {
		if ( m_allchildpids[i] > 0 ) {
			kill ( m_allchildpids[i], SIGTERM );
			ret ++;
		}
	}
	return ret;
}

void CMitubeRender::waitforallchildren()
{
	pid_t died_child_pid;
	__WAIT_STATUS  state = NULL;
	while (m_nAliveChildren >= 1 ) {
		died_child_pid = wait(state);
		onChildExit( died_child_pid );
	}
}

int CMitubeRender::onChildExit(pid_t died_child_pid)
{
	m_nAliveChildren --;
	if ( m_nAliveChildren < 1 ) {
		m_cout << "All children died, parent so need not be alive.\n";
		keyResponse(27, 0, 0);
	}

	m_cout << "One more child, pid = " << died_child_pid << " died.\n";

	// notify the children still alive that they have lost one more sibling
	for (size_t i = 0; i < m_allchildpids.size(); i++) {
		if ( died_child_pid != m_allchildpids[i] ) {
			// we use userdefined signal SIGUSR1 as the hint for the alive
			// children to decrease the number of siblings by 1
			kill(m_allchildpids[i], SIGUSR1);
		}
	}

	/* an alternative way is to broadcast the process group */
	//killpg(getpid(), SIGUSR1);

	return m_nAliveChildren;
}

/*
 * we set a layout in which each row holds at most two subwindows, then
 * followed by other rows if needed
 */
int CMitubeRender::_setWindowLayout()
{
	int nRender = (int)m_renders.size();
	if ( nRender < 1 ) {
		return -1;
	}

	int w, h, x = 0, y = 0;
	CGLApp::getScreenSize(&w, &h);

	w -= nRender*10;
	h -= nRender*10;

	w /= (nRender<=1?1:2);
	h /= nRender<=1?1:(int)(1.0*nRender/2+.5); 
	for (int i = 0; i < nRender; i++) {
		if ( i % 2 == 0 ) {
			x = 0;
		}
		else {
			x = (w+10);
		}

		if ( i != 0 && i % 2 == 0 ) {
			y += (h+10);
		}

		ostringstream ostrTitle;
		ostrTitle << " DTI voxel model render - " << i;
		m_renders[i]->setGlutInfo(ostrTitle.str().c_str(),
				x, y, w, h);
	}

	return 0;
}

int CMitubeRender::_assign_argvs(int renderIdx)
{
	// number of common arguments
	int nItmes = m_argc - (int)m_strfnsrcs.size()*2 - (int)m_strdwidirs.size()*2;
	// -f is compulsory
	nItmes +=2;
	// -d is optional, the dwi dirs will be assigned from left to right when
	// it is exhausted, the later render will have not got this argument 
	if ( renderIdx < (int)m_strdwidirs.size() ) {
		nItmes+=2;
	}

	m_allargvs[renderIdx] = new char*[nItmes];
	int cntf = 0, cntd = 0, cntthis = 0;
	// scan the gross argv in m_argv and pick up to assign to each render
	for (int i = 0; i < m_argc; i++) {
		if ( 0 == strncmp( m_argv[i], "-f", 2 ) ) {
			if (renderIdx == cntf) {
				m_allargvs[renderIdx][cntthis++] = m_argv[i];
				m_allargvs[renderIdx][cntthis++] = m_argv[i+1];
			}
			i ++;
			cntf ++;
			continue;
		}

		if ( 0 == strncmp( m_argv[i], "-d", 2 ) ) {
			if (renderIdx == cntd) {
				m_allargvs[renderIdx][cntthis++] = m_argv[i];
				m_allargvs[renderIdx][cntthis++] = m_argv[i+1];
			}
			i ++;
			cntd ++;
			continue;
		}

		// others are all common arguments, just collect as ther are
		m_allargvs[renderIdx][cntthis++] = m_argv[ i ];
	}
	return nItmes;
}

int CMitubeRender::_createProcesses()
{
	int nRender = static_cast<int>( m_strfnsrcs.size() );
	if ( nRender < 1 ) {
		// nothing to do, let it go
		return -1;
	}

	m_allargvs.resize ( nRender );
	m_allargvs.assign( nRender, (char**)NULL );

	m_allchildpids.resize ( nRender );
	m_allchildpids.assign( nRender, -1 );

	/* prepare command line arguments for each render
	 * since "-d" is optional and "-f" is compulsory, we count the number of
	 * input arguments for "-f" and presume the same number of renders
	 * will be needed
	 */
	int nItmes;
	for (int i = 0; i < nRender; i++) {
		nItmes = _assign_argvs( i );
		if ( nItmes < 1 ) {
			return -1;
		}

		m_cout << "----- command line for process " << i << " -------\n";
		for (int j = 0; j < nItmes; j++) {
			m_cout << m_allargvs[i][j] << " ";
		}
		m_cout << "\n";

		m_renders.push_back( new CSitubeRender( nItmes, m_allargvs[ i ] ) );
	}

	return 0;
}

void CMitubeRender::_on_child_exit(int sig)
{
	__WAIT_STATUS  state = NULL;
	if (m_pmitInstance && SIGCHLD == sig) {
		m_pmitInstance->onChildExit( wait(state) );
	}
}

void CMitubeRender::_on_all_exit(int sig)
{
	if (m_pmitInstance && SIGUSR2 == sig ) {
		m_pmitInstance->killallchildren();
	}
}

/* set ts=4 sts=4 tw=80 sw=4 */

