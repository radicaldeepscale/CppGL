// ----------------------------------------------------------------------------
// multipleitr.cpp : a multiple-window itubeRender capsule, loading multiple
//					DTI voxel model each assigned with a separate window using
//					multiple process implementation;
//
//					this capsule provides centralized interaction events
//					dispatching, thus offering a sort of synchronous interaction
//					response
//
// Creation : Mar. 2nd 2011
//
// Revision:
//	@Mar. 4th
//		.sublimate the servant class CMitubeRender for the previous holistic
//		multipleitr into an independent component for the purpose of
//		extensibility
//		.add glutIdelFunc callback overloading to set a place for the IPC with
//		the ultimate aim of the synchronous user interaction dispatching
//	@Mar. 5th
//		.establish the child process management by process grouping; when one of
//		the children in the group exits, parent will get to know exactly which
//		one sends the SIGCHLD and then update the number of currently alive
//		children, and finally broadcast the update to the group
//	@Mar. 6th
//		.Fix the bug that when the desired number of windows is odd, layout
//		computation is incorrect by rounding off "nRender/2" before it is about
//		to divide the whole screen height
//		.Add another signal catching procedure : when one of the children wants
//		to kill all the process group members by sending SIGUSR2, the parent
//		will assist in accomplishing that by invoking killallchildren
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#include "situbeRender.h"

#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>

using std::vector;
using std::string;

class CMitubeRender: public CGLIBoxApp {
public:
	CMitubeRender(int argc, char **argv);
	virtual ~CMitubeRender();

	// overloadings to those in the parent class for customized actions
	int handleOptions(int optv);
	void keyResponse(unsigned char key, int x, int y);
	int mainstay();

	// create thread for each of the DTI models
	int startchildren();

	// kill all threads that are still alive
	int killallchildren();

	// loop into the waiting for all children to finish
	void waitforallchildren();

	// when children terminate
	int onChildExit(pid_t died_child_pid);

private:
	// a set of source geometry files
	vector<string> m_strfnsrcs;
	// a set of DWI image directories
	vector<string> m_strdwidirs;

	// a set of renders, each for a single model, associated with a single
	// source geometry file and a DWI image directory, but they will share all
	// other command line arguments like lod, tube radius and number of
	// selection boxes, etc.
	vector<CSitubeRender*> m_renders;

	vector<char**>		m_allargvs;
	vector<pid_t>		m_allchildpids;

	// calculate a default, evenlys spaced, window layout
	int _setWindowLayout();

	// extract from the original command line to form command line for each
	// process
	int _assign_argvs(int renderIdx);

	// just create instances of single itubeRender for each of the DTI models
	int _createProcesses();

	//  signal handler for SIGCHLD, when notified by the OS that one of the
	//  child process terminates
	static void _on_child_exit(int);

	// signal handler for SIGUSR2, when notified by one of children that he
	// wants to destroy the whole process group
	static void _on_all_exit(int);

	// this should do the trick : 
	// to bypass the callback function limitations and OO design
	static CMitubeRender* m_pmitInstance;

	// current # of alive children; when it is decreased to 0, parent will also
	// rest
	int m_nAliveChildren; 
	
};

/* set ts=4 sts=4 tw=80 sw=4 */

