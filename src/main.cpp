#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

	ofAppGLFWWindow window;
    window.setWindowTitle("videoBenchmark test");
	ofSetupOpenGL(&window, 960, 640, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
