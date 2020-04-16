#pragma once

#include "wx/wx.h"
#include "cMain.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;
using namespace std;


class cApp : public wxApp
{

public:
	cApp();
private:
	cMain* frame = nullptr;

public:
	virtual bool OnInit();


};

