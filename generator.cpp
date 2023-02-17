#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <functional>
#include <map>
// #include <chrono>
// #include <thread>


std::string help_message =
"��������� ������ ��������� ��������� �������:"
"\n /g:��������_���������� - ��������� �� ����� ��������� ���, ��������� "
"��������� ��������: lc, add, 5p, lfsr, nfsr, mt, rc4, rsa, bbs"
"\n /i:���������������_������ - ���������� � ������� {x,y,z}, ��� x, y � z - "
"��������� ����������."
"\n /n:����������_������������_����� - ����� �����, ������������ ���������� "
"������������ �����. �� ��������� ����� �������� 10000."
"\n /f:���_����� - ������ ��� �����, � ������� ����� ������������ ������. "
"�� ��������� ����� �������� 'rnd.dat'."
"\n /h - ���������� � ���������� ��������� ������. ��� �������� ��������� /g: "
"����� ����� ���������� �������������� ���������� � ���������� ����������.\n";

std::string lc_help_message =
"��� ��������� ������������� ���������� ���������� ������ ��������� ��������� "
"�������: '/i:m,a,c,last_x' - ���:"
"\n m - ������,"
"\n a - ���������,"
"\n c - ���������� (���������),"
"\n last_x - ��������� ��������\n";

std::string other_gen_help_message = "������ ��������� ��� �� ����� ��� ������ "
"� �������� ����������!\n";


void show_help_message(std::string g="", bool h=false)
{
    if (h || g == "")
    {
        std::cout << help_message << std::endl;
    }
    if (g == "lc")
    {
        std::cout << std::endl << lc_help_message << std::endl;        
    }
    else if (g != "")
    {
        std::cout << std::endl << other_gen_help_message << std::endl;
    }
}


void show_progress(int i, int n)
{
    if (i % (static_cast<int> (static_cast<float> (n) / 10) + 1) == 0)
    {
        std::cout << "\r" << "��������� ��������� �� " << (i * 100) / n 
                  << "%" << std::flush;
    }
}


int linear_congruent_method(int n, std::vector<int> params, std::string f)
{
    int m = params[0];
    int a = params[1];
    int c = params[2];
    int last_x = params[3];

    std::ofstream output_file;
    output_file.open(f);
    std::cout << "��������� ��������� �� ";
    for (int i = 0; i < n; i++)
    {
        last_x = (a * last_x + c) % m;
        output_file << last_x << " ";
        show_progress(i, n);
    }
    std::cout << "\r" << "��������� ��������� �� " << 100 << "%" << std::flush 
              << std::endl;
    output_file.close();
    std::cout << "��������� ������ ���������� �������� � " + f + "\n";
    return 0;
}


std::vector<int> split(std::string str, std::string token)
{
    std::vector<int> result;
    while(str.size())
    {
        int index = str.find(token);
        if(index != std::string::npos)
        {
            result.push_back(std::stoi(str.substr(0,index)));
            str = str.substr(index + token.size());
            if (str.size() == 0) result.push_back(std::stoi(str));
        }
        else
        {
            result.push_back(std::stoi(str));
            str = "";
        }
    }
    return result;
}


void parse_args(std::string &g, std::vector<int> &init, int &n, std::string &f, 
                bool &h, int arg_amount, char** args)
{
    std::vector<std::pair<std::string, int>> default_args = {{"/g:", 0},
        {"/i:", 1}, {"/n:", 2}, {"/f:", 3}, {"/h", 4}};
    int default_args_amount = default_args.size();
    f = "rnd.dat";
    n = 10000;
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
                        h = true;
                        break;
                    }
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        show_help_message(g, h);
    }
}


int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");
    
    std::string g;
    std::vector<int> init;
    int n;
    std::string f;
    bool h = false;

    parse_args(g, init, n, f, h, argc, argv);
    
    if (g != "" && !init.empty())
    {
        if (h)
        {
            show_help_message(g, h);
        }
        if (g == "lc")
        {
            linear_congruent_method(n, init, f);
        }
    }
    else
    {
        show_help_message(g, h);
    }
}
