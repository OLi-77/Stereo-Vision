/*DptNoF_Map.cpp
Захоплення та декодування відео потоків
створення карти глибини c безперервними
записом \ перезаписом кадрів в файли
використовуйте дві USB відеокамери сумісні з ПК
які підтримують розподільчу здатність (640х480)
Підключення камер в різні USB чіпи, наприклад першу камеру в USB-3.0 другу в USB-2.0
Або одну камеру до фронтального роз'єму іншу подлючайте до тиловіх роз'ємів
*/
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <conio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	VideoCapture leftCam(0);      //перша фізично підлючена камера визначиться як "ліва"
	if (leftCam.isOpened() == false) { cout << "error: Webcam Left connect unsuccessful\n"; system("pause"); return(0); }
	VideoCapture rightCam(1);     //друга фізично підлючена камера визначиться як "права"
	if (rightCam.isOpened() == false) { cout << "error: Webcam Right connect unsuccessful\n"; system("pause"); return(0); }
	Mat left, right;
	Mat leftClone, rightClone;
	//Параметри cv::StereoMatcher за умовчуванням
	Ptr<cv::StereoBM> sbm = StereoBM::create();
	
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && leftCam.isOpened() && rightCam.isOpened())
	{
		leftCam.read(left);
		if (left.empty()) { cout << "No frame to read" << endl; system("pause"); break; }
		leftClone = left.clone();               //копія потоків лівої камери
		imwrite("left.png", leftClone);         //зберігаємо кадр в файл робочої директорії

		rightCam.read(right);
		if (right.empty()) { cout << "No frame to read" << endl; system("pause"); break; }
		rightClone = right.clone();             //копія потоків правої камери 
		imwrite("right.png", rightClone);       //зберігаємо кадр в файл робочої директорії
		//зчитуємо збережені кадри
		Mat im_left = imread("left.png"); 
		Mat im_right = imread("right.png");

		Size imagesize = im_left.size();
		Mat disparity_Out = Mat(imagesize.height, imagesize.width, CV_DEPTH_MAX);
		Mat g1, g2, dispClone, disp8;
		cvtColor(im_left, g1, COLOR_BGR2GRAY);
		cvtColor(im_right, g2, COLOR_BGR2GRAY);
		
		sbm->setDisp12MaxDiff(1);
		sbm->setSpeckleRange(8);
		sbm->setSpeckleWindowSize(9);
		sbm->setUniquenessRatio(0);
		sbm->setTextureThreshold(507);
		sbm->setMinDisparity(-39);
		sbm->setPreFilterCap(61);
		sbm->setPreFilterSize(5);
		sbm->compute(g1, g2, disparity_Out);
		
		normalize(disparity_Out, disp8, 0, 255, NORM_MINMAX, CV_DEPTH_MAX);
		dispClone = disp8.clone(); //копія карти глибини
		imwrite("DSPT.png", dispClone); //збереження кадру глибини

		namedWindow("Left", WINDOW_AUTOSIZE);
		imshow("Left", im_left);
		namedWindow("Right", WINDOW_AUTOSIZE);
		imshow("Right", im_right);

		namedWindow("Depth map", WINDOW_AUTOSIZE);
		imshow("Depth map", disp8);

		charCheckForEscKey = waitKey(1);
	}
	destroyAllWindows();
	return(0);
}