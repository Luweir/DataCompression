//游程编码

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <assert.h>

using namespace std;

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
    RLE *r = new RLE();
    r->RandomCommonString(28);
    cout << r->GetRaw() << endl;
    r->CompressCommonString();
    cout << r->GetComp() << endl;
    cout << r->DecompressCommonString() << endl;
    return 0;
}