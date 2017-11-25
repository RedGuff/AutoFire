#include <iostream>
#include <fstream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
string readStupidlyINI(string file, string section, string variable)  // TODO: Optimisation! Lazy!
{
// ("AutoFire.ini", "[User]", "language")
// Problems if commands are "". TODO.

    string line = ""; // Line read in the file now.
    string sectionNow = ""; // The section we are in.
    bool sectionDone = false; // Was at least one time in the good section: the good section exists.
    string variableVal = ""; // The value of the variable.
    string equality = "="; // Separator, 1 char.
    ifstream flow(file.c_str());  // Open the file, to read it.
    if(!flow)
    {
        cerr << "ERROR: Impossible to open the file " << file << " to read it!" << endl;
        cerr << "ERREUR : Impossible d'ouvrir le fichier  " << file << " en lecture !" << endl;
        return ""; // => a replacement will be done.
    }
    else
    {
        do
// Recall: // for Douglas Quaid!
// .size();// size of a string
// .substr(start, length wanted or to the end); // Substring.
        {
            getline(flow, line);
            // cout << "line : " << line << endl; // Test ok.

            if ((line.substr(0, 1) == "[") && (line.substr(line.size() - 1) == "]")) // A section. Test ok.
            {
                sectionNow = line;
                // cout << "section : " << sectionNow << endl; // Test ok.
            }
            else if ((section == sectionNow) && (line.substr(0,variable.size()) == variable) && (equality ==(line.substr(variable.size(), 1 ))))   // If the size is good, it may be good.
            {
                variableVal=line.substr(1+ variable.size());; // +1 because of "=". // The rest of the line.
                // cout << "variableVal : " << variableVal << endl; // Test ok.
                return variableVal; // Ok: out!
            }
            else if (section == sectionNow)   // In the good section. The message or error can be different.
            {
                sectionDone = true;
            }
        }
        while (!(flow.eof()));
        // Not out after the end of the file: bad.
        flow.close();
        cerr << "Error when reading file " << file << ": " << variable << " is not in the section "<< section << "!" << endl; // Separated message possible for different problems.
        return ""; // => a replacement will be done.



    }
}

// Objet ? Pointeur ? Foncteur ?
// Better way to read INI:
// My own code, too.
// On y envoie le chemin et nom de l'INI, la saction, la variable.
// On regarde si le tableau existe.
// ???
// Ce tableau sera renvoyé en "return".
// Ce tableau s'agrandit d'une dimention par valeur au fur et à mesure.
// Ce tableau contient une rangée, pour chaque valeur, avec : id, fichier, section, variable, valeur.
// Si la valeur n'existe pas à la demande, on lit tout le fichier.
// Il parcourt le fichier ligne par ligne.
// Il fabrique (comme pour le comptage des bananes) un tableau de valeurs.





string generator(int difficulte, string strMethod) // Just make a room, with difficulty and method parameter.
{
    // For every method:
    const int MAXX = 16; // The format.
    const int MAXY = 12; // The format.
    string resultRoom = "";
    int unlucky = 0; // Lucky now: new room.
    int XBall=1; // Horizontal position of the ball.
    int YBall=1; // Vertical position of the ball.
    int XBox=1; // Horizontal position of the box.
    int YBox=1; // Vertical position of the box.
// (The "do", 3 lines after, can be here, but it'd be a very little slower, without benefit, but I accept the idea that "neither ball nor box are more responsible of "same place" ".)
    XBall = rand()%MAXX + 1;
    YBall = rand()%MAXY + 1;
    do // In order to restart if ballLocation === boxLocation.
    {

        XBox = rand()%MAXX + 1; // ?
// (The "do", 3 lines earlier, can be here, it'd be a very little speeder if You are lucky.
// but it'd be a very little slower if you are not.)
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

// Now, it depends of the method wanted:
// switch (strMethod) // switch is only for int.
    {

        if (strMethod == "0")  // difficulte = number of flames, in one simple line.
        {
// For each position of the room:
            for (int j = 1;  j < MAXY + 1; j++)
            {
                for (int i = 1; i < MAXX + 1; i++ )
                {
                    if ((XBall == i) && (YBall == j))
                    {
                        resultRoom += "B"; // Ball ("Balle" in French).
                    }
                    else if((XBox == i) && (YBox == j))
                    {
                        resultRoom += "C"; // Box ("Caisse" is French for Box).
                    }
                    else if (difficulte>0)
                    {
                        resultRoom += "F"; // Fire ("Feu" in French).
                        difficulte--; // The more the room is difficult, the more fire You have.
                    }
                    else
                    {
                        resultRoom += "V"; // Void ("Vide" in French).
                    }
                }
            }
            // End of method 0.
        }
        else if (strMethod == "1")
        {
            int NbRemaining = MAXX * MAXY ; // Number of remaining compartments. You can try to calculate it each time, but it's simpler like this.
            // difficulte = number of flames, in random places.
// For each position of the room:
            for (int j = 1;  j < MAXY + 1; j++)
            {
                for (int i = 1; i < MAXX + 1; i++ )
                {
                    //   NbRemaining--;
                    if ((XBall == i) && (YBall == j))
                    {
                        resultRoom += "B"; // Ball.
                    }
                    else if((XBox == i) && (YBox == j))
                    {
                        resultRoom += "C"; // Box.
                    }
                    else if (((double)rand()/(double)RAND_MAX)<((double)difficulte/(double)NbRemaining)) // If the probability of case to have a flame is good, depending of the real number of flames and of the number of flames to do.
//    (double)rand()/(double)RAND_MAX is always between 0 and 1.
// If not double: int => 000000011111.
                    {

                        resultRoom += "F"; // Fire.
                        difficulte--; // The more the room is difficult, the more fire You have.
                    }
                    else
                    {
                        resultRoom += "V"; // Void.
                    }
                    NbRemaining--; // End of this place(i;j).

                }
            } // End of for.


        }
        else if (strMethod == "2")   // End of method 1.
        {
            int NbRemaining = MAXX * MAXY ; // Number of remaining compartments. You can try to calculate it each time, but it's simpler like this.

            difficulte *=2; // difficulte = number of flames AND walls, in random places.
// For each position of the room:
            for (int j = 1;  j < MAXY + 1; j++)
            {
                for (int i = 1; i < MAXX + 1; i++ )
                {
                    //   NbRemaining--;
                    if ((XBall == i) && (YBall == j))
                    {
                        resultRoom += "B"; // Ball.
                    }
                    else if((XBox == i) && (YBox == j))
                    {
                        resultRoom += "C"; // Box.
                    }
                    else if (((double)rand()/(double)RAND_MAX)<((double)difficulte/(double)NbRemaining)) // If the probability of case to have a flame is good, depending of the real number of flames and of the number of flames to do.
//    (double)rand()/(double)RAND_MAX is always between 0 and 1.
// If not double: int => 000000011111.
                    {

                        resultRoom += "F"; // Fire.
                        difficulte--; // The more the room is difficult, the more fire You have.
                    }

                    else if (((double)rand()/(double)RAND_MAX)<((double)difficulte/(double)NbRemaining)) // If the probability of case to have a flame is good, depending of the real number of flames and of the number of flames to do.

                    {

                        resultRoom += "W"; // Fire.
                        difficulte--; // The more the room is difficult, the more wall You have.
                    }
                    else
                    {
                        resultRoom += "V"; // Void.
                    }
                    NbRemaining--; // End of this place(i;j).

                }
            } // End of for.
        } // End of method 2.
    } // End of all methods.
    return resultRoom;
}


int main()
{
    srand (time(NULL)); // Usual initialisation; Pseudo rand is enough.
    string language = "";
    language = readStupidlyINI("AutoFire.ini", "[User]", "language"); // A simple function is enough, in general,
    // (and a general is more ranked than a lieutenant).
    if (language == "")   // Problem with INI file.
    {
        string language = "en"; // If problem: english.
    }
    // cout << language << endl; // Test ok.
// LGG
    cout << "Fire (http://stephane.mitermite.free.fr/)" << endl;
    cout << "AutoFire 0.3 (RedGuff)." << endl;
    cout << "nbRoom? : " << endl; // File. // Max string in C++ ?
    int nbRoom = 1; // Numbr of rooms of the file.
    int diffStart = 1; // Start easy (the first room).
    int diffEnd = 1; // End hard (the last room).
    string name = "temp";
    cin >> nbRoom; // TODO: Pas texte !!! Non nul, entier, positif, pas trop grand.
    if (nbRoom >1)
    {
        cout << "diffStart ? : " << endl; // File.
        cin >> diffStart; // Pas texte !!!

        cout << "diffEnd ? : " << endl; // File.

        cin >> diffEnd; // Pas texte !!!
    }
    else
    {
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
        // cerr << "Error2, file ok!/n"; // Test ok.
        int diff = diffStart;
        for (int room = 1; room < nbRoom +1; room++) // For each room to make.
        {
            if(nbRoom>1)
            {
                diff = diffStart + ((room -1) * ((double)(diffEnd - diffStart)/ (nbRoom-1))); // Interpolation or this room. Pb with INT (approx) solved by double.
            }

            string roomText = ""; // Room now.
            do
            {
                string method="2"; // Futur INI.
                roomText=generator(diff, method);
                // string method="2"; // Last known good.
            }
            while (roomText=="");  // If problem of method.
            myFile << roomText;// append file.
        }// End room.
        myFile.close(); // Not needed.

        cout << "OK." << endl;
    }
    return 0;
}
