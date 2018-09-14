/*#pragma once

void second_operation(Mat Top, int top, int side, int front, int amount)
{
	Mat win_mat(Size(Top.cols + Top.cols + Top.cols, Top.rows), CV_8UC1);
	Top.copyTo(win_mat(cv::Rect(0, 0, Top.cols, Top.rows)));
	for (int i = 0; i < amount; i++)
	{
		Mat image = up = imread(dirPath + "\\image_top\\image_top_" + to_string(topal) + ".png", 0);
			for (int x = 0; x < image.cols; x++)
			{
				for (int y = 0; y < image.rows; y++)
				{
					if (x == front)
					{

					}
					if (y == side)
					{
					}
				}
			}
	}
	side.copyTo(win_mat(cv::Rect(front.cols, 0, side.cols, side.rows)));
	top.copyTo(win_mat(cv::Rect(front.cols + side.cols, 0, top.cols, top.rows)));
	cv::imshow("Images", win_mat);

}*/