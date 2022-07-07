#include "Second.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    interface MAIN;

    MAIN.Fill();

    MAIN.ReadCommand();

    for (auto& el : MAIN.instructions)
    {
        if (!MAIN.Check(el))
        {
            MAIN.Log_wrong_command(el);
            return 1; //est' oshibki v "instraction.txt"
        }
        else if (el == "EXIT") return 0; //programma srabotala bez oshibok
        int code = MAIN.CONTROLLER(el);
        if (code == 0)
        {
            MAIN.Log_wrong_means(el);
            return 1;
        }
        if (code == 2)
        {
            MAIN.Log_access_denied(el);
            return 1;
        }
    }
    return 0;
}
