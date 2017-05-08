// AdaptiveHuffman.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
using namespace std;

#define NUM 9

const char NYT = (char)257; 
const char none = (char)256;  //非字符

typedef struct Node{
	char letter; //符号
	int weight; //权重
	int left;  //节点编号
	int right;
	int parent;

	bool operator==(const Node& x) {
		return (letter == x.letter) && (weight == x.weight)
			&& (left == x.left) && (right == x.right) && (parent == x.parent);
	}
}HuffTree;

HuffTree tree[NUM+1];
int root;

/*初始化哈夫曼树*/
void initHuffmanTree(int rootnumber) {
	for (int i = 0; i < 2 * (NUM + 1); i++) {
		tree[i].weight = 0;
		tree[i].left = 0;
		tree[i].right = 0;
		tree[i].parent = 0;
		tree[i].letter = none;
	}
	root = rootnumber;
	tree[root].letter = NYT;
}

/*判断输入的字符是否为新字符*/
bool charIsNew(char c, char u[NUM]) {
	for (int i = 0; i < NUM; i++) {
		if (c == u[i]) return false;
	}
	return true;
}

/*找到字符所在节点*/
int findChar(char letter) {
	for (int i = 0; i < 2 * (NUM + 1); i++) {
		if (tree[i].letter == letter) return i;
	}
}

/*返回块内最大的节点编号*/
int highestInBlock(int weight) {
	int highest = -1;
	for (int i = 0; i < 2 * (NUM + 1); i++) {
		if (tree[i].weight == weight) highest = i;
	}
	return highest;
}

/*用包含新符号和新NYT的子树替换原NYT*/
int spawn(char newchar) {
	int oldNYTindex;
	HuffTree oldNYT;

	///找到旧的NYT子树
	oldNYTindex = findChar(NYT);
	oldNYT = tree[oldNYTindex];

	
	//创建新的节点，并且替换
	oldNYT.letter = none;
	tree[oldNYTindex].letter = none;

	oldNYT.right = oldNYTindex - 1;
	tree[oldNYTindex].right = oldNYTindex - 1;
	tree[oldNYT.right].letter = newchar;
	tree[oldNYT.right].weight = 1;
	tree[oldNYT.right].parent = oldNYTindex;
	

	oldNYT.left = oldNYTindex - 2;
	tree[oldNYTindex].left = oldNYTindex - 2;
	tree[oldNYT.left].letter = NYT;
	tree[oldNYT.left].parent = oldNYTindex;
	

	return oldNYTindex - 2;


}

/*交换两个节点*/
void swap(int first, int second) {
	int tmp;
	char tmpChar;

	tree[tree[first].left].parent = second;
	tree[tree[first].right].parent = second;
	tree[tree[second].left].parent = first;
	tree[tree[second].right].parent = first;

	
	tmp = tree[second].left;
	tree[second].left = tree[first].left;
	tree[first].left = tmp;

	tmp = tree[second].right;
	tree[second].right = tree[first].right;
	tree[first].right = tmp;

	tmpChar = tree[second].letter;
	tree[second].letter = tree[first].letter;
	tree[first].letter = tmpChar;

	tmp = tree[second].weight;
	tree[second].weight = tree[first].weight;
	tree[first].weight = tmp;


}
void updateHuffmanTree(char c,char u[NUM]) {
	int current;
	int max;
	if (charIsNew(c,u)) {
		current = spawn(c);
		current = tree[current].parent;
		tree[current].weight++;
	}
	else {
		current = findChar(c);
		max = highestInBlock(tree[current].weight);
		if (current != max && tree[current].parent != max) {
			swap(current, max);
			current = max;
		}
		tree[current].weight++;
	}

	while (current != root) {
		current = tree[current].parent;
		max = highestInBlock(tree[current].weight);
		if (current != max && tree[current].parent != max) {
			swap(current, max);
			current = max;
		}
		tree[current].weight++;
	}

}
void updateHuffmanTree(char c, bool isNew) {
	int current;
	int max;
	if (isNew) {
		current = spawn(c);
		current = tree[current].parent;
		tree[current].weight++;
	}
	else {
		current = findChar(c);
		max = highestInBlock(tree[current].weight);
		if (current != max && tree[current].parent != max) {
			swap(current, max);
			current = max;
		}
		tree[current].weight++;
	}

	while (current != root) {
		current = tree[current].parent;
		max = highestInBlock(tree[current].weight);
		if (current != max && tree[current].parent != max) {
			swap(current, max);
			current = max;
		}
		tree[current].weight++;
	}

}
string charToCode(char letter) {
	string bincode;
	
	HuffTree current;

	current = tree[findChar(letter)];

	while (!(current == tree[root]))
	{
		if (tree[tree[current.parent].left] == current) {
			bincode.insert(0, "0");
		}
		else if (tree[tree[current.parent].right] == current) {
			bincode.insert(0, "1");
		}
		else {

		}
		current = tree[current.parent];
	}
	
	 return ( bincode);
}
string huffmanEncode(char c,char used[NUM]) {
	string codeForOut = ""; //用于输出的
	string codeForDec = ""; //用于解码的
	string initCode = "";
	
	if (charIsNew(c, used)) {
		switch (c)
		{

		case 'A': initCode = "00001"; //initCode = ""
			break;
		case 'B': initCode = "00010";
			break;
		case 'C': initCode = "00011";
			break;
		case 'D': initCode = "00100";
			break;
		default:
			break;
		}
		codeForOut = " " + charToCode(NYT) + " "+ initCode;
		codeForDec =  charToCode(NYT) + initCode; 
		cout << codeForOut;
		
	}
	else {
		codeForOut = " " + charToCode(c) ;
		codeForDec = charToCode(c);
		cout << codeForOut;
	}
	return codeForDec;
	
}

static int decPos = 1; //编码位置，因为第0位一定是NYT，所以从第一位开始
char newChar(string bincode) {
	string s;
	for (int i = decPos; i < decPos + 5; i++) {
		s += bincode.at(i);
	}
	if (s == "00001") return 'A';
	else if (s == "00010") return 'B';
	else if (s == "00011")return 'C';
	else if (s == "00100")return 'D';
	else {}
	
}

int main()
{
	char input[NUM] = "ABBCADAD";
	char used[NUM] ="";
	char c ;
	string output = "0"; //NYT初始值为0
	cout << "编码结果：";
	cout << output;
	//////编码
	initHuffmanTree(NUM);
	for (int i = 0; i < strlen(input); i++) {
		c = input[i];
		output += huffmanEncode(c,used);
		updateHuffmanTree(c,used);
		used[i] = c;
	}
	cout << '\n';
	cout << "用作解码输入，去除了空格之后的编码结果:";
	cout << output << endl;
	
	///////解码
	char c_d;  //解码出来的字符
	initHuffmanTree(NUM);
	c_d = newChar(output);
	decPos = decPos + 5;
	cout <<"解码结果：";
	cout << c_d;
	updateHuffmanTree(c_d, true);
	int n = strlen(input)-1;
	while(n){ //总共strlen(input)字符 ,还剩下strlen(input)-1个字符
		HuffTree current = tree[root];
		for (int i = decPos; i < output.length(); i++) {
			if (output.at(i) == '0' && current.left != 0) {
				current = tree[current.left];
				decPos++;
				if (current.letter == NYT) { //因为NYT不会出现在右边的叶节点，所以在此判断
					c_d = newChar(output); //出现NYT说明接下来的后五个字符是新输入的
					decPos = decPos + 5;
					cout << c_d;
					updateHuffmanTree(c_d, true);
					
					break;
				}
				if (i == output.length() - 1 && current.letter != none) {
					c_d = current.letter;
					cout << c_d;
					updateHuffmanTree(c_d, false);
					break;
				}
				
			}

			else if (output.at(i) == '1' && current.right != 0) {
				decPos++;
				current = tree[current.right];
				
				if (i == output.length() - 1 && current.letter != none) {
					c_d = current.letter;
					cout << c_d;
					updateHuffmanTree(c_d, false);
					break;
				}
			}
			else {	

				if (current.letter != none) {
					c_d = current.letter;
					cout << c_d;
					updateHuffmanTree(c_d, false);
					break;
				}
				
				
			}
		
		}
		n--;
	
	}
	

	int i;
	scanf_s("%d", &i);
    return 0;
}

  