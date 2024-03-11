#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <opencv2/core.hpp>

#include <opencv2/core/utils/logger.defines.hpp>
#include <opencv2/core/utils/logtag.hpp>
#include <opencv2/core/utils/logger.hpp> 
#define OPENCV_LOGGER_DEFINES_HPP

using namespace std;
using namespace cv;

Mat img, img_save;
Point pt;

int rgb[3] = { 0,0,0 };

void image();
void chg_brightness(int pos, void* userdata);
void chg_RGB(int, void* userdata);
void image_save();
void onMouse(int event, int x, int y, int flags, void* param);


int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);

	namedWindow("image");
	cout << "---------- OpenCV 미니 포토샵 ----------"	<< endl;
	cout << "사진 수정 후 아무 키나 눌러주세요 ..."		<< endl;
	cout << "이후 저장할 경로를 입력해주세요 ..."		<< endl;
	cout << "(ex: C:\\Users\\PC1\\Desktop\\Test.png )"	<< endl;
	cout << "----------------------------------------"	<< endl;
	image();

	//트랙바 
	//createTrackbar(트랙바이름, 트랙바 생성창, 트랙바 위치를 받을 주소, 트랙바 최대 위치, 콜백함수, 사용자 포인터)
	createTrackbar("brightness", "image", 0, 100, chg_brightness, (void*)&img); //밝기

	createTrackbar("R", "image", &rgb[2], 255, chg_RGB, (void*)&image);
	createTrackbar("G", "image", &rgb[1], 255, chg_RGB, (void*)&image);
	createTrackbar("B", "image", &rgb[0], 255, chg_RGB, (void*)&image);
	
	imshow("image", img);

	setMouseCallback("image", onMouse);
	
	img_save = img.clone();

	waitKey(0);

	image_save();

	return 0;
}

//파일 가져오기
void image() {

	String path;

	cout << "파일 경로를 입력하세요" << endl;
	cout << ">> " << ends;
	cin  >> path;

	img = imread(path);

	if (img.empty()) {
		cout << "이미지를 읽을 수 없습니다." << endl;
		exit(-1);
	}
}

void image_save() {
	// 저장할 파일 경로 및 파일명
	String save_path;

	cout << "저장할 경로를 입력하세요" << endl;
	cout << "(ex: C:\\Users\\PC1\\Desktop\\Test.png )" << endl;
	cout << ">> " << ends;
	cin >> save_path;

	if (save_path.empty()) {
		cout << "경로를 입력하지 않았습니다." << endl;
		return;
	}

	// 파일 확장자 확인
	string extension = save_path.substr(save_path.find_last_of(".") + 1);
	if (extension != "jpg" && extension != "jpeg" && extension != "png") {
		cout << "올바른 파일 확장자를 입력하세요 (jpg, jpeg, png)" << endl;
		return;
	}

	// 이미지 저장
	imwrite(save_path, img_save);

	cout << "수정된 이미지가 " << save_path << "에 저장되었습니다." << endl;
}

//트랙바 밝기
void chg_brightness(int pos, void* userdata) {

	Mat* img_ptr = (Mat*)userdata;

	Mat dst; 
	img_ptr->convertTo(dst, -1, 1, pos); // 밝기 조절

	imshow("image", dst);
	img_save = dst.clone();
	
}

//트랙바 RGB
void chg_RGB(int, void* userdata) {
	//Mat* img_ptr = &img;
	Mat* img_ptr = (Mat*)userdata;
	Mat dstImage = img.clone();

	//픽셀을 탐색
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b rgb_temp = img.at<Vec3b>(i, j);	//탐색한 픽셀의 RGB값을 저장하는 배열
			Vec3b rgb_sum;							//조절한 RGB값을 저장해는 변수

			for (int k = 0; k < 3; k++) {
				int val = rgb_temp[k] + rgb[k];		//현재 픽셀의 RGB값과 조절한 RGB값을 더한다
				rgb_sum[k] = (val < 0) ? 0 : (val > 255) ? 255 : val;
			}

			dstImage.at<Vec3b>(i, j) = rgb_sum;		//복사한 이미지 배열에 조절한 RGB값을 넣는다
		}
	}

	imshow("image", dstImage);
	img_save = dstImage.clone(); 
	
}

void onMouse(int event, int x, int y, int flags, void* param) {

	switch (event) {
		case EVENT_LBUTTONDOWN:
			pt = Point(x, y);
			break;
		case EVENT_LBUTTONUP:
			
			break;
		case EVENT_MOUSEMOVE:
			if (flags & EVENT_FLAG_LBUTTON) {
				line(img, pt, Point(x, y), Scalar(0, 0, 0), 2);
				imshow("image", img);
				pt = Point(x, y);

				img_save = img.clone();
			}
			break;
		default:
			break;
		}

}

