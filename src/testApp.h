#pragma once

#include "ofMain.h"
#include "ofxThreadedVideo.h"

#define MAX_THREADED_VIDEOS 1

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		int runTest;
		bool processing;

		int pickVideo;
        ofDirectory files;

		// TEST 1: based on ofxThreadedVideoBenchmark
        int lastLoadTime, loadInterval;
        unsigned long long startVideoLoadingTime;

        ofxThreadedVideo videos[MAX_THREADED_VIDEOS];

        void threadedVideoEvent(ofxThreadedVideoEvent & event);


        // TEST 2: investigate glitches
        ofPtr<ofVideoPlayer> movie;
        bool mediaLoaded;
        unsigned long long startVideoPlayTime;
        int glitchInterval;
        string testMovie;
        float sinus;
		
};
