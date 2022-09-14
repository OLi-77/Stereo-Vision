/*DptNoF_Map.cpp
���������� �� ����������� ���� ������
��������� ����� ������� c �������������
������� \ ����������� ����� � �����
�������������� �� USB ���������� ����� � ��
�� ���������� ���������� �������� (640�480)
ϳ��������� ����� � ��� USB ����, ��������� ����� ������ � USB-3.0 ����� � USB-2.0
��� ���� ������ �� ������������ ���'��� ���� ���������� �� ������ ���'���
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
	VideoCapture leftCam(0);      //����� ������� �������� ������ ����������� �� "���"
	if (leftCam.isOpened() == false) { cout << "error: Webcam Left connect unsuccessful\n"; system("pause"); return(0); }
	VideoCapture rightCam(1);     //����� ������� �������� ������ ����������� �� "�����"
	if (rightCam.isOpened() == false) { cout << "error: Webcam Right connect unsuccessful\n"; system("pause"); return(0); }
	Mat left, right;
	Mat leftClone, rightClone;
	//��������� cv::StereoMatcher �� ������������
	Ptr<cv::StereoBM> sbm = StereoBM::create();
	
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && leftCam.isOpened() && rightCam.isOpened())
	{
		leftCam.read(left);
		if (left.empty()) { cout << "No frame to read" << endl; system("pause"); break; }
		leftClone = left.clone();               //���� ������ ��� ������
		imwrite("left.png", leftClone);         //�������� ���� � ���� ������ ��������

		rightCam.read(right);
		if (right.empty()) { cout << "No frame to read" << endl; system("pause"); break; }
		rightClone = right.clone();             //���� ������ ����� ������ 
		imwrite("right.png", rightClone);       //�������� ���� � ���� ������ ��������
		//������� �������� �����
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
		dispClone = disp8.clone(); //���� ����� �������
		imwrite("DSPT.png", dispClone); //���������� ����� �������

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