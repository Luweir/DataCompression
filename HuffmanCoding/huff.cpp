#include <iostream>
#include <assert.h>
#include <bits/stdc++.h>
using namespace std;

#define BITS_PER_CHAR 8

// Error handler: print message if any, and exit
void quit(const char *message = 0)
{
    if (message)
        printf("%s\n", message);
    exit(1);
}

class HuffmanTreeNode
{
public:
    unsigned char ch;
    int count;
    HuffmanTreeNode *left;
    HuffmanTreeNode *right;

public:
    HuffmanTreeNode(int chh, int countt) : ch(chh), count(countt), left(nullptr), right(nullptr){};
    HuffmanTreeNode(int chh, int countt, HuffmanTreeNode *l, HuffmanTreeNode *r) : ch(chh), count(countt), left(l), right(r){};
    HuffmanTreeNode() : ch(0), count(0), left(nullptr), right(nullptr){};
    // 拷贝构造函数一定要，因为我们放的是节点，一定要把内容复制过去，避免假拷贝的问题出现；
    HuffmanTreeNode(const HuffmanTreeNode &N)
    {
        ch = N.ch;
        count = N.count;
        if (N.left != nullptr)
        {
            left = new HuffmanTreeNode();
            *left = *(N.left);
        }
        else
        {
            left = nullptr;
        }
        if (N.right != nullptr)
        {
            right = new HuffmanTreeNode();
            *right = *(N.right);
        }
        else
        {
            right = nullptr;
        }
    }
};
// 重载"<"符号，令其为最小值优先
bool operator<(const HuffmanTreeNode &N1, const HuffmanTreeNode &N2)
{
    return N1.count > N2.count;
}

class Huff
{
private:
    HuffmanTreeNode *root;
    map<unsigned char, int> statistic;    // 权重统计表
    map<unsigned char, string> codeTable; // 编码表

public:
    Huff()
    {
        root = new HuffmanTreeNode();
    }
    // 释放对象空间
    void FreeNode(HuffmanTreeNode *curNode)
    {
        if (curNode)
        {
            FreeNode(curNode->left);
            FreeNode(curNode->right);
        }
        delete (curNode);
    }
    // 重新初始化
    void Init()
    {
        statistic.clear();
        codeTable.clear();
        FreeNode(root);
        root = new HuffmanTreeNode();
    }
    // 统计词频
    void WordFrequencyStatistics(FILE *in)
    {
        int c;
        fseek(in, 0, SEEK_END);
        int size = ftell(in);
        if (size < 0 || size >= 0x7FFFFFFF)
            quit("input file too big");
        fseek(in, 0, SEEK_SET);
        cout << size << endl;
        while ((c = getc(in)) != EOF)
        {
            statistic[c]++;
        }
    }
    // 创建哈夫曼树
    void CreateHuffmanTree()
    {
        priority_queue<HuffmanTreeNode> myq;
        for (auto pair : statistic)
        {
            HuffmanTreeNode temp = HuffmanTreeNode(pair.first, pair.second);
            myq.push(temp);
        }
        // 如果不确保真拷贝，构建霍夫曼树就会出错
        while (myq.size() > 1)
        {
            HuffmanTreeNode left = myq.top();
            myq.pop();
            HuffmanTreeNode right = myq.top();
            myq.pop();
            // 创建新子树的根节点，将其压入队中
            HuffmanTreeNode parent = HuffmanTreeNode(-1, left.count + right.count, &left, &right);
            myq.push(parent);
        }
        *root = myq.top();
        myq.pop();
    }
    void Build(HuffmanTreeNode *curNode, string code)
    {
        // 如果root是叶子结点
        if (curNode->left == nullptr && curNode->right == nullptr && (int)(root->ch) > 0)
        {
            cout << int(curNode->ch) << " " << code << endl;
            codeTable[curNode->ch] = code;
            return;
        }
        if (curNode->left != nullptr)
            Build(curNode->left, code + '0');
        if (curNode->right != nullptr)
            Build(curNode->right, code + '1');
    }
    // 生成编码表
    void BuildCode()
    {
        if (root == nullptr)
        {
            quit("the root is null");
        }
        string code = "";
        HuffmanTreeNode *curNode = root;
        Build(curNode, code);
    }
    // 将词频表写入文件
    void WriteStatistic(FILE *out)
    {
        int len = statistic.size();
        // 第一个字节表示，词频有多少对，即statistic有多少对
        putc(len, out);
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

    // 压缩文件
    void CompressFile(FILE *in, FILE *out)
    {
        WordFrequencyStatistics(in);
        CreateHuffmanTree();
        BuildCode();
        // 把词频表写入文件
        WriteStatistic(out);
        int c = 0;
        string res = "";
        // 因为计算频率统计的时候in的指针到文件尾部了，所以这里要重置
        fseek(in, 0, SEEK_SET);
        while ((c = getc(in)) != EOF)
        {
            res += codeTable[c];
        }
        int len = res.size();
        cout << len / 8 << endl;
        // 之后的第一个字节表示最后的那个字节有几位，暂时想不到更好的办法，
        putc(len % 8, out);
        for (int i = 0; i < len;)
        {
            int sum = 0;
            for (int j = i; j < len && j < i + 8; j++)
            {
                sum = sum * 2 + res[j] - '0';
            }
            i += 8;
            putc(sum, out);
        }
    }
    // 解压缩时从输入文件读取词频表
    void ReadStatistic(FILE *in)
    {
        int numberFrequency = getc(in);
        int c = 0;
        while (numberFrequency-- && ((c = getc(in)) != EOF))
        {
            int i = 0;
            int frequency = 0;
            int cc = 0;
            while (i < 4 && (cc = getc(in)) != EOF)
            {
                frequency = frequency << 8;
                frequency += cc;
                i++;
            }
            statistic[c] = frequency;
        }
    }
    // 整数转二进制
    string IntegerToBinary(int tar)
    {
        int bitNumber = 0;
        string ret = "";
        while (bitNumber < 8)
        {
            // 注意，位运算的优先级很低，一定要记得括号
            ret += ((tar & 1) + '0');
            tar >>= 1;
            bitNumber++;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
    // 解压缩文件
    void DecompressFile(FILE *in, FILE *out)
    {
        // 先读词频表，建立哈夫曼树
        ReadStatistic(in);
        CreateHuffmanTree();

        // 除词频表外的部分进行解压缩，这些用huffman编码表示的
        // 结尾的那个字节具体有多少位
        int AtEndDigits = getc(in);

        int last = getc(in);
        int c = 0;
        string res = "";
        while ((c = getc(in)) != EOF)
        {
            res += IntegerToBinary(last);
            last = c;
        }
        // 对最后一位特殊的处理，比如截断
        res += IntegerToBinary(last).substr(8 - AtEndDigits, AtEndDigits);

        HuffmanTreeNode *roott = new HuffmanTreeNode();
        int index = 0;
        int len = res.size();
        while (index < len)
        {
            if (res[index] == '0')
            {
                roott = roott->left;
                index++;
                // roott是叶子节点
                if (roott != nullptr && roott->left == nullptr && roott->right == nullptr)
                {
                    putc(int(roott->ch), out);
                    continue;
                }
            }
            if (res[index] == '1')
            {
                roott = roott->right;
                index++;
                // roott是叶子节点
                if (roott != nullptr && roott->left == nullptr && roott->right == nullptr)
                {
                    putc(int(roott->ch), out);
                    continue;
                }
            }
        }
    }
};

int main()
{
    FILE *in = fopen("./project/DataCompression/HuffmanCoding/out.huf", "rb");
    FILE *out = fopen("./project/DataCompression/HuffmanCoding/data1.txt", "wb");
    Huff *huf = new Huff();
    huf->DecompressFile(in, out);

    fclose(in);
    fclose(out);

    return 0;
}