#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <functional>
#include <map>
#include <bitset>
// #include <boo

void print_vector(std::vector<int> some_vec)
{
    // вывести содержимое вектора - используется при тестировании
    std::cout << std::endl;
    for (int i = 0; i < some_vec.size(); i++)
    {
        std::cout << some_vec[i] << ',';
    }
    std::cout << std::endl;
}


std::vector<int> split_to_int(std::string str, std::string token)
{
    // функция конвертации строки с числами в вектор с числами
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


std::vector<std::string> split_to_str(std::string str, std::string token)
{
    // функция конвертации строки с числами в вектор со строками
    std::vector<std::string> result;
    while(str.size())
    {
        int index = str.find(token);
        if(index != std::string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index + token.size());
            if (str.size() == 0) result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


std::string help_message =
"Аргументы должны вводиться следующим образом:"
"\n /g:название_генератора - указывает на метод генерации ПСЧ, принимает "
"следующие значения: lc, add, 5p, lfsr, nfsr, mt, rc4, rsa, bbs"
"\n /i:инициализирущий_вектор - записывать в формате {x,y,z}, где x, y и z - "
"параметры генератора."
"\n /n:количество_генерируемых_чисел - целое число, определяющее количество "
"генерируемых чисел. По умолчанию имеет значение 10000."
"\n /f:имя_файла - полное имя файла, в который будут записываться данные. "
"По умолчанию имеет значение 'rnd.dat'."
"\n /h - информация о параметрах командной строки. При указании параметра /g: "
"будет также появляется дополнительная информация о параметрах генератора.\n";

std::string lc_help_message =
"Для линейного конгруэнтного генератора необходимо ввести параметры следующим "
"образом: '/i:m,a,c,last_x' - где:"
"\n m - модуль,"
"\n a - множитель,"
"\n c - приращение (инкремент),"
"\n last_x - начальное значение\n";

std::string add_help_message =
"Для аддитивного генератора необходимо ввести параметры следующим "
"образом: '/i:m,j,k,x_0,x_1,...,x_n' - где:"
"\n m - модуль,"
"\n j - некоторый индекс (j > k >= 1),"
"\n k - некоторый индекс (j > k >= 1),"
"\n x_0,x_1,...,x_n - вводимая последовательность\n";

std::string lfsr_help_message =
"Для генератора РСЛОС необходимо ввести параметры следующим "
"образом: '/i:r,c' - где:"
"\n r - начальное состояние регистра (32 числа '0' или '1' без разделителей),"
"\n c - коэффициенты многочлена (32 числа '0' или '1' без разделителей)\n";

std::string other_gen_help_message = "Данный генератор ещё не готов или ошибка "
"в названии генератора!\n";


std::string find_generate_method(std::string method)
{
    // функция для нахождения названия метода в 
    // списке допустимых названий методов
    std::vector<std::string> method_list = {"lc", "add", "5p", "lfsr"};
    for (int i = 0; i < method_list.size(); i++)
    {
        if (method == method_list[i])
        {
            return method;
        }
    }
    return "";
}


void show_help_message(std::string g="", bool h=false)
{
    // функция печати в консоли вспомогательной информации
    if (h || g == "")
    {
        std::cout << help_message << std::endl;
    }
    if (g == "lc")
    {
        std::cout << std::endl << lc_help_message << std::endl;        
    }
    else if (g == "add")
    {
        std::cout << std::endl << add_help_message << std::endl;        
    }
    else if (g == "lfsr")
    {
        std::cout << std::endl << lfsr_help_message << std::endl;        
    }
    else if (g != "")
    {
        std::cout << std::endl << other_gen_help_message << std::endl;
    }
}


void show_progress(int i, int n)
{
    // функция для отображения в консоли статуса генерации последовательности
    if (i % (static_cast<int> (static_cast<float> (n) / 10) + 1) == 0)
    {
        std::cout << "\r" << "Генерация выполнена на " << (i * 100) / n 
                  << "%" << std::flush;
    }
}


int linear_congruent_method(int n, std::vector<int> params, std::string f)
{
    // реализация линейного конгруэнтного метода
    int m = params[0];
    int a = params[1];
    int c = params[2];
    int last_x = params[3];

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        last_x = (a * last_x + c) % m;
        output_file << last_x << ',';
        show_progress(i, n);
    }
    output_file.close();
    return 0;
}


int additive_method(int n, std::vector<int> params, std::string f)
{
    // реализация аддитивного метода
    int m = params[0];
    params.erase(params.begin());
    int j = params[1];
    params.erase(params.begin());
    int k = params[2];
    params.erase(params.begin());
    int seq_size = params.size();

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        int next_x = (params[seq_size - k] + params[seq_size - j]) % m;
        params.push_back(next_x);
        params.erase(params.begin());
        output_file << next_x << ',';
        show_progress(i, n);
    }
    output_file.close();
    return 0;
}


int lfsr_method(int n, std::vector<std::string> str_init, std::string f)
{
    // реализация РСЛОС метода
    const int bit_amount = 32;
    std::bitset<bit_amount> init_register(str_init[0]);
    std::bitset<bit_amount> poly_coeffs(str_init[1]);

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        output_file << init_register.to_ulong() << ',';

        bool current_bit = 0;
        for (int j = 0; j < bit_amount; j++)
        {
            current_bit ^= init_register[j] * poly_coeffs[j];
        }
        init_register >>= 1;
        init_register[bit_amount - 1] = current_bit;

        show_progress(i, n);
    }
    output_file.close();
    return 0;
}


void parse_args(std::string &g, std::vector<int> &init, int &n, std::string &f, 
                bool &h, std::vector<std::string> &str_init, int arg_amount,
                char** args)
{
    // функция для парсинга параметров
    std::vector<std::pair<std::string, int>> default_args = {{"/g:", 0},
        {"/i:", 1}, {"/n:", 2}, {"/f:", 3}, {"/h", 4}};
    int default_args_amount = default_args.size();
    f = "rnd.dat";
    n = 10000;
    std::string pre_init;
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
                        pre_init = cur_arg;
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
        if (g == "lfsr")
        {
            str_init = split_to_str(pre_init, ",");
        }
        else
        {
            init = split_to_int(pre_init, ",");
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
    std::vector<std::string> str_init;
    int n;
    std::string f;
    bool h = false;

    parse_args(g, init, n, f, h, str_init, argc, argv);
    
    g = find_generate_method(g);

    if (g != "" && (!init.empty() || !str_init.empty()))
    {
        if (h)
        {
            show_help_message(g, h);
        }
        std::cout << "Генерация выполнена на ";
        if (g == "lc")
        {
            linear_congruent_method(n, init, f);
        }
        else if (g == "add")
        {
            additive_method(n, init, f);
        }
        else if (g == "lfsr")
        {
            lfsr_method(n, str_init, f);
        }
        
        std::cout << "\r" << "Генерация выполнена на " << 100 << "%" << std::flush 
              << std::endl;
        std::cout << "Результат работы генератора сохранен в " + f + "\n";

    }
    else
    {
        show_help_message(g, h);
    }
}
