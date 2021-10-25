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
    map<unsigned char, int> statistic;
    map<unsigned char, pair<double, double>> probabilityTable;

public:
    // 统计词频
    void WordFrequencyStatistics(FILE *in)
    {
        int c;
        fseek(in, 0, SEEK_END);
        int size = ftell(in);
        if (size < 0 || size >= 0x7FFFFFFF)
            quit("input file too big");
        fseek(in, 0, SEEK_SET);
        while ((c = getc(in)) != EOF)
        {
            statistic[c]++;
        }
    }
    // 生成概率
    void GenerateProbability(int size)
    {
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
        double high = 1;
        double newLow, newHigh;
        int c = 0;
        while ((c = getc(in) != EOF))
        {
            newLow = low + (high - low) * probabilityTable[c].first;
            newHigh = low + (high - low) * probabilityTable[c].second;
            low = newLow;
            high = newHigh;
        }
        low = (low + high) / 2.0;
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
        
    }
};

int main()
{
    int argc = 4;
    char argv[][50] = {"", "", "", ""};
    long long a = 0;
    cout << sizeof(a) << endl;
    return 0;
}