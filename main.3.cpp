#include <opencv2/core.hpp>

#include <chrono>
#include "tracker-arb/TrackerARB.h"
#include "tracker-hsv/TrackerHSV.h"

#define DEFAULT_PORT 0

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    const float markerLength = 3.62;
    const float markerSeparation = 2.63;
    const int markersX = 6;
    const int markersY = 8;
    CVCalibration cvl("CalibParams.txt");
    TrackerHSV tracker(cvl, true);
  
    auto start = chrono::system_clock::now();
    time_t start_time = std::chrono::system_clock::to_time_t(start);
    ostringstream filename;
    filename << "Test Video - " << std::ctime(&start_time) << ".avi";
    string sFilename = filename.str();
    bool saveVideo = true;
    
    int port = argc > 1 ? stoi(argv[1]) : DEFAULT_PORT;
    
    tracker.startVideoTrack("TestTakeOff4.mkv");
//    tracker.startStreamingTrack(port, saveVideo, sFilename);
    return 0;
}
