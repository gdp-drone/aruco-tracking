#include <opencv2/core.hpp>
#include <iostream>

#include "tracker-arb/TrackerARB.h"
#include "tracker-hsv/TrackerHSV.h"
#include "agent-xb/AgentXB.h"

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
  unique_ptr<Tracker> tracker_s(
      new TrackerARB(cvl, markerLength, markerSeparation, markersX, markersY, markerDict, true));
  
  // Instantiate tracker of larger markers
  markerLength = 9.89;
  markerSeparation = 15.15;
  markersX = 2;
  markersY = 2;
  markerDict = 4;
  
  unique_ptr<Tracker> tracker_l(
      new TrackerARB(cvl, markerLength, markerSeparation, markersX, markersY, markerDict, true));
  
  AgentXB trackingAgent(AgentXB::MODE_GREEDY, true);
  trackingAgent.addTracker(move(tracker_l));
  trackingAgent.addTracker(move(tracker_s));
  
  string sFilename = "./output/Test Video";
  auto saveVideo = true;
  
  int port = argc > 1 ? stoi(argv[1]) : DEFAULT_PORT;
  
  trackingAgent.startVideoTrack("./data/TestTakeOff4.mp4", saveVideo, sFilename);
  
//  trackingAgent.trackers[0]->startVideoTrack("./data/TestTakeOff4.mp4", saveVideo, sFilename);
//    tracker.startStreamingTrack(port, saveVideo, sFilename);
  return 0;
}
