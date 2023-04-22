import math
import os
import sys
import textwrap
import argparse


class ArgumentParser(argparse.ArgumentParser):
    def __init__(self, *args, **kwargs):
        super(ArgumentParser, self).__init__(*args, **kwargs)
        self.program = {key: kwargs[key] for key in kwargs }
        self.options = []

    def add_argument(self, *args, **kwargs):
        super(ArgumentParser, self).add_argument(*args, **kwargs)
        option = {}
        option["flags"] = [item for item in args]
        for key in kwargs:
            option[key] = kwargs[key]
        self.options.append(option)

    def print_help(self):
        wrapper = textwrap.TextWrapper(width=100)
        if "usage" in self.program:
            print("Usage: %s" % self.program["usage"])
        else:
            usage = []
            for option in self.options:
                usage += ["[%s %s]" % (item, option["metavar"])
                            if "metavar" in option
                            else "[%s %s]" % (item, option["dest"].upper())
                                 if "dest" in option
                                 else " [%s]" % item for item in option["flags"]]
            wrapper.initial_indent = "Usage: %s" % os.path.basename(sys.argv[0])
            wrapper.subsequent_indent = len(wrapper.initial_indent) * " "
            output = str.join(" ", usage)
            output = wrapper.fill(output)
            print(output)
        print()

        if "description" in self.program:
            print(self.program["description"])
            print()

        print("Аргументы:")
        maxlen = 0
        for option in self.options:
            option["flags2"] = str.join(", ", [" %s %s" % (item, option["metavar"])
                                                if "metavar" in option
                                                else "%s %s" % (item, option["dest"].upper())
                                                     if "dest" in option
                                                     else item for item in option["flags"] ])
            if len(option["flags2"]) > maxlen:
                maxlen = len(option["flags2"])
        for option in self.options:
            template = "  %-" + str(maxlen) + "s  "
            wrapper.initial_indent = template % option["flags2"]
            wrapper.subsequent_indent = len(wrapper.initial_indent) * " "
            if "help" in option and "default" in option:
                output = option["help"]
                output += " (по умолчанию: '%s')" % option["default"] if isinstance(option["default"], str) else " (по умолчанию: %s)" % str(option["default"])
                output = wrapper.fill(output)
            elif "help" in option:
                output = option["help"]
                output = wrapper.fill(output)
            elif "default" in option:
                output = "По умолчанию: '%s'" % option["default"] if isinstance(option["default"], str) else "Default: %s" % str(option["default"])
                output = wrapper.fill(output)
            else:
                output = wrapper.initial_indent
            print(output)


def standard_uniform_distribution_with_interval(seq, m, a, b):
    return [b * (elem / m) + a for elem in seq]


def triangle_distribution(seq, m, a, b):
    new_seq = []
    for i in range(0, len(seq) - 1, 2):
        new_seq.append(a + b * ((seq[i] / m) + (seq[i + 1] / m) - 1))
    return new_seq


def general_exponential_distribution(seq, m, a, b):
    return [-b * math.log(elem / m) + a for elem in seq]


def normal_distribution(seq, m, a, b):
    new_seq = []
    for i in range(0, len(seq) - 1, 2):
        new_seq.append(a + b *
                           math.sqrt(-2 * math.log(1 - (seq[i] / m))) *
                           math.cos(2 * math.pi * (seq[i + 1] / m)))
        new_seq.append(a + b *
                           math.sqrt(-2 * math.log(1 - (seq[i] / m))) *
                           math.sin(2 * math.pi * (seq[i + 1] / m)))
    return new_seq


def gamma_distribution(seq, m, a, b, c):
    new_seq = []
    for i in range(0, len(seq), c):
        mult_values = 1
        for j in range(c):
            mult_values *= 1 - (seq[i + j] / m) 
        new_seq.append(a - b * math.log(mult_values))
    return new_seq


def lognormal_distribution(seq, m, a, b):
    norm_seq = normal_distribution(seq, m, a, b)
    return [a + math.exp(b - elem) for elem in norm_seq]


def logistic_distribution(seq, m, a, b):
    return [a + b * math.log((elem / m) / (1 - elem / m)) for elem in seq]


def binom_coef(n, k):
    if k > n:
        return 0
    if not k or n == k:
        return 1
    return binom_coef(n - 1, k - 1) + binom_coef(n - 1, k)


def binomial_distribution(seq, m, a, b):
    new_seq = []
    for i in range(len(seq)):
        u = seq[i] / m
        s = 0
        k = 0
        while True:
            s += binom_coef(b, k) * pow(a, k) * pow(1 - a, b - k)
            if s > u:
                new_seq.append(k)
                break
            if k < (b - 1):
                k += 1
                continue
            new_seq.append(b)
    return new_seq


METHOD_NAME_TO_FUNC = {"st": standard_uniform_distribution_with_interval,
                       "tr": triangle_distribution,
                       "ex": general_exponential_distribution,
                       "nr": normal_distribution,
                       "ln": lognormal_distribution,
                       "ls": logistic_distribution,
                       "bi": binomial_distribution}


def main(args):
    # получение числовой последовательности из файла
    file_name = args.fn
    with open(file_name) as f:
        lines = f.readlines()
    file_content = "".join(lines)
    num_seq_str = file_content.split(",")
    num_seq = [int(num) for num in num_seq_str if len(num)]
    
    # получение максимального элемента последовательности
    max_elem = max(num_seq)
    m = max_elem + 1
    method_name = args.dist

    # преобразование к заданному распределению
    if method_name != "gm":
        processed_seq = METHOD_NAME_TO_FUNC[method_name](num_seq, m,
                                                         args.p1, args.p2)
    else:
        processed_seq = gamma_distribution(num_seq, m, args.p1, args.p2,
                                           args.p3)
    
    f = open(f"distr-{method_name}.dat", "w")
    f.write(",".join([str(elem) for elem in processed_seq]))
    f.close()


if __name__ == "__main__":
    parser = ArgumentParser(description="Программа приводит последовательность чисел из входного файла к заданному распределению.", argument_default=argparse.SUPPRESS, allow_abbrev=False, add_help=False)
    parser.add_argument("-f", "--fn", type=str, help="имя файла с входной последовательностью")
    parser.add_argument("-d", "--dist", type=str, help="код распределения для преобразования последовательности (st, tr, ex, nr, gm, ln, ls, bi)")
    parser.add_argument("-p1", "-a", type=float, help="1-й параметр для генерации ПСЧ заданного распределения")
    parser.add_argument("-p2", "-b", type=int, help="2-й параметр для генерации ПСЧ заданного распределения")
    parser.add_argument("-p3", "-c", type=int, default=None, help="3-й параметр для генерации ПСЧ гамма-распределением")
    parser.add_argument("-h", "--help", action="help", help="выводит текст помощи")
    args = parser.parse_args()
    main(args)