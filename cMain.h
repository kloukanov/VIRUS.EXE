#pragma once
#include "wx/wx.h"
#include <opencv2/core.hpp>
#include "Image.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <filesystem>
#include <wx/grid.h>


class cMain : public wxFrame
{
public:
	Image* images = nullptr;
	Image* GT = nullptr;
	int index = 0;
	int gridIndex = 1;
	int count = 0;
	
	wxButton *loadButton = nullptr;
	wxButton* startButton = nullptr;
	wxButton* viewButton = nullptr;
	wxButton* PRFButton = nullptr;
	wxGrid* grid = nullptr;

	wxStaticText *txt = nullptr;
	wxStaticText* precision = nullptr;
	wxStaticText* recall = nullptr;
	wxStaticText* f1 = nullptr;
	wxStaticText* total = nullptr;
	wxListBox *list = nullptr;

	void StartFindTotal(wxCommandEvent& evt);
	void CalculatePRF(wxCommandEvent& evt);
	void LoadImages(wxCommandEvent& evt);
	void ViewImage(wxCommandEvent& evt);
	string findTotal(string s);
	void readGT(string filename);
	string is_number(const std::string& s);
	void calcPR(Image input[], Image gt[], int len);
	double calcFb(double P, double R, double beta);

	cMain();
	wxDECLARE_EVENT_TABLE();
};

