#include "Image.h"

Image::Image()
{
	name = "";
	id = 0;
}

void Image::setImage(string &n, unsigned int id, Mat &m)
{
	name = n;
	this->id = id;
	img = m;
}

string Image::getTotal()
{
	return this->totalSum;
}

void Image::setTotal(string tot)
{
	this->totalSum = tot;
}

Mat Image::getImg()
{
	return this->img;
}

void Image::setName(string n)
{
	this->name = n;
}

string Image::getName()
{
	return this->name;
}