#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <opencv2/aruco.hpp>

#include <iostream>
#include <fstream>
#include <chrono>
#include <sys/timeb.h>

using namespace cv;
using namespace std;

int CLOCK()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

double _avgdur=0;
int _fpsstart=0;
double _avgfps=0;
double _fps1sec=0;

double avgdur(double newdur)
{
    _avgdur=0.98*_avgdur+0.02*newdur;
    return _avgdur;
}

double avgfps()
{
    if(CLOCK()-_fpsstart>1000)
    {
        _fpsstart=CLOCK();
        _avgfps=0.7*_avgfps+0.3*_fps1sec;
        _fps1sec=0;
    }
    
    _fps1sec++;
    return _avgfps;
}

int startMarkerTracking(const Mat &cameraMatrix, const Mat &distanceCoeffs, const float arucoSquareDimension) {
    Mat frame;
    
    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCorners;
//    aruco::DetectorParameters markerParams;
    
    auto markerDict = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    
    VideoCapture vid(0);
    
    if (!vid.isOpened()) return -1;
    
//    namedWindow("Camera Feed", WINDOW_AUTOSIZE);
    
    vector<Vec3d> rotationVec, translationVec;
    
    int frameno=0;
    ofstream outfile;
    outfile.open("VsionData.txt", fstream::out);
    cout << "FrameNo \t Timestamp \t RunningTime \t FPS \t MarkerID \t Dist1 \t Dist2 \t Dist3\n";
    outfile << "FrameNo \t Timestamp \t RunningTime \t FPS \t MarkerID \t Dist1 \t Dist2 \t Dist3\n";
    
    while (true) {
        clock_t start=CLOCK();
        if (!vid.read(frame)) break;
//        Mat reduced_noise, processed, dst;
    
        aruco::detectMarkers(frame, markerDict, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffs, rotationVec,
                                         translationVec);
        //        blur(frame, reduced_noise, Size(7,7) );
        //        Canny(reduced_noise, processed, 400, 1200, 5);
        //        dst.create( frame.size(), frame.type() );
        //        dst = Scalar::all(0);
        double dur = CLOCK()-start;
        auto curr = chrono::system_clock::now();
        time_t currT = chrono::system_clock::to_time_t(curr);
        frameno++;
        if(markerIds.size() == 0) {
            cout << frameno << "\t"
                 << ctime(&currT) << "\t"
                 << avgdur(dur) << "\t"
                 << avgfps() << "\t"
                 << -1 << "\t"
                 << "0\t0\t0\t\n";
            outfile << frameno << "\t"
                 << ctime(&currT) << "\t"
                 << avgdur(dur) << "\t"
                 << avgfps() << "\t"
                 << -1 << "\t"
                 << "0\t0\t0\t\n";
        }
        for (int i = 0; i < markerIds.size(); i++) {
//            aruco::drawAxis(frame, cameraMatrix, distanceCoeffs, rotationVec[i], translationVec[i], 0.08f);
            cout << frameno << "\t"
                 << ctime(&currT) << "\t"
                 << avgdur(dur) << "\t"
                 << avgfps() << "\t"
                 << markerIds[i];
            outfile << frameno << "\t"
                 << ctime(&currT) << "\t"
                 << avgdur(dur) << "\t"
                 << avgfps() << "\t"
                 << markerIds[i];
            
            for(int j = 0; j < 3; ++j) {
                cout << "\t" << translationVec[i][j];
                outfile << "\t" << translationVec[i][j];
            }
            cout << endl;
            outfile << endl;
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
