//游程编码

#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>

using namespace std;

typedef unsigned int U32;
// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        cout << message << endl;
    exit(1);
}

class RLE
{
private:
    string raw;
    string comp;

public:
    RLE()
    {
    }
    string GetComp()
    {
        return comp;
    }
    // 压缩纯字母字符串数据
    void CompressPureString()
    {
        int len = raw.size();
        if (len == 0)
        {
            cout << "the size of s is 0" << endl;
        }
        comp.clear();
        int count = 1;
        for (int i = 1; i < len; i++)
        {
            if (raw[i] == raw[i - 1])
            {
                count++;
            }
            else
            {
                comp += to_string(count) + raw[i - 1];
                count = 1;
            }
        }
        comp += to_string(count) + raw[len - 1];
    }
    // 解压缩纯字母字符串数据
    string DecompressPureString(string ss = "")
    {
        string curS = ss == "" ? comp : ss;
        string res = "";
        int i = 0, j = 0;
        int len = curS.size();
        while (j < len)
        {
            if (curS[j] >= '0' && curS[j] <= '9')
            {
                j++;
                continue;
            }
            else
            {
                res += string(stoi(curS.substr(i, j - i + 1)), curS[j]);
                j++;
                i = j;
            }
        }
        return res;
    }
    // 通用字符串压缩
    void CompressCommonString()
    {
        int len = raw.size();
        if (len <= 1)
        {
            cout << "Illegal size" << endl;
        }
        comp.clear();
        int count = 1;
        for (int i = 1; i < len; i++)
        {
            if (raw[i] == raw[i - 1] && count < 9)
            {
                count++;
            }
            else
            {
                comp += to_string(count) + raw[i - 1];
                count = 1;
            }
        }
        comp += to_string(count) + raw[len - 1];
    }
    // 通用字符串解压缩
    string DecompressCommonString(string ss = "")
    {
        string curS = ss == "" ? comp : ss;
        string res = "";
        int j = 1;
        int len = curS.size();
        while (j < len)
        {
            res += string(curS[j - 1] - '0', curS[j]);
            j += 2;
        }
        return res;
    }
    // 随机生成RLE友好型纯字母字符串
    void RandomPureString(int length)
    {
        raw.clear();
        srand(int(time(0)));
        while (length > 0)
        {
            int cur_length = rand() % length + 1;
            char cur_char = rand() % 27 + 'a';
            raw += string(cur_length, cur_char);
            length -= cur_length;
        }
    }
    // 随机生成RLE友好型字符串
    void RandomCommonString(int length)
    {
        raw.clear();
        srand(int(time(0)));
        while (length > 0)
        {
            int cur_length = rand() % length + 1;
            char cur_char = rand() % 95 + 32;
            raw += string(cur_length, cur_char);
            length -= cur_length;
        }
    }
    // 压缩文件
    void CompressFile(fstream &in, char *tarName)
    {
        fstream out(tarName, ios::trunc | ios::binary | ofstream::out);
        unsigned char c;
        unsigned char last;
        in.read((char *)&last, sizeof(c));
        unsigned char count = 1;
        while (in.read((char *)&c, sizeof(c)))
        {
            if (c == last && count < 255)
            {
                count++;
            }
            else
            {
                out.write((char *)&count, sizeof(count));
                out.write((char *)&last, sizeof(c));
                count = 1;
                last = c;
            }
        }
        out.write((char *)&count, sizeof(count));
        out.write((char *)&c, sizeof(c));
        out.close();
    }
    void DecompressFile(FILE *in, char *tarName)
    {
        FILE *out = fopen(tarName, "wb");
    }
    void SetRaw(string newS)
    {
        raw.clear(); //s.clear()的目的只是把s[0]='\0',
        raw = newS;
    }
    string GetRaw()
    {
        return raw;
    }
};
int main()
{
    int argc = 2;
    char argv[][60] = {"./project/DataCompression/RunLengthEncoding/data.txt", "data.txt.rle"};
    RLE *r = new RLE();

    // begin -> test
    // r->RandomCommonString(28);
    // cout << r->GetRaw() << endl;
    // r->CompressCommonString();
    // cout << r->GetComp() << endl;
    // cout << r->DecompressCommonString() << endl;
    // FILE *in = fopen(argv[0], "rb");
    // end

    fstream in(argv[0], ifstream::in | ios::binary);
    if (!in)
        perror(argv[0]), exit(1);
    r->CompressFile(in, argv[1]);

    // C语言貌似能解决，因为putc()能写入这样的数据，他会截断最后一个字节
    // int getc(FILE * stream);
    // int putc(int char, FILE *stream)
    return 0;
}