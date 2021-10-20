/*
游程编码
游程编码是一种比较简单的压缩算法，其基本思想是将重复且连续出现多次的字符使用（连续出现次数，某个字符）来描述;

比如一个字符串：AAAAABBBBCCC 使用游程编码可以将其描述为：5A4B3C

5A表示这个地方有5个连续的A，同理4B表示有4个连续的B，3C表示有3个连续的C，其它情况以此类推。
原字符串需要12个字符才能描述，而使用游程编码压缩之后只需要6个字符就可以表示，还原回去的时候只需要将字符重复n次即可，这是个原理非常简单的算法。

那么在不同情况下这个编码的效果如何呢，假如采用定长1个字节来描述连续出现次数，并且一个字符占用1个字节，
那么描述（连续出现次数，某个字符）需要的空间是2个字节，只要这个连续出现次数大于2就能够节省空间，
比如AAA占用3个字节，编码为(3,A)占用两个字节，能够节省一个字节的空间，可以看出连续出现的次数越多压缩效果越好，节省的空间越大，
对一个字符编码能够节省的空间等于=连续出现次数-2，于是就很容易推出连续出现次数等于2时占用空间不变，
比如AA占用两个字节，编码为（2,A）仍然占用两个字节，白白浪费了对其编码的资源却没有达到节省空间的效果，
还有更惨的情况，就是连续出现次数总是为1，这个时候会越压越大，比如A占用一个字节，编码为(1,A)占用两个字节，比原来多了一个字节，这种情况就很悲剧，
一个1M的文件可能一下给压缩成了2M（真是效果奇佳啊），这是能够出现的最糟糕的情况，
相当于在文件的每一个字节前面都插入了一个多余的字节0X01（这个字节表示连续出现次数为1），这种情况说明不适合使用游程编码，
事实上，绝大多数数据的特征都属于第三种情况，不适合使用游程编码。

*/

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
        printf("%s\n", message);
    exit(1);
}

class RLE
{
private:
    string s;
    string result;

public:
    RLE()
    {
    }
    void RleCompressString()
    {
        int len = s.size();
        if (len == 0)
        {
            cout << "the size of s is 0" << endl;
        }
        result.clear();
        int count = 1;
        for (int i = 0; i < s.size() - 1; i++)
        {
            if (s[i + 1] == s[i])
            {
                count++;
                continue;
            }
            
            result += to_string(count);
            result.push_back(s[i]);
            count = 1;
        }
    }
    // 随机生成RLE友好型字符串
    void RandomRleFriendlyString(int length)
    {
        s.clear();
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
            s += string(cur_length, cur_char);
            length -= cur_length;
        }
    }
    void SetS(string newS)
    {
        s.clear(); //s.clear()的目的只是把s[0]='\0'
        s = newS;
    }
    string GetS()
    {
        return s;
    }
};
int main()
{
    // RLE *r = new RLE();
    // r->RandomRleFriendlyString(18);
    // cout << r->getS() << endl;
    string s = "1223";
    s[2] = '\0';
    cout << s << endl;
    cout << "hello rle" << endl;
    return 0;
}