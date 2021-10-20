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
            // 法一：
            // char *array_char = new char[cur_length];
            // memset(array_char, cur_char, cur_length);
            // s += string(array_char, &array_char[cur_length]);
            // delete[] array_char;
            // 法二：
            // string ss(cur_length, cur_char); // cur_length 个 cur_char

            /*
            关于C字符数组和C++字符串的几个注意事项：

            1.C++的string构造函数会在末尾自动添加一个'\0'，并且size和capacity函数都不计算'\0'的；

            2.C语言用char*指针作为字符串时，在读取字符串时需要一个特殊字符0来标记指针的结束位置，也就是通常认为的字符串结束标记。
            而c++语言则是面向对象的，长度信息直接被存储在了对象的成员中，读取字符串可以直接根据这个长度来读取，所以就没必要需要结束标记了。
            而且结束标记也不利于读取字符串中夹杂0字符的字符串。
            */
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