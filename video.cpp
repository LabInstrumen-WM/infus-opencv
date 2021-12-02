#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp>
#include <mysql/mysql.h>
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

int main(int, char**)
{
Mat frame; 
VideoCapture cap;
int deviceID=0;
int apiID=cv::CAP_ANY;
cap.open(deviceID + apiID);
cap.set(CAP_PROP_FRAME_WIDTH,1280);
cap.set(CAP_PROP_FRAME_HEIGHT,720);
namedWindow("frame",WINDOW_AUTOSIZE);
while(1)
{
	cap.read(frame);
	imshow("frame",frame);
	int key=waitKey(1);
if((key==27)||(key==32)) break;
}
cap.release();
destroyAllWindows();
return 0;
}
