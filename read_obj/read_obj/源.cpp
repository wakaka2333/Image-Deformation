#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <vector>
#include <random>
#include "onMouse.h"

#include <fstream>
#include <windows.h>

using namespace cv;
using namespace std;
#define MAXNUM 99999

Point MLS(const Point &t,vector<Point> control_p,vector<Point> control_q)// t是输入点
{
	//p为原图像
	vector<Point> control_p;
	vector<Point> control_q;
	vector<double> w;
	if(control_p.empty()) return t;
	Point fv;
	double A[2][2], B[2][2], M[2][2];

	auto iter = control_p.begin();
	w.erase(w.begin(), w.end());

	//计算各个控制顶点的权重，也就是计算点t到各个顶点的距离1/sqr(d);
	while (iter!=control_p.end())
	{
		double temp;
		if (iter->x != t.x || iter->y != t.y)
		{
			temp = 1 / ((iter->x - t.x)*(iter->x - t.x)+(iter->y-t.y)*(iter->y-t.y));
		}
		else
			//t是控制点的话将权重设置为无限大
		{
			temp = MAXNUM;
		}
		w.push_back(temp);
		iter++;
	}
	vector<double>::iterator iterw = w.begin();
	vector<Point>::iterator iterq = control_q.begin();
	iter = control_p.begin();

	Point pc, qc;
	double px = 0, py = 0, qx = 0, qy = 0, tw = 0;
	while (iterw!=w.end())
	{
		px += (*iterw)*(iter->x);//控制点p的加权位置
		py += (*iterw)*(iter->y);

		qx += (*iterw)*(iterq->x);//控制点q的加权位置
		qy += (*iterw)*(iterq->y);

		tw += *iterw;//总权重
		iter++;
		iterw++;
		iterq++;
	}
	pc.x = px / tw;
	pc.y = py / tw;

	qc.x = qx / tw;
	qc.y = qy / tw;

	iter = control_p.begin();
	iterw = w.begin();
	iterq = control_q.begin();
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<2;++j)
		{
			A[i][j] = 0;
			B[i][j] = 0;
			M[i][j] = 0;
		}
	}
	while (iter!=control_p.end())
	{
		double p[2] = { iter->x - pc.x,iter->y - pc.y };
		double PT[2][1];
		PT[0][0] = iter->x - pc.x;
		PT[1][0] = iter->y - pc.y;

		double Q[2] = { iter->x - qc.x,iter->y - qc.y };
		double T[2][2];

		T[0][0] = PT[0][0] * p[0];
		T[0][1] = PT[0][0] * p[1];
		T[1][0] = PT[1][0] * p[0];
		T[1][1] = PT[1][0] * p[1];

		for (int i=0;i<2;++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				A[i][j] += (*iterw)*T[i][j];
			}
		}
		T[0][0] = PT[0][0] * Q[0];
		T[0][1] = PT[0][0] * Q[1];
		T[1][0] = PT[1][0] * Q[0];
		T[1][1] = PT[1][0] * Q[1];

		for (int i=0;i<2;++i)
		{
			for (int j=0;j<2;++j)
			{
				B[i][j] += (*iterw)*T[i][j];
			}
		}
		iter++;
		iterw++;
		iterq++;
	}

	//CvInvert(A,M);
	double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
	if (det < 0.0000001)
	{
		fv.x = t.x + qc.x - pc.x;
		fv.y = t.y + qc.y - pc.y;
		return fv;
	}
	double temp1, temp2, temp3, temp4;
	temp1 = A[1][1] / det;
	temp2 = -A[0][1] / det;
	temp3 = -A[1][0] / det;
	temp4 = A[0][0] / det;
	A[0][0] = temp1;
	A[0][1] = temp2;
	A[1][0] = temp3;
	A[1][1] = temp4;

	M[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
	M[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
	M[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
	M[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];

	double V[2] = { t.x - pc.x,t.y-pc.y };
	double R[2][1];

	R[0][0] = V[0] * M[0][0] + V[1] * M[1][0];
	R[1][0] = V[0] * M[0][1] + V[1] * M[1][1];
	fv.x = R[0][0] + qc.x;
	fv.y = R[1][0] + qc.y;

	return fv;
}

int main()
{
	auto IR_X =1, IR_Y = 1;
	auto pImage = cv::imread("C:/Users/82158/source/repos/read_obj/read_obj/test.jpg");
	//auto pImage1=cv::open
	//auto dImage = cvCreateImage(cvSize(2 * pImage->width, 2*pImage->height), pImage->depth,pImage->channels);
	cv::imshow("1", pImage);
	cv::setMouseCallback("1", onMouse, reinterpret_cast<void*> (&pImage));

	cout <<"Cols:"<<pImage.cols<<"-------Rows:"<<pImage.rows << endl;
	Mat dImage(2 * pImage.cols, 2 * pImage.rows, CV_8UC3, Scalar(0, 0, 0));
	Point Orig = MLS(Point(IR_X, IR_Y),control_p,);
	  
	int orig_x = (int)(Orig.x) - (int)(pImage.cols / 2);
	int orig_y = (int)(Orig.y) - (int)(pImage.rows / 2);

	for (int i=0;i<pImage.rows;++i)
	{
		for (int j = 0; j < pImage.cols; ++j)
		{   
			double x = j + IR_X;
			double y = i + IR_Y;
			Point t = MLS(Point(x, y));
			 
			int m = (int)(t.x);
			int n = (int)(t.y);

			m -= orig_x; 
			n -= orig_y;
			auto color = pImage.at<Vec3b>(i, j);
			//CvScalar color = cvGet2D(pImage, i, j);
			if (0 <= m && dImage.rows > m && 0 <= n && dImage.cols > n)
			{
				dImage.at<Vec3b>(n, m) = Vec3b(color(0), color(1), color(2));
				//cvSet2D(dImage, n, m, color);
			}
		}
	}
	//imshow("test", dImage);
	imwrite("testbak.jpg", dImage);
	cvWaitKey();
	system("pause");
	return 0;
}