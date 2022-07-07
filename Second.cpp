#include "Second.h"
#define E 0.0001

using namespace std;

field::field() //constructor classa "field"
{
    Np = 0; //chislo tochek (potom pomehayetsya)
    Nc = 0; //chislo oblakov (potom pomehayetsya)
}

field::~field() //distructor classa "field"
{
    for (auto& el : clouds) delete el;
}

point::point(double xx, double yy) //constructor classa "point"
{
    x = xx;
    y = yy;
    id_point = ++pid;
}

point::point(const point& p)
{
    x = p.x;
    y = p.y;
    tag = p.tag;
    metka = p.metka;
    mark = p.mark;
}

point::point(point* p)
{
    x = p->x;
    y = p->y;
    tag = p->tag;
    metka = p->metka;
    mark = p->mark;
}

cloud::cloud(double mX, double mY, double sX, double sY, int Np)
{
    cid++;
    Id = cid;
    N = Np;
    double tempx = 0, tempy = 0, constant = 0;
    srand(time_t(NULL));
    for (int i = 0; i < Np * 10; i++)
    {
        int t = rand();
        if ((t % 4) > 1) constant += (double)t * (-1) / (Np / 2);
        else constant += (double)t / (Np / 2);
    }
    for (int i = 0; i < Np; i++)
    {
        for (int j = 0; j < Np * 3; j++)
        {
            int t = rand();
            if ((t % 4) > 1) tempx += (double)t * (-1) / constant;
            else tempx += (double)t / constant;
            t = rand();
            if ((t % 4) > 1) tempy += (double)t * (-1) / constant;
            else tempy += (double)t / constant;
        }
        point* Pnt = new point(mX + (tempx / (3 * Np)) * sX, mY + (tempy / (3 * Np)) * sY);
        points.push_back(Pnt);
        tempx = 0;
        tempy = 0;
    }
}

cloud::cloud()
{
    cid++;
    Id = cid;
    N = 0;
}

Find_Cluster::Find_Cluster()
{
    Ncl = 0;
    matrixB = {};
}

Find_Cluster::~Find_Cluster()
{
    for (auto& el : matrixB) delete el;
}

line::line(double x1, double y1, double x2, double y2)
{
    point* Point1 = new point(x1, y1);
    point* Point2 = new point(x2, y2);
    single_line.push_back(Point1);
    single_line.push_back(Point2);
    length = dist(*Point1, *Point2);
}

line::~line()
{
    delete single_line[1];
    delete single_line[0];
}

cluster::cluster()
{
    clustid++;
    Nclusters = clustid;
    Single_Cluster = {};
}

cluster::~cluster()
{
    for (auto& el : Single_Cluster) delete el;
}

cloud::~cloud()
{
    for (auto& el : points) delete el;
}

interface::interface()
{
    log_interface.open("C:/3hell/Second/Second/log_interface.txt", std::ofstream::out | std::ofstream::trunc);
    log_interface.close();
    log_interface.open("C:/3hell/Second/Second/log_interface.txt", ios_base::app);
    id++;
    ID = id;
}

interface::~interface()
{
    id--;
}

buffer::~buffer()
{
    for (auto& el : temp) delete el;
}

controller::controller()
{
    log_controller.open("C:/3hell/Second/Second/log_controller.txt", std::ofstream::out | std::ofstream::trunc);
    log_controller.close();
    log_controller.open("C:/3hell/Second/Second/log_controller.txt", ios_base::app);
}

controller::~controller() //distructor classa "controller"
{
    delete Field;
    delete Buffer;
}

formal_element::formal_element(int k)
{
    for (int i = 0; i < k; i++)
    {
        point* New = new point(0, 0);
        elements.push_back(New);
    }
}

formal_element::~formal_element()
{
    for (int i = 0; i < elements.size(); i++)
        delete elements[i];
}

void interface::Log_wrong_command(string command)
{

    ofstream out("C:/3hell/Second/Second/log_mistake.txt");
    out << "Введена неправильная команда: " << command;
    out.close();
    cout << "Ошибка. Создан файл log.txt" << endl;
}

void interface::Log_wrong_means(string command)
{
    ofstream out("C:/3hell/Second/Second/log_mistake.txt");
    out << "Введены неправильные параметры генерации: " << command;
    out.close();
    cout << "Ошибка. Создан файл log.txt" << endl;
}

void interface::Log_access_denied(string command)
{
    ofstream out("C:/3hell/Second/Second/log_mistake.txt");
    out << "Попытка изменить поле в неподходящем состоянии." << endl << "Команда: " << command;
    out.close();
    cout << "Ошибка. Создан файл log.txt" << endl;
}

void interface::writeLog(const string& message)
{
    log_interface << timeLog() << " "  << message << endl;
}

void interface::ReadCommand() //chityvaem comandyi iz faila v vector "instraction"
{
    ifstream in("C:/3hell/Second/Second/instruction.txt");
    if (!in) return;
    string temp;
    while (!in.eof())
    {
        getline(in, temp);
        instructions.push_back(temp);
    }
    in.close();
    writeLog("Считана инструкция для работы из файла");
    cout << "Считали инструкцию для работы из файла" << endl;
}

void controller::writeLog(const string& message)
{
    log_controller << timeLog() << " " << message << endl;
}

void controller::HELP() //vydaet spisok vozmoznyh comand
{
    ifstream in("C:/3hell/Second/Second/help.txt");
    string temp;
    cout << endl << "Список возможных команд:" << endl;
    while (!in.eof())
    {
        in >> temp;
        cout << temp << endl;
    }
    cout << endl;
    in.close();
    writeLog("На экран выведен список доступных команд");
}

void controller::GEN_CLOUD(double mX, double mY, double sX, double sY, int N)
{
    Field->Nc++;
    Field->Np += N;
    cloud* New = new cloud(mX, mY, sX, sY, N);
    for (auto& el : New->points) Field->all_points.push_back(el); //zdes hranyatsya vse tochki
    Field->clouds.push_back(New); //zdes hranyatsya oblaka
    writeLog("Сгенерировано облако");
    cout << "Сгенерировано облако" << endl;
}

void interface::Fill() //sozdaem vector dostupnyx comand
{
    ifstream in("C:/3hell/Second/Second/help.txt");
    string temp;
    while (!in.eof())
    {
        getline(in, temp);
        comands.push_back(temp);
    }
    in.close();
    writeLog("Составлен список возможных команд");
    cout << "Составили список возможных команд" << endl;
}

int interface::Check(string cmd) //proverka na ochibki v comande (vozvrachaet 1, kogda comanda pravilnaya, 0 - inache)
{
    for (auto& el : comands) //budem sravnivat s elementami iz faila "help.txt"
    {
        if (cmd.find(el) == 0 & el.length() == cmd.length()) return 1; //dlya komand bez parametrov
        else if (cmd.substr(0, cmd.find(" ")).find(el.substr(0, el.find(" "))) == 0) //proverka sovpadeniya komand do 1 probela
        {
            string str = cmd + " ", ethalon = el + " ";
            str.erase(0, str.find(" ") + 1); //udalit' nazvanie komandyi, ostavit' tol'ko parametriy
            ethalon.erase(0, ethalon.find(" ") + 1);
            while (ethalon.length() != 0) //idti do konza etalona
            {
                if (ethalon.substr(0, ethalon.find(" ")) == "double")
                {
                    while (str <= ":" & str >= "-" & str.find("/") == string::npos) str.erase(0, 1); //chitaem czifryi
                    if (str.find(" ") != 0) return 0;
                    str.erase(0, 1);
                    ethalon.erase(0, ethalon.find(" ") + 1);
                }

                if (ethalon.substr(0, ethalon.find(" ")) == "int")
                {
                    while (str <= ":" & str >= "-" & str.find("/") == string::npos & str.find(".") == string::npos) str.erase(0, 1);
                    if (str.find(" ") != 0) return 0;
                    str.erase(0, 1);
                    ethalon.erase(0, ethalon.find(" ") + 1);
                }

            }
            if (str.length() == 0) return 1;
        }
    }
    return 0;
    writeLog("Выполнена проверка команд");
    cout << "Сделали проверку команды" << endl;
}

int interface::CONTROLLER(string Command) //nachinaetsya vesel'e
{
    writeLog("Получена команда " + Command);
    if (Command == "INIT") Controller.INIT();
    if (Command == "INFO") Controller.INFO();
    if (Command == "HELP") Controller.HELP();

    if (Command.find("HISTOGRAM_FOR_FIELD") == 0)
    {
        int parametr = 0;
        string temp = Command;
        temp.erase(0, 20);
        //cout << temp << endl;
        if (Controller.HISTOGRAM_FOR_FIELD(stoi(temp)) == 0) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        else Controller.HISTOGRAM_FOR_FIELD(stoi(temp)); //proverka na otrizcatelnoe kolichestvo otrezkov
    }

    if (Command.find("HISTOGRAM_FOR_CLOUD") == 0)
    {
        int parametr = 0;
        string temp = Command;
        temp.erase(0, 20);
        parametr = stoi(temp.substr(0, temp.find(" ")));
        temp.erase(0, temp.find(" ") + 1);
        if (!Controller.HISTOGRAM_FOR_CLOUD(parametr, stoi(temp))) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        else Controller.HISTOGRAM_FOR_CLOUD(parametr, stoi(temp)); //proverka na otrizcatelnoe kolichestvo otrezkov

    }

    if (Command == "GET_BUFFER_FOR_FIELD")
    {
        if (Controller.Field->state) Controller.GET_BUFFER_FOR_FIELD(); //proverka na sostoyanie polya
        else { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
    }

    if (Command.find("GET_BUFFER_FOR_CLOUD") == 0)
    {
        if (Controller.Field->state) //proverka na sostoyanie polya
        {
            string temp = Command;
            temp.erase(0, 21);
            if (!Controller.GET_BUFFER_FOR_CLOUD(stoi(temp))) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; } //proverka na pravil'nost' parametrov
        }
        else { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
    }

    if (Command.find("BUFFER_ROTATION") == 0)
    {
        while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
        double xo, yo, fi;
        Command += " ";
        Command.erase(0, 16);
        xo = stod(Command.substr(0, Command.find(" ")));
        Command.erase(0, Command.find(" ") + 1);
        yo = stod(Command.substr(0, Command.find(" ")));
        Command.erase(0, Command.find(" ") + 1);
        fi = stod(Command.substr(0, Command.find(" ")));
        Controller.BUFFER_ROTATION(xo, yo, fi);
    }

    if (Command.find("BUFFER_HOMOTETIA") == 0)
    {
        while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
        double xo, yo, fi;
        Command += " ";
        Command.erase(0, 17);
        xo = stod(Command.substr(0, Command.find(" ")));
        Command.erase(0, Command.find(" ") + 1);
        yo = stod(Command.substr(0, Command.find(" ")));
        Command.erase(0, Command.find(" ") + 1);
        fi = stod(Command.substr(0, Command.find(" ")));
        Controller.BUFFER_HOMOTETIA(xo, yo, fi);
    }

    if (Command.find("BUFFER_SHIFT") == 0)
    {
        while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
        double xo, yo, fi;
        Command += " ";
        Command.erase(0, 13);
        xo = stod(Command.substr(0, Command.find(" ")));
        Command.erase(0, Command.find(" ") + 1);
        yo = stod(Command.substr(0, Command.find(" ")));
        Controller.BUFFER_SHIFT(xo, yo);
    }

    if (Command == "SET_BUFFER")
    {
        if (Controller.Field->state) Controller.SET_BUFFER(); //proverka na sostoyanie polya
        else { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
    }

    if (Command.find("GC") == 0)
    {
        if (Controller.Field->state)
        {
            while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
            double mX, mY, sX, sY, temp;
            int N;
            Command += " ";
            Command.erase(0, 3);
            mX = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            mY = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            sX = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            sY = stod(Command.substr(0, Command.find(" ")));
            Command.erase(0, Command.find(" ") + 1);
            temp = stod(Command.substr(0, Command.find(" ")));
            if ((double)((int)temp) != temp) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
            N = (int)temp;
            if (N < 1) return 0;
            Controller.GEN_CLOUD(mX, mY, sX, sY, N);
        }
        else { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
    }

    if (Command.find("MATRIX") == 0)
    {
        Controller.Field->state = 0;
        Controller.MATRIX();
    }

    if (Command.find("BINARY") == 0)
    {
        if (Controller.Field->state == 0)
        {
            while (Command.find(".") != string::npos) Command.replace(Command.find("."), 1, ",");
            string temp = Command;
            temp.erase(0, 7);
            Controller.BINARY(stod(temp));
        }
        else { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
    }

    if (Command.find("WAVE") == 0)
    {
        if (Controller.Field->state) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
        string temp = Command;
        temp.erase(0, 5);
        if (Controller.WAVE(stoi(temp)) == 0) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
    }

    if (Command.find("DBSCAN") == 0)
    {
        if (Controller.Field->state) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
        int parametr = 0;
        string temp = Command;
        temp.erase(0, 7);
        parametr = stoi(temp.substr(0, temp.find(" ")));
        A = parametr;
        temp.erase(0, temp.find(" ") + 1);
        if (!Controller.DBSCAN(parametr, stoi(temp))) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
    }

    if (Command.find("DBWAVE") == 0)
    {
        if (Controller.Field->state) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
        if (A == -10) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 2; }
        else Controller.DBWAVE(A);
    }

    if (Command.find("INFO_Find_Clusters") == 0) Controller.Find_Clusters->INFO_Find_Clusters();
    if (Command == "DISPLAY_DBSCAN") Controller.DISPLAY_DBSCAN();
    if (Command == "STREE") Controller.STREE();

    if (Command.find("K_MEANS") == 0)
    {
        string temp = Command;
        temp.erase(0, 8);
        if (Controller.K_MEANS(stoi(temp)) == 0) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
    }

    if (Command.find("HISTOGRAM_FOR_LINES") == 0)
    {
        string temp = Command;
        temp.erase(0, 20);
        if (stoi(temp) < 1) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        else Controller.HISTOGRAM_FOR_LINES(stoi(temp));
    }

    if (Command.find("EM_MEANS") == 0)
    {
        string temp = Command;
        temp.erase(0, 9);
        if (stoi(temp) < 1) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        else Controller.EM_MEANS(stoi(temp));
    }

    if (Command.find("K_MEANS_CORE") == 0)
    {
        string temp = Command;
        temp.erase(0, 13);
        if (stoi(temp) < 1) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        else Controller.K_MEANS_CORE(stoi(temp));
    }

    if (Command.find("FOREL") == 0)
    {
        //if (Controller.Field->state) return 2;
        double parametr = 0;
        string temp = Command;
        temp.erase(0, 6);
        parametr = stod(temp.substr(0, temp.find(" ")));
        if (parametr < 0) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        temp.erase(0, temp.find(" ") + 1);
        if (stod(temp) < 1) { writeLog("Ошибка в команде " + Command + "! Напечатан файл log_mistake.txt");  return 0; }
        Controller.FOREL(parametr, stod(temp));
    }

    if (Command.find("IERA") == 0)
    {
        if (Controller.Field->state) return 2;
        else Controller.IERA();
    }

    if (Command.find("FACTOR") == 0)  Controller.FACTOR();

    writeLog("Выполнена команда " + Command);
    return 1;
}

void controller::INIT()
{
    Field = new field;
    Buffer = new buffer;
    Find_Clusters = new Find_Cluster;
    writeLog("Выполнена команда INIT");
    cout << "Выполнена команда INIT" << endl;
}

void controller::INFO() //vyigrucghaet tochki v fail
{
    for (auto& el : (*Field).clouds)
    {
        ofstream out("C:/3hell/Second/Second/Cloud" + to_string(el->Id) + ".txt");
        out.precision(6);
        for (auto& points : el->points) out << points->x << "\t" << points->y << endl;
        out.close();
        writeLog("Выполнена команда INFO");
        cout << "Выполнена команда INFO. В файлы напечатаны координаты" << endl;
    }
}

int controller::HISTOGRAM_FOR_FIELD(int H) //dlya vseh tochek
{
    int q = 0;
    if (H < 2) return 0;
    vector<double> x, y;
    for (auto& el : Field->all_points)
    {
        x.push_back(el->x);
        y.push_back(el->y);
    }
    double minx = *min_element(x.begin(), x.end());
    double maxx = *max_element(x.begin(), x.end());
    double miny = *min_element(y.begin(), y.end());
    double maxy = *max_element(y.begin(), y.end());
    vector<int> histx, histy;
    for (int i = 0; i < H; i++) histx.push_back(count_if(x.begin(), x.end(), [minx, maxx, i, H](double val) {return (val >= minx + i * (maxx - minx) / H & val < minx + (i + 1) * (maxx - minx) / H); }));
    for (int i = 0; i < H; i++) histy.push_back(count_if(y.begin(), y.end(), [miny, maxy, i, H](double val) {return (val >= miny + i * (maxy - miny) / H & val < miny + (i + 1) * (maxy - miny) / H); }));
    ofstream outx("C:/3hell/Second/Second/histogram_x.txt");
    ofstream outy("C:/3hell/Second/Second/histogram_y.txt");
    outx << "number_of_piece amount_of_points" << endl;
    outy << "number_of_piece amount_of_points" << endl;
    for (auto& el : histx) outx << ++q << " " << el << endl;
    q = 0;
    for (auto& el : histy) outy << ++q << " " << el << endl;
    outx.close();
    outy.close();
    writeLog("Выполнена команда HISTOGRAM_FOR_FIELD - создана гистограмма поля");
    cout << "Выполнена команда HISTOGRAM_FOR_FIELD" << endl;
    return 1;
}

int controller::HISTOGRAM_FOR_CLOUD(int H, int W) //dlya oblaka nomer W
{
    if (H < 2) return 0;
    int q = 0;
    vector<double> x, y;
    vector<cloud*>::iterator it = Field->clouds.begin();
    for (int i = 1; i < W; i++)
    {
        if (it != Field->clouds.end()) it++;
        else return 0;
    }
    for (auto& el : (*it)->points)
    {
        x.push_back(el->x);
        y.push_back(el->y);
    }
    double minx = *min_element(x.begin(), x.end());
    double maxx = *max_element(x.begin(), x.end());
    double miny = *min_element(y.begin(), y.end());
    double maxy = *max_element(y.begin(), y.end());
    vector<int> histx, histy;
    for (int i = 0; i < H; i++) histx.push_back(count_if(x.begin(), x.end(), [minx, maxx, i, H](double val) {return (val >= minx + i * (maxx - minx) / H & val < minx + (i + 1) * (maxx - minx) / H); }));
    for (int i = 0; i < H; i++) histy.push_back(count_if(y.begin(), y.end(), [miny, maxy, i, H](double val) {return (val >= miny + i * (maxy - miny) / H & val < miny + (i + 1) * (maxy - miny) / H); }));
    ofstream outx("C:/3hell/Second/Second/histogram_x.txt");
    ofstream outy("C:/3hell/Second/Second/histogram_y.txt");
    outx << "number_of_piece amount_of_points" << endl;
    outy << "number_of_piece amount_of_points" << endl;
    for (auto& el : histx) outx << ++q << " " << el << endl;
    q = 0;
    for (auto& el : histy) outy << ++q << " " << el << endl;
    outx.close();
    outy.close();
    writeLog("Выполнена команда HISTOGRAM_FOR_CLOUD - создана гистограмма облака");
    cout << "Выполнена команда HISTOGRAM_FOR_CLOUD" << endl;
    return 1;
}

void controller::GET_BUFFER_FOR_FIELD() //zdes' vse tochki
{
    for (auto& el : Field->all_points)
    {
        point* t = new point(*el);
        Buffer->temp.push_back(t);
    }
    writeLog("Выполнена команда GET_BUFFER_FOR_FIELD - точки поля помещены в буффер");
    cout << "Выполнена команда GET_BUFFER_FOR_FIELD" << endl;
}

int controller::GET_BUFFER_FOR_CLOUD(int number) //zdes' tochki oblaka N
{
    vector<cloud*>::iterator it = Field->clouds.begin();
    for (int i = 1; i < number; i++) //pereveli iterator na oblako number
    {
        if (it != Field->clouds.end()) it++;
        else return 0; //esli takogo net
    }
    for (auto& el : (*it)->points)
    {
        point* t = new point(*el);
        Buffer->temp.push_back(t);
    }
    writeLog("Выполнена команда GET_BUFFER_FOR_CLOUD - точки облака помещены в буффер");
    cout << "Выполнена команда GET_BUFFER_FOR_CLOUD" << endl;
    return 1;
}

void controller::BUFFER_ROTATION(double xo, double yo, double fi)
{
    double a, b;
    for (auto& el : Buffer->temp)
    {
        a = el->x, b = el->y;
        el->x = (a - xo) * cos(fi) - (b - yo) * sin(fi) + xo;
        el->y = (a - xo) * sin(fi) + (b - yo) * cos(fi) + yo;
    }
    writeLog("Выполнена команда BUFFER_ROTATION");
    cout << "Выполнена команда BUFFER_ROTATION" << endl;
}

void controller::BUFFER_HOMOTETIA(double xo, double yo, double fi)
{
    for (auto& el : Buffer->temp)
    {
        el->x = (el->x - xo) * fi + xo;
        el->y = (el->y - yo) * fi + yo;
    }
    writeLog("Выполнена команда BUFFER_HOMOTETIA");
    cout << "Выполнена команда BUFFER_HOMOTETIA" << endl;
}

void controller::BUFFER_SHIFT(double xo, double yo)
{
    for (auto& el : Buffer->temp)
    {
        el->x = el->x + xo;
        el->y = el->y + yo;
    }
    writeLog("Выполнена команда BUFFER_SHIFT");
    cout << "Выполнена команда BUFFER_SHIFT" << endl;
}

void controller::SET_BUFFER()
{
    cloud* New = new cloud();
    for (auto& el : Buffer->temp)
    {
        point* NEW = new point(*el);
        New->points.push_back(NEW);
    }
    for (auto& elp : New->points) Field->all_points.push_back(elp);
    Field->clouds.push_back(New);
    for (auto& ell : Buffer->temp) delete ell;
    Buffer->temp.clear();
    writeLog("Выполнена команда SET_BUFFER");
    cout << "Выполнена команда SET_BUFFER" << endl;
}

void controller::MATRIX()
{
    int K = Field->all_points.size();
    Field->D = new double* [K];
    for (int i = 0; i < K; i++) Field->D[i] = new double[K];
    vector<point*>::iterator it1 = Field->all_points.begin();

    for (int j = 0; j < K; j++)
    {
        vector<point*>::iterator it2 = Field->all_points.begin();
        for (int i = 0; i < K; i++)
        {
            Field->D[j][i] = dist(**it1, **it2);
            it2++;
        }
        it1++;
    }

    double max = -10000; 
    for (int i = 0; i < K; i++) for (int j = i + 1; j < K; j++) { if (Field->D[i][j] > max) { max = Field->D[i][j]; Field->maxD = max; } }

    writeLog("Выполнена команда MATRIX");
    cout << "Построена матрица расстояний D" << endl;
}

void controller::BINARY(double r)
{
    int K = Field->all_points.size();
    int** B;
    B = new int* [K];
    for (int i = 0; i < K; i++) B[i] = new int[K];
    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < K; j++) if ((Field->D[i][j] > r) || (i == j)) B[i][j] = 0; else B[i][j] = 1;
    }
    Find_Clusters->matrixB.push_back(B);
    writeLog("Выполнена команда BINARY");
    cout << "Построена бинарная матрица с радиусом " << r << endl;
}

int controller::WAVE(int k)
{
    int Q = Find_Clusters->matrixB.size();
    if ((k > Q) || (k < 1)) return 0;
    int** B = Find_Clusters->matrixB[k - 1];
    int K = Field->all_points.size();
    int i;
    for (i = 0; i < K; i++) for (int j = 0; j < K; j++) if (B[i][j])
    {
        cluster* New = new cluster;
        New->type = "wave";
        point* temp = new point(*(Field->all_points[i]));
        New->Single_Cluster.push_back(temp);
        map<int, point*> old_front = { {i, Field->all_points[i] } };
        map<int, point*> new_front;
        vector<point*> yet = { Field->all_points[i] };
        ofstream out("C:/3hell/Second/Second/cluster" + to_string(New->Nclusters) + ".txt");
        while (old_front.size() != 0)
        {
            for (auto& el : old_front) for (int t = 0; t < Field->all_points.size(); t++) if (B[el.first][t] == 1)
            {

                if (find(yet.begin(), yet.end(), Field->all_points[t]) == yet.end())
                {
                    yet.push_back(Field->all_points[t]);
                    point* temp = new point(*(Field->all_points[t]));
                    New->Single_Cluster.push_back(temp);
                    out << el.second->x << "\t" << el.second->y << endl;
                    out << Field->all_points[t]->x << "\t" << Field->all_points[t]->y << endl << endl << endl;
                }
                new_front[t] = Field->all_points[t];
                B[t][el.first] = B[el.first][t] = 0;
            }
            old_front = new_front;
            new_front.clear();
        }
        Find_Clusters->Cluster.push_back(New);
        out.close();
    }
    writeLog("Выполнена команда WAVE");
    cout << "Сформированы кластеры путём волнового алгоритма" << endl;
    return 1;
}

int controller::DBSCAN(int K, int G) //K - nomer matrizyi, G - gustota
{
    if (K < 0) return 0;
    if (K > (Find_Clusters->matrixB.size())) return 0;
    if (G < 1) return 0;
    int s = 0, Q = Field->all_points.size();
    for (int i = 0; i < Q; i++)
    {
        for (int j = 0; j < Q; j++) s += Find_Clusters->matrixB[0][i][j];
        if (s >= G) Field->all_points[i]->metka = "core";
        //cout << s << endl;
        s = 0;
    }
    for (int i = 0; i < Q; i++)
        if (Field->all_points[i]->metka == "core")
            for (int t = 0; t < Q; t++)
                if ((Find_Clusters->matrixB[K - 1][i][t] == 1) && (Field->all_points[t]->metka == "dust"))
                    Field->all_points[i]->metka = "per";
    writeLog("Выполнена команда DBSCAN");
    cout << "Точкам присвоены метки" << endl;
    return 1;
}

void controller::DBWAVE(int K) //K - nomer matriziy
{
    int Q = Find_Clusters->matrixB.size();
    int** B = Find_Clusters->matrixB[K - 1];
    int T = Field->all_points.size();
    int i;
    for (i = 0; i < T; i++)
        if (Field->all_points[i]->metka != "dust")
        {
            for (int j = 0; j < T; j++) if (B[i][j] == 1)
            {
                //cout << "ne dust" << endl;
                cluster* New = new cluster;
                New->type = "dbscan";
                point* temp = new point(*(Field->all_points[i]));
                New->Single_Cluster.push_back(temp);
                map<int, point*> old_front = { {i, Field->all_points[i] } };
                map<int, point*> new_front;
                vector<point*> yet = { Field->all_points[i] };
                ofstream out("C:/3hell/Second/Second/cluster" + to_string(New->Nclusters) + ".txt");
                while (old_front.size() != 0)
                {
                    for (auto& el : old_front) for (int t = 0; t < Field->all_points.size(); t++) if (B[el.first][t] == 1)
                    {

                        if (find(yet.begin(), yet.end(), Field->all_points[t]) == yet.end())
                        {
                            yet.push_back(Field->all_points[t]);
                            point* temp = new point(*(Field->all_points[t]));
                            New->Single_Cluster.push_back(temp);
                            out << el.second->x << "\t" << el.second->y << endl;
                            out << Field->all_points[t]->x << "\t" << Field->all_points[t]->y << endl << endl << endl;
                        }
                        new_front[t] = Field->all_points[t];
                        B[t][el.first] = B[el.first][t] = 0;
                    }
                    old_front = new_front;
                    new_front.clear();
                }
                Find_Clusters->Cluster.push_back(New);
            }
        }
    writeLog("Выполнена команда DBWAVE");
    cout << "Сформированы кластеры путём алгоритма DBSCAN" << endl;
}

void Find_Cluster::INFO_Find_Clusters()
{
    ofstream out("C:/3hell/Second/Second/INFO_Find_Cluster.txt");
    out << "Количество кластеров: " << Cluster.size() << endl;
    for (int i = 0; i < Cluster.size(); i++) out << i << "-ый кластер имеет метку " << Cluster[i]->type << endl;
    out.close();
    cout << "Информация о кластерах выведена в файл INFO_Find_Cluster.txt" << endl;
}

void controller::DISPLAY_DBSCAN()
{
    ofstream out1("C:/3hell/Second/Second/dust.txt");
    ofstream out2("C:/3hell/Second/Second/per.txt");
    ofstream out3("C:/3hell/Second/Second/core.txt");
    for (int i = 0; i < Field->all_points.size(); i++)
    {
        if (Field->all_points[i]->metka == "dust") out1 << Field->all_points[i]->x << " " << Field->all_points[i]->y << endl;
        if (Field->all_points[i]->metka == "per") out2 << Field->all_points[i]->x << " " << Field->all_points[i]->y << endl;
        if (Field->all_points[i]->metka == "core") out3 << Field->all_points[i]->x << " " << Field->all_points[i]->y << endl;
    }
    out1.close();
    out2.close();
    out3.close();
    writeLog("Выполнена команда DISPLAY_DBSCAN");
    cout << "Точки пыли, периферии и ядра помещены в отдельные файлы" << endl;
}

double dist(point first, point second)
{
    return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

void controller::STREE()
{
    set<int> tree = { 0 };
    int P = Field->all_points.size();
    int a, b;
    double min = 10000;

    for (int k = 0; k < P - 1; k++)
    {
        for (auto it = tree.begin(); it != tree.end(); it++)
        {
            for (int j = 1; j < P; j++) if ((Field->D[(*it)][j] < min) & (tree.find(j) == tree.end()))
            {
                min = Field->D[(*it)][j];
                a = j;
                b = (*it);
            }
        }
        min = 10000;
        tree.insert(a);
        line* New = new line(Field->all_points[b]->x, Field->all_points[b]->y, Field->all_points[a]->x, Field->all_points[a]->y);
        Lines.push_back(New);
    }

    ofstream out("C:/3hell/Second/Second/tree.txt"); //vyivid v fayl
    for (auto it = Lines.begin(); it != Lines.end(); it++)
    {
        out << (*it)->single_line[1]->x << " " << (*it)->single_line[1]->y << endl;
        out << (*it)->single_line[0]->x << " " << (*it)->single_line[0]->y << endl << endl << endl;
    }
    writeLog("Выполнена команда STREE");
    cout << "Построено минимальное покрывающее дерево" << endl;
}

int controller::K_MEANS(int k)
{
    if (k < 1) return 0;
    double maxX = -10000, minX = 10000, maxY = -10000, minY = 10000, centerX = 0, centerY = 0;
    vector<double> z; //tam hranyatsya chisla, srednee kotoryih nado poschitat'
    vector<point*> kpoints; //vektor iz randomnyih k tochek
    double** D;
    int K = Field->all_points.size();

    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) { if ((*it)->x < minX) minX = (*it)->x; if ((*it)->y < minY) minY = (*it)->y; if ((*it)->x > maxX) maxX = (*it)->x; if ((*it)->y > maxY) maxY = (*it)->y; }
    z.push_back(maxX); z.push_back(minX); centerX = srednee(z); z.clear();
    z.push_back(maxY); z.push_back(minY); centerY = srednee(z); z.clear();
    double a = centerX, b = centerY + 0.7 * (maxY - centerY); //tochka, kotoruyu budem dvigat'
    for (int i = 0; i < k; i++)
    {
        point* New = new point((a - centerX) * cos(6.28 / k) - (b - centerY) * sin(6.28 / k) + centerX, (a - centerX) * sin(6.28 / k) + (b - centerY) * cos(6.28 / k) + centerY);
        kpoints.push_back(New);
        a = New->x; b = New->y;
    }

    int f = 1;
    while (f)
    {
        f = 0;
        for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it)
        {
            int old_tag = (*it)->tag;
            double min = 10000;
            for (int i = 0; i < kpoints.size(); i++)
            {
                double distance = dist(**it, *kpoints[i]);
                if (distance < min) { min = distance; (*it)->tag = i; } //nahodim novuyu metku
            }
            if (old_tag != (*it)->tag) f = 1;
        }

        vector<vector<point*>> nearest_points; //tocki, blicsheychie k i-oy
        for (int i = 0; i < k; i++) nearest_points.push_back(vector<point*>());
        for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) nearest_points[(*it)->tag].push_back(*it);

        for (int i = 0; i < k; i++) //pereschityivaem tcentr
        {

            vector<double> X;
            vector<double> Y;
            for (int j = 0; j < nearest_points[i].size(); j++)
            {
                X.push_back(nearest_points[i][j]->x);
                Y.push_back(nearest_points[i][j]->y);
            }
            kpoints[i]->x = srednee(X);
            kpoints[i]->y = srednee(Y);
        }
    }

    for (int i = 0; i < k; i++) //dobavlyaem klasteryi
    {
        cluster* NEW = new cluster;
        Find_Clusters->Cluster.push_back(NEW);
        Find_Clusters->Cluster[i]->type = "k-means";
    }
    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) Find_Clusters->Cluster[(*it)->tag]->Single_Cluster.push_back(*it);

    for (int i = 0; i < k; i++)
    {
        ofstream out("C:/3hell/Second/Second/cluster" + to_string(i + 1) + ".txt"); //vyivid v fayl
        for (auto it = Find_Clusters->Cluster[i]->Single_Cluster.begin(); it != Find_Clusters->Cluster[i]->Single_Cluster.end(); it++)
        {
            out << kpoints[i]->x << " " << kpoints[i]->y << endl;
            out << (*it)->x << " " << (*it)->y << endl;
        }
    }
    writeLog("Выполнена команда K_MEANS");
    cout << "Выполнено построение кластеров способом k-means" << endl;
    return 1;
}

double srednee(vector<double> z)
{
    return accumulate(z.begin(), z.end(), 0.0) / z.size();
}

void controller::HISTOGRAM_FOR_LINES(int K)
{
    double min = 10000, max = -1;
    int J;
    vector<double*> OX; //infinum dlinyi rebra v stokbthe
    vector<int*> OY; //znacheniya stolbtzov
    for (int i = 0; i < K; i++) { OY.push_back(new int); *OY[i] = 0; }
    for (auto it = Lines.begin(); it != Lines.end(); ++it) //nahodim min i max
    {
        if ((*it)->length < min) min = (*it)->length;
        if ((*it)->length > max) max = (*it)->length;
    }
    for (int i = 0; i < K; i++) //delim na otrezki
    {
        OX.push_back(new double);
        *OX[i] = min + i * (max - min) / K;
    }
    for (auto it = Lines.begin(); it != Lines.end(); ++it)
    {
        double delta = 10000;
        for (int j = 0; j < K; j++) if ((abs((*it)->length - *OX[j]) < delta) & (((*it)->length - *OX[j]) >= 0)) J = j;
        *OY[J] += 1;
    }
    ofstream outx("C:/3hell/Second/Second/histogram_lines.txt");
    outx.precision(3);
    outx << "length_range amount_of_lines" << endl;
    for (int i = 0; i < K; i++)  outx << *OX[i] << "-" << min + (i + 1) * (max - min) / K << " " << *OY[i] << endl;
    outx.close();
    writeLog("Выполнена команда HISTOGRAM_FOR_LINES");
    cout << "Выполнена команда HISTOGRAM_FOR_LINES" << endl;
}

void controller::EM_MEANS(int k)
{
    vector <double> W; //P(c)
    vector <double> Mu; //tochki tzentov
    vector < vector <double> > Sigma; //dispersii
    vector < vector <double> > A; //P(c, x_i)
    vector < vector <double> > AA; //indikator bol'shey veroyatnosti
    vector < vector <double> > Sigma_1; //obratnaya k matritze distersiy
    vector < vector <double> > P_x_c; //P(x_i, c)
    int n = 0; //iteratziya

    for (int i = 0; i < k; i++) { double m = 1; W.push_back(m / k); }
    for (int j = 0; j < k; j++) { vector <double> C; A.push_back(C); AA.push_back(C);
        for (int i = 0; i < Field->all_points.size(); i++)  { A[j].push_back(0); AA[j].push_back(0);} }
    for (int i = 0; i < k; i++) { vector <double> d; P_x_c.push_back(d);
        for (int j = 0; j < Field->all_points.size(); j++)
        { P_x_c[i].push_back(0); } }
    for (int i = 0; i < 2 * k; i++) { Mu.push_back(0); } //po chetnyim x, po nechetnyim y

    vector <int> N; //kolichestvo tochek v klustere
    for (int i = 0; i < k; i++) { N.push_back(0); }

    //schitaem tzentryi pervyiy raz
    for (int i = 0; i < Field->all_points.size(); i++)
    {
        Mu[(i % k) * 2] = Mu[(i % k) * 2] + Field->all_points[i]->x;
        Mu[(i % k) * 2 + 1] = Mu[(i % k) * 2 + 1] + Field->all_points[i]->y;
        N[(i % k)]++;
    }
    for (int i = 0; i < k; i++)
    {
        Mu[(i % k) * 2] = Mu[(i % k) * 2] / N[i];
        Mu[(i % k) * 2 + 1] = Mu[(i % k) * 2 + 1] / N[i];
    }

    //zapolnyaem dispersii pervyiy raz
    for (int j = 0; j < k; j++) { vector <double> d; Sigma.push_back(d); Sigma_1.push_back(d); Sigma[j].push_back(1); Sigma[j].push_back(0); Sigma[j].push_back(0);Sigma[j].push_back(1); //1 0      1 1
        for (int i = 0; i < 4; i++) { Sigma_1[j].push_back(1); } }                                                                                                                        //0 1      1 1

    double Stop = 1;
    char o;
    while (Stop == 1)
    {
        Stop = 0;
        for (int i = 0; i < k; i++) //schitaem P(x_i, c)
        {
            double DetSigma = Sigma[i][0] * Sigma[i][3] - Sigma[i][1] * Sigma[i][2];
            double ABSDetSigma = fabs(DetSigma);
            Sigma_1[i][0] = (Sigma[i][3]) / DetSigma; Sigma_1[i][1] = (-Sigma[i][1] / DetSigma); Sigma_1[i][2] = (-Sigma[i][2] / DetSigma); Sigma_1[i][3] = (Sigma[i][0] / DetSigma);
            for (int j = 0; j < Field->all_points.size(); j++)
            {
                double e = (Field->all_points[j]->x - Mu[i * 2]);
                double r = (Field->all_points[j]->y - Mu[i * 2 + 1]);
                e = e * (Sigma_1[i][0] * (Field->all_points[j]->x - Mu[i * 2]) + Sigma_1[i][1] * (Field->all_points[j]->y - Mu[i * 2 + 1]));
                r = r * (Sigma_1[i][2] * (Field->all_points[j]->x - Mu[i * 2]) + Sigma_1[i][3] * (Field->all_points[j]->y - Mu[i * 2 + 1]));
                r = (-0.5) * (e + r);
                P_x_c[i][j] = (1 / sqrt(2 * 3.14 * ABSDetSigma));
                P_x_c[i][j] = P_x_c[i][j] * exp(r);
            }

        }
        for (int i = 0; i < Field->all_points.size(); i++) //schitaem P(c, x_i)
        {
            double Summa = 0;
            for (int j = 0; j < k; j++) { Summa = Summa + P_x_c[j][i] * W[j]; }
            for (int p = 0; p < k; p++) { A[p][i] = P_x_c[p][i] * W[p] / Summa; } 
        }

        string s = "C:/3hell/Second/Second/EM_";
        string b = to_string(n);;
        s = s + b;
        s = s + ".txt";
        string fi = s;
        fi.pop_back();
        fi.pop_back();
        fi.pop_back();
        fi.pop_back();
        fi = fi + "instruction.txt";
 
        for (int i = 0; i < Field->all_points.size(); i++) { for (int f = 0; f < k; f++) { AA[f][i] = 0; }  } //obnulyaem matritzu indikatorov

        for (int i = 0; i < Field->all_points.size(); i++) //vyibiraem bol'shuyu veroyatnost'
        {
            int F;
            double M = A[0][i];
            F = 0;
            for (int f = 0; f < k; f++)
            {
                if (A[f][i] >= M)
                {
                    M = A[f][i];
                    F = f;
                }
            }
            AA[F][i] = 1;
            Field->all_points[i]->tag = F;
        }

        vector <point> Arr; //tochki tzentra
        for (int f = 0; f < k; f++)
        {
            point point(Mu[f * 2], Mu[f * 2 + 1]);
            Arr.push_back(point);
        }

        ofstream g, h;
        g.open(s);
        h.open(fi);
        vector <double> x_1, x_2, psi;

        for (int i = 0; i < k; i++) //stroim ellipse
        {
            x_1.push_back(0.5 * ((Sigma[i][0] + Sigma[i][3]) + sqrt((Sigma[i][0] + Sigma[i][3]) * (Sigma[i][0] + Sigma[i][3]) - 4 * (Sigma[i][0] * Sigma[i][3] - Sigma[i][1] * Sigma[i][2]))));
            x_2.push_back(0.5 * ((Sigma[i][0] + Sigma[i][3]) - sqrt((Sigma[i][0] + Sigma[i][3]) * (Sigma[i][0] + Sigma[i][3]) - 4 * (Sigma[i][0] * Sigma[i][3] - Sigma[i][1] * Sigma[i][2]))));
            if (sqrt(Sigma[i][1] * Sigma[i][1] + (Sigma[i][0] - x_1[i]) * (Sigma[i][0] - x_1[i])) == 0) { psi.push_back(0); }
            else {
                if (Sigma[i][1] <= 0) { psi.push_back(acos(-Sigma[i][1] / (sqrt(Sigma[i][1] * Sigma[i][1] + (Sigma[i][0] - x_1[i]) * (Sigma[i][0] - x_1[i])))) * 180 / 3.14); }
                if (Sigma[i][1] > 0) { psi.push_back(acos(Sigma[i][1] / (sqrt(Sigma[i][1] * Sigma[i][1] + (Sigma[i][0] - x_1[i]) * (Sigma[i][0] - x_1[i])))) * 180 / 3.14); }
            }

            h << "set object " << i + 1 << " ellipse center " << Mu[i * 2] << ", " << Mu[i * 2 + 1] << " size " << x_1[i] * 2 << ", " << x_2[i] * 2 << " angle " << psi[i] << " front fs empty bo " << i + 1 << endl;
        }
        h << "plot ";
        int ol;
        for (int a = 0; a < k; a++)
        {
            for (int z = 0; z < Field->all_points.size(); z++)
            {
                if (AA[a][z] == 1)
                {
                    g << Field->all_points[z]->x << " " << Field->all_points[z]->y << endl;
                }
            }
            h << "'" << s << "'index " << a << " w points title '" << a << "' ,\\" << endl;
            g << endl;
            g << endl;
            g << endl;
            g << endl;
            ol = a;
        }
        ol++;
        for (int m = 0; m < k; m++)
        {
            g << Arr[m].x << " " << Arr[m].y << endl;
            h << "'" << s << "'index " << ol << " w points title '" << ol << "' ,\\" << endl;
            g << endl;
            g << endl;
            g << endl;
            g << endl;
            ol++;
        }
        h.close();
        g.close();

        n++;

        vector <double> W_1 = W;
        vector <double> Mu_1 = Mu;
        vector < vector <double> > Sigma_1 = Sigma;
        double Proverka = 0;
        for (int i = 0; i < k; i++) //P(c)
        {
            W[i] = 0;
            for (int j = 0; j < Field->all_points.size(); j++)
            {
                W[i] = W[i] + A[i][j];
            }
            W[i] = W[i] / Field->all_points.size();
            Proverka += W[i];
        }

        for (int i = 0; i < k; i++) //schitaem tzentryi
        {
            Mu[i * 2] = Mu[i * 2 + 1] = 0;
            for (int j = 0; j < Field->all_points.size(); j++)
            {
                Mu[i * 2] = Mu[i * 2] + (A[i][j] / Field->all_points.size()) / W[i] * Field->all_points[j]->x;
                Mu[i * 2 + 1] = Mu[i * 2 + 1] + (A[i][j] / Field->all_points.size()) / W[i] * Field->all_points[j]->y;
            }
        }

        for (int c = 0; c < k; c++) //schitaem dispersii
        {
            Sigma[c][0] = Sigma[c][1] = Sigma[c][2] = Sigma[c][3] = 0;
            for (int p = 0; p < Field->all_points.size(); p++)
            {
                Sigma[c][0] = Sigma[c][0] + (A[c][p] / Field->all_points.size()) / W[c] * (Field->all_points[p]->x - Mu[c * 2]) * (Field->all_points[p]->x - Mu[c * 2]);
                Sigma[c][1] = Sigma[c][1] + (A[c][p] / Field->all_points.size()) / W[c] * (Field->all_points[p]->x - Mu[c * 2]) * (Field->all_points[p]->y - Mu[c * 2 + 1]);
                Sigma[c][2] = Sigma[c][2] + (A[c][p] / Field->all_points.size()) / W[c] * (Field->all_points[p]->y - Mu[c * 2 + 1]) * (Field->all_points[p]->x - Mu[c * 2]);
                Sigma[c][3] = Sigma[c][3] + (A[c][p] / Field->all_points.size()) / W[c] * (Field->all_points[p]->y - Mu[c * 2 + 1]) * (Field->all_points[p]->y - Mu[c * 2 + 1]);
            }
       }

        for (int c = 0; c < k; c++) //uslovie ostanovki
        {
            if (abs(W_1[c] - W[c]) > E) { Stop = 1; }
            if (abs(Sigma_1[c][0] - Sigma[c][0]) > E) { Stop = 1; }
            if (abs(Sigma_1[c][1] - Sigma[c][1]) > E) { Stop = 1; }
            if (abs(Sigma_1[c][2] - Sigma[c][2]) > E) { Stop = 1; }
            if (abs(Sigma_1[c][3] - Sigma[c][3]) > E) { Stop = 1; }
            if (abs(Mu_1[c * 2] - Mu[c * 2] > E)) { Stop = 1; }
            if (abs(Mu_1[c * 2 + 1] - Mu[c * 2 + 1] > E)) { Stop = 1; }
        }
    }

    for (int i = 0; i < k; i++) //dobavlyaem klasteryi
    {
        cluster* NEW = new cluster;
        Find_Clusters->Cluster.push_back(NEW);
        Find_Clusters->Cluster[i]->type = "em-means";
    }
    for (int j = 0; j < Field->all_points.size(); j++)
    {
        Find_Clusters->Cluster[Field->all_points[j]->tag]->Single_Cluster.push_back(Field->all_points[j]);
    }
    for (int i = 0; i < k; i++)
    {
        ofstream out("C:/3hell/Second/Second/cluster" + to_string(i + 1) + ".txt"); //vyivid v fayl
        for (auto it = Find_Clusters->Cluster[i]->Single_Cluster.begin(); it != Find_Clusters->Cluster[i]->Single_Cluster.end(); it++)
        {
            out << Mu[i * 2] << " " << Mu[i * 2 + 1] << endl;
            out << (*it)->x << " " << (*it)->y << endl << endl << endl;
        }
    }
    writeLog("Выполнена команда EM_MEANS");
    cout << "Выполнено построение кластеров способом em-means" << endl;
}

void controller::K_MEANS_CORE(int k)
{
    double maxX = -10000, minX = 10000, maxY = -10000, minY = 10000, centerX = 0, centerY = 0;
    vector<double> z; //tam hranyatsya chisla, srednee kotoryih nado poschitat'
    vector<point*> kpoints; //vektor iz randomnyih k tochek
    vector<vector<point*>> core; //vektor iz  tochek yadra
    double** D;
    int K = Field->all_points.size();

    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) { if ((*it)->x < minX) minX = (*it)->x; if ((*it)->y < minY) minY = (*it)->y; if ((*it)->x > maxX) maxX = (*it)->x; if ((*it)->y > maxY) maxY = (*it)->y; }
    z.push_back(maxX); z.push_back(minX); centerX = srednee(z); z.clear();
    z.push_back(maxY); z.push_back(minY); centerY = srednee(z); z.clear();
    double a = centerX, b = centerY + 0.7 * (maxY - centerY); //tochka, kotoruyu budem dvigat'
    for (int i = 0; i < k; i++)
    {
        point* New = new point((a - centerX) * cos(6.28 / k) - (b - centerY) * sin(6.28 / k) + centerX, (a - centerX) * sin(6.28 / k) + (b - centerY) * cos(6.28 / k) + centerY);
        kpoints.push_back(New);
        a = New->x; b = New->y;
    }
    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it)
    {
        double min = 10000;
        for (int i = 0; i < kpoints.size(); i++)
        {
            double distance = dist(**it, *kpoints[i]);
            if (distance < min) { min = distance; (*it)->tag = i; } //nahodim novuyu metku
        }
    }

    vector<vector<point>> nearest_points; //tocki, blicsheychie k i-oy
    for (int i = 0; i < k; i++) nearest_points.push_back(vector<point>());
    for (int i = 0; i < Field->all_points.size(); i++) {
        point p(*Field->all_points[i]);
        nearest_points[Field->all_points[i]->tag].push_back(p);
    }
    for (int i = 0; i < k; i++) { core.push_back(vector<point*>());  for (int j = 0; j < k; j++) { point* New = new point(0, 0);  core[i].push_back(New); } }

    int f = 1;
    while (f)
    {
        f = 0;
        for (int i = 0; i < k; i++)
        {
            //vector<point*> temp; for (int i = 0; i < k; i++) { point* New = new point(0, 0);  temp.push_back(New); }
            //temp = K_MEANS_function(k, nearest_points[i]);
            core[i] = K_MEANS_function(k, nearest_points[i]);
        }
        nearest_points.clear();
        for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it)
        {
            int old_tag = (*it)->tag;
            double min = 10000;
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++)
                {
                    double distance = dist(**it, *core[i][j]);
                    if (distance < min) { min = distance; (*it)->tag = i; } //nahodim novuyu metku
                }
            if (old_tag != (*it)->tag) f = 1;
        }
        for (int i = 0; i < k; i++) nearest_points.push_back(vector<point>());
        for (int i = 0; i < Field->all_points.size(); i++) {
            point p(*Field->all_points[i]);
            nearest_points[Field->all_points[i]->tag].push_back(p);
        }
    }

    for (int i = 0; i < k; i++) //dobavlyaem klasteryi
    {
        cluster* NEW = new cluster;
        Find_Clusters->Cluster.push_back(NEW);
        Find_Clusters->Cluster[i]->type = "k-means with cores";
    }
    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) Find_Clusters->Cluster[(*it)->tag]->Single_Cluster.push_back(*it);

    for (int i = 0; i < k; i++)
    {
        ofstream out("C:/3hell/Second/Second/cluster" + to_string(i + 1) + ".txt"); //vyivid v fayl
        for (auto it = Find_Clusters->Cluster[i]->Single_Cluster.begin(); it != Find_Clusters->Cluster[i]->Single_Cluster.end(); it++)
        {
            out << (*it)->x << " " << (*it)->y << endl;
        }
    }
    ofstream out1("C:/3hell/Second/Second/cores of k-means.txt");
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
    {
         out1 << core[i][j]->x << " " << core[i][j]->y << endl;
    }
    out1.close();
    writeLog("Выполнена команда EM_MEANS_CORE");
    cout << "Выполнено построение кластеров способом k-means c ядрами" << endl;
}

void controller::FOREL(double r, double e)
{
    vector<point*> vhod = Field->all_points;
    double R = 0.2;
    for (int i = 0; i < 3; i++)
    {
        formal_element* New = forel_function(vhod, R, i);
        stree_function(R + E, vhod, i);
        Find_Clusters->Formal_elements.push_back(New);
        vhod.clear();
        
        for (int j = 0; j < Find_Clusters->Formal_elements[i]->elements.size(); j++) { 
            vhod.push_back(Find_Clusters->Formal_elements[i]->elements[j]); }
        R *= e;
    }
    writeLog("Выполнен алгоритм форель");
    cout << "Выполнен алгоритм форель" << endl;
}

formal_element* forel_function(vector<point*> vhod, double r, int A)
{
    formal_element* FE = new formal_element(0);
    int shag = 0;
    string s2;
    s2 = "C:/3hell/Second/Second/forel/all_points_" + to_string(A) + ".txt";
    ofstream all; all.open(s2);
    for (int i = 0; i < vhod.size(); i++) { all << vhod[i]->x << " " << vhod[i]->y << endl; }
    int K = vhod.size();
    double** D = new double* [K];
    for (int i = 0; i < K; i++) D[i] = new double[K];
    vector<point*>::iterator it1 = vhod.begin();
    
    for (int j = 0; j < K; j++)
    {
        vector<point*>::iterator it2 = vhod.begin();
        for (int i = 0; i < K; i++)
        {
            D[j][i] = dist(**it1, **it2);
            it2++;
        }
        it1++;
    }

    int f = 1;
    while (f)
    {
        f = 0;
        shag++;
        ofstream instruction;
        string s0 = "C:/3hell/Second/Second/forel/forel_instruction_" + to_string(A) + "_shag_" + to_string(shag) + ".txt";
        instruction.open(s0);
        ofstream points;
        string s1 = "C:/3hell/Second/Second/forel/points_with_marks_" + to_string(A) + "_shag_" + to_string(shag) + ".txt";
        points.open(s1);
        ofstream fe;
        string s3 = "C:/3hell/Second/Second/forel/formal_elements_" + to_string(A) + "_shag_" + to_string(shag) + ".txt";
        fe.open(s3);
        ofstream circles;
        string s4 = "C:/3hell/Second/Second/forel/circles_" + to_string(A) + "_shag_" + to_string(shag) + ".txt";
        circles.open(s4);

        for (int i = 0; i < vhod.size(); i++)
        {
            if (vhod[i]->mark == 0)
            {
                //cout << "jhg" << shag<< endl;
                f = 1;
                double summa_x = 0, summa_y = 0; int kolichestvo = 0;
                for (int j = 0; j < K; j++)
                {
                   
                    if ((D[i][j] < r) & (vhod[j]->mark == 0))
                    {
                        vhod[j]->mark = 1;
                        summa_x += vhod[j]->x; summa_y += vhod[j]->y; kolichestvo++;
                        //cout << summa_x << endl;
                    }
                }
                point* New = new point(summa_x / kolichestvo, summa_y / kolichestvo);
                FE->elements.push_back(New);
                break;
            }
        }
        for (int i = 0; i < vhod.size(); i++) { if (vhod[i]->mark == 1) { points << vhod[i]->x << " " << vhod[i]->y << endl;} }

        for (int i = 0; i < FE->elements.size(); i++) 
        {  
            circles << FE->elements[i]->x << " " << FE->elements[i]->y << " " << r << endl;
            fe << FE->elements[i]->x << " " << FE->elements[i]->y << endl; 
        } 
        instruction << "plot 'C:/3hell/Second/Second/forel/points_with_marks_" + to_string(A) + "_shag_" + to_string(shag) + ".txt' with points lc rgb 'black', 'C:/3hell/Second/Second/forel/all_points_" + to_string(A) + ".txt' with points lc rgb 'purple' , 'C:/3hell/Second/Second/forel/formal_elements_" + to_string(A) + "_shag_" + to_string(shag) + ".txt' lc rgb 'red', 'C:/3hell/Second/Second/forel/circles_" + to_string(A) + "_shag_" + to_string(shag) + ".txt' using 1:2:3 with circle lc rgb 'green' ";
    }
    return FE;
}

vector<point*> K_MEANS_function(int k, vector<point> all_points)
{
    double maxX = -10000, minX = 10000, maxY = -10000, minY = 10000, centerX = 0, centerY = 0;
    vector<double> z; //tam hranyatsya chisla, srednee kotoryih nado poschitat'
    vector<point*> kpoints; //vektor iz randomnyih k tochek
    double** D;
    int K = all_points.size();

    for (auto it = all_points.begin(); it != all_points.end(); ++it) { if ((it)->x < minX) minX = (it)->x; if ((it)->y < minY) minY = (it)->y; if ((it)->x > maxX) maxX = (it)->x; if ((it)->y > maxY) maxY = (it)->y; }
    z.push_back(maxX); z.push_back(minX); centerX = srednee(z); z.clear();
    z.push_back(maxY); z.push_back(minY); centerY = srednee(z); z.clear();
    double a = centerX, b = centerY + 0.7 * (maxY - centerY); //tochka, kotoruyu budem dvigat'
    for (int i = 0; i < k; i++)
    {
        point* New = new point((a - centerX) * cos(6.28 / k) - (b - centerY) * sin(6.28 / k) + centerX, (a - centerX) * sin(6.28 / k) + (b - centerY) * cos(6.28 / k) + centerY);
        kpoints.push_back(New);
        a = New->x; b = New->y;
    }

    /*ofstream out("C:/3hell/Second/Second/kpoints.txt");
    for (int i = 0; i < kpoints.size(); i++) out << kpoints[i]->x << " " << kpoints[i]->y << endl;
    out.close();*/
    int f = 1;
    while (f)
    {
        f = 0;
        for (auto it = all_points.begin(); it != all_points.end(); ++it)
        {
            int old_tag = (it)->tag;
            double min = 10000;
            for (int i = 0; i < kpoints.size(); i++)
            {
                double distance = dist(*it, *kpoints[i]);
                if (distance < min) { min = distance; (it)->tag = i; } //nahodim novuyu metku
            }
            if (old_tag != (it)->tag) f = 1;
        }

        vector<vector<point*>> nearest_points; //tocki, blicsheychie k i-oy
        for (int i = 0; i < k; i++) nearest_points.push_back(vector<point*>());
        for (int i = 0; i < all_points.size(); i++) nearest_points[all_points[i].tag].push_back(&all_points[i]);

        for (int i = 0; i < k; i++) //pereschityivaem tcentr
        {

            vector<double> X;
            vector<double> Y;
            for (int j = 0; j < nearest_points[i].size(); j++)
            {
                X.push_back(nearest_points[i][j]->x);
                Y.push_back(nearest_points[i][j]->y);
            }
            kpoints[i]->x = srednee(X);
            kpoints[i]->y = srednee(Y);
        }
    }
    return kpoints;
}

string timeLog()
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);
    string tmp = to_string(newtime.tm_mday);
    tmp += "." + to_string(newtime.tm_mon + 1) + " " + to_string(newtime.tm_hour);
    tmp += ":" + to_string(newtime.tm_min) + ":" + to_string(newtime.tm_sec) + " -> ";
    return tmp;
}

void controller::stree_function(double r, vector<point*> points, int shag)
{
    set<int> tree = { 0 };
    int P = points.size();
    int a, b;
    double** D = new double* [P];
    for (int i = 0; i < P; i++) D[i] = new double[P];
    double min = 10000;
    vector<point*>::iterator it1 = points.begin();

    for (int j = 0; j < P; j++)
    {
        vector<point*>::iterator it2 = points.begin();
        for (int i = 0; i < P; i++)
        {
            D[j][i] = dist(**it1, **it2);
            it2++;
        }
        it1++;
    }


    for (int k = 0; k < P - 1; k++)
    {
        for (auto it = tree.begin(); it != tree.end(); it++)
        {
            for (int j = 1; j < P; j++) if ((D[(*it)][j] < min) & (tree.find(j) == tree.end()))
            {
                min = D[(*it)][j];
                a = j;
                b = (*it);
            }
        }
        min = 10000;
        tree.insert(a);
        line* New = new line(points[b]->x, points[b]->y, points[a]->x, points[a]->y);
        Lines.push_back(New);
    }
    
    string s = "C:/3hell/Second/Second/forel/tree_" + to_string(shag) + ".txt";
    ofstream out; //vyivid v fayl
    out.open(s);
    for (auto it = Lines.begin(); it != Lines.end(); it++)
    {
        if (dist((*it)->single_line[1], (*it)->single_line[0]) < r) {
            out << (*it)->single_line[1]->x << " " << (*it)->single_line[1]->y << endl;
            out << (*it)->single_line[0]->x << " " << (*it)->single_line[0]->y << endl << endl << endl;
        }
    }
    out.close();
    Lines.clear();
}

void controller::IERA()
{
    //double r = Field->maxD / k; 
    ofstream out("C:/3hell/Second/Second/iera/all_lines.txt");
    int amount = Field->all_points.size() + 1;
    int shag = -1;
    vector <point> temp; //tochi v dannyiy moment vremeni
    vector < vector <double> > MATRIX;
    for (int j = 0; j < amount - 1; j++) 
    {
        vector <double> C; MATRIX.push_back(C); for (int i = 0; i < amount - 1; i++) { MATRIX[j].push_back(Field->D[j][i]); }
    }
    for (int i = 0; i < amount - 1; i++) { point P(Field->all_points[i]->x, Field->all_points[i]->y); temp.push_back(P); }

    while (amount > 2)
    {
        amount--; shag++;
        point New(0, 0);
        ofstream all;
        string s1 = "C:/3hell/Second/Second/iera/all_points_shag_" + to_string(shag) + ".txt";
        all.open(s1);
        ofstream line;
        string s2 = "C:/3hell/Second/Second/iera/line_shag_" + to_string(shag) + ".txt";
        line.open(s2);
        ofstream new_point;
        string s3 = "C:/3hell/Second/Second/iera/point_shag_" + to_string(shag) + ".txt";
        new_point.open(s3);

        for (int i = 0; i < temp.size(); i++) all << temp[i].x << " " << temp[i].y << endl;
        double min = 10000; int a = 0; int b = 0;
        for (int i = 0; i < amount; i++) for (int j = i + 1; j < amount; j++) { if (MATRIX[i][j] < min) { min = MATRIX[i][j]; a = i; b = j; }}
        out << temp[a].x << " " << temp[a].y << endl << temp[b].x << " " << temp[b].y << endl << endl << endl;
        line << temp[a].x << " " << temp[a].y << endl << temp[b].x << " " << temp[b].y << endl;
        New.x = (temp[a].x + temp[b].x) / 2; New.y = (temp[a].y + temp[b].y) / 2;
        new_point << New.x << " " << New.y << endl;

        temp.erase(temp.begin() + b); temp.erase(temp.begin() + a); temp.push_back(New); 
        MATRIX.erase(MATRIX.begin() + b); MATRIX.erase(MATRIX.begin() + a); for (int i = 0; i < amount - 2; i++) { MATRIX[i].erase(MATRIX[i].begin() + b);  MATRIX[i].erase(MATRIX[i].begin() + a); }
        vector<double> NewM; for (int i = 0; i < amount - 1; i++) { NewM.push_back(dist(New, temp[i])); } MATRIX.push_back(NewM); for (int i = 0; i < amount - 2; i++) { MATRIX[i].push_back(dist(New, temp[i])); }
    }
    writeLog("Выполнен иерархический алгоритм");
    cout << "Выполнен иерархический алгоритм" << endl;
}

void controller::FACTOR()
{
    vector<vector<double>> X; //2*N
    vector<vector<double>> XT; //N*2
    vector<vector<double>> P; //2*2
    int N = Field->all_points.size();
    for (int i = 0; i < N; i++) { vector<double> d; d.push_back(Field->all_points[i]->x); d.push_back(Field->all_points[i]->y); X.push_back(d); }
    vector<double> x; vector<double> y;
    for (int i = 0; i < N; i++) x.push_back(Field->all_points[i]->x); for (int i = 0; i < N; i++) y.push_back(Field->all_points[i]->y);  XT.push_back(x); XT.push_back(y);
    for (int i = 0; i < 2; i++) { vector<double> d; d.push_back(0); d.push_back(0); P.push_back(d); }

    //for (int i = 0; i < 2; i++) { for (int j = 0; j < N; j++) cout << X[j][i] << " "; cout << endl; } cout << endl;
    //for (int i = 0; i < N; i++) { for (int j = 0; j < 2; j++) cout << XT[j][i] << " "; cout << endl; } cout << endl;

    //umnojenie matritz
    for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < N; k++) P[i][j] += X[k][j] * XT[i][k]; 

    //for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) cout << P[j][i] << " "; cout << endl; } cout << endl;

    //normirovanie matritzyi
    for (int i = 0; i < 2; i++) {
        double d = 0; for (int j = 0; j < 2; j++) d += P[i][j] * P[i][j]; d = sqrt(d);
        for (int j = 0; j < 2; j++) P[i][j] /= d;
    }
    for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) cout << P[j][i] << " "; cout << endl; } cout << endl;

    //nahojdenie sobstvennyih znacheniy
    double transP = P[0][0] + P[1][1], detP = P[0][0] * P[1][1] - P[0][1] * P[1][0];
    double lyambda1 = (sqrt(transP * transP - 4 * detP) + transP) / 2, lyambda2 = (- sqrt(transP * transP - 4 * detP) + transP) / 2;

    cout << lyambda1 << endl << lyambda2 << endl;
   // for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) cout << P[j][i] << " "; cout << endl; } cout << endl;

    //nahojdenie sobstvennyih vectorov i tochki
    point p1(0, 0), p2(0, 0), center(0, 0);
    p1.x = P[1][0] * (-1); p1.y = P[0][0] - lyambda1; p2.x = P[1][0] * (-1); p2.y = P[0][0] - lyambda2;
    double maxX = -10000, minX = 10000, maxY = -10000, minY = 10000;
    vector<double> z; //tam hranyatsya chisla, srednee kotoryih nado poschitat'
    for (auto it = Field->all_points.begin(); it != Field->all_points.end(); ++it) { if ((*it)->x < minX) minX = (*it)->x; if ((*it)->y < minY) minY = (*it)->y; if ((*it)->x > maxX) maxX = (*it)->x; if ((*it)->y > maxY) maxY = (*it)->y; }
    z.push_back(maxX); z.push_back(minX); center.x = srednee(z); z.clear();
    z.push_back(maxY); z.push_back(minY); center.y = srednee(z); z.clear();
    p1.x += center.x; p1.y += center.y; p2.x += center.x; p2.y += center.y;
    ofstream out("C:/3hell/Second/Second/factors.txt");
    out << center.x << " " << center.y << endl << p1.x << " " << p1.y << endl << endl << endl;
    out << center.x << " " << center.y << endl << p2.x << " " << p2.y << endl << endl << endl;

    writeLog("Найдены факторы");
    cout << "Найдены факторы" << endl;
}