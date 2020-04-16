#include "cMain.h"


using namespace cv;
namespace fs = std::filesystem;

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, StartFindTotal)
	EVT_BUTTON(10002, LoadImages)
	EVT_BUTTON(10003, ViewImage)
	EVT_BUTTON(10004, CalculatePRF)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Virus.exe", wxPoint(30,30), wxSize(800, 550))
{
	images = new Image[10];
	GT = new Image[10];
	startButton = new wxButton(this, 10001, "Start", wxPoint(180, 340), wxSize(150, 50));
	loadButton = new wxButton(this, 10002, "Load Images", wxPoint(10, 340), wxSize(150, 50));
	viewButton = new wxButton(this, 10003, "View Image", wxPoint(350, 340), wxSize(150, 50));
	PRFButton = new wxButton(this, 10004, "Calculate P/R/F", wxPoint(520, 340), wxSize(150, 50));
	total = new wxStaticText(this, wxID_ANY, "Total", wxPoint(600,280), wxSize(300,30));

	recall = new wxStaticText(this, wxID_ANY, "Recall is ", wxPoint(600, 10), wxSize(300, 30));
	precision = new wxStaticText(this, wxID_ANY, "Precision is ", wxPoint(600, 40), wxSize(300, 30));
	f1 = new wxStaticText(this, wxID_ANY, "F1 is ", wxPoint(600, 70), wxSize(300, 30));

	list = new wxListBox(this, wxID_ANY, wxPoint(10,10), wxSize(150,300));

	grid = new wxGrid(this, -1, wxPoint(200, 10), wxSize(340, 300));
	grid->CreateGrid(21, 3);
	grid->SetCellValue(0, 0, "IMAGE");
	grid->SetCellValue(0, 1, "GT");
	grid->SetCellValue(0, 2, "ALG");
}


void cMain::StartFindTotal(wxCommandEvent& evt)
{
	Mat im = images[index].getImg();
	string outText;
	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	tess.SetImage((uchar*)im.data, im.size().width, im.size().height, im.channels(), im.step1());
	tess.Recognize(0);
	outText = tess.GetUTF8Text();

	string tot = findTotal(outText);
	images[index].setTotal(tot);
	
	total->SetLabel("Total is " + images[index].getTotal());

	grid->SetCellValue(gridIndex, 0, images[index].getName());

	for (int i = 0; i < count; i++)
	{
		if (GT[i].getName().compare(images[index].getName()) == 0)
		{
			grid->SetCellValue(gridIndex, 1, GT[i].getTotal());
		}
	}

	grid->SetCellValue(gridIndex, 2, images[index].getTotal());

	gridIndex++;

	evt.Skip();
}


string cMain::is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	int c = 0;

	while (it != s.end() && !std::isdigit(*it))
	{
		++it;
		++c;
	}
		
	int p1 = c;

	return s.substr(p1, 5);
}


string cMain::findTotal(string input)
{
	size_t pos = input.rfind("Total:");
	size_t pos1 = input.rfind("TOTAL:");
	size_t pos2 = input.rfind("TOTAL");
	size_t pos3 = input.rfind("Total");

	if (pos == 0 || pos > 1000000)
		if (pos1 == 0 || pos1 > 1000000)
			if (pos2 == 0 || pos2 > 1000000)
				if (pos3 == 0 || pos3 > 1000000)
					return "undefined";
				else
					return is_number(input.substr(pos3));		
			else
				return is_number(input.substr(pos2));
		else
			return is_number(input.substr(pos1));
	else
		return is_number(input.substr(pos));
	
}

void cMain::CalculatePRF(wxCommandEvent& evt)
{
	calcPR(images, GT, count);
}


void cMain::LoadImages(wxCommandEvent& evt)
{
	std::string path = "./training";
	for (const auto& entry : fs::directory_iterator(path))
		count++;
	
	delete[] images;
	images = nullptr;
	images = new Image[count];
	int i = 0;

	for (const auto& entry : fs::directory_iterator(path))
	{
		string filename = entry.path().u8string().substr(11); // substr(11) for training.csv, substr(7) for test.csv
		string pathname = "./training/" + filename;
		Mat im = imread(pathname, IMREAD_GRAYSCALE);
		images[i].setImage(filename, i, im);
		list->AppendString(images[i].getName());
		i++;
	}

	readGT("training.csv");
	
	evt.Skip();
}

void cMain::readGT(string filename)
{
	ifstream myfile;
	myfile.open(filename);

	delete[] GT;
	GT = nullptr;
	GT = new Image[count];

	string* line = new string[count];
	int i = 0;
	while (!myfile.eof())
	{
		getline(myfile, line[i], '\n');
		for (int j = 0; j < line[i].length(); j++)
		{
			if (line[i][j] == ',')
			{
				j++;
				GT[i].setName(line[i].substr(0,j-1));
				GT[i].setTotal(line[i].substr(j));
			}
		}
		i++;
	}
	myfile.close();
}


bool checkAns(string gt, string in)
{
	int counter = 0;
	for (int i = 0; i < gt.size(); i++)
	{
		for (int j = 0; j < in.size(); j++)
		{
			if (gt[i] == in[j])
			{
				counter++;
			}
		}
	}

	return counter >= 3;
}

void sortarrs(Image arr[], int n)
{
	int i, j;
	string key;
	for (i = 1; i < n; i++)
	{
		key = arr[i].getName();
		j = i - 1;

		while (j >= 0 && arr[j].getName() > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1].getName() = key;
	}
}

void cMain::calcPR(Image input[], Image gt[], int len)
{
	double P, R, F;

	sortarrs(input, count);
	sortarrs(gt, count);

	double TP = 0, FN = 0, TN = 0, FP = 0;
	for (int i = 0; i < len; i++)
	{
		if (checkAns(gt[i].getTotal(), input[i].getTotal())) // check if algorithm is returning right value (TP)
		{
			TP++;
		}
		else if (input[i].getTotal() != gt[i].getTotal() && input[i].getTotal() != "undefined") // check if algorithm is returning a wrong value (FP)
		{
			FP++;
		}
		else if (input[i].getTotal() == "undefined" && gt[i].getTotal() == "undefined") // check if algorithm is rightly returning a wrong value (TN)
		{
			TN++;
		}
		else if (input[i].getTotal() == "undefined" && gt[i].getTotal() != "undefined") // check if algorithm is wrongly returning a wrong value (FN)
		{
			FN++;
		}
	}

	R = TP / (TP + FN);
	P = TP / (TP + FP);
	if (FP == 0 && TP == 0)
		P = 0;
	
	if (FN == 0 && TP == 0)
		R = 0;
	
	F = calcFb(P, R, 1);

	recall->SetLabel("Recall is " + to_string(R));
	precision->SetLabel("Precision is " + to_string(P));
	f1->SetLabel("F1 is " + to_string(F));
}

double cMain::calcFb(double P, double R, double beta) {
	double F = (1 + pow(beta, 2)) * ((P * R) / ((pow(beta, 2) * P) + R));
	if (R == 0 && P == 0)
		F = 0;

	return F;
}


void cMain::ViewImage(wxCommandEvent& evt)
{
	Mat res;
	index = list->GetSelection();
	resize(images[index].getImg(), res, Size(600, 800));
	imshow("Image", res);
	waitKey(0);
	evt.Skip();
}


