# VIRUS.EXE
Receipt Reader Application using C++


Visual Instrument for Recording User Spending - VIRUS.EXE

## Purpose
The purpose of this project was better understand optical character recognition and how it can be applied to real world scenarios. We decided that we will do OCR in order to try and determine the total amount spent in a store from a receipt. This would be useful because most people will like to keep track of their spending habits and this can be diffcult to do. <br>
With this software, it can assist in reusing the amount of time taken to do this making the software identify the amount spend by reading it from a simple image of a receipt and recording it for the user. 

## User Inferface of the Application
We used wxWidgets that allows us to simply the way Visual Studio does GUI with C++. <br>
The first image is a template on how the UI looks when it is first open to the user. 
![alt text](https://github.com/kloukanov/VIRUS.EXE/blob/master/images/1.jpg) <br>
The load images button loads all the image (receipt) in a given folder that our program reads from. <br>
The start button starts the actual program where it reads the total spending on a receipt. <br>
The view image button allows the user to view any images that is loaded. <br>
Inside the grid where you see column A, the name of column A is called image. To showcase which image we have selected. <br>
In column B, it says GT. GT stands for Ground Truth value. We manually have to enter the actual value inside the program to match each image that is loaded. <br>
In column C, it says ALG, which is short for algorithm. This is the actual tesseract part where the program reads the receipt and output the total spending. With this value, we can determine if tesseract is right or wrong or how far off it is. <br>

