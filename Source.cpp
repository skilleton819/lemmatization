#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "Header.h"
#include "Unused_func.h"
#include "dcmtk_process.h"
using namespace std;
using namespace cv; 
vector<Point3i> points;
vector<vector<Point3i>> model;
ofstream secondary;
vector<Mat> saggital, axial, coronal;
int sag_num = 0, axi_num = 0, cor_num = 0;
string dirPath;
int xt1 = 0, yt1 = 0, xt2 = 0, yt2 = 0, yf1 = 0, yf2 = 0, xt3 = 0, xt4 = 0, frontal = 0, topal = 0, sidal = 0, maxi =0, mini=0;
double dimencion = 0.2;
Mat front, up, process, grscl, side, win_mat;
int amount;

/// rewrite to use Mat from global vector (no variable)
void view()
{
	Mat win_mat(Size(saggital[0].cols + axial[0].cols + coronal[0].cols, saggital[0].rows), CV_16U);
	saggital[sag_num].copyTo(win_mat(Rect(0, 0, saggital[sag_num].cols, saggital[sag_num].rows)));
	axial[axi_num].copyTo(win_mat(Rect(saggital[sag_num].cols, 0, axial[axi_num].cols, axial[axi_num].rows)));
	coronal[cor_num].copyTo(win_mat(Rect(saggital[sag_num].cols + axial[axi_num].cols, 0, coronal[cor_num].cols, coronal[cor_num].rows)));
	rectangle(win_mat, Point(xt3, xt1), Point(xt4, xt2), Scalar(255, 0, 0));
	rectangle(win_mat, Point(xt4 + saggital[sag_num].cols, yt2), Point(xt3 + saggital[sag_num].cols, yt1), Scalar(255, 0, 0));
	rectangle(win_mat, Point(xt2 + saggital[sag_num].cols + axial[axi_num].cols, yt2), Point(xt1 + saggital[sag_num].cols + axial[axi_num].cols, yt1), Scalar(255, 0, 0));
	imshow("Images", win_mat);
}


/// rewrite, to use Mat from vector
void drawing()
{
	up = imread(dirPath + "\\image_top\\image_top_" + to_string(topal) + ".png", 0);
	front = imread(dirPath + "\\image_front\\image_front_" + to_string(frontal) + ".png", 0);
	side = imread(dirPath + "\\image_side\\image_side_" + to_string(sidal) + ".png", 0);

	view(front, side, up);
}

void ShowimageFront(int, void*)///sag
{
	front = imread(dirPath + "\\image_front\\image_front_" + to_string(frontal) + ".png" , 0);
	drawing();
	view(front, side, up);
}

void ShowimageTop(int, void*)///axi
{
	up = imread(dirPath + "\\image_top\\image_top_" + to_string(topal) + ".png", 0);
	drawing();
	view(front, side, up);
}
void ShowimageSide(int, void*) ///cor
{
	side = imread(dirPath + "\\image_side\\image_side_" + to_string(sidal) + ".png", 0);
	drawing();
	view(front, side, up);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) ///add event on mouse movement as a ruler + add settings for treshhold
{
	if (x < front.cols)
	{
		if (event == EVENT_LBUTTONDOWN)
		{
			xt3 = x;
			yt1 = y;
			drawing();
			view(front, side, up);
		}
		else if (event == EVENT_RBUTTONDOWN)
		{
			xt4 = x;
			yt2 = y;
			drawing();
			view(front, side, up);
		}
	}
	else if (x > front.cols && x < side.cols + front.cols)
	{
		if (event == EVENT_LBUTTONDOWN)
		{
			xt1 = x - front.cols;
			yt1 = y;
			drawing();
		}
		else if (event == EVENT_RBUTTONDOWN)
		{
			xt2 = x - front.cols;
			yt2 = y;
			drawing();
		}
		else if (event == EVENT_MBUTTONDOWN)
		{
			int length = 512, posXfrst, posXscnd, posY, k , j;
			process = imread(dirPath + "\\image_side\\image_side_" + to_string(sidal) + ".png", 0);
			threshold(process, grscl, 66, 255, 4);
			for (int t = yt1; t < yt2; t++)
			{
				for (int l = xt1; l < xt2; l++)
				{
					int temp = l + 1;
					int res = l - 1;
					if ((int)grscl.at<uchar>(t, l) != 0 && (int)grscl.at<uchar>(t, res) == 0 && (int)grscl.at<uchar>(t, temp) !=0)
						posXfrst = l;
					if ((int)grscl.at<uchar>(t, l) != 0 && (int)grscl.at<uchar>(t, temp) == 0 && (int)grscl.at<uchar>(t, res) !=0)
						posXscnd = l;
				}
				if (posXscnd - posXfrst < length)
				{
					k = posXfrst;
					j = posXscnd;
					length = posXscnd - posXfrst;
					posY = t;
				}
			}
			cout << "The smalles region is " << length * dimencion << "mm width" << endl;
			line(side, Point(k, posY), Point(j, posY), Scalar(255, 255, 255));
			view(front, side, up);		
		}
	}
		else if(x> side.cols + front.cols)
		{
			if (event == EVENT_LBUTTONDOWN)
			{
				xt3 = x - front.cols - side.cols;
				xt1 = y;
				drawing();
				view(front, side, up);
			}
			else if (event == EVENT_RBUTTONDOWN)
			{
				xt4 = x - front.cols - side.cols;
				xt2 = y;
				drawing();
				view(front, side, up);

			}
			else if (event == EVENT_MBUTTONDOWN)
			{

				secondary.open(dirPath + "\\model.ply");
				if (yf1 > yf2)
				{
					int a = yf2;
					yf2 = yf1;
					yf1 = a;
				}
				for (int start = yt1; start < yt2; start++)
				{
					process = imread(dirPath + "\\image_top\\image_top_" + to_string(start) + ".png", 0);
					threshold(process, grscl, 66, 255, 4);
					for (int iks = xt3; iks < xt4; iks++)
					{
						for (int igr = xt1; igr < xt2; igr++)
						{
							if ((int)grscl.at<uchar>(igr, iks) > 30)
							{
								points.push_back(Point3i(iks, igr, start));
							}
						}
					}
					if (start == yt1)
					{
						maxi = points.size();
						mini = points.size();
					}
					model.push_back(points);
					if (points.size() > maxi)
						maxi = points.size();
					else if (points.size() < mini)
						mini = points.size();
					points.clear();
				}
				int size = 0;
				for (int e = 0; e < model.size(); e++)
					size += model[e].size();
				secondary << "ply" << endl << "format ascii 1.0" << endl << "element vertex " << size << endl << "property float x" << endl << "property float y" << endl << "property float z" << endl
					<< "property uchar red" << endl << "property uchar green" << endl << "property uchar blue" << endl << "end_header" << endl;

				double delta = maxi - mini;
				double scale = 510 / delta;
				cout << "Complete volume of a model is: " << size *dimencion*dimencion*dimencion << "  mm^3" << endl;
				cout << "Smallest area is: " << mini *dimencion*dimencion << " square mm" << endl;
				int R = 0, G = 0, B = 0;
				for (int u = 0; u < model.size(); u++)
				{
					int diff = model[u].size() - mini;
					int sch = diff*scale;
					if (sch > 255)
					{
						R = sch - 255;
						G = 255;
					}
					else if (sch < 255)
					{
						R = 255;
						G = sch;
					}
					for (int k = 0; k < model[u].size(); k++)
						secondary << model[u][k].x << " " << model[u][k].y << " " << model[u][k].z << " " << R << " " << G << " " << B << endl;
				}
				secondary.close();
				string dimens = dirPath + "\\meshlab.exe " + dirPath + "\\model.ply";
				char *y = new char[512];
				strcpy(y, dimens.c_str());
				system(y);
				delete[] y;
			}
		}
}

int main(int argc, char** argv)
{ 
	if (argv[1] != NULL)
	{
		char filename[MAX_PATH];
		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "DICOM files\0*.dcm\0Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Load DICOM";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
		if (GetOpenFileNameA(&ofn))
		{
			//load_dcm(filename, argv[0]);
			open_dcm(filename);
		}
		else
		{
			cout << "You cancelled.\n";
			return 1;
		}
	}
	/*
	ifstream spec(dirPath + "\\sec.txt");
	spec >> dimencion;


	string folder(argv[0]);
	size_t slash = folder.find_last_of("\\");
	dirPath = (slash != std::string::npos) ? folder.substr(0, slash) : folder;
	string k = dirPath + "\\image_top";
	string l = dirPath + "\\image_front";
	string m = dirPath + "\\image_side";
	int top = get_all_files_names_within_folder(k);
	int frontik =	get_all_files_names_within_folder(l);
	int sidik = get_all_files_names_within_folder(m);

	front = imread(dirPath + "\\image_front\\image_front_" + to_string(frontal) + ".png", 0);
	up = imread(dirPath + "\\image_top\\image_top_" + to_string(topal) + ".png", 0);
	side = imread(dirPath + "\\image_side\\image_side_" + to_string(sidal) + ".png", 0);
	view(front, side, up);
	*/
	view();
	createTrackbar("Front", "Images", &sag_num, saggital.size(), view);
	createTrackbar("Side", "Images", &axi_num , axial.size() - 1, view);
	createTrackbar("Top", "Images", &cor_num, coronal.size(), ShowimageTop);
	//ShowimageFront(0, 0);
	//ShowimageTop(0, 0);
	setMouseCallback("Images", CallBackFunc, NULL);

	waitKey(0);
}