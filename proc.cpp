#include <iostream>				
#include <stdio.h>              
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>         
#include <opencv2/highgui.hpp>		
#include <opencv2/imgproc.hpp>      
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>


using namespace cv;
using namespace std;

int main(int ac, char** av) {
	//사용할 객체 생성
	cv::VideoCapture cap("video/coffee.mp4");
	cv::VideoWriter videoWriter;
	cv::Mat videoFrame;
	cv::Mat select_ROI;

	//capture 오픈
	if (!cap.isOpened()) return 0;

	float videoFPS = cap.get(cv::CAP_PROP_FPS);
	//트랙커 설정
	cv::Ptr<cv::Tracker> trac = cv::TrackerCSRT::create();
	//비디오 첫 프레임 가져오기
	cap.read(videoFrame);
	//roi 설정 
	cv::Rect2d roi = cv::selectROI("Select ROI", videoFrame, false);
	destroyWindow("Select ROI");
	
	//트랙커에 트랙킹 할 roi설정
	trac -> init(videoFrame, roi);
	
	//비디오 재생
	while (cap.read(videoFrame))
	{
		//비디오 재생에 맞추어 roi 트랙킹하도록 설정
		if (trac->update(videoFrame, roi)) {
			select_ROI = videoFrame(Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width));
			cv::rectangle(videoFrame, roi, cv::Scalar(255, 0, 0), 2, 8);
		}
		
		//트랙킹한 비디오 저장
		videoWriter << select_ROI;
        
		//비디오 재생
		cv::imshow("select_ROI", select_ROI);
		cv::imshow("video", videoFrame);
		if (cv::waitKey(1000/ videoFPS) == 27) break;
	};
	
	//opencv끝나면 종료
	cap.release();
	destroyAllWindows;

	return 0;
}


