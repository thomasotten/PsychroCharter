#include <iostream>
#include <algorithm> //transform
#include <sstream> //stringstream
#include "PsychroChartSI.h"
#include "PsychroChartEnglish.h"

using namespace std;

/* template for a string to number converter */
template <typename T> T StringToNumber ( const string &Text )//Text not by const reference so that the function can be used with a
{
    //character array as argument
    std::stringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

/* InputPrompter: prompt and accept input, checks input from one or two list of options */
string InputPrompter(string prompt_str, int num_opts, string options[], int num_opts2, string options2[])
{
    string val;
    bool valid_response = false;
    do
    {
        cout << prompt_str;
        getline(cin,val);
        transform(val.begin(), val.end(), val.begin(), ::toupper);

        for (int i = 0; i < num_opts; i++)
        {
            if (val.compare(options[i]))
            {
                valid_response = true;
                break;
            }
        }
        if (num_opts2 > 0)
        {
            for (int i = 0; i < num_opts2; i++)
            {
                if (val.compare(options2[i]))
                {
                    valid_response = true;
                    break;
                }
            }
        }

    }
    while(!valid_response);
    return val;
}

/* InputPrompter: prompt and accept only a number, and make sure it is within range */
double InputPrompter(string prompt_str, double min_range, double max_range)
{
    string input_str = "";
    double val = 0;
    do
    {
        cout << prompt_str;
        getline(cin,input_str);
        if (input_str.empty() || input_str.find_first_not_of(".0123456789") != std::string::npos)
            val = max_range+1;  //ask again
        else
            val = StringToNumber<double>(input_str);
    }
    while( val < min_range || val > max_range );
    return val;
}

/* arr2str:  turns an array of strings into a comma delimited string. */
string arr2str(int num_opts, string opts[])
{
    string opts_str = "";
    for (int i = 0; i < num_opts; i++)
    {
        opts_str = opts_str +  opts[i];
        if (i != num_opts-1)
            opts_str += ",";
    }
    return opts_str;
}

int main()
{
    Chart *psychro = 0;
    cout << "Welcome to / Willkommen zu PsychroCharter..." << endl;
    const int num_prop_opts = 5;
    string prop_opts[num_prop_opts] = {"ELEV","DB","WB","RH","GR"};
    string prop_opts_str = arr2str(num_prop_opts, prop_opts);
    const int num_cmd_opts = 5;
    string cmd_opts[num_cmd_opts] = {"DBWB","DBDP","DBRH","DBGR", "QUIT"};
    string cmd_opts_str = arr2str(num_cmd_opts, cmd_opts);

    string unit_opts[] = {"SI", "ENG"};
    string units = InputPrompter("Please choose a unit-system [SI,ENG] > ", 2, unit_opts, 0, 0);
    AirState state1(units);

    if (units == "SI")
    {
        psychro = (Chart *) (new PsychroChartSI());
        state1.setElev(240.0);
        state1.setDB(23.0);
        state1.setWB(21.0);
    }
    else if (units == "ENG")
    {
        psychro = (Chart *) (new PsychroChartEnglish());
        state1.setElev(788.0);
        state1.setDB(75.0);
        state1.setWB(70.0);
    }

    psychro->DBWB(&state1);  //run once on default values to have something to modify
    state1.print();

    string chng;
    do
    {

        cout << "Enter a property to change [" + prop_opts_str + "], then" << endl;
        chng = InputPrompter("enter a command to run [" + cmd_opts_str + "]  > ", num_prop_opts, prop_opts, num_cmd_opts, cmd_opts);

        if (chng == "DB")
        {
            state1.setDB(InputPrompter("Enter the DB Temperature > ", psychro->min_t, psychro->max_t));
        }
        else if (chng == "WB")
        {
            state1.setWB(InputPrompter("Enter the WB Temperature > ", psychro->min_t, psychro->max_t));
        }
        else if (chng == "DP")
        {
            state1.setDP(InputPrompter("Enter the DP Temperature > ", psychro->min_t, psychro->max_t));
        }
        else if (chng == "GR")
        {
            state1.setGR(InputPrompter("Enter the Grains > ", 0, psychro->max_gr));
        }
        else if (chng == "RH")
        {
            state1.setRH(InputPrompter("Enter the RH > ", 0, 100));
        }
        else if (chng == "ELEV")
        {
            state1.setElev(InputPrompter("Enter the elevation > ", psychro->min_elev, psychro->max_elev));
        }
        else if (chng == "DBWB")
        {
            psychro->DBWB(&state1);
            state1.print();
        }
        else if (chng == "DBRH")
        {
            psychro->DBRH(&state1);
            state1.print();
        }
        else if (chng == "DBGR")
        {
            psychro->DBGR(&state1);
            state1.print();
        }
        else if (chng == "DBDP")
        {
            psychro->DBDP(&state1);
            state1.print();
        }
    }
    while (chng != "QUIT");

    cout << "Auf Wiedersehen!" << endl;
    return 0;
}

