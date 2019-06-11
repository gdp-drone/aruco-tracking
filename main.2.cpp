#include <opencv2/core.hpp>
#include "tracker-arb/TrackerARB.h"

#define DEFAULT_PORT 0

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    const float markerLength = 3.62;
    const float markerSeparation = 2.63;
    const int markersX = 6;
    const int markersY = 8;
    CVCalibration cvl("CalibParams.txt");
    TrackerARB tracker(cvl, markerLength, markerSeparation, markersX, markersY, true);
    
    int port = argc > 1 ? stoi(argv[1]) : DEFAULT_PORT;
    
    tracker.startStreamingTrack(port);
    return 0;
}
