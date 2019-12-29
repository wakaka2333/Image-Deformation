/*#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#define NUM_SIZE 30
using namespace std;
class CvPoint 
{
public:
	double x;
	double y;
	CvPoint() { x = 0.0; y = 0.0; }
	CvPoint(double a, double b) { x = a; y = b; }
protected:
private:
};
void curve4(vector<CvPoint> &p,
	double x1, double y1,
	double x2, double y2,
	double x3, double y3,
	double x4, double y4)
{
	CvPoint tmp0(x1, y1);
	p.push_back(tmp0);
	double dx1 = x2 - x1; 
	double dy1 = y2 - y1;
	double dx2 = x3 - x2;
	double dy2 = y3 - y2;
	double dx3 = x4 - x3;
	double dy3 = y4 - y3;

	double subdiv_step = 1.0 / (NUM_SIZE + 1);
	double subdiv_step2 = pow(subdiv_step, 2);
	double subdiv_step3 = pow(subdiv_step, 3);

	double pre1 = 3.0*subdiv_step;
	double pre2 = 3.0*subdiv_step2;
	double pre4 = 6.0*subdiv_step2;
	double pre5 = 6.0*subdiv_step3;

	double tmp1x = x1 - x2 * 2.0 + x3;
	double tmp1y = y1 - y2 * 2.0 + y3;

	double tmp2x = (x2 - x3)*3.0 - x1 + x4;
	double tmp2y = (y2 - y3)*3.0 - y1 + y4;

	double fx = x1;
	double fy = y1;

	double dfx = (x2 - x1)*pre1 + tmp1x * pre2 + tmp2x * subdiv_step3;
	double dfy = (y2 - y1)*pre1 + tmp1y * pre2 + tmp2y * subdiv_step3;

	double ddfx = tmp1x * pre4 + tmp2x * pre5;
	double ddfy = tmp1y * pre4 + tmp2y * pre5;

	double dddfx = tmp2x * pre5;
	double dddfy = tmp2y * pre5;

	int step = NUM_SIZE;
	while (step--)
	{
		fx += dfx;
		fy += dfy;
		dfx += ddfx;
		dfy += ddfy;
		ddfx += dddfx;
		ddfy += dddfy;
		CvPoint tmp1(fx, fy);
		p.push_back(tmp1);
	}
	CvPoint tmp2(x4, y4);
	p.push_back(tmp2);
}
int main()
{
	CvPoint point[4];
	point[0].x = 1.0;
	point[0].y = 4.0;
	point[1].x = 2.2;
	point[1].y = 5.0;
	point[2].x = 6;
	point[2].y = 3;
	point[3].x = 8;
	point[3].y = 9;
	vector<CvPoint> curvePoint;
	curve4(curvePoint,
		point[0].x, point[0].y,
		point[1].x, point[1].y,
		point[2].x, point[2].y,
		point[3].x, point[3].y);
	int i = 0;
	for (; i < curvePoint.size(); i++)
	{
		cout << "(" << curvePoint[i].x << "," << curvePoint[i].y << ")";
		if ((i + 1) % 2 == 0)
			cout << endl;
	}
	cout << endl << "点的个数： " << i << endl;
	system("pause");
	return 0;
}*/
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <vector>

#include <fstream>
#include <windows.h>

using namespace cv;
using namespace std;

void DrawArc(Mat *src, Point ArcCenter, Point StartPoint, Point EndPoint, int Fill)
{
	if (Fill <= 0)
		return;
	vector<Point> Dots;
	double Angle1 = atan2((StartPoint.y - ArcCenter.y), (StartPoint.x - ArcCenter.x));
	double Angle2 = atan2((EndPoint.y - ArcCenter.y), (EndPoint.x - ArcCenter.x));
	double Angle = Angle1 - Angle2;
	Angle = Angle * 180.0 / CV_PI;

	if (Angle < 0)
		Angle = 360 + Angle;
	if (Angle == 0)
		Angle = 360;
	int brim = floor(Angle / 10);

	Dots.push_back(StartPoint);

	for (int i=0;i<brim;++i)
	{
		double dSinRot = sin(-(10 * (i + 1))*CV_PI / 180); 
		double dCosRot = cos(-(10 * (i + 1))*CV_PI / 180);
		int x = ArcCenter.x + dCosRot * (StartPoint.x - ArcCenter.x) - dSinRot * (StartPoint.y - ArcCenter.y);
		int y = ArcCenter.y + dSinRot * (StartPoint.x - ArcCenter.x) + dCosRot * (StartPoint.y - ArcCenter.y);
		Dots.push_back(Point(x, y));
	}
	Dots.push_back(EndPoint);
	RNG &rng = theRNG();

	Scalar color = Scalar(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255));
	for (int i=0;i<Dots.size()-1;++i)
	{
		line(*src, Dots[i], Dots[i + 1], color, Fill);												
	}
	Dots.clear();
}
/*int main()
{
	Mat Img = Mat::zeros(800, 800, CV_8UC3);
	int64 tim = getTickCount();
	

	//坐标零点400.400
	Point ZeroPoint = Point(400, 400);
	//起始坐标150.-100
	Point StartPoint = Point(ZeroPoint.x + 150, ZeroPoint.y - (-100));
	//结束坐标-150，-100
	Point EndPoint = Point(ZeroPoint.x - 150, ZeroPoint.y - (-100));

	//圆心相对起始点的坐标-150，200
	int I = StartPoint.x - 150;
	int J = StartPoint.y - (+200);

	Point Arc = Point(I, J);

	//显示圆心坐标
	circle(Img, Arc, 1, Scalar(0, 0, 255), -1);
	//显示起点坐标
	circle(Img, StartPoint, 5, Scalar(255, 0, 0), -1);
	//显示结束点坐标
	circle(Img, EndPoint, 5, Scalar(0, 255, 0), -1);

	//园像，圆心，开始点，结束点，线宽
	DrawArc(&Img, Arc, StartPoint, EndPoint, 1);
	imshow("正多边形",Img);

	tim = getTickCount() - tim;
	cout << "处理耗时："<<1000*tim/getTickFrequency()<<"" << endl;
	waitKey(0);
	return 0;
}*/
/*
void Drow(Mat *src,vector<Point> Dots ,int fill)
{
	if (fill <= 0)
		return;

	RNG &rng = theRNG();
	Scalar color = Scalar(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255));
	for (int i = 0; i < Dots.size() - 1; ++i)
	{
		line(*src , Dots[i] , Dots[i + 1] ,color, fill);
	}
	Dots.clear();
}
vector<Point> good_randVec()
{
	static default_random_engine e;
	static uniform_int_distribution<unsigned> u(0, 800);
	vector<Point> cood;
	for (int i = 0; i < 10; ++i)
	{
		cood.push_back(Point(u(e), u(e)));
		std::cout << cood[i].x << "   " << cood[i].y << endl;
	}
	return cood;
}
int main()
{
	Mat Img = Mat::zeros(800, 800, CV_8UC3);
	vector<Point> src = good_randVec();
	Drow(&Img, src ,1);

	imshow("随机曲线", Img);
	waitKey(0);
	return 0;
}*/