#include "INIParser.h"
#include "findCircle.h"
#include "findRect.h"

using namespace cv;
using namespace std;


template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}


int main()
{
	INIParser ini_parser;
	ini_parser.ReadINI("./config/conf.ini");
	//寻找圆的误差容忍
	// 1 椭圆度
	float WH_ERR = stringToNum<float>(ini_parser.GetValue("default", "WH_ERR"));
	// 2 外接矩形填充度
	float CR_ERR = stringToNum<float>(ini_parser.GetValue("default", "CR_ERR"));
	// 3 圆的残缺度
	float CH_ERR = stringToNum<float>(ini_parser.GetValue("default", "CH_ERR"));
	// 4 二值化阈值
	int MY_BINARY_TH = stringToNum<int>(ini_parser.GetValue("default", "MY_BINARY_TH"));


	//string fileName = "F:/Dataset/circle/blue-stable-12s.264";
	//string fileName = "F:/Dataset/circle/blue-swift-2min.264";
	//string fileName = "F:/Dataset/circle/orange-5min.264";
	//string fileName = "F:/Dataset/circle/target.264";
	string fileName = "C:/Users/Administrator/Desktop/pic/03_both.mp4";
	/*
	string fileName1 = "C:/Users/Administrator/Desktop/pic/10.png";
	Mat frame;
	frame = imread(fileName1);
	int nRows = 250;
	int nCols = frame.cols * 250 / frame.rows;
	Mat dst(nRows, nCols, frame.type());

	resize(frame, dst, dst.size(), 0, 0, INTER_LINEAR);

	findRect(dst);

	imshow("处理后", dst);
	waitKey(0);
	*/
	
	VideoCapture capture;//参数为0时读取摄像头
	capture.open(fileName);
	if (!capture.isOpened()) return -1;

	double rate = capture.get(CV_CAP_PROP_FPS);//获取视频文件的帧率
	int delay = cvRound(1000.000 / rate);//取整

	while (1)
	{
		Mat img;
		capture >> img;
		if (img.empty()) break;


		int nRows = 250;
		int nCols = img.cols * 250 / img.rows;
		Mat dst(nRows, nCols, img.type());

		resize(img, dst, dst.size(), 0, 0, INTER_LINEAR);

		//将osd的信息裁剪掉
		int x = dst.cols*0.08;
		int y = dst.rows*0.08;
		Rect rect(x, y, dst.cols-2*x, dst.rows-2*y);
		Mat frame(dst, rect);


		long start = clock();
		findRect(frame);
		long finish = clock();
		long t = finish - start;
		if (t < 100) Sleep(100-t);
		//printf("The run time is:%9.3lf  %s\n", (double)t / CLOCKS_PER_SEC, "s!"); //输出时间	
		imshow("处理后", frame);
		waitKey(delay);
	}
	
	system("pause");
	return 0;
}



