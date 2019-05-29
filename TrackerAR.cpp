#include "TrackerAR.h"
#include <opencv2/core>

TrackerAR::TrackerAR(Mat cameraMatrix, Mat distCoeffs, const float arucoSquareDimension)

int TrackerAR::startMarkerTracking(const Mat &cameraMatrix, const Mat &distanceCoeffs, const float arucoSquareDimension) {
  Mat frame;
  
  vector<int> markerIds;
  vector<vector<Point2f>> markerCorners, rejectedCorners;
  aruco::DetectorParameters markerParams;
  
  auto markerDict = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
  
  VideoCapture vid(0);
  
  if (!vid.isOpened()) return -1;
  
  namedWindow("Camera Feed", WINDOW_AUTOSIZE);
  
  vector<Vec3d> rotationVec, translationVec;
  
  while (true) {
    if (!vid.read(frame)) break;
    Mat reduced_noise, processed, dst;
    
    aruco::detectMarkers(frame, markerDict, markerCorners, markerIds);
    aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffs, rotationVec,
                                     translationVec);
    //        blur(frame, reduced_noise, Size(7,7) );
    //        Canny(reduced_noise, processed, 400, 1200, 5);
    //        dst.create( frame.size(), frame.type() );
    //        dst = Scalar::all(0);
    for (int i = 0; i < markerIds.size(); i++) {
      aruco::drawAxis(frame, cameraMatrix, distanceCoeffs, rotationVec[i], translationVec[i], 0.08f);
    }
    imshow("Camera Feed", frame);
    if (waitKey(60) >= 0) break;
  }
  return 1;
}
