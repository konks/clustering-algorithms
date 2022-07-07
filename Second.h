#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
#include <set>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <time.h>

using namespace std;

static int id = 0;
static int pid = 0;
static int cid = 0;
static int clustid = 0;
static int A = -10;

class point
{
public:
    double x, y;
    int id_point;
    string metka = "dust";
    int tag = 0;
    int mark = 0;
    int label = 0;
    point(double xx, double yy);
    point(const point& p);
    point(point* p);
};

class buffer
{
public:
    vector<point*> temp;
    ~buffer();
};

class cloud
{
public:
    int N, Id;
    vector<point*> points;
    cloud(double mX, double mY, double sX, double sY, int Np);
    cloud();
    ~cloud();
};

class field
{
public:
    int Np, Nc, state = 1;
    vector<cloud*> clouds;
    vector<point*> all_points;
    vector<cloud*> clusters;
    double** D;
    double maxD;
    field();
    ~field();
};

class cluster
{
public:
    int Nclusters;
    string type;
    vector<point*> Single_Cluster;
    cluster();
    ~cluster();
};

class formal_element
{
public:
    vector<point*> elements;
    formal_element(int k);
    ~formal_element();
};

class Find_Cluster
{
public:
    int Ncl;
    vector<int**> matrixB;
    vector<cluster*> Cluster;
    vector<formal_element*> Formal_elements;
    void INFO_Find_Clusters();
    Find_Cluster();
    ~Find_Cluster();
};

class line
{
public:
    vector<point*> single_line;
    double length;
    line(double x1, double y1, double x2, double y2);
    ~line();
};


class controller
{
public:
    ofstream log_controller;
    field* Field;
    buffer* Buffer;
    Find_Cluster* Find_Clusters;
    vector<line*> Lines;
    controller();
    void writeLog(const string& message);
    void INIT();
    void INFO();
    void GEN_CLOUD(double mX, double mY, double sX, double sY, int N);
    void HELP();
    int HISTOGRAM_FOR_FIELD(int H);
    int HISTOGRAM_FOR_CLOUD(int H, int number);
    void GET_BUFFER_FOR_FIELD();
    int GET_BUFFER_FOR_CLOUD(int number);
    void BUFFER_ROTATION(double xo, double yo, double fi);
    void BUFFER_HOMOTETIA(double xo, double yo, double fi);
    void BUFFER_SHIFT(double xo, double yo);
    void SET_BUFFER();
    void MATRIX();
    void BINARY(double r);
    int WAVE(int K);
    int DBSCAN(int K, int G);
    void DBWAVE(int K);
    void DISPLAY_DBSCAN();
    void STREE();
    int K_MEANS(int k);
    void EM_MEANS(int k);
    void HISTOGRAM_FOR_LINES(int K);
    void K_MEANS_CORE(int k);
    void FOREL(double r, double e);
    void stree_function(double r, vector<point*> points, int shag);
    void IERA();
    void FACTOR();
    ~controller();
};

class interface
{
public:
    int ID;
    controller Controller;
    vector<string> comands;
    ofstream log_interface;
    vector<string> instructions;
    string GetCommand();
    interface();
    ~interface();
    void Log_wrong_command(string command);
    void Log_wrong_means(string command);
    void Log_access_denied(string command);
    void writeLog(const string& message);
    void ReadCommand();
    int Check(string cmd);
    void Fill();
    int CONTROLLER(string Command);
};

double dist(point first, point second);
double srednee(vector<double> z);
vector<point*> K_MEANS_function(int k, vector<point> all_points);
string timeLog();
formal_element* forel_function(vector<point*> vhod, double r, int A);
