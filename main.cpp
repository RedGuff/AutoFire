#include <iostream>
#include <fstream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

string ini(string file, string section, string variable)  // TODO: Optimisation! Lazy!
{
// TODO: File!
    return "";
}

string generator(int difficulte)
{
    const int MAXX = 16; // The format.
    const int MAXY = 12; // The format.
    string resultRoom = "";
    int unlucky = 0; // Lucky now: new room.
    int XBall=1; // Horizontal position of the ball.
    int YBall=1; // Vertical position of the ball.
    int XBox=1; //  Horizontal position of the box.
    int YBox=1; //  Vertical position of the box.
// (The "do", 3 lines after, can be here, but it'd be a very little slower.)
    XBall = rand()%MAXX + 1;
    YBall = rand()%MAXY + 1;
    do // In order to restart if Ball === Box.
    {

        XBox = rand()%MAXX + 1; // ?
// (The "do", 3 lines earlier, can be here, it'd be a very little speeder if You are lucky.
//    but it'd be a very little slower if you are not.)
        YBox = rand()%MAXY + 1; // ?
        unlucky++; // Very unlucky if restart a lot.
    }
    while ((XBox == XBall) && (YBox == YBall) && (unlucky<42));   // If you are VERY unlucky:
    if (unlucky==42) // The rand function is ok, in general.
    {
        XBall = 1; // Horizontal default position of the ball.
        YBall = 1; // Vertical default position of the ball.
        XBox = 10; // Horizontal default position of the box.
        YBox = 10; // Vertical default position of the box.
    }

// cout << XBall << endl << YBall << endl << XBox << endl<< YBox << endl; // Tests ok.
// For each position of the room:
    for (int j = 1;  j < MAXY + 1; j++)
    {
        for (int i = 1; i < MAXX + 1; i++ )
        {
            if ((XBall == i) && (YBall == j))
            {
                resultRoom += "B"; // Ball.
            }
            else if((XBox == i) && (YBox == j))
            {
                resultRoom += "C"; // "Caisse" is French for Box.
            }
            else if (difficulte>0)
            {
                resultRoom += "F"; // TODO: pas que.
                difficulte--; // The more the room is difficult, the more fire You have.
            }
            else
            {
                resultRoom += "V"; // TODO: pas que.
            }
        }
    }

    return resultRoom;
}


int main()
{
    srand (time(NULL)); // Usual init; Pseudo rand is enough.
    string language = "en";
    language = ini("AutoFire.txt", "[User]", "language");

// LGG
    cout << "Fire (http://stephane.mitermite.free.fr/)" << endl;
    cout << "AutoFire 0.1 (RedGuff)." << endl;
    cout << "nbRoom? : " << endl; // File. // Max string in C++ ?
    int nbRoom = 1; //  Numbr of rooms of the file.
    int diffStart = 1; // Start easy (the first room).
    int diffEnd = 1; // End hard (the last room).
    string name = "temp";
    cin >> nbRoom; // Pas texte !!! Non nul, entier, positif, pas trop grand.
    if (nbRoom >1)
    {
cout << "diffStart ? : " << endl; // File.
    cin >> diffStart; // Pas texte !!!

        cout << "diffEnd ? : " << endl; // File.

        cin >> diffEnd; // Pas texte !!!
    }
    else {
cout << "diff ? : " << endl; // File.
    cin >> diffStart; // Pas texte !!!


    }
    cout << "name ? (Just the name: I will add .niv): " << endl; // File.
    cin >> name;
    name +=".niv";
// BCVWF

    ofstream myFile(name.c_str(), ios::app); // Open to append, ".niv" included.
    if (!myFile)
    {
        cerr << "Error1, file!";

    }
    else
    {
        //      cerr << "Error2, file ok!/n"; // Test ok.
                int diff = diffStart;
        for (int room = 1; room < nbRoom +1; room++) // For each room to make.
        {
            if(nbRoom>1){
                diff = diffStart + ((room -1) * ((diffEnd - diffStart)/ (nbRoom-1))); // Approx affine.
        }

            string roomText = ""; // Room now.
            roomText=generator(diff);

            //roomText="TEST"; // Test ok.
         //   cout << "room : " << room << endl; // Test ok.
          //  cout << "diff : " << diff << endl; // Test ok.
           // cout << roomText << endl; // Test ok.


            myFile << roomText;// append file.
        }// End room.
        myFile.close(); // Not needed.

        cout << "OK." << endl;
    }
    return 0;
}
