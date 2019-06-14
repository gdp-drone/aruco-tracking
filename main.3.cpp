#include <opencv2/core.hpp>

#include <chrono>
#include "tracker-arb/TrackerARB.h"
#include "tracker-hsv/TrackerHSV.h"

#define DEFAULT_PORT 0

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    CVCalibration cvl("CalibParams.txt");
  
    // Instantiate tracker of small markers
    float markerLength = 3.62;
    float markerSeparation = 2.63;
    int markersX = 6;
    int markersY = 8;
    int markerDict = 0;
    TrackerARB tracker_s(cvl, markerLength, markerSeparation, markersX, markersY, markerDict, true);
  
    // Instantiate tracker of larger markers
    markerLength = 9.89;
    markerSeparation = 15.15;
    markersX = 2;
    markersY = 2;
    markerDict = 4;
    TrackerARB tracker_l(cvl, markerLength, markerSeparation, markersX, markersY, markerDict, true);
    
    
    auto start = chrono::system_clock::now();
    time_t start_time = std::chrono::system_clock::to_time_t(start);
    ostringstream filename;
    filename << "Test Video - " << std::ctime(&start_time) << ".avi";
    string sFilename = filename.str();
    bool saveVideo = false;
    
    int port = argc > 1 ? stoi(argv[1]) : DEFAULT_PORT;
    
    tracker_l.startVideoTrack("./data/TestTakeOff4.mp4", saveVideo, sFilename);
//    tracker.startStreamingTrack(port, saveVideo, sFilename);
    return 0;
}
