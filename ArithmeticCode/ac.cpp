#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>

using namespace std;

using std::uint32_t;
using std::uint64_t;

// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        printf("%s\n", message);
    exit(1);
}

////////////////////////////////////// FrequencyTable ////////////////////////////////////

class FrequencyTable
{
public:
    vector<uint32_t> statistic;  // 频率表
    vector<uint32_t> cumulative; // 累计频率表
    uint32_t total;
    FrequencyTable()
    {
        statistic = vector<uint32_t>(257, 0);
        int size = statistic.size();
        cumulative.reserve(size + 1);
        total = 0;
        initCumulative();
        total = getHigh(size - 1);
    }
    // 增加对应字符的频率
    void IncreaseFrequency(uint32_t symbol)
    {
        total++;
        statistic[symbol]++;
        cumulative.clear();
    }
    // 获得symbol的频率下界
    uint32_t getLow(uint32_t symbol)
    {
        initCumulative();
        return cumulative[symbol];
    }
    // 获得symbol的频率上届
    uint32_t getHigh(uint32_t symbol)
    {
        initCumulative();
        return cumulative[symbol + 1];
    }
    // 向文件out写入频率表
    void WriteFrequencyTable(FILE *out)
    {
        for (int i = 0; i < 256; i++)
        {
            uint32_t tar = statistic[i];
            fputc(tar >> 24, out);
            fputc(tar >> 16, out);
            fputc(tar >> 8, out);
            fputc(tar, out);
        }
    }
    void initCumulative()
    {
        if (!cumulative.empty())
        {
            return;
        }
        uint32_t sum = 0;
        cumulative.push_back(sum);
        for (uint32_t freq : statistic)
        {
            sum += freq;
            cumulative.push_back(sum);
        }
        if (sum != total)
            quit("the sum is not equal total");
    }
};

////////////////////////////////////// BitOutPut ////////////////////////////////////
class BitOutPut
{
public:
    FILE *out;
    int currentByte;
    int numBitsFilled;
    BitOutPut(FILE *&outt)
    {
        out = outt;
        currentByte = 0;
        numBitsFilled = 0;
    }
    // 保存位b，凑够8位后组成unsigned char 写入文件
    void writeBit(int b)
    {
        if (b != 0 && b != 1)
            quit("the write value of Bitoutput must be 0 or 1");
        currentByte = (currentByte << 1) | b;
        numBitsFilled++;
        if (numBitsFilled == 8)
        {
            putc(currentByte, out);
            currentByte = 0;
            numBitsFilled = 0;
        }
    }
    // 最后凑位数的
    void finish()
    {
        while (numBitsFilled != 0)
            writeBit(0);
    }
};

////////////////////////////////////// ArithmeticEnCoder ////////////////////////////////////
class EnCoder
{
private:
    int numStateBits;      // 状态位数量
    uint32_t numUnderflow; // 下溢位数
    uint64_t fullRange;    // 总范围
    uint64_t halfRange;    // 半范围
    uint64_t quarterRange; // 四分范围
    uint64_t minimumRange;
    uint64_t maximumTotal;
    uint64_t stateMask; // 状态掩码
    uint64_t low;       // 下界
    uint64_t high;      // 上届
    BitOutPut *output;

public:
    EnCoder(int numBits, BitOutPut *&bout)
    {
        numStateBits = numBits;
        fullRange = static_cast<decltype(fullRange)>(1) << numStateBits;
        halfRange = fullRange >> 1;                                                                         // halfRange表示最高位为1，其他位为0，方便后面shift
        quarterRange = halfRange >> 1;                                                                      // Can be zero ???为什么要设立这些
        minimumRange = quarterRange + 2;                                                                    // At least 2 ???
        maximumTotal = std::min(std::numeric_limits<decltype(fullRange)>::max() / fullRange, minimumRange); // ???
        stateMask = fullRange - 1;                                                                          // 即stateMask的numStateBits个位全为1
        low = 0;
        high = stateMask;
        output = bout;
        numUnderflow = 0;
    }

    // 结果更新此算术编码器的代码范围（低和高）
    void update(FrequencyTable *&freqs, uint32_t symbol)
    {
        uint64_t range = high - low + 1;
        uint32_t total = freqs->total;
        uint32_t symLow = freqs->getLow(symbol);
        uint32_t symHigh = freqs->getHigh(symbol);
        // Update range
        uint64_t newLow = low + symLow * range / total;
        uint64_t newHigh = low + symHigh * range / total - 1;
        low = newLow;
        high = newHigh;

        //如果low和high具有相同的最高位值，将它们移出
        while (((low ^ high) & halfRange) == 0)
        {
            shift();
            low = ((low << 1) & stateMask);       // 删除 low 的MSD（MSD 代表数字中的最高有效数字或最左边的数字）并将新的 0 移到low
            high = ((high << 1) & stateMask) | 1; // 删除高的 MSD 并将新的 1 移到high
        }

        // 现在low的最高位必须为 0，high的最高位必须为 1

        // 当low的高两位是 01 和high是 10 时，删除两者的第二高位;可能是二者接近，后面有溢出风险，所以要做变换；
        while ((low & ~high & quarterRange) != 0)
        {
            underflow();
            low = (low << 1) ^ halfRange;
            high = ((high ^ halfRange) << 1) | halfRange | 1;
        }
    }
    //根据给定的频率表对给定的符号进行编码,还更新此算术编码器的状态并可能写出一些位
    void write(FrequencyTable *&freqs, uint32_t symbol)
    {
        update(freqs, symbol);
    }
    // 调用以处理 low 和 high 的最高位相等时的情况
    void shift()
    {
        int bit = (int)(low >> (numStateBits - 1));
        output->writeBit(bit);
        for (; numUnderflow > 0; numUnderflow--)
            output->writeBit(bit ^ 1);
    }
    void finish()
    {
        output->writeBit(1);
    }
    // 当 low=01(...) 和 high=10(...) 时调用处理这种情况
    void underflow()
    {
        numUnderflow++;
    }
};
class Compress
{
public:
    void Run(FILE *in, FILE *out)
    {
        // 生成频率表
        FrequencyTable *freqs = new FrequencyTable();
        int symbol = 0;
        while ((symbol = getc(in)) != EOF)
        {
            freqs->IncreaseFrequency((uint32_t)symbol);
        }
        fseek(in, 0, SEEK_SET);

        // 向out写入频率表
        freqs->WriteFrequencyTable(out);
        BitOutPut *bout = new BitOutPut(out);

        // 编码压缩
        EnCoder *encoder = new EnCoder(32, bout);
        while ((symbol = getc(in)) != EOF)
        {
            encoder->write(freqs, static_cast<uint32_t>(symbol));
        }
        encoder->write(freqs, 256); // EOF
        encoder->finish();
        bout->finish();
    }
};
int main()
{
    int argc = 4;
    char argv[][50] = {"ac", "c", "./project/DataCompression/ArithmeticCode/data.txt", "./project/DataCompression/ArithmeticCode/data.ac"};
    FILE *in = fopen(argv[2], "rb");
    if (!in)
    {
        perror(argv[0]);
        exit(1);
    }
    FILE *out = fopen(argv[3], "wb");
    Compress *c = new Compress();
    c->Run(in, out);
    return 0;
}