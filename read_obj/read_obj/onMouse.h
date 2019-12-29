#pragma once
#ifndef _ONMOUSE
#define _ONMOUSE
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <vector>
using namespace cv;
using namespace std;
vector<Point> control_p;
//回调函数签名
void onMouse(int event, int x, int y, int flags, void *param);

//定义回调函数
void onMouse(int event, int x, int y, int flags, void *param)
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
	//Mat *im = reinterpret_cast<Mat*>(param);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		control_p.push_back(Point(x, y));
		std::cout << "at(" << x << "," << y <<")"<< endl;
			break;
	/*case  CV_EVENT_RBUTTONDOWN:
		std::cout << "input(x,y)" << endl;
		std::cout << "x = " << endl;
		cin >> x;
		std::cout << "y = " << endl;
		cin >> y;
		std::cout << "at(" << x << "," << y << ")value is:" << static_cast<int>(im->at<uchar>(cv::Point(x, y))) << endl;
		break;*/
	default:
		break;
	}
}
#endif