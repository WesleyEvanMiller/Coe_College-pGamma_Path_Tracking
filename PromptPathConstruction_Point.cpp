//PromptPathConstruction path object

#include <iostream>

using namespace std;

class point{
    public:
        void setValues(double bar, double layer, double nstime, double path);
        double getPathID(){return pathID;}
        double getBarNum(){return barNum;}
        double getLayerNum(){return layerNum;}
        double getTime(){return time;}

        double pathID;                     //pathID *found in a earlier module
        double barNum;                     //bar number in sensor
        double layerNum;                   //layer number in sensor
        double time;                       //in nanoseconds


};

void point::setValues(double bar, double layer, double nstime, double path){
    pathID = path;
    barNum = bar;
    layerNum = layer;
    time = nstime;
}