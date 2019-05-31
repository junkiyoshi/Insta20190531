#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);

	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	// Link pixels And pixels_mat
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(19);

	this->fbo.begin();
	ofClear(0);
	ofTranslate(this->fbo.getWidth() * 0.5, this->fbo.getHeight() * 0.5);

	ofColor color;
	for (int i = 0; i < 8; i++) {

		auto len = 20;
		auto radius = ofRandom(50 + i * 30, 80 + i * 25);
		auto deg_start = ofRandom(360) + ofGetFrameNum() * ofRandom(2, 4);
		auto deg_end = deg_start + ofRandom(100, 270);
		vector<glm::vec2> vertices_1, vertices_2;
		for (int deg = deg_start; deg < deg_end; deg++) {

			vertices_1.push_back(glm::vec2((radius - len * 0.5) * cos(deg * DEG_TO_RAD), (radius - len * 0.5) * sin(deg * DEG_TO_RAD)));
			vertices_2.push_back(glm::vec2((radius + len * 0.5) * cos(deg * DEG_TO_RAD), (radius + len * 0.5) * sin(deg * DEG_TO_RAD)));
		}
		reverse(vertices_2.begin(), vertices_2.end());

		color.setHsb(ofRandom(255), 180, 255);
		ofSetColor(color);

		ofBeginShape();
		ofVertices(vertices_1);
		ofVertices(vertices_2);
		ofEndShape(true);
	}

	this->fbo.end();
	// Copy Fbo To pixels
	this->fbo.readToPixels(this->pixels);

	// Blur Process
	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(19, 15), 8, 6);
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Shadow
	ofSetColor(255, 192);
	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(-5, -5);

	// Draw Fbo Image 
	ofSetColor(255);
	this->fbo.draw(-30, -30);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}