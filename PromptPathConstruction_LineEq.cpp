//PromptPathConstruction line equation object (y=az+b or x=az+b)

#include <iostream>

using namespace std;

class line{
    public:
        void setValues(double a, double b, double c, double d);
        double getAx(){return a;}
        double getBx(){return b;}
        double getAy(){return c;}
        double getBy(){return d;}

        double a;                     //ax value for x equation
        double b;                     //bx value for x equation
        double c;                     //ay value for y equation
        double d;                     //by value for y equation

};

void line::setValues(double aVal, double bVal, double cVal, double dVal){
    a = aVal;
    b = bVal;
    c = cVal;
    d = dVal;
}