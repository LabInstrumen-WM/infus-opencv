#include <iomanip>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

Mat frame, src_gray;
int thresh = 50;
RNG rng(12345);
double value;
void thresh_callback(int, void *);

void thresh_callback(int, void *) {
  Mat canny_output;
  value = (double)threshold(src_gray, canny_output, 0, 255,
                            CV_THRESH_BINARY | CV_THRESH_OTSU);
  // Canny(src_gray, canny_output, thresh, thresh * 2, 3);
  vector<vector<Point>> contours;
  findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
  vector<Moments> mu(contours.size());
  for (size_t i = 0; i < contours.size(); i++) {
    mu[i] = moments(contours[i]);
  }
  vector<Point2f> mc(contours.size());
  for (size_t i = 0; i < contours.size(); i++) {
    // add 1e-5 to avoid division by zero
    mc[i] = Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                    static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)));
    cout << "mc[" << i << "]=" << mc[i] << endl;
  }
  Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
  for (size_t i = 0; i < contours.size(); i++) {
    Scalar color =
        Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
    drawContours(drawing, contours, (int)i, color, 2);
    circle(drawing, mc[i], 4, color, -1);
  }
  imshow("Contours", drawing);
  cout << "\t Info: Area and Contour Length \n";
  for (size_t i = 0; i < contours.size(); i++) {
    cout << " * Contour[" << i << "] - Area (M_00) = " << std::fixed
         << std::setprecision(2) << mu[i].m00
         << " - Area OpenCV: " << contourArea(contours[i])
         << " - Length: " << arcLength(contours[i], true) << endl;
  }
}

int main(int, char **) {
  Mat canny_output;
  Rect rect; /* bounding box */
  VideoCapture cap;
  int deviceID = 0;
  int apiID = cv::CAP_ANY;
  cap.open(deviceID + apiID);
  cap.set(CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CAP_PROP_FRAME_HEIGHT, 720);
  while (1) {
    cap.read(frame);
    rect = Rect(760, 0, 380, 720); // infus kanan
    frame = frame(rect);
    cvtColor(frame, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    const char *source_window = "Source";
    namedWindow(source_window);
    imshow(source_window, frame);
    const int max_thresh = 255;
    createTrackbar("Canny thresh:", source_window, &thresh, max_thresh,
                   thresh_callback);
    thresh_callback(0, 0); // image moment
    int key = waitKey(1);
    if ((key == 27) || (key == 32))
      break;
  }
  cap.release();
  destroyAllWindows();
  return 0;
}
