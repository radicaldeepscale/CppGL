// ----------------------------------------------------------------------------
// multipleitr.cpp : a boost of multiple-window interactive DTI voxel model
//					render supporting box-selection ROI customization. It is
//					working in multiple process modality with an IPC of shared
//					memory
//
// Creation : Mar. 4th 2011
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#include "mitubeRender.h"

int main(int argc, char** argv)
{
	CMitubeRender mit(argc, argv);
	mit.setGlutInfo(" multiple DTI model visualization", 0, 0, 10, 10);
	mit.run();
	return 0;
}

/* set ts=4 sts=4 tw=80 sw=4 */

