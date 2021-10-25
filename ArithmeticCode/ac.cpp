#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>
using namespace std;

// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        printf("%s\n", message);
    exit(1);
}

class ArithmeticCode
{
private:
    int totalLength;
    map<unsigned char, int> statistic;
    map<unsigned char, pair<double, double>> probabilityTable;

public:
    // 统计词频
    void WordFrequencyStatistics(FILE *in)
    {
        int c;
        fseek(in, 0, SEEK_END);
        int size = ftell(in);
        totalLength = size;
        if (size < 0 || size >= 0x7FFFFFFF)
            quit("input file too big");
        fseek(in, 0, SEEK_SET);
        while ((c = getc(in)) != EOF)
        {
            statistic[c]++;
        }
    }
    // 生成概率
    void GenerateProbability()
    {
        int size = totalLength;
        double count = 0;
        for (const auto &p : statistic)
        {
            probabilityTable[p.first] = pair<double, double>(count / (double)size, (count + p.second) / (double)size);
            count += p.second;
        }
    }
    // 编码
    void Encode(FILE *in, FILE *out)
    {
        fseek(in, 0, SEEK_SET);
        double low = 0;
        double high = 100;
        double newLow, newHigh;
        int c = 0;
        while ((c = getc(in)) != EOF)
        {
            newLow = low + (high - low) * probabilityTable[c].first;
            newHigh = low + (high - low) * probabilityTable[c].second;
            low = newLow;
            high = newHigh;
        }
        // 算术编码的溢出问题难以解决
        cout << (low == high) << endl;
        cout << setprecision(15) << low << endl
             << setprecision(15) << high << endl;
        low = (low + high) / 2.0;

        char *str = new char[50];
        memset(str, 0, 20);
        sprintf(str, "%.20f", low); //将a格式化输出到str中，取小数点后7位
        cout << str << endl;
        unsigned long long num = (unsigned long long)low;
        // 把小数输出到文件
        putc(num >> 56, out);
        putc(num >> 48, out);
        putc(num >> 40, out);
        putc(num >> 32, out);
        putc(num >> 24, out);
        putc(num >> 16, out);
        putc(num >> 8, out);
        putc(low, out);
        // 把统计词频输出到文件
        for (auto pair : statistic)
        {
            // 字符0~255，一个字节直接写入
            putc((int)pair.first, out);
            // 次数的话，由于一个文件随随便便上万个重复字符，所以这里用四个字节来存频率
            // 相当于以（1个字节的字符，4个字节的频率）将每个不同的字符频率都写进去：1个字节*10^9=1G，基本够用
            // 尤其对于大文件
            putc((int)(pair.second >> 24), out);
            putc((int)(pair.second >> 16), out);
            putc((int)(pair.second >> 8), out);
            putc((int)(pair.second), out);
        }
    }
    void CompressFile(FILE *in, FILE *out)
    {
        WordFrequencyStatistics(in);
        GenerateProbability();
        Encode(in, out);
    }
};

int main()
{
    int argc = 4;
    char argv[][50] = {"ac", "c", "./project/DataCompression/ArithmeticCode/data.txt", "./project/DataCompression/ArithmeticCode/data.ac"};
    ArithmeticCode *ac = new ArithmeticCode();
    FILE *in = fopen(argv[2], "rb");
    if (!in)
    {
        perror(argv[0]);
        exit(1);
    }
    FILE *out = fopen(argv[3], "wb");
    ac->CompressFile(in, out);
    return 0;
}