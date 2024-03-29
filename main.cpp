#include <iostream>
#include <fstream>
#include <string>

#define WORD_LENGHT 40

char vowels[12] = { 'E','Y','U','I','O','A','e','y','u,','i','o','a' };

bool is_alpha(char s)
{
	for (int i = 65; i <= 90; i++)  if (int(s) == i) return true;
	for (int i = 97; i <= 122; i++) if (int(s) == i) return true;

	return false;
}

bool is_vowel(char s)
{
	for (int i = 0; i < 12; i++) if (int(s) == vowels[i]) return true;
		return false;
}

bool is_word(char *str, int n)
{
	for (int i = 0; i < n; i++) if (!is_alpha(str[i]) && str[i] != ' ' && str[i] != '\0' && str[i] != '\t' && str[i] != '\n') return false;
	return true;
}

struct Node
{
	char word[WORD_LENGHT];
	
	Node *back, *next;

	Node(void)
	{
		back = next = nullptr;
	}

	void print(void) const
	{
		std::cout << word << " ";
	}
};

struct List
{
	Node *LeftHead, *RightHead;

	List(void)
	{
		LeftHead = RightHead = nullptr;
	}

	void AddLeft(char *word)
	{
		Node *node = new Node;
		for (int i = 0; i < WORD_LENGHT; i++)
		{
			node->word[i] = word[i];
		}

		if (LeftHead == nullptr)
		{
			LeftHead = RightHead = node;
			return;
		}

		LeftHead->back = node;
		node->next = LeftHead;
		LeftHead = node;
	}

	void AddRight(char *word)
	{
		Node *node = new Node;
		for (int i = 0; i < WORD_LENGHT; i++)
		{
			node->word[i] = word[i];
		}

		if (LeftHead == nullptr)
		{
			LeftHead = RightHead = node;
			return;
		}

		RightHead->next = node;
		node->back = RightHead;
		RightHead = node;
	}

	void print_list(void)
	{
		Node *node = LeftHead;

		while (node)
		{
			node->print();
			node = node->next;
		}
	}

	Node* pop(void)
	{
		if (LeftHead == nullptr) return nullptr;
		Node *node;

		node = LeftHead;
		LeftHead = LeftHead->next;

		return node;
	}
};

struct TreeNode
{
	TreeNode *left, *right;

	char word[WORD_LENGHT];

	TreeNode(void)
	{
		left = right = nullptr;
	}	
};

void add(TreeNode *&root, char *word)
{
	if (!root)
	{
		root = new TreeNode();
		for (int i = 0; i < WORD_LENGHT; i++)
		{
			root->word[i] = word[i];
		}
	}
	else if   (std::strcmp(word,root->word) < 0) add(root->left, word);
	else	  add(root->right, word);	
}

void print_in_file(TreeNode *root, std::fstream *File)
{
	if (root)
	{
		print_in_file(root->left,File);
		*File << root->word << "\n";
		print_in_file(root->right, File);
	}
}

void search(TreeNode *root,int l,int &k)
{
	if (root)
	{
		search(root->left,l,k);
		search(root->right,l,k);
		if (std::strlen(root->word) == l) k++;
	}
}

void free(TreeNode *&root)
{
	if (root)
	{
		free(root->left);
		free(root->right);
		delete root;
		root = nullptr;
	}
}

int main(void)
{	
	setlocale(LC_ALL, "Russian");

	std::ifstream file("File.txt");

	if (!file.is_open())
	{
		std::cout << "���� � ������ file.txt �� ����� ���� ������ !" << std::endl;

		std::system("pause");
		return 0;
	}

	char str[40];

	for (int i = 0; i < 40; i++)
	{
		str[i] = ' ';
	}

	List list;

	while (!file.eof())
	{
		file >> str;
		if (is_word(str, 40))
		{
			std::cout << str << " ";
			if (is_vowel(str[0])) list.AddLeft(str);
			else				  list.AddRight(str);
		}
	}

	std::cout << std::endl;
	list.print_list();
	std::cout << std::endl;

	file.close();

	TreeNode *root = nullptr;
	
	Node *node = list.pop();

	while (node)
	{
		add(root, node->word);
		node = list.pop();
	}

	std::cout << std::endl;

	std::fstream *_file = new std::fstream("_File.txt");

	if (!_file->is_open())
	{
		std::cout << "���� � ������ _File.txt �� ����� ���� ������ !" << std::endl;

		std::system("pause");
		return 0;
	}

	print_in_file(root,_file);

	int k = 0;

	search(root, 3, k);

	std::cout << k << std::endl;

	_file->close();

	std::system("pause");
	return 0;
}