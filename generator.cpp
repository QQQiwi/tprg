#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <functional>
#include <map>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <numeric>


const int BIT_AMOUNT = 32;

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
"образом: '/i:m,k,j,x_0,x_1,...,x_n' - где:"
"\n m - модуль,"
"\n k - некоторый младший индекс (j > k >= 1),"
"\n j - некоторый старший индекс (j > k >= 1),"
"\n x_0,x_1,...,x_n - вводимая последовательность\n";

std::string lfsr_help_message =
"Для генератора РСЛОС необходимо ввести параметры следующим "
"образом: '/i:r,c' - где:"
"\n r - начальное состояние регистра (32 числа '0' или '1' без разделителей),"
"\n c - коэффициенты многочлена (32 числа '0' или '1' без разделителей)\n";

std::string fp_help_message =
"Для пятипараметрического генератора необходимо ввести параметры следующим "
"образом: '/i:s,q_1,q_2,q_3,w' - где:"
"\n q_1,q_2,q_3 - коэффициенты пятипараметрического метода,"
"\n w - размерность чисел в битах (целое положительное число)"
"\n p - начальное состояние регистра (целое положительное число)";

std::string nfsr_help_message =
"Для нелинейной комбинации РСЛОС необходимо ввести параметры следующим "
"образом: '/i:r1,c1,r2,c2,r3,c3' - где:"
"\n r1 - начальное состояние регистра 1 (32 числа '0' или '1' без разделителей),"
"\n c1 - коэффициенты многочлена 1 (32 числа '0' или '1' без разделителей),"
"\n r2 - начальное состояние регистра 2,"
"\n c2 - коэффициенты многочлена 2,"
"\n r3 - начальное состояние регистра 3,"
"\n c3 - коэффициенты многочлена 3\n";

std::string rc4_help_message =
"Для метода RC4 необходимо ввести параметры следующим "
"образом: '/i:x1,x2,...,x256' - где:"
"\n x1,... - первые 256 начальных значений\n";

std::string rsa_help_message =
"Для метода RSA необходимо ввести параметры следующим "
"образом: '/i:n,e,x' - где:"
"\n n - модуль, n = pq, где p и q - простые числа,"
"\n e - случайное целое число, такое, что: 1 < e < (p - 1)(q - 1),"
"\n x - начальное значение из интервала [1, n - 1]\n";


std::string other_gen_help_message = "Данный генератор ещё не готов или ошибка "
"в названии генератора!\n";

std::string find_generate_method(std::string method)
{
    // функция для нахождения названия метода в 
    // списке допустимых названий методов
    std::vector<std::string> method_list = {"lc", "add", "5p", "lfsr", "nfsr",
                                            "rc4", "rsa"};
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
    else if (g == "5p")
    {
        std::cout << std::endl << fp_help_message << std::endl;        
    }
    else if (g == "nfsr")
    {
        std::cout << std::endl << nfsr_help_message << std::endl;        
    }
    else if (g == "rc4")
    {
        std::cout << std::endl << rc4_help_message << std::endl;        
    }
    else if (g == "rsa")
    {
        std::cout << std::endl << rsa_help_message << std::endl;        
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


std::vector<int> get_prime_factors(int n)
{
    std::vector<int> divs;
    while (n % 2 == 0)
    {
        divs.push_back(2);
        n = n/2;
    }
    for (int i = 3; i <= sqrt(n); i = i + 2)
    {
        while (n % i == 0)
        {
            divs.push_back(i);
            n = n/i;
        }
    }
    if (n > 2)
        divs.push_back(n);
    
    return divs;
}

bool check_linear_params(int m, int a, int c)
{
    bool is_pass = false;

    // 1-е условие
    if (std::__gcd(m, c) != 1)
    {
        return false;
    }

    // 2-е условие
    std::vector<int> m_divisors = get_prime_factors(m);
    int a_min_one = a - 1;
    for (int i = 0; i < m_divisors.size(); i++)
    {
        int p = m_divisors[i];
        if (a_min_one % p == 0)
        {
            is_pass = true;
            break;
        }
    }

    // 3-е условие
    if (m % 4 == 0)
    {
        if (a_min_one % 4 == 0 && is_pass) return true;
        return false;
    }
    else
    {
        if (is_pass) return true;
    }

    return false;
}


int linear_congruent_method(int n, std::vector<int> params, std::string f)
{
    // реализация линейного конгруэнтного метода
    int m = params[0];
    int a = params[1];
    int c = params[2];
    int last_x = params[3];

    if (!check_linear_params(m, a, c))
    {
        std::cout << std::endl;
        std::cout << "Генерация отменена - свойства теоремы не выполняются! "
                     "Измените входные параметры.";
        return 1;
    }

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
    int k = params[1];
    params.erase(params.begin());
    int j = params[2];
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


std::bitset<BIT_AMOUNT> lfsr_iteration(std::bitset<BIT_AMOUNT> init_register,
                                       std::bitset<BIT_AMOUNT> poly_coeffs)
{
    bool current_bit = 0;
    for (int j = 0; j < BIT_AMOUNT; j++)
    {
        current_bit ^= init_register[j] * poly_coeffs[j];
    }
    init_register >>= 1;
    init_register[BIT_AMOUNT - 1] = current_bit;
    return init_register;
}


int lfsr_method(int n, std::vector<std::string> str_init, std::string f)
{
    // реализация РСЛОС метода
    std::bitset<BIT_AMOUNT> poly_coeffs(str_init[0]);
    std::bitset<BIT_AMOUNT> init_register(str_init[1]);

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        output_file << init_register.to_ulong() << ',';
        init_register = lfsr_iteration(init_register, poly_coeffs);
        show_progress(i, n);
    }
    output_file.close();
    return 0;
}


int five_param_method(int n, std::vector<int> str_init, std::string f)
{
    // реализация пятипараметрического метода
    int p = str_init[0];
    int q_1 = str_init[1];
    int q_2 = str_init[2];
    int q_3 = str_init[3];
    int w = str_init[4];
    
    boost::dynamic_bitset<> init_register(w, p);

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        output_file << init_register.to_ulong() << ',';

        bool current_bit = 0;
        current_bit = init_register[0]
                      ^ init_register[q_3]
                      ^ init_register[q_2]
                      ^ init_register[q_1];
        init_register >>= 1;
        init_register[w - 1] = current_bit;

        show_progress(i, n);
    }
    output_file.close();
    return 0;
}

// 00000000000000000000000000000110
// 00000000000000000000000000000001
// 00000000000000000000000000000100
// 00000000000000000000000000000001
// 00000000000000000000000000000010
// 00000000000000000000000000000001

int nfsr_method(int n, std::vector<std::string> str_init, std::string f)
{
    std::bitset<BIT_AMOUNT> pc_1(str_init[0]);
    std::bitset<BIT_AMOUNT> ir_1(str_init[1]);
    std::bitset<BIT_AMOUNT> pc_2(str_init[2]);
    std::bitset<BIT_AMOUNT> ir_2(str_init[3]);
    std::bitset<BIT_AMOUNT> pc_3(str_init[4]);
    std::bitset<BIT_AMOUNT> ir_3(str_init[5]);
    std::bitset<BIT_AMOUNT> cur_reg;

    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        cur_reg = (ir_1 & ir_2) ^ (ir_2 & ir_3) ^ ir_3;
        output_file << cur_reg.to_ulong() << ',';

        ir_1 = lfsr_iteration(ir_1, pc_1);
        ir_2 = lfsr_iteration(ir_2, pc_2);
        ir_3 = lfsr_iteration(ir_3, pc_3);

        show_progress(i, n);
    }
    output_file.close();
    return 0;
}


int rc4_method(int n, std::vector<int> str_init, std::string f)
{
    // Метод RC4
    std::vector<int> s_block;
    for (int i = 0; i < 256; i++)
    {
        s_block.push_back(i);
    }
    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + s_block[i] + str_init[i]) % 256;
        std::swap(s_block[i], s_block[j]);
    }

    std::ofstream output_file;
    output_file.open(f);
    
    int i = 0;
    j = 0;
    for (int l = 0; l < n; l++)
    {
        i = (i + 1) % 256;
        j = (j + s_block[i]) % 256;
        std::swap(s_block[i], s_block[j]);
        int t = (s_block[i] + s_block[j]) % 256;
        int k = s_block[t];

        output_file << k << ',';
        show_progress(i, n);
    }
    
    output_file.close();
    return 0;
}


int module_power(int x, int y, int mod)
{
    // возведение в степень по модулю
    if (y == 0)
    {
        return 1;
    }
    int temp = module_power(x, y / 2, mod) % mod;
    temp = (temp * temp) % mod;
    if (y % 2 == 1)
    {
        temp = (temp * x) % mod;
    }
    return temp;
}


int rsa_method(int n, std::vector<int> str_init, std::string f)
{
    // Метод RSA
    int mod = str_init[0];
    int e = str_init[1];
    int x = str_init[2];

    int l = 10;
    std::ofstream output_file;
    output_file.open(f);
    for (int i = 0; i < n; i++)
    {
        std::string z_seq = "";
        for (int j = 0; j < l; j++)
        {
            x = module_power(x, e, mod);
            int cur_z = x % 2;
            z_seq.append(std::to_string(cur_z));
        }
        output_file << std::stoi(z_seq, nullptr, 2) << ',';
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
        if (g == "lfsr" or g == "nfsr")
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
        int generation_status = 0;
        if (h)
        {
            show_help_message(g, h);
        }
        std::cout << "Генерация выполнена на ";
        if (g == "lc")
        {
            generation_status = linear_congruent_method(n, init, f);
        }
        else if (g == "add")
        {
            generation_status = additive_method(n, init, f);
        }
        else if (g == "lfsr")
        {
            generation_status = lfsr_method(n, str_init, f);
        }
        else if (g == "5p")
        {
            generation_status = five_param_method(n, init, f);
        }
        else if (g == "nfsr")
        {
            generation_status = nfsr_method(n, str_init, f);
        }
        else if (g == "rc4")
        {
            generation_status = rc4_method(n, init, f);
        }
        else if (g == "rsa")
        {
            generation_status = rsa_method(n, init, f);
        }
        if (!generation_status)
        {
            std::cout << "\r" << "Генерация выполнена на " << 100 << "%" << std::flush 
                << std::endl;
            std::cout << "Результат работы генератора сохранен в " + f + "\n";
        }        
    }
    else
    {
        show_help_message(g, h);
    }
}

// 1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8