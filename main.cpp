// TODO:
// 1 boucle de poursuite de parcours.
// Revoir la boucle de comptage, pour obtenir le nombre de salles souhaité.
// Tests et débuggages.
// Fichiers de langues (ou documentation et la langue reste (l'anglais)).

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
const int HZ = 16; // Format of Fire
const int VT = 12; // Format of Fire
/* Capital letters = format of Fire, lowercase letters = my choice for provisional cases.
W Wall.
V Void.
F Flam.
a ashes, former flam.
u Unknown, for start.
C Crate.
B Ball.
*/
// I know that int is enough, but the compute wants long unsigned int to avoid warnings.
// It's useless, but without warning, the debugging is easier.
int verbose = 0;
long unsigned int XB = 0;
long unsigned int YB = 0;
long unsigned int XC = 0;
long unsigned int YC = 0;

int readMapChar2D(vector<vector<char>> &mapLevel) { // Ok.
    cout << "mapLevel:" << endl;
    for (long unsigned int indiceVt = 0; indiceVt < mapLevel[0].size(); indiceVt++) {
        for (long unsigned int indiceHz = 0; indiceHz < mapLevel.size(); indiceHz++) {
            cout << mapLevel[indiceHz][indiceVt];
            cout << "  " ;
            }
        cout << "" << endl;
        }
    return 0; // Ok.
    }

bool testMove(vector<vector<char>> &mapLevel, long unsigned int XB, long unsigned int YB, bool moveBall, int dX, int dY) {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> randFloat(0, 1);
    if ((dX==0) && (dY==0)) {
        if (verbose>2) {
            cerr << "After change object 1?" << endl;
            }

        return false; // To avoid bad loop: restart random move.
        }
    // Next position to try:
    long unsigned int XB0 = XB;
    long unsigned int YB0 = YB;
    long unsigned int XC0 = XC;
    long unsigned int YC0 = YC;
    if (moveBall) {
        XB0 = XB+dX ;
        YB0 = YB+dY  ;
        }
    else { // Crane to move.
        XC0 = XC+dX ;
        YC0 = YC+dY;
        }
    if (((XB==XC) && (YB==YC)) || ((XB0==XC) && (YB0==YC)) || ((XB==XC0) && (YB==YC0))) {
        return false;
        }
    if ((XB0<0) || (XC0 <0) || (YB0<0) ||  (YC<0) || (XB0>-1 + mapLevel.size()) || (XC0>-1 + mapLevel.size()) || (YC0>-1 + mapLevel[0].size())|| (YB0>-1 + mapLevel[0].size())) { // Out.
        return false;
        }
    if ( !moveBall && (mapLevel[XC0][YC0]=='u')) {
        if (randFloat(rd)<0.2) {
            mapLevel[XC0][YC0] = 'W';
            return false;
            }
        else {
            mapLevel[XC0][YC0] = 'V';
            return true;
            }
        }
    if ( moveBall && (mapLevel[XB0][YB0]=='u')) {
        if (randFloat(rd)<0.3) {
            mapLevel[XB0][YB0] = 'W';
            return false;
            }
        else if (randFloat(rd)<0.45) {
            mapLevel[XB0][YB0] = 'a';
            return true;
            }
        else {
            mapLevel[XB0][YB0] = 'V';
            return true;
            }
        }
    if ((mapLevel[XB0][YB0]=='W')||(mapLevel[XC0][YC0]=='W')) {
        return false;
        }
    if ( !moveBall && (mapLevel[XC0][YC0]=='F')) {
        return false;
        }
    if ( moveBall && (mapLevel[XB0][YB0]=='F')) {
        mapLevel[XB0][YB0] = 'a';
        return true;
        }
    if ((moveBall && (mapLevel[XB0][YB0]=='V'))||(!moveBall && (mapLevel[XC0][YC0]=='V'))) {
        return true;
        }
    if (verbose>3) {
        clog << "Reste :" << endl;
        clog << "moveBall: " << moveBall << endl;
        clog << "XB0: " << XB0 << endl;
        clog << "XC0: " << XC0 << endl;
        clog << "YC0: " << YC0 << endl;
        clog << "YB0: " << YB0 << endl;
        clog << "XC: " << XC << endl;
        clog << "YC: " << YC << endl;
        clog << "XB: " << XB << endl;
        clog << "YB: " << YB << endl;
        clog << "mapLevel[XB][YB]: " << mapLevel[XB][YB] << endl;
        clog << "mapLevel[XC][YC]: " << mapLevel[XC][YC] << endl;
        clog << "mapLevel[XB0][YB0]: " << mapLevel[XB0][YB0] << endl;
        clog << "mapLevel[XC0][YC0]: " << mapLevel[XC0][YC0] << endl;
        }
    return true;
    }

void roomMaker(vector<vector<char>> &mapLevel, int diffSalle = 42) {
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> randDir(1, 6); // Change object, or direction.
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> randFloat(0, 1);
    bool moveBall=true; // If false: move crate.
    if (randFloat(rd)<0.5) {
        moveBall= false;

        }
    if (verbose>0) {
        clog << "\ndiffSalle: " << diffSalle <<". ";
        }
// Tirage :
    int dX = 0;
    int dY = 0;
    for (int diffEnCours=diffSalle-1; diffEnCours>=0; diffEnCours--) {
        do {
            if (verbose>2) {
                clog << " diffEnCours: " << diffEnCours  << ". ";
                }
            int direction = randDir(dev); // Change position or object.
            //       clog << "direction: " << direction << endl;
            switch (direction) {
                case 1:
                    dX = 1;
                    dY = 0;
                    break;
                case 2:
                    dX = 0;
                    dY = 1;
                    break;
                case 3:
                    dX = -1;
                    dY = 0;
                    break;
                case 4:
                    dX = 0;
                    dY = -1;
                    break;
                default:

                    moveBall = !moveBall;
                    break;
                }
            if (verbose>2) {
                clog << "direction: " << direction  << ". ";
                clog << "dX: " << dX  << ". ";
                clog << "dY: " << dY  << ". ";
                }
            }

        while (!testMove(mapLevel, XB, YB, moveBall, dX, dY));
        do {

            // arrêt que si obstacle : do while, tests et enregistrements en carte.
            if (moveBall) {
                if ((mapLevel[XB][YB]!='a') && (mapLevel[XB][YB]!='F') && (mapLevel[XB][YB]!='W')) {
                    mapLevel[XB][YB]='V';
                    }
                XB = XB+dX ;
                YB = YB+dY ;
                if ((mapLevel[XB][YB]!='a') && (mapLevel[XB][YB]!='F') && (mapLevel[XB][YB]!='W')) {
                    mapLevel[XB][YB]='V';
                    }
                }
            else {
                if ((mapLevel[XB][YB]!='a') && (mapLevel[XB][YB]!='F') && (mapLevel[XB][YB]!='W')) {
                    mapLevel[XB][YB]='V';
                    }
                XC = XC+dX;
                YC = YC+dY;
                if ((mapLevel[XB][YB]!='a') && (mapLevel[XB][YB]!='F') && (mapLevel[XB][YB]!='W')) {
                    mapLevel[XB][YB]='V';
                    }
                }

            }
        while (testMove(mapLevel, XB, YB, moveBall, dX, dY));
        //   readMapChar2D(mapLevel);
        if (verbose>2) {
            clog << "moveBall: " << moveBall << endl;
            clog << "XC: " << XC << endl;
            clog << "XB: " << XB << endl;
            clog << "YC: " << YC << endl;
            clog << "YB: " << YB << endl;
            }
        }
    }

void help(string language = "en") { // File TODO!
    if (language=="eo") {
        cout << "AutoFire V0.9.5  https://github.com/RedGuff/AutoFire/" << endl;
        cout << "Fire (http://stephane.mitermite.free.fr/)" << endl;
        cout << "Uzu:" << endl;
        cout << "Autofire [NovaCxambro.niv NombroDaCxambro KomencaMalfacilajxo FinaMalfacilajxo [lingvo [babilado]]]" << endl;
        cout << "Ekzemple: " << endl;
        cout << "Autofire Cxambro.niv 10 10 42" << endl;
        cout << "Alie, normaj valoroj estos prenitaj!" << endl;
        cout << "Se la etendo .niv ne ekzistas, gxi estos aldonita." << endl;
        cout << "Se la dezirata dosiero ekzistas, nombro estos metitaj:" << endl;
        cout << "a => a.niv" << endl;
        cout << "b.niv => b0.niv" << endl;
        cout << "c.niv => c0.niv and then c.niv => c1.niv" << endl;
        cout << "d0.niv => d01.niv, kompreneble !" << endl;
        }
    else if (language=="fr") {
        cout << "AutoFire V0.9.0  https://github.com/RedGuff/AutoFire/" << endl;
        cout << "Fire (http://stephane.mitermite.free.fr/)" << endl;
        cout << "Usage :" << endl;
        cout << "Autofire [NouvSalle.niv nbSalle diffCommencement diffFin [langage [bavardage]]]" << endl;
        cout << "Par example : " << endl;
        cout << "Autofire Salle.niv 10 10 42" << endl;
        cout << "Des valeurs standards seront prises, sinon !" << endl;
        cout << "Si l\'extension .niv n'existe pas, elle sera mise." << endl;
        cout << "Si le fichier voulu existe, un nombre sera mis :" << endl;
        cout << "a => a.niv" << endl;
        cout << "b.niv => b0.niv" << endl;
        cout << "c.niv => c0.niv et ensuite c.niv => c1.niv" << endl;
        cout << "d0.niv => d01.niv, bien entendu !" << endl;
        }
    else { // en = default.
        cout << "AutoFire V0.9.0  https://github.com/RedGuff/AutoFire/" << endl;
        cout << "Fire (http://stephane.mitermite.free.fr/)" << endl;
        cout << "Usage:" << endl;
        cout << "Autofire [NextLevel.niv nbRoom diffStart diffEnd [language [verbose]]]" << endl;
        cout << "For example: " << endl;
        cout << "Autofire YAFireLevel.niv 10 10 42" << endl;
        cout << "Else, some default value will be used!" << endl;
        cout << "If the extention .niv does not exists, it will be added." << endl;
        cout << "If the file wanted exists, a number will be added:" << endl;
        cout << "a => a.niv" << endl;
        cout << "b.niv => b0.niv" << endl;
        cout << "c.niv => c0.niv and then c.niv => c1.niv" << endl;
        cout << "d0.niv => d01.niv, of course!" << endl;
        }
    }

string checkFile(string file = "") { // Ok.
    if (file.size()>4) {
        if (file.substr(file.size()-4)==".niv") {
            file = file.substr(0, file.size()-4)       ;
            }
        }
    int initFile = 0;
    string   file0 = file;
    while (ifstream(file0 + ".niv")) {
        if (verbose>3) {
            cerr << "file++" << endl;
            }
        file0 = file + to_string(initFile);
        initFile++;
        }
    return file0 + ".niv";
    }

bool checkMap(vector<vector<char>> &mapLevel, long unsigned int XCinit, long unsigned int YCinit, long unsigned int XBinit, long unsigned int YBinit) {
    int nbFlam = 0;
    int nbCrate = 0;
    int nbBall = 0;
    for (long unsigned int indiceVt = 0; indiceVt < mapLevel[0].size(); indiceVt++) {
        for (long unsigned int indiceHz = 0; indiceHz < mapLevel.size(); indiceHz++) {
            if ((indiceHz == XCinit) && (indiceVt == YCinit)) {
                mapLevel[indiceHz][indiceVt]='C';
                }
            if ((indiceHz == XBinit) && (indiceVt == YBinit)) {
                mapLevel[indiceHz][indiceVt]='B';
                }
            if ((mapLevel[indiceHz][indiceVt]=='a')|| (mapLevel[indiceHz][indiceVt]=='F')) {
                mapLevel[indiceHz][indiceVt]='F';
                nbFlam++    ;
                }
            if ((mapLevel[indiceHz][indiceVt]=='C')) {
                nbCrate++    ;
                }
            if ((mapLevel[indiceHz][indiceVt]=='B')) {
                nbBall++    ;
                }
            }
        }
    if ((nbFlam > 3) && (nbCrate == 1) && (nbBall == 1)) {
        if (verbose>0) {
            clog << "Map True and fun enough!" << endl;
            }
        return true;
        }
    else {
        if (verbose>2) {
            clog << "Nocheck:" << endl;
            clog << "nbFlam: " << nbFlam << endl;
            clog << "nbCrate: " << nbCrate << endl;
            clog << "nbBall: " << nbBall << endl;
            }
        return false;
        }
    }

int main(int argc, char* argv[]) { // Arguments: file(.niv) nbRoom DiffStart DiffEnd
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> randHZ(0, HZ-1); // Not from 1.
    uniform_int_distribution<std::mt19937::result_type> randVT(0, VT-1); // Not from 1.
    uniform_int_distribution<std::mt19937::result_type> randVTm1(1, VT-1); // Not from YC, not YC.
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> randFloat(0, 1);
    int difficult = 10;
    string file = "AutoFire";
    int nbRoom = 10;
    int diffStart = 300;
    int diffEnd = 500;
    string lgg = "en";
    if ((argc == 5) || (argc == 6) || (argc == 7)) {
        file = argv[1];
        nbRoom = atoi(argv[2]);
        diffStart = atoi(argv[3]);
        diffEnd = atoi(argv[4]);
        }
    if ((argc == 6)  || (argc == 7)) {
        lgg = argv[5];
        }
    if (argc == 7) {
        verbose = atoi(argv[6]);
        }
    help(lgg);
    file = checkFile(file);
    if (verbose>0) {
        clog << "file: " << file << endl;
        clog << "nbRoom: " << nbRoom << endl;
        clog << "DiffStart: " << diffStart << endl;
        clog << "DiffEnd: " << diffEnd << endl;
        }
    ofstream myFile(file.c_str(), ios::app); // Open to append, ".niv" included.
    if (!myFile) {
        cerr << "Error1! Impossible to write in the file: " << file << "!" << endl;
        }
    else {
        for (int room=1; room<nbRoom + 1; ++room) {
            if (verbose>0) {
                clog << "room: " << room << endl;
                }
            if(nbRoom>1) {
                difficult = diffStart + ((room -1) * ((double)(diffEnd - diffStart)/ (nbRoom-1))); // Interpolation or this room. Pb with INT (approx) solved by double.
                };
            if (verbose>2) {
                clog << "init" << endl;
                }
            vector<vector<char>> mapLevel(HZ, vector<char> (VT, 'u'));
            XB = randHZ(dev);
            YB = randVT(dev);
            XC = randHZ(dev);
            YC = randVT(dev);
            if ((XB==XC) && (YB==YC)) {
                YC=(YB + randVTm1(dev) )% (VT);// Other.
                }
            long unsigned int XBinit = XB;
            long unsigned int YBinit = YB;
            long unsigned int XCinit = XC;
            long unsigned int YCinit = YC;
            if (verbose>1) {
                clog << "XBinit: " << XBinit << endl;
                clog << "YBinit: " << YBinit << endl;
                clog << "XCinit: " << XCinit << endl;
                clog << "YCinit: " << YCinit << endl;
                }
            mapLevel[XB][YB] = 'B';
            mapLevel[XC][YC] = 'C';
            if (verbose>2) {
                clog << "read" << endl;
                readMapChar2D(mapLevel);
                }
            if (verbose>0) {
                cout << "Init ok." << endl;
                }
            roomMaker(mapLevel, difficult);
            if (verbose>1) {
                clog << "Result:" << endl;
                readMapChar2D(mapLevel);
                }
            if (!checkMap(mapLevel, XCinit, YCinit, XBinit,  YBinit )) {
                nbRoom--;
                }
            else {
                for (long unsigned int indiceVt = 0; indiceVt < mapLevel[0].size(); indiceVt++) {
                    for (long unsigned int indiceHz = 0; indiceHz < mapLevel.size(); indiceHz++) {
                        if (mapLevel[indiceHz][indiceVt]=='u') {
                            if (randFloat(rd)<0.1) {
                                mapLevel[indiceHz][indiceVt]='W';
                                }
                            else {
                                mapLevel[indiceHz][indiceVt]='V';
                                }
                            }
                        if (mapLevel[indiceHz][indiceVt]=='a') {
                            mapLevel[indiceHz][indiceVt]='F';
                            }
                        if ((indiceHz == XCinit) && (indiceVt == YCinit)) {
                            mapLevel[indiceHz][indiceVt]='C';
                            }
                        if ((indiceHz == XBinit) && (indiceVt == YBinit)) {
                            mapLevel[indiceHz][indiceVt]='B';
                            }
                        myFile << mapLevel[indiceHz][indiceVt];
                        }
                    }
                }
            }
        }
    cout << "\n\nOk with the file: " << file << "!" << endl;
    return 0;
    }
