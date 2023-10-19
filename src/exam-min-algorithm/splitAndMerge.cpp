#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

const int th = 30;
const int tsize = 4;

class TNode {
	private:
		Rect region;
		TNode *UL, *UR, *LR, *LL;
		vector<TNode*> merged;
		vector<bool> mergedB = vector<bool>(4, false);
		Scalar mean = Scalar(0,0,0);
		double stddev = 0.0f;
	public:
		TNode(Rect region) { this->region = region; UL=UR=LR=LL=nullptr; }
		Rect& getRegion() { return region; }
		TNode* getUL() { return UL; }
		TNode* getUR() { return UR; }
		TNode* getLR() { return LR; }
		TNode* getLL() { return LL; }
		vector<TNode*>& getMerged() { return merged; }
		bool getMergedB(int i) { return mergedB.at(i); }
		Scalar getMean() { return mean; }
		double getStddev() { return stddev; }
		void setUL(TNode* UL) { this->UL = UL; }
		void setUR(TNode* UR) { this->UR = UR; }
		void setLR(TNode* LR) { this->LR = LR; }
		void setLL(TNode* LL) { this->LL = LL; }
		void setMergedB(int i) { mergedB.at(i) = true; }
		void setMean(Scalar mean) { this->mean = mean; }
		void setStddev(double stddev) { this->stddev = stddev; }
		void addRegion(TNode* region) { merged.push_back(region); }
};

TNode* split(Mat& src, Rect R) {
	TNode* root = new TNode(R);
	Scalar mean, stddev;
	meanStdDev( src(R), mean, stddev );
	root->setMean( mean );
	root->setStddev( sqrt(pow(stddev[0]+stddev[2]+stddev[1],2)) );
	if ( R.width > tsize && root->getStddev() > th ) {
		Rect ul( R.x, R.y, R.width/2, R.height/2  );
		root->setUL( split(src, ul) );
		Rect ur( R.x, R.y+R.width/2, R.width/2, R.height/2  );
		root->setUR( split(src, ur) );
		Rect lr( R.x+R.height/2, R.y+R.width/2, R.width/2, R.height/2  );
		root->setLR( split(src, lr) );
		Rect ll( R.x+R.height/2, R.y, R.width/2, R.height/2  );
		root->setLL( split(src, ll) );
	}
	rectangle(src, R, Scalar(0,0,0));
	return root;
}

void merge(TNode* root) {
	if ( root->getRegion().width > tsize && root->getStddev() > th ) {
		if ( root->getUL()->getStddev() <= th && root->getUR()->getStddev() <= th ) { // UL-UR
			root->addRegion( root->getUL() ); root->setMergedB(0);
			root->addRegion( root->getUR() ); root->setMergedB(1);
			if ( root->getLR()->getStddev() <= th && root->getLL()->getStddev() <= th ) {
				merge( root->getLR() ); root->setMergedB(2);
				merge( root->getLL() ); root->setMergedB(3);
			} else {
				merge( root->getLR() );
				merge( root->getLL() );
			}
		} else if ( root->getUR()->getStddev() <= th && root->getLR()->getStddev() <= th ) { // UR-LR
			root->addRegion( root->getUR() ); root->setMergedB(1);
			root->addRegion( root->getLR() ); root->setMergedB(2);
			if ( root->getLL()->getStddev() <= th && root->getUL()->getStddev() <= th ) {
				merge( root->getLL() ); root->setMergedB(3);
				merge( root->getUL() ); root->setMergedB(0);
			} else {
				merge( root->getLL() );
				merge( root->getUL() );
			}
		} else if ( root->getLR()->getStddev() <= th && root->getLL()->getStddev() <= th ) { // LR-LL
			root->addRegion( root->getLR() ); root->setMergedB(2);
			root->addRegion( root->getLL() ); root->setMergedB(3);
			if ( root->getUL()->getStddev() <= th && root->getUR()->getStddev() <= th ) {
				merge( root->getUL() ); root->setMergedB(0);
				merge( root->getUR() ); root->setMergedB(1);
			} else {
				merge( root->getUL() );
				merge( root->getUR() );
			}
		} else if ( root->getLL()->getStddev() <= th && root->getUL()->getStddev() <= th ) { // LL-UL
			root->addRegion( root->getLL() ); root->setMergedB(3);
			root->addRegion( root->getUL() ); root->setMergedB(0);
			if ( root->getUR()->getStddev() <= th && root->getLR()->getStddev() <= th ) {
				merge( root->getUR() ); root->setMergedB(1);
				merge( root->getLR() ); root->setMergedB(2);
			} else {
				merge( root->getUR() );
				merge( root->getLR() );
			}
		} else {
			merge( root->getUL() );
			merge( root->getUR() );
			merge( root->getLR() );
			merge( root->getLL() );
		}
	} else {
		root->addRegion( root );
		root->setMergedB(0); root->setMergedB(1); root->setMergedB(2); root->setMergedB(3); 
	}
}

void segment(Mat& src, TNode* root) {
	vector<TNode*> merged = root->getMerged();
	if (!merged.size()) {
		segment( src, root->getUL() );
		segment( src, root->getUR() );
		segment( src, root->getLR() );
		segment( src, root->getLL() );
	} else {
		Scalar intensity = Scalar(0,0,0);
		for (auto x: merged)
			intensity += x->getMean();
		intensity[0] /= merged.size();
		intensity[1] /= merged.size();
		intensity[2] /= merged.size();
		for (auto x: merged)
			src(x->getRegion()) = intensity;
		if ( merged.size() > 1 ) {
			if ( !root->getMergedB(0) )
				segment( src, root->getUL() );
			if ( !root->getMergedB(1) )
				segment( src, root->getUR() );
			if ( !root->getMergedB(2) )
				segment( src, root->getLR() );
			if ( !root->getMergedB(3) )
				segment( src, root->getLL() );
		}
	}
}

void drawMerged(Mat& src, TNode* root) {
	vector<TNode*> merged = root->getMerged();
	if ( !merged.size() ) {
		drawMerged( src, root->getUL() );
		drawMerged( src, root->getUR() );
		drawMerged( src, root->getLR() );
		drawMerged( src, root->getLL() );
	} else if ( merged.size() == 1 ) 
		rectangle( src, merged.at(0)->getRegion(), Scalar(0,0,0) );
	else {
		if ( (root->getMergedB(0) && root->getMergedB(1)) || (root->getMergedB(2) && root->getMergedB(3)) )
			rectangle( src, Rect( merged.at(0)->getRegion().x, merged.at(0)->getRegion().y, merged.at(0)->getRegion().height, merged.at(0)->getRegion().width * 2 ), Scalar(0,0,0) );
		
		if ( (root->getMergedB(0) && root->getMergedB(3)) || (root->getMergedB(1) && root->getMergedB(2)) )
			rectangle( src, Rect( merged.at(0)->getRegion().x, merged.at(0)->getRegion().y, merged.at(0)->getRegion().height * 2, merged.at(0)->getRegion().width ), Scalar(0,0,0) );
			
		if ( !root->getMergedB(0) )
            drawMerged( src, root->getUL() );
        if ( !root->getMergedB(1) )
            drawMerged( src, root->getUR() );
        if ( !root->getMergedB(2) )
            drawMerged( src, root->getLR() );
        if ( !root->getMergedB(3) )
            drawMerged( src, root->getLL() );
	}
}

void splitAndMerge(Mat& src) {
	GaussianBlur(src, src, Size(3,3), 0, 0);
	const int exp = log( min(src.rows, src.cols) ) / log(2);
	const int s = pow( 2, exp );
	Rect square( 0, 0, s, s );
	src = src(square).clone();
	Mat srcSplit = src.clone();
	Mat srcSeg = src.clone();
	Mat srcM = src.clone();
	TNode* root = split(srcSplit, Rect(0, 0, src.rows, src.cols));  
	merge(root);
	drawMerged(srcM, root);
	imshow("merge", srcM);
	imshow("split", srcSplit);
	segment( srcSeg, root );
	imshow("segment", srcSeg);
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1], IMREAD_COLOR );
	if(src.empty()) return -1;
	splitAndMerge(src);
	waitKey(0);
	return 0;
}
