#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;
using namespace cv;

class Image
{
private:
	string name;
	unsigned int id;
	Mat img;
	string totalSum;

public:
	Image();
	void setImage(string &n, unsigned int id, Mat &m);
	void setName(string n);
	string getName();
	string getTotal();
	Mat getImg();
	void setTotal(string tot);
};

