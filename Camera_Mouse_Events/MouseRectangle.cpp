// Camera Mouse Events sample code

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

cv::Mat frame;		// input image
cv::Mat roiImg;
const char* src_window_name = "Select ROI";

cv::Point point1, point2;
cv::Rect roiRectangle;
int drag = 0, select_flag = 0;
bool callback = false;

// Implement mouse callback
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		/* left button clicked. ROI selection begins */
		point1 = cv::Point(x, y);
		drag = 1;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		/* mouse dragged. ROI being selected */
		cv::Mat img1 = frame.clone();
		point2 = cv::Point(x, y);
		rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 2, 8, 0);
		cv::imshow(src_window_name, img1);
	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		point2 = cv::Point(x, y);
		//roiRectangle = cv::Rect(point1.x, point1.y, x - point1.x, y - point1.y);
		roiRectangle = cv::Rect(point1.x, point1.y, point2.x - point1.x, point2.y - point1.y);
		drag = 0;
		roiImg = frame(roiRectangle);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		/* ROI selected */
		select_flag = 1;
		drag = 0;
	}
}

int main() {
	char charCheckForEscKey = 0;
	cv::VideoCapture capWebcam(1);		// declare a VideoCapture object and associate to webcam, 0 => use 1st laptop webcam, 1=> use 2nd usb webcam!

	if (capWebcam.isOpened() == false) {				// check if VideoCapture object was associated to webcam successfully
		std::cout << "error: capWebcam not accessed successfully\n\n";	// if not, print error message to std out
		return(0);														// and exit program
	}
	capWebcam >> frame;
	cv::namedWindow(src_window_name, CV_WINDOW_AUTOSIZE);
	cv::imshow(src_window_name, frame);
	cv::setMouseCallback(src_window_name, mouseHandler, 0);


	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {		// until the Esc key is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(frame);		// get next frame

		if (!blnFrameReadSuccessfully || frame.empty()) {		// if frame not read successfully
			std::cout << "error: frame not read from webcam\n";		// print error message to std out
			break;													// and jump out of while loop
		}		
		capWebcam >> frame;
		if (frame.empty())
			break;
		cv::imshow(src_window_name, frame);
		if (select_flag == 1)
		{
			cv::imshow("ROI", roiImg); /* show the image bounded by the ROI rectangle */
		}
		charCheckForEscKey = cv::waitKey(1);			// delay (in ms) and get key press, if any
	}	// end while

	return(0);
}