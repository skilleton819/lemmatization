#pragma once

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

void open_dcm(char* filename)
{
	DicomImage DCM(filename);
	amount = DCM.getNumberOfFrames();
	for (int k = 0; k < amount; k++)
	{
		Mat img_cor(int(DCM.getWidth()), int(DCM.getHeight()), CV_16U, (uchar*)DCM.getOutputData(16, k));
		Mat img_sag(int(DCM.getWidth()), int(DCM.getHeight()), CV_16U), img_axi(int(DCM.getWidth()), int(DCM.getHeight()), CV_16U);
		for (int i = 0; i < amount; i++)
		{
			Mat tcp(int(DCM.getWidth()), int(DCM.getHeight()), CV_16U, (uchar*)DCM.getOutputData(16, i));
			tcp.row(k).copyTo(img_sag.row(i));
			tcp.col(k).copyTo(img_axi.col(i));

		}
		//cv::rotate(axial, axial, ROTATE_180);
		cv::rotate(axial, axial, ROTATE_90_CLOCKWISE);
		coronal.push_back(img_cor);
		saggital.push_back(img_sag);
		axial.push_back(img_axi);
	}
}
