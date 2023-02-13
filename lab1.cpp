#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <functional>
#include <map>

std::string help_message =
"this is help message!";


int linear_congruent_method(int n, std::vector<int> params, std::string f)
{
    int m = params[0];
    int a = params[1];
    int c = params[2];
    int last_x = params[3];

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        last_x = (a * last_x + c) % m;
        output_file << last_x << " ";
    }
    output_file.close();
    return 0;
}

// using MapType = std::map<std::string, std::function<void()>>;

std::vector<int> split(std::string str, std::string token)
{
    std::vector<int>result;
    while(str.size()){
        int index = str.find(token);
        if(index != std::string::npos){
            result.push_back(std::stoi(str.substr(0,index)));
            str = str.substr(index + token.size());
            if (str.size() == 0) result.push_back(std::stoi(str));
        }else{
            result.push_back(std::stoi(str));
            str = "";
        }
    }
    return result;
}


void parse_args(std::string &g, std::vector<int> &init, int &n, std::string &f, bool &h, int arg_amount, char** args)
{
    std::vector<std::pair<std::string, int>> default_args = {{"/g:", 0}, {"/i:", 1}, {"/n:", 2}, {"/f:", 3}, {"/h", 4}};
    int default_args_amount = 5;
    f = "rnd.dat";
    try
    {
        for (int i = 1; i < arg_amount; ++i)
        {
            std::string cur_arg = args[i];
            for (int j = 0; j < default_args_amount; j++)
            {
                if (cur_arg.find(default_args[j].first) != std::string::npos)
                {
                    switch (default_args[j].second)
                    {
                    case 0:
                        cur_arg.erase(0, 3);
                        g = cur_arg;
                        break;
                    case 1:
                        cur_arg.erase(0, 3);
                        init = split(cur_arg, ",");
                        break;
                    case 2:
                        cur_arg.erase(0, 3);
                        n = std::stoi(cur_arg);
                        break;
                    case 3:
                        cur_arg.erase(0, 3);
                        f = cur_arg;
                        break;
                    case 4:
                        std::cout << help_message << std::endl;
                        break;
                    }
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << help_message << std::endl;
    }
}

int main(int argc, char** argv)
{
    // MapType myMap = {
    //     { 'lc', linear_congruent_method }
    // };

    std::string g;
    std::vector<int> init;
    int n;
    std::string f;
    bool h;

    parse_args(g, init, n, f, h, argc, argv);

    try
    {
        linear_congruent_method(n, init, f);
    }
    catch(std::exception& e)
    {
        std::cout << help_message << '\n';
    }
}