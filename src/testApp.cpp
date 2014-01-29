#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    processing = true;
    runTest = 2;

    pickVideo = 0;
    files.allowExt("mov");
    files.allowExt("mp4");
    files.listDir(""); // put a video path here with several video files in a folder

    if (runTest==1) {

        // TEST 1: based on ofxThreadedVideoBenchmark
        // load one video after another, and note down loadtime, FPS and video FPS

        lastLoadTime = ofGetElapsedTimeMillis();
        loadInterval = 10000;   // every 10 seconds
        startVideoLoadingTime = ofGetElapsedTimeMicros();
        cout << endl;

        ofBackground(0, 0, 0);

        for(int i = 0; i < MAX_THREADED_VIDEOS;i++){
        	string filePath = files.getPath(pickVideo++);
            videos[i].loadMovie(filePath);
            videos[i].play();
            // videos[i].setVerbose(true);
            ofAddListener(videos[i].threadedVideoEvent, this, &testApp::threadedVideoEvent);
        }

    } else if (runTest==2) {

        testMovie = files.getPath(pickVideo);

        movie = ofPtr<ofVideoPlayer>( new ofVideoPlayer() );
        movie->setPixelFormat(OF_PIXELS_BGRA);

        mediaLoaded = false;
        glitchInterval = 500;

        lastLoadTime = ofGetElapsedTimeMillis();
        mediaLoaded = movie->loadMovie(testMovie);
        if (mediaLoaded) {
            movie->play();
            movie->setLoopState( OF_LOOP_NORMAL);
        }

    }

    sinus = 0;
}

//--------------------------------------------------------------
void testApp::update(){

    if (runTest==1) {

        for(int i = 0; i < MAX_THREADED_VIDEOS;i++){
            videos[i].update();
        }
        if(processing && ofGetElapsedTimeMillis() - lastLoadTime >= loadInterval){
            if (pickVideo < files.numFiles()) {
                // log out FPS for previous video
                ofLogVerbose() << "FPS: \t\t" << ofGetFrameRate();
                ofLogVerbose() << "Video FPS: \t" << videos[0].getFrameRate() << "\n";
            }
            if (pickVideo+1 >= files.numFiles()) {
                // stop loading
                pickVideo++;
            } else {
                string filePath = files.getPath(pickVideo++);
                int i = (int)ofRandom(MAX_THREADED_VIDEOS);
                lastLoadTime = ofGetElapsedTimeMillis();
                startVideoLoadingTime = ofGetElapsedTimeMicros();
                videos[i].loadMovie(filePath);
                videos[i].play();
            }
        }

    } else if (runTest==2) {

        if (mediaLoaded) {
            movie->update();
        }

        if (processing && ofGetElapsedTimeMillis() - lastLoadTime >= glitchInterval) {
            // reload video
            if (pickVideo < files.numFiles()) {
                ofLogVerbose() << "FPS: \t\t" << ofGetFrameRate() << "\n";
            }
            if (pickVideo >= files.numFiles()) {
                // stop loading
            } else {
                testMovie = files.getPath(pickVideo);
                ofLogVerbose() << "file \t\t" << testMovie;
                mediaLoaded = false;
                startVideoLoadingTime = ofGetElapsedTimeMicros();
                lastLoadTime = ofGetElapsedTimeMillis();
                mediaLoaded = movie->loadMovie(testMovie);
                if (mediaLoaded) {
                    ofLogVerbose() << "loaded in \t" << (ofGetElapsedTimeMillis() - lastLoadTime) << " ms";
                    ofLogVerbose() << "loaded in \t" << (ofGetElapsedTimeMicros() - startVideoLoadingTime) << " us";
                    startVideoPlayTime = ofGetElapsedTimeMicros();
                    movie->play();
                    ofLogVerbose() << "play() \t" << (ofGetElapsedTimeMicros() - startVideoPlayTime) << " us";
                }
            }
        }

    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);


    if (runTest==1) {

        int xM = 0; int yM = 0;
        int tilesWide = 4;
        for(int i = 0; i < MAX_THREADED_VIDEOS; i++){

            float width = (ofGetWidth() / tilesWide);
            float height = width * (videos[i].getHeight() / videos[i].getWidth());

            if(xM == tilesWide - 1) yM++;
            xM = i%tilesWide;

            videos[i].draw(xM * width, yM * height, width, height);

        }

        ofSetColor(0, 255, 0);

        ostringstream os;
        os << "FPS: " << ofGetFrameRate() << " loadInterval = " << loadInterval << " ms" << endl;
        for(int i = 0; i < MAX_THREADED_VIDEOS;i++){
            os << i << " " << videos[i].getFrameRate() << " " << videos[i].getCurrentFrame() << " / " << videos[i].getTotalNumFrames() << " " << videos[i].getQueueSize() << " " << videos[i].getMovieName() << endl;
        }
        ofDrawBitmapString(os.str(), 20, ofGetHeight() - 20 * MAX_THREADED_VIDEOS);

    } else if (runTest==2) {

        if (mediaLoaded) {
            movie->draw(0,0);
        }

        ofSetColor(0, 255, 0);
        ostringstream os;
        os << "FPS: " << ofGetFrameRate() << " glitchInterval = " << glitchInterval << " ms" << endl;
        ofDrawBitmapString(os.str(), 20, ofGetHeight() - 20);
    }

    // draw something smooth to observing freezing of app
    ofSetColor(255,255,255);
    ofRect( ofGetWidth()/2.0 + sin(sinus+=0.03)*ofGetWidth()/2.3, 30, 10, 10);
}


//--------------------------------------------------------------
void testApp::threadedVideoEvent(ofxThreadedVideoEvent & event){
    // ofLogVerbose() << "VideoEvent: " << event.eventTypeAsString << " for " << event.path;
    ofLogVerbose() << "file \t\t" << event.path;
    ofLogVerbose() << "loaded in \t" << (ofGetElapsedTimeMillis() - lastLoadTime) << " ms";
    ofLogVerbose() << "loaded in \t" << (ofGetElapsedTimeMicros() - startVideoLoadingTime) << " us";
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case ' ':
        {
            if (runTest==1) {
                int i = (int)ofRandom(MAX_THREADED_VIDEOS);
                lastLoadTime = ofGetElapsedTimeMillis();
                videos[i].loadMovie(files.getPath(ofRandom(files.numFiles())));
                videos[i].play();
                break;

            } else if (runTest==2) {
                pickVideo++;
                break;
            }
        }
        case OF_KEY_UP:
            loadInterval = loadInterval + 10;
            break;
        case OF_KEY_DOWN:
            loadInterval = loadInterval - 10;
            break;
        case 'p':
            processing = !processing;
            break;
    }
    CLAMP(loadInterval, 0, INFINITY);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
