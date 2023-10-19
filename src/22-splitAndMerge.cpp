/**
 * SEGMENTAZIONE: SPLIT AND MERGE
 * 
 * 1. Dividere in quattro quadranti tutte le regioni per cui il predicato Q risulta falso
 * 
 * 2. Quando non è più possibile dividere le regioni, applicare il processo di
 *    merging a tutte le regioni adiacenti 𝑅i 𝑒 𝑅j , per cui 𝑄( 𝑅i ∪ Rj ) = Vero
 * 
 * 3. Il processo termina quando non è più possibile effettuare unioni
 * 
 * NB: 
 *  ¡ Solitamente si definisce una dimesione minima della regione oltre la 
 *    quale non si effettua lo split
 * 
 *  ¡ Per ragioni di efficienza, la fase di merge si può eseguire se il predicato è
 *    vero per le singole regioni adiacenti (non si effettua l'unione)
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

double smthreshold; // 35.8
int tsize; // 4

// Nodo quadTree
class TNode {
    private:
        Rect region;
        TNode *UL, *UR, *LL, *LR; // 4 figli
        vector<TNode*> merged; // Regioni unite
        vector<bool> mergedB = vector<bool>( 4, false ); // Quali regioni possono essere unite
        Scalar mean = Scalar( 0, 0, 0 );
        double stddev = 0.0f;
    public:
        TNode( Rect R ){ region = R; UL = UR = LL = LR = nullptr; };
        
        TNode* getUL(){ return UL; };
        TNode* getUR(){ return UR; };
        TNode* getLL(){ return LL; };
        TNode* getLR(){ return LR; };
        
        void setUL( TNode* N ){ UL = N; };
        void setUR( TNode* N ){ UR = N; };
        void setLL( TNode* N ){ LL = N; };
        void setLR( TNode* N ){ LR = N; };
        
        double getStdDev(){ return stddev; };
        Scalar getMean(){ return mean; };
        
        void setStdDev( double stddev ){ this->stddev = stddev; };
        void setMean( Scalar mean ){ this->mean = mean; };

        void addRegion( TNode* R ){ merged.push_back(R); };
        vector<TNode*>& getMerged(){ return merged; };

        Rect& getRegion(){ return region; };

        void setMergedB( int i ){ mergedB[i] = true; };
        bool getMergedB( int i ){ return mergedB[i]; };
};

TNode* split( Mat& img, Rect R ) {

    TNode* root = new TNode(R);

    // Calcolo deviazione standard e media della regione 
    Scalar stddev, mean;
    meanStdDev( img(R), mean, stddev );

    // Set stddev e mena della radice del sotto albero radicato ( inizialmente tutta l'immagine )
    root->setMean( mean );
    root->setStdDev( sqrt( pow( stddev[0] + stddev[1] + stddev[2], 2 ) ) );

    // Se la dimensione della regione è maggiore di quella minima
    // E se la dev std è minore di th ( La dispersione dei valori di grigio è maggiore di un threshold )
    if ( R.width > tsize && root->getStdDev() > smthreshold ) {

        Rect ul( R.x, R.y, R.height / 2, R.width / 2 );
        root->setUL( split( img, ul ) ); // Ricorsione x 4

        Rect ur( R.x, R.y + R.width / 2, R.height / 2, R.width / 2 );
        root->setUR( split( img, ur ) );

        Rect ll( R.x + R.height / 2, R.y, R.height / 2, R.width / 2 );
        root->setLL( split( img, ll ) );

        Rect lr( R.x + R.height / 2, R.y + R.width / 2, R.height / 2, R.width / 2 );
        root->setLR( split( img, lr ) );
    }
    rectangle( img, R, Scalar(0) );
    return root;
}

/**
 * _______ _____
 * |UL|UR| |0|1|
 * ------- -----
 * |LL|LR| |3|2|
 * ------- -----
 * 
 * Merge a partire dalla radice
 * 
 * Considera le varie regioni e vede se all'interno delle stesse regioni va a considerare le regioni
 * adiacenti ( vedo a coppie se posso fare il merge )
 * 
 * Se il predicato è vero setto il setter merge cosi da capire che vanno unite
 * E poi vedo se una delle restanti due può essere unita
 * ( Se la regione è stata splittata non posso riunire tutte e 4 altrimenti non sarebbero state splittate )
 * 
 * Unisco le due regioni e poi vedo il predicato sulla regione unita
 * 
 * Molte unione non porteranno ad un reale merge aumento di costo di complessità
 * 
 * Qui abbiamo una segmentazione meno precisa, ma con un costo minore
 * 
 * Altrimenti ricorsivamente vado a fare il merge
 * 
 * Se non riesco a fare nessun merge, vado a fare il merge delle sotto regioni
 * L'idea è che cerco di fare prima il merge delle regioni più grandi e poi via via quelle più piccole
*/
void merge( TNode* root ) {

    if ( root->getRegion().width > tsize && root->getStdDev() > smthreshold ) {
        if ( root->getUL()->getStdDev() <= smthreshold && root->getUR()->getStdDev() <= smthreshold ) { //UL-UR
            root->addRegion( root->getUL() ); root->setMergedB(0);
            root->addRegion( root->getUR() ); root->setMergedB(1);
            if ( root->getLL()->getStdDev() <= smthreshold && root->getLR()->getStdDev() <= smthreshold ) {
                root->addRegion( root->getLL() ); root->setMergedB(3);
                root->addRegion( root->getLR() ); root->setMergedB(2);
            } else {
                merge( root->getLL() );
                merge( root->getLR() );
            }
        } else if ( root->getUR()->getStdDev() <= smthreshold && root->getLR()->getStdDev() <= smthreshold ) { //UR-LR
            root->addRegion( root->getUR() ); root->setMergedB(1);
            root->addRegion( root->getLR() ); root->setMergedB(2);
            if ( root->getUL()->getStdDev() <= smthreshold && root->getLL()->getStdDev() <= smthreshold ) {
                root->addRegion( root->getUL() ); root->setMergedB(0);
                root->addRegion( root->getLL() ); root->setMergedB(3);
            } else {
                merge( root->getUL() );
                merge( root->getLL() );
            }
        } else if ( root->getLL()->getStdDev() <= smthreshold && root->getLR()->getStdDev() <= smthreshold ) { //LL-LR
            root->addRegion( root->getLL() ); root->setMergedB(3);
            root->addRegion( root->getLR() ); root->setMergedB(2);
            if ( root->getUL()->getStdDev() <= smthreshold && root->getUR()->getStdDev() <= smthreshold ) {
                root->addRegion( root->getUL() ); root->setMergedB(0);
                root->addRegion( root->getUR() ); root->setMergedB(1);
            } else {
                merge( root->getUL() );
                merge( root->getUR() );
            }
        } else if ( root->getUL()->getStdDev() <= smthreshold && root->getLL()->getStdDev() <= smthreshold ) { //UL-LL
            root->addRegion( root->getUL() ); root->setMergedB(0);
            root->addRegion( root->getLL() ); root->setMergedB(3);
            if ( root->getUR()->getStdDev() <= smthreshold && root->getLR()->getStdDev() <= smthreshold ) {
                root->addRegion( root->getUR() ); root->setMergedB(1);
                root->addRegion( root->getLR() ); root->setMergedB(2);
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

/**
 * Dopo il merge segment assegna alla regione il livello di grigio per la visualizzazione
*/
void segment( TNode* root, Mat& img ) {

    vector<TNode*> tmp = root->getMerged();

    if ( !tmp.size() ) {
        segment( root->getUL(), img );
        segment( root->getUR(), img );
        segment( root->getLR(), img );
        segment( root->getLL(), img );
    } else {
        Scalar val = Scalar( 0, 0, 0 );
        
        for ( auto x: tmp )
            val += x->getMean();
        
        val[0] /= tmp.size();
        val[1] /= tmp.size();
        val[2] /= tmp.size();

        for ( auto x: tmp )
            img( x->getRegion() ) = val;

        if ( tmp.size() > 1 ) {
            
            if ( !root->getMergedB(0) )
                segment( root->getUL(), img );
            
            if ( !root->getMergedB(1) )
                segment( root->getUR(), img );
            
            if ( !root->getMergedB(2) )
                segment( root->getLR(), img );
            
            if ( !root->getMergedB(3) )
                segment( root->getLL(), img );
        }
    }
}

// Disegna un contorno alle regioni NB: Facoltativo
void drawMerged( TNode* root, Mat& img ) {
    vector<TNode*> tmp = root->getMerged();

    if ( !tmp.size() ) {
        drawMerged( root->getUL(), img );
        drawMerged( root->getUR(), img );
        drawMerged( root->getLR(), img );
        drawMerged( root->getLL(), img );
    } else if ( tmp.size() == 1 ) {
        rectangle( img, tmp[0]->getRegion(), Scalar(0) );
    } else {
        
        if ( ( root->getMergedB(0) && root->getMergedB(1)) || ( root->getMergedB(2) && root->getMergedB(3) ) )
            rectangle( img, Rect( tmp[0]->getRegion().x, tmp[0]->getRegion().y, 
                       tmp[0]->getRegion().height, tmp[0]->getRegion().width * 2 ), Scalar(0) );
        
        else if ( ( root->getMergedB(0)&&root->getMergedB(3)) || ( root->getMergedB(1) && root->getMergedB(2) ) )
            rectangle( img, Rect(tmp[0]->getRegion().x, tmp[0]->getRegion().y,
                       tmp[0]->getRegion().height * 2, tmp[0]->getRegion().width ), Scalar(0) );
        
        if ( !root->getMergedB(0) )
            drawMerged( root->getUL(), img );
        if ( !root->getMergedB(1) )
            drawMerged( root->getUR(), img );
        if ( !root->getMergedB(2) )
            drawMerged( root->getLR(), img );
        if ( !root->getMergedB(3) )
            drawMerged( root->getLL(), img );
    }  
}

int main( int argc, char** argv ) {

    if ( argc != 2 ) {
        cout << "\nusage: " << argv[0] << " <name_image>" << endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    smthreshold = 40;
    tsize = 6;

    GaussianBlur( src, src, Size( 5, 5 ), 0, 0 ); // Evito rumore

    // Estrazione porzione quadrata dell'immagine
    int exponent = log( min( src.cols, src.rows ) ) / log (2);
    int s = pow( 2.0, ( double ) exponent );
    Rect square = Rect( 0, 0, s, s );
    src = src( square ).clone();
    Mat srcSplit = src.clone();
    Mat srcMer = src.clone();
    Mat srcSeg = src.clone();

    TNode* root = split( srcSplit, Rect( 0, 0, src.rows, src.cols ) );
    merge( root );
    drawMerged( root, srcMer );
    segment( root, srcSeg );

    imshow( "src", src );
    imshow( "Split", srcSplit );
    imshow( "Merge", srcMer );
    imshow( "Segment", srcSeg );
    waitKey(0);

    return 0;
}