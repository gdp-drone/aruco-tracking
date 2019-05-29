#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <opencv2/aruco.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int startMarkerTracking(const Mat &cameraMatrix, const Mat &distanceCoeffs, const float arucoSquareDimension) {
    Mat frame;
    
    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCorners;
//    aruco::DetectorParameters markerParams;
    
    auto markerDict = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    
    VideoCapture vid(1);
    
    if (!vid.isOpened()) return -1;
    
//    namedWindow("Camera Feed", WINDOW_AUTOSIZE);
    
    vector<Vec3d> rotationVec, translationVec;
    
    while (true) {
        if (!vid.read(frame)) break;
//        Mat reduced_noise, processed, dst;
        
        aruco::detectMarkers(frame, markerDict, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffs, rotationVec,
                                         translationVec);
        //        blur(frame, reduced_noise, Size(7,7) );
        //        Canny(reduced_noise, processed, 400, 1200, 5);
        //        dst.create( frame.size(), frame.type() );
        //        dst = Scalar::all(0);
        for (int i = 0; i < markerIds.size(); i++) {
//            aruco::drawAxis(frame, cameraMatrix, distanceCoeffs, rotationVec[i], translationVec[i], 0.08f);
            cout << "Marker Id: ";
            for(int j = 0; j < 3; ++j) {
                cout << translationVec[i][j] << ", ";
            }
            cout << endl;
        }
//        imshow("Camera Feed", frame);
//        if (waitKey(60) >= 0) break;
    }
    return 1;
}

void loadCalibrationMatrices(string fname, Mat& cameraMatrix, Mat& distanceCoeffs) {
    ifstream calibrationFile (fname);
    string line;
    if (calibrationFile.is_open()) {
        for(int r = 0; r < cameraMatrix.rows; r++) {
            for(int c = 0; c < cameraMatrix.cols; c++) {
                getline(calibrationFile, line);
                cameraMatrix.at<double>(r, c) = stod(line);
            }
        }
        for(int r = 0; r < distanceCoeffs.rows; r++) {
            for(int c = 0; c < distanceCoeffs.cols; c++) {
                getline(calibrationFile, line);
                distanceCoeffs.at<double>(r, c) = stod(line);
            }
        }
        calibrationFile.close();
    }
}

int main(int argc, char **argv) {
    const auto arucoSquareDimension = 0.0370f;

    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distanceCoeffs = Mat::zeros(5, 1, CV_64F);
    loadCalibrationMatrices("CalibParams.txt", cameraMatrix, distanceCoeffs);

    startMarkerTracking(cameraMatrix, distanceCoeffs, arucoSquareDimension);

    return 0;
}
