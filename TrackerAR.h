//
//  TrackerAR.hpp
//  aruco_tracker
//
//  Created by Vishnu R Menon on 27/5/19.
//

#ifndef TrackerAR_h
#define TrackerAR_h

#include <stdio.h>

TrackerAR {
  Mat cameraMatrix, distCoeffs;
  const float arucoSquareDimension;
public:
  TrackerAR(const float arucoSquareDimension, Mat cameraMatrix, Mat distCoeffs);
  int TrackerAR::startMarkerTracking(vector<Vec3d> translationVec, vector<Vec3d> rotationVec);
}

#endif /* TrackerAR_h */
