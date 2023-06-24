#include <iostream>
#include <chrono>
#include <string>
#include <fstream>

using namespace std;

//��������� ��� �������� �������
struct Node
{
    string field;
    int color;
    int depth;
    struct Node* left;
    struct Node* right;
};
//������� ��� ���������� ������� � ���������� � �������
struct Node* AddNode(string field, int color, Node* tree, int depth=0)
{
    if (tree == nullptr)
    {
        tree = new Node;
        tree->field = field;
        tree->color = color;
        tree->depth = depth;
        tree->left = nullptr;
        tree->right = nullptr;
    }
    else if (field < tree->field)
        tree->left = AddNode(field, color, tree->left, depth + 1);
    else
        tree->right = AddNode(field, color, tree->right,  depth + 1);
    return tree;
}
//������� ������ �� �����
struct Node* ReadFromFile(string path, Node* tree, int* num)
{
    string field;
    int color;
    ifstream inp(path);
    inp >> *num;
    for (int i=0; i<*num; i++)
    {
        inp >> field >> color;
        tree = AddNode(field, color, tree);
    }
    inp.close();
    return tree;
};  //������ ���������� ������� � ����� ����
void WriteNodeFile(string path, Node* tree)
{
    if (tree != nullptr)
    {
        ofstream out(path, ios_base::app);
        out << tree->field << ' ' << tree->color << '\n';
        out.close();
        WriteNodeFile(path, tree->left);
        WriteNodeFile(path, tree->right);
    }
}   //������� ������ ����� ������ � ����
void WriteToFile(string path, Node* tree, int num)
{
    ofstream out(path);
    out << num << '\n';
    out.close();
    WriteNodeFile(path, tree);
}   //������� �������� ������ ��� ���������
void DelTree(Node* tree)
{
    if (tree != nullptr)
    {
        DelTree(tree->left);
        DelTree(tree->right);
        delete tree;
    }
}   //����� ������ � �� ������(����� � �������)
void PrintNodes(Node* tree)
{
    if (tree != nullptr)
    {
        PrintNodes(tree->left);
        cout << tree->field << ' ' << tree->color << ' ' << tree->depth << endl;
        PrintNodes(tree->right);
    }
}
    //���������� ������������ ������� �����
int MaxDepth(Node* tree)
{
    if (tree->left == nullptr && tree->right == nullptr)
    {
        if (tree->color == 1)
        {
            return tree->depth;
        }
        else
        {
            return 0;
        }
    }
    int left, right, maximum;
    left = 0;
    right = 0;
    if (tree->left != nullptr)
        left = MaxDepth(tree->left);
    if (tree->right != nullptr)
        right = MaxDepth(tree->right);
    maximum = left > right ? left : right;
    return maximum;
}   //���������� ����������� ������� �����
int MinBWDepth(Node* tree)
{
    if (tree == nullptr)
    {
        return 1000;
    }
    if (tree->left == nullptr && tree->right == nullptr)
    {
        if (tree->color == 2 || tree->color == 3)
        {
            return tree->depth;
        }
        else
        {
            return 1000;
        }
    }
    int left, right, minimum;
    left = MinBWDepth(tree->left);
    right = MinBWDepth(tree->right);
    minimum = left < right ? left : right;
    return minimum;
}
//������� �������� ������ �������, ����������� ��������� �� �����
void FindMaxY(Node* tree, int max_depth, int* num, Node* parent=nullptr)
{
    if (tree != nullptr)
    {
        if (tree->depth == max_depth && tree->color == 1)
        {
            if (tree->left == nullptr && tree->right == nullptr)
            {
                if (parent->left == tree)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
                cout << tree->field << '-' << tree->color << '-' << tree->depth << '\n';
                delete tree;
                *num -= 1;
            }
        }
        else
        {
            FindMaxY(tree->left, max_depth, num, tree);
            FindMaxY(tree->right, max_depth, num, tree);
        }
    }
}
//������� �������� ����� � s������ �������, ����� ������� � ������
void FindMinBW(Node* tree, int min_depth, int* num, Node* parent=nullptr)
{
    if (tree != nullptr)
    {
        if ((tree->depth) == min_depth && (tree->color == 2 || tree->color == 3))
        {
            if (tree->left == nullptr && tree->right == nullptr)
            {
                if (parent->left == tree)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
                cout << tree->field << '-' << tree->color << '-' << tree->depth << '\n';
                delete tree;
                *num -= 1;
            }
        }
        else
        {
            FindMinBW(tree->left, min_depth, num, tree);
            FindMinBW(tree->right, min_depth, num, tree);
        }
    }
}
    //������� ������ ������ � ����������� �����
void PrintTree(Node* tree)
{
    if (tree->left != nullptr)
        PrintTree(tree->left);
    for (int i=0; i<tree->depth; i++)
    {
        cout << "    ";
    }
    cout << "->" << tree->field << '-' << tree->color << '-' << tree->depth << '\n';
    if (tree->right != nullptr)
        PrintTree(tree->right);
}

/*
Color:
    Yellow = 1
    Black = 2
    White = 3
*/

int main()
{
    Node* root = nullptr;
    int max_depth = 0;
    int min_depth = 0;
    int num = 0;
    string path = "example_2_file.txt";
    string path_out = "example_2_out.txt";
    root = ReadFromFile(path, root, &num);
    cout << num << '\n';
    cout << "Occupied memory: " << sizeof(Node) * num << " bytes\n";
//    PrintTree(root);

    auto t1_start = chrono::steady_clock::now();
    max_depth = MaxDepth(root);
    min_depth = MinBWDepth(root);
    int copy_num = num;
    FindMaxY(root, max_depth, &num);
    if (copy_num == num)
       FindMinBW(root, min_depth, &num);
    auto t1_end = chrono::steady_clock::now();
    auto t1_duration = chrono::duration_cast<chrono::microseconds>(t1_end - t1_start).count();
    cout << "Execution time: " << t1_duration << " mksec" << endl;

    cout << num << '\n';
//    PrintTree(root);
    WriteToFile(path_out, root, num);
    return 0;
}
