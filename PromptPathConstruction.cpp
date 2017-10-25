/*Test code for path construction of prompt gamma rays*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "PromptPathConstruction.hpp"

using namespace std;

vector<point> points;                                           //Vector to hold all points recorded
vector<double> pathNums;                                        //Vector to hold unique path ID's
vector<point> path;                                             //Vector to hold unique paths
vector<line> lineConstants;                                     //Vector to hold all As and Bs for each set of points 

promptPathConstruction::promptPathConstruction(){
   	buildPaths();                                               //Calls build path function to make a collection of all points
                                         
    for(i=0;i<points.size();i++){                               //Loops through points vector and find all unique paths
        if(points[i].getPathID() != points[i+1].getPathID()){
            pathNums.push_back(points[i].getPathID());          //If unique path is found it adds it to a paths vector
        }
    }

    for(j=0;j<pathNums.size();j++){                             //Runs for the amount of all unique paths
        path.clear();                                           //clears path vector each itteration
        for(i=0;i<points.size();i++){                           //Runs for all points
            if(points[i].getPathID() == pathNums[j]){           //Compairs all point's IDs to the unique paths to find all values in one path
                path.push_back(points[i]);                      //Adds points to path vector
                    if(points[i].getLayerNum() >= points[i+1].getLayerNum()){ //If path changes direction the internal loop breaks (shown by decreasing layer number) 
                        break;
                    }
                }
        }
        if(path[0].getLayerNum()==0 && path.size() > 2){        //If the path is generated outside the sensor it will go through the first layer. Also Path size must be greater than 2 for least squares
            line tempLine;                                      //Generate temp line object
            tempLine.setValues(aFromXY(path,'x',bFromXY(path,'x')),bFromXY(path,'x'),aFromXY(path,'y',bFromXY(path,'y')),bFromXY(path,'y'));        //Sets the values for temp line via a and b functions
            lineConstants.push_back(tempLine);                  // Adds temp line to lineConstants vector
        }
    }

    for(i=0;i<lineConstants.size();i++){                        //The following nested loop checks for duplicate objects within the vector and removes them (compares a value to all following values) 
        for(j=i+1;j<lineConstants.size();j++){
            if(lineConstants[i].getAx()==lineConstants[j].getAx() && lineConstants[i].getBx()==lineConstants[j].getBx() &&
            lineConstants[i].getAy()==lineConstants[j].getAy() && lineConstants[i].getBy()==lineConstants[j].getBy())
                lineConstants.erase(lineConstants.begin()+(i));
        }
    }

    for(i = 0 ; i < lineConstants.size()-2; i++){               //The following nested loop compares all lines to all other lines to acheve various z values the bragg peak could be coming from
        for(j = i+1 ; j < lineConstants.size()-1; j++){
            for(k = j+1 ; k < lineConstants.size(); k++){
                braggCoords(findZ(lineConstants[i] , lineConstants[j] , lineConstants[k]),lineConstants[i] , lineConstants[j] , lineConstants[k]);
            }
        }
    }


        
}

double promptPathConstruction::bFromXY(vector<point> & currPath, char xOrY){
    sumOfZ = 0;
    sumOfYOrX = 0;
    sumOfZSqr = 0;
    sumOfZYOrX = 0;

    b = 0;

    if(xOrY == 'x') varMod = 0;                 
    if(xOrY == 'y') varMod = 1;

    for(i=0;i<currPath.size();i+=2){
        sumOfZ += (currPath[i].getLayerNum() + .5);
    }

    for(i=varMod;i<currPath.size();i+=2){
        sumOfYOrX += currPath[i].getBarNum() - 0.5;
    }

    for(i=0;i<currPath.size();i+=2){
        sumOfZSqr += pow((currPath[i].getLayerNum()+.5),2);
    }

    for(i=varMod;i<currPath.size();i+=2){
        if(xOrY == 'x')
            sumOfZYOrX += ((currPath[i].getLayerNum()+.5) * (currPath[i].getBarNum()-0.5));
        if(xOrY == 'y')
            sumOfZYOrX += ((currPath[i].getLayerNum()-.5) * (currPath[i].getBarNum()-0.5));
    }

    b = (((sumOfZYOrX * sumOfZ)/sumOfZSqr)-sumOfYOrX) / ((pow(sumOfZ,2)/sumOfZSqr)-(currPath.size()/2));

    return(b);
}

double promptPathConstruction::aFromXY(vector<point> & currPath, char xOrY, double bVal){

    sumOfZ = 0;
    sumOfZSqr = 0;
    sumOfZYOrX = 0;

    a = 0;

    if(xOrY == 'x') varMod = 0;
    if(xOrY == 'y') varMod = 1;

    for(i=0;i<currPath.size();i+=2){
        sumOfZ += (currPath[i].getLayerNum() + .5);
    }

    for(i=0;i<currPath.size();i+=2){
        sumOfZSqr += pow((currPath[i].getLayerNum()+.5),2);
    }

    for(i=varMod;i<currPath.size();i+=2){
        if(xOrY == 'x')
            sumOfZYOrX += ((currPath[i].getLayerNum()+.5) * (currPath[i].getBarNum()-0.5));
        if(xOrY == 'y')
            sumOfZYOrX += ((currPath[i].getLayerNum()-.5) * (currPath[i].getBarNum()-0.5));
    }

    a = (sumOfZYOrX - (bVal * sumOfZ))/sumOfZSqr;
    
    return(a);
}

double promptPathConstruction::findZ(line line1, line line2, line line3){
    
    zVal=0;
    
    a1 = line1.getAx();
    b1 = line1.getBx();
    c1 = line1.getAy();
    d1 = line1.getBy();

    a2 = line2.getAx();
    b2 = line2.getBx();
    c2 = line2.getAy();
    d2 = line2.getBy();

    a3 = line3.getAx();
    b3 = line3.getBx();
    c3 = line3.getAy();
    d3 = line3.getBy();

    zVal = (((a1*d2)-(a1*d3)+(b1*c2)-(b1*c3)-(c1*b2)+(c1*b3)+(d1*a2)+(d1*a3)+(a2*d3)+(b2*c3)-(c2*b3)-(d2*a3)) / ((a1*c2)-(a1*c3)-(c1*a2)+(c1*a3)+(a2*c3)-(c2*a3)))/(-20);

    return(zVal);

}

void promptPathConstruction::braggCoords(double z,line line1, line line2, line line3){

    a1 = line1.getAx();
    b1 = line1.getBx();
    c1 = line1.getAy();
    d1 = line1.getBy();

    a2 = line2.getAx();
    b2 = line2.getBx();
    c2 = line2.getAy();
    d2 = line2.getBy();

    a3 = line3.getAx();
    b3 = line3.getBx();
    c3 = line3.getAy();
    d3 = line3.getBy();

    /*cout << a1 << " " << b1 << " " << c1 << " " << d1 << endl;
    cout << a2 << " " << b2 << " " << c2 << " " << d2 << endl;
    cout << a3 << " " << b3 << " " << c3 << " " << d3 << endl;*/

    y1 = z*a1 + b1;
    x1 = z*c1 + d1;
 
    y2 = z*a2 + b2;
    x2 = z*c2 + d2;

    y3 = z*a2 + b2;
    x3 = z*c3 + d3;

    avgX = (x1+x2+x3)/30;

    avgY = (y1+y2+y3)/30;

    cout << "(x,y,z)" << "(" << avgX << "," << avgY << "," << z << ")" << endl;

}

void promptPathConstruction::buildPaths(){
    string fileName = "PromptData.txt";                                         //File name string to be opened
    ifstream filein;                                                            //Declear ifstream for txt file reading
    filein.open(fileName.c_str());                                              //Opens file
    
    if(filein.is_open()){
        while(filein.good()){                                                   //while file has more lines
            getline(filein, rawPData);                                          //Get Line and place in a temp string contanter for manipulation

            arrPos = 0;                                                         //Counter for tempPData posistion in array
            for(i = 0; i < rawPData.length();i++){                              //Loop through the temp string rawPdata 
                tillNxtSpace = 0;                                               //Counter for till the next space in the string it resets each time it loops
                if(i == 0||isspace(rawPData.at(i-1))){                          //If the start of the string or a space starts nested loop
                    for(j = i+1; j < rawPData.length();j++){                    //Starts one after i which is the start or a space and goes to the end if needed
                        tillNxtSpace++;                                         //Counter till next space is inncrimeneded as the loop runs
                        if(isspace(rawPData.at(j))||j == rawPData.length()-1){  //If the loop reaches the end or another space it adds that value to the temp array
                            if(j == rawPData.length()-1) tillNxtSpace++;        //Adds one to the end to get the last character
                   	        tempPData[arrPos] = strtod(rawPData.substr(i,tillNxtSpace).c_str(),NULL);       //Adds to the arrPos
                            arrPos++;                                           //Inciments array position for next input value
                            break;                                              //Breaks nested loops to continue forward
                        }
                    }
                }
            }

            point temp;                                                         //Creates temp point object to be placed in the points vector 
            temp.setValues(tempPData[0], tempPData[1], tempPData[2], tempPData[3]);     //Sets the values of the temp point object
            points.push_back(temp);                                             //Adds the object to the points vector
        }
        filein.close();                                                         //Closes input file
    }

    else cout << "Error opening file." << endl;                                 //If file not found error is shown
}

int main(){
    promptPathConstruction ppc;
    return 0;
}
