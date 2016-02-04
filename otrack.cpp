#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


int iLowH = 0;
int iHighH = 179;
int iLowS = 0;
int iHighS = 255;
int iLowV = 0;
int iHighV = 255;

const int MAX_NUM_OBJECTS = 50;
const int MIN_OBJECT_AREA = 20*20;

string intToString(int number)
{

  std::stringstream ss;
  ss << number;
  return ss.str();

}
void createWindows()
{

  namedWindow("Control", CV_WINDOW_AUTOSIZE);

  cvCreateTrackbar("LowH", "Control", &iLowH, 179);//Hue(0-179)
  cvCreateTrackbar("HighH","Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255);//Saturation(0-255)
  cvCreateTrackbar("HighS","Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value(0-255)
  cvCreateTrackbar("HighV","Control", &iHighV, 255);
  return;
}
void drawCrosshair(int x, int y,int obj_radius, Mat &frame)
{
  circle(frame,Point(x,y),obj_radius,Scalar(0,255,0),2);
  putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);
}

void morphOps(Mat &thresh)
{
  //morphological openings (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
  dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

  //morphological closings (fill small holes in the foreground)
  dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
}

int main(int argc, char** argv)
{
  VideoCapture cap(0);

  if (!cap.isOpened())
  {
    cout<<"Cannot load camera"<< endl;
    return -1;
  }

  cap.set(CV_CAP_PROP_FPS, 60); //setting capture fps to 60

  createWindows();

  while (true)
  {
    Mat imgOriginal;

    bool bSuccess = cap.read(imgOriginal);

    if (!bSuccess) //break loop if not successful
    {
      cout << "Cannot read a frame from video stream" << endl;
      break;
    }

    Mat imgHSV;

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);


    imshow("Thresholded Image", imgThresholded); //show thresholded image
    imshow("Original",imgOriginal);//show original image
    imshow("HSV Image",imgHSV);

    if(waitKey(30) == 27) //wait for esc key press for 30ms, if esc key is pressed, break loop
    {
      cout << "esc key pressed by user" << endl;
      break;
    }

  }

  return 0;

}
