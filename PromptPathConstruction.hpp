/*Header for test code for path construction of prompt gamma rays*/

#ifndef PROMPTPATHCONSTRUCTION_HPP
#define PROMPTPATHCONSTRUCTION_HPP

#include <iostream>

#include "PromptPathConstruction_Point.cpp"
#include "PromptPathConstruction_LineEq.cpp"

using namespace std;

class promptPathConstruction{
    public:
        promptPathConstruction();
            double xPhantomSensor, pi, indxOfRefrac, theta, maxDistPos, timeErrZ, timeErrXY;
            int i,j,k;
        double bFromXY(vector<point> & currPath, char xOrY);
            double sumOfZ, sumOfYOrX, sumOfZSqr, sumOfZYOrX, b;
            int varMod;

        double aFromXY(vector<point> & currPath, char xOrY, double bVal);
            double a;

        double findZ(line line1, line line2, line line3);
            double a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3,zVal;

        void braggCoords(double z,line line1, line line2, line line3);
            double x1,y1,x2,y2,x3,y3,avgX,avgY;

        void buildPaths();
            string rawPData;
            double tempPData[4];
            int arrPos,tillNxtSpace;

        int indexOfSpace(string a);             
        
    private:
    
    protected:

};
#endif