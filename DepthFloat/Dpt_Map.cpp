/*Dpt_Map.cpp
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
#include <conio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{ 
	VideoCapture leftCam(0);      //перша фізично підключена до ПК камера визначиться як "Ліва"
	if (leftCam.isOpened() == false) { cout << "error: Webcam Left connect unsuccessful\n"; system("pause"); return(0); }
	VideoCapture rightCam(1);     //друга фізично підключена до ПК камера визначиться як "Права"
	if (rightCam.isOpened() == false) { cout << "error: Webcam Right connect unsuccessful\n"; system("pause"); return(0); }
	Mat left, right;
	Mat leftClone, rightClone;
	int PreFilterType = 0, PreFilterCap = 0, MinDisparity = 0, UniqnessRatio = 0.000001, TextureThreshold = 0.00001, BlockSize=7,
		SpeckleRange = 0, SADWindowSize = 5, SpackleWindowSize = 0.01, NumDisparities = 0, numDisparities = 1, PreFilterSize = 5;
	Ptr<cv::StereoBM> sbm = StereoBM::create(PreFilterCap, SADWindowSize);
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && leftCam.isOpened() && rightCam.isOpened())
	{
		leftCam.read(left);
		if (left.empty()) { cout << "No left frame to read" << endl; system("pause"); break; }
		leftClone = left.clone();               //копія кадру лівої камери 
		imwrite("left.png", leftClone);         //збереження кадру в файл

		rightCam.read(right);
		if (right.empty()) { cout << "No right frame to read" << endl; system("pause"); break; }
		rightClone = right.clone();             //копія кадру правої камери
		imwrite("right.png", rightClone);       // збереження кадру в файл
		//зчитуємо кадри
		Mat im_left = imread("left.png");
		Mat im_right = imread("right.png");

		Size imagesize = im_left.size();
		Mat disparity_Out = Mat(imagesize.height, imagesize.width, CV_BGR2HSV);
		Mat g1, g2, dispClone,disp8;
		cvtColor(im_left, g1, COLOR_BGR2GRAY);
		cvtColor(im_right, g2, COLOR_BGR2GRAY);
		//застосовуємо фільтрацію
		sbm->setDisp12MaxDiff(1);
		sbm->setSpeckleRange(SpeckleRange);
		sbm->setSpeckleWindowSize(SpackleWindowSize);
		sbm->setTextureThreshold(TextureThreshold);
		sbm->setUniquenessRatio(UniqnessRatio);
		sbm->setBlockSize(BlockSize);
		sbm->setMinDisparity(MinDisparity-39);
		sbm->setPreFilterCap(61);
		sbm->setPreFilterSize(PreFilterSize);
		sbm->compute(g1, g2, disparity_Out);
		if (SADWindowSize % 2 == 0)
		{
			SADWindowSize = SADWindowSize + 1;
		}

		if (SADWindowSize < 21)//5
		{
			SADWindowSize = 21;//5
		}
		if (NumDisparities % 128 != 0)
		{
			NumDisparities = NumDisparities + (128 - NumDisparities % 128);
		}
		//namedWindow("Track Bar", CV_WINDOW_NORMAL);
		cvCreateTrackbar("Disparity", "Depth map", &MinDisparity, 200);//200
		cvCreateTrackbar("U.Ratio", "Depth map", &UniqnessRatio, 21);//2500
		cvCreateTrackbar("Texture.T", "Depth map", &TextureThreshold, 2500);//10000
		cvCreateTrackbar("Range", "Depth map", &SpeckleRange, 1000);//2500
		cvCreateTrackbar("WindowSize", "Depth map", &SpackleWindowSize, 300);//10000

		normalize(disparity_Out, disp8, 0, 255, NORM_MINMAX, CV_BGR2HSV);

		dispClone = disp8.clone(); //копія кадру глибини
		imwrite("DSPT.png", dispClone); //зберігаємо кадр в файл
		//відображення результату на екрані
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