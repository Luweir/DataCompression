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
    HuffmanTreeNode() : ch(0), count(0), left(nullptr), right(nullptr){};
    // 重载"<"符号，令其为最小值优先
};

bool operator<(const HuffmanTreeNode &N1, const HuffmanTreeNode &N2)
{
    return N1.count > N2.count;
}

class Huff
{
private:
    HuffmanTreeNode *root;
    unsigned int totalCount;              // 总字符多少个
    map<unsigned char, int> statistic;    // 权重统计表
    map<unsigned char, string> codeTable; // 编码表

public:
    Huff()
    {
        root = new HuffmanTreeNode();
    }
    // 统计词频
    void WordFrequencyStatistics(FILE *in)
    {
        int c;

        fseek(in, 0, SEEK_END);
        int size = ftell(in);
        this->totalCount = size;
        if (size < 0 || size >= 0x7FFFFFFF)
            quit("input file too big");
        fseek(in, 0, SEEK_SET);

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
            HuffmanTreeNode temp(pair.first, pair.second);
            myq.push(temp);
        }
        while (myq.size() > 1)
        {
            HuffmanTreeNode left = myq.top();
            myq.pop();
            HuffmanTreeNode right = myq.top();
            myq.pop();
            // 创建新子树的根节点，将其压入队中
            HuffmanTreeNode *parent = new HuffmanTreeNode(-1, left.count + right.count);
            parent->left = &left;
            parent->right = &right;
            myq.push(*parent);
        }
        *root = myq.top();
        myq.pop();
    }
    void Build(HuffmanTreeNode *curNode, string code)
    {
        // 如果root是叶子结点
        if (curNode->left == nullptr && curNode->right == nullptr && root->ch > 0)
        {
            cout << int(curNode->ch) << " " << code << endl;
            codeTable[curNode->ch] = code;
            return;
        }
        if (curNode->left != nullptr)
            Build(root->left, code + '0');
        if (curNode->right != nullptr)
            Build(root->right, code + '1');
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
    // 压缩文件
    void compress(FILE *in, FILE *out)
    {
        WordFrequencyStatistics(in);
        CreateHuffmanTree();
        BuildCode();
        int c;
        string res = "";
        while ((c = getc(in)) != EOF)
        {
            res += codeTable[c];
        }
        int len = res.size();
        cout << len << endl;
        // 输出文件第一个字节表示最后的那个字节有几位，暂时想不到更好的办法，
        putc(len % 8, out);
        for (int i = 0; i < len; i++)
        {
            int sum = 0;
            for (int j = i; j < len && j < i + 8; i++)
            {
                sum = sum * 2 + res[j] - '0';
            }
            putc(sum, out);
        }
    }
};

int main()
{
    FILE *in = fopen("./project/DataCompression/HuffmanCoding/data.txt", "rb");
    FILE *out = fopen("./project/DataCompression/HuffmanCoding/out.huf", "wb");
    Huff *huf = new Huff();
    huf->compress(in, out);
    fclose(in);
    fclose(out);
    return 0;
}