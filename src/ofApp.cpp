#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //for debugging
    int runOnce = 0;
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    ofSetFrameRate(60);
    
    image.loadImage("pattern.jpg");
    image.resize(640, 480);
    
    int width = image.getWidth() / 10;
    int height = image.getHeight() / 10;
    
    //add vertex and texture coordinates every 10 pixels
    for (int y = 0; y < height; y++){
        for (int x = 0; x<width; x++){
            mesh.addVertex(ofPoint(x*10,y*10,ofRandom(0,10) ));
            mesh.addTexCoord(ofPoint(x*10,y*10));
        }
    }
    
    //tell which vertex corresponds to triangle
    int w = width;
    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            
            mesh.addIndex(x+y*w);
            mesh.addIndex(x+(y+1)*w);
            mesh.addIndex((x+1)+(y+1)*w);
            
            mesh.addIndex(x+y*w);
            mesh.addIndex((x+1)+(y+1)*w);
            mesh.addIndex((x+1)+y*w);
            
        }
    }
    
    glEnable(GL_DEPTH_TEST);
    
    //print out the coordinates of each vertex in indexed order
//    for( int i = 0; i < mesh.getNumVertices(); i++){
//        ofPoint p = mesh.getVertex(i);
//        cout<<"Vertex "<<i<<": "<<p.x<<", "<<p.y<<endl;
//        }

    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        //debugging
//        if (runOnce == 0){
//            for (int i = 0; i < ofGetWidth(); i+=10){
//                for(int j=0; j < ofGetHeight(); j+=10){
//                    float dist = kinect.getDepthPixelsRef()[j*(ofGetWidth()-1)+i];
//                    cout<<"Dist at "<<i<<", "<<j<<": "<<dist<<endl;
//                }
//            }
//            runOnce++;
//        }

    }
    
    float scaledTime = ofGetElapsedTimef()*.75;

   //first attempt to set the mesh z-value to grayscale value of kinect image - NOT WORKING
//    for( int i = 0; i < mesh.getNumVertices(); i++){
//        ofPoint p = mesh.getVertex(i);
//        float zVal = (kinect.getDepthPixelsRef()[p.y*(grayImage.getWidth()-1)+p.x])/10;
//        p.z = zVal;
//        cout<<"Vertex "<<i<<": "<<p.z<<endl;
//        mesh.setVertex(i, p);
//    }
    
    int width = grayImage.getWidth() / 10;
    int height = grayImage.getHeight() / 10;
    int imgW = grayImage.getWidth();
    
    //add vertex and texture coordinates every 10 pixels
    for (int y = 0; y < height; y++){
        for (int x = 0; x<width; x++){
            int i = x+y*width;
            ofPoint p = mesh.getVertex(i);
            int i2 = (x*10) + (y*10) * imgW;
            float zVal = kinect.getDepthPixelsRef()[i2];
            p.z = zVal;
            mesh.setVertex(i, p);
            
        }
    }
    
    //attempt 2 - crashes the program
//    for (int i = 0; i < kinect.width/10; i++){
//        for(int j=0; j < kinect.height/10; j++){
//            float gray = kinect.getDepthPixelsRef()[j*10*(ofGetWidth()-1)+i*10];
//            int ind = j*(kinect.width-1)+i;
//            ofPoint p = mesh.getVertex(ind);
//            p.z = gray;
//            mesh.setVertex(ind,p);
//        }
//    }
    
    }

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    ofSetColor(255);
    
    cam.begin();
    
    ofPushMatrix();
    ofTranslate(-image.getWidth()*.5,-image.getHeight()*.5);
    
    image.getTextureReference().bind();
    mesh.drawFaces();
    image.getTextureReference().unbind();
    
    
    ofTranslate(0,0,1);
    mesh.drawWireframe();
    ofPopMatrix();
    
    
    
    ofNoFill();
    ofBox(0,0,0,image.getWidth());
    cam.end();

    kinect.drawDepth(0,0, 200, 200);
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;

    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


