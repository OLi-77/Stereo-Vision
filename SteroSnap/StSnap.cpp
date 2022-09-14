/*StSnap.cpp
Захоплення та декодування відео потоків
використовуйте дві USB відеокамери сумісні з ПК
які підтримують розподільчу здатність(640х480)
Підключення камер в різні USB чіпи, наприклад першу камеру в USB-3.0 другу в USB-2.0
Або одну камеру до фронтального роз'єму іншу подлючайте до тиловіх роз'ємів
*/
#include <windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include "Config.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	
	VideoCapture capr(0);      //визначаємо камери
	if (capr.isOpened() == false) { cout << "error: Webcam No.1 connect unsuccessful\n";  system("pause");  return(0); }
	VideoCapture capl(1);
	if (capl.isOpened() == false) { cout << "error: Webcam No.2 connect unsuccessful\n";  system("pause"); return(0); }
	// Camera "NoName"
	capl.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	capl.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	// Camera " HQ-TECH, model: WU-66512.0M"
	capr.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	capr.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	/*
	// Camera "NoName"
	capl.set(CV_CAP_PROP_FRAME_HEIGHT, 240);//480 max; 240- OK
	capl.set(CV_CAP_PROP_FRAME_WIDTH, 352);//640 max; 352 - OK
	// Camera " HQ-TECH, model: WU-66512.0M"
	capr.set(CV_CAP_PROP_FRAME_HEIGHT, 240);//480 max; 240 - OK
	capr.set(CV_CAP_PROP_FRAME_WIDTH, 352);//640 max; 352 - OK
	*/
	namedWindow("Left");
	namedWindow("Right");

	cout << "Press 's' to save capture ..." << endl;
	char choice = 0;
	int count = 0;
	while (choice != 'q') {
		//захоплення потоків
		capl.grab();
		capr.grab();
		//операії декодування
		Mat framel, framer;
	
		capl.retrieve(framel);
		capr.retrieve(framer);

		if (framer.empty() || framel.empty()) break;

		imshow("Left", framel);
		imshow("Right", framer);
		if (choice == 's')
		{
			//збереження *.jpg та очікування «Key» 's' або 'q'
			stringstream r_name, l_name;
			l_name << "left" << setw(4) << setfill('0') << count << ".jpg";
			r_name << "right" << setw(4) << setfill('0') << count << ".jpg";
			imwrite(string (LEFT_FOLDER) + l_name.str(), framel);
			imwrite(string(LEFT_FOLDER) + l_name.str(), framel);
			cout << "Saved set left "<< count<< endl;
			imwrite(string (RIGHT_FOLDER) + r_name.str(), framer);
			imwrite(string(RIGHT_FOLDER) + r_name.str(), framer);
			cout << "Saved set right" << count << endl;
			count++;
		}
		choice = char(waitKey(1));
	}
	destroyAllWindows();
	capl.release();
	capr.release();
	return 0;
}