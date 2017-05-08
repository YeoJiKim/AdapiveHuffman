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
const char none = (char)256;  //���ַ�

typedef struct Node{
	char letter; //����
	int weight; //Ȩ��
	int left;  //�ڵ���
	int right;
	int parent;

	bool operator==(const Node& x) {
		return (letter == x.letter) && (weight == x.weight)
			&& (left == x.left) && (right == x.right) && (parent == x.parent);
	}
}HuffTree;

HuffTree tree[NUM+1];
int root;

/*��ʼ����������*/
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

/*�ж�������ַ��Ƿ�Ϊ���ַ�*/
bool charIsNew(char c, char u[NUM]) {
	for (int i = 0; i < NUM; i++) {
		if (c == u[i]) return false;
	}
	return true;
}

/*�ҵ��ַ����ڽڵ�*/
int findChar(char letter) {
	for (int i = 0; i < 2 * (NUM + 1); i++) {
		if (tree[i].letter == letter) return i;
	}
}

/*���ؿ������Ľڵ���*/
int highestInBlock(int weight) {
	int highest = -1;
	for (int i = 0; i < 2 * (NUM + 1); i++) {
		if (tree[i].weight == weight) highest = i;
	}
	return highest;
}

/*�ð����·��ź���NYT�������滻ԭNYT*/
int spawn(char newchar) {
	int oldNYTindex;
	HuffTree oldNYT;

	///�ҵ��ɵ�NYT����
	oldNYTindex = findChar(NYT);
	oldNYT = tree[oldNYTindex];

	
	//�����µĽڵ㣬�����滻
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

/*���������ڵ�*/
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
	string codeForOut = ""; //���������
	string codeForDec = ""; //���ڽ����
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

static int decPos = 1; //����λ�ã���Ϊ��0λһ����NYT�����Դӵ�һλ��ʼ
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
	string output = "0"; //NYT��ʼֵΪ0
	cout << "��������";
	cout << output;
	//////����
	initHuffmanTree(NUM);
	for (int i = 0; i < strlen(input); i++) {
		c = input[i];
		output += huffmanEncode(c,used);
		updateHuffmanTree(c,used);
		used[i] = c;
	}
	cout << '\n';
	cout << "�����������룬ȥ���˿ո�֮��ı�����:";
	cout << output << endl;
	
	///////����
	char c_d;  //����������ַ�
	initHuffmanTree(NUM);
	c_d = newChar(output);
	decPos = decPos + 5;
	cout <<"��������";
	cout << c_d;
	updateHuffmanTree(c_d, true);
	int n = strlen(input)-1;
	while(n){ //�ܹ�strlen(input)�ַ� ,��ʣ��strlen(input)-1���ַ�
		HuffTree current = tree[root];
		for (int i = decPos; i < output.length(); i++) {
			if (output.at(i) == '0' && current.left != 0) {
				current = tree[current.left];
				decPos++;
				if (current.letter == NYT) { //��ΪNYT����������ұߵ�Ҷ�ڵ㣬�����ڴ��ж�
					c_d = newChar(output); //����NYT˵���������ĺ�����ַ����������
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

  