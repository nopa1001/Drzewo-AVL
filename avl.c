#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
typedef struct Node
{
	int key;
	int height;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
}Node;

typedef struct Tree
{
	Node*root;
}Tree;


int height(Node* node)
{
	return node ? node->height : 0;
}


void recalc(Node* node)
{
	node->height = 1 + height(node->left) >= height(node->right) ? height(node->left) : height(node->right);
	if (node->parent) recalc(node->parent);
}

void rotate_right(Node* node,Tree*tree)
{
	Node*temp = node->right;
	Node*parent = NULL;
	if (node->parent)
		parent = node->parent;
	node->right = temp->left;
	if (node->right) node->right->parent = node;
	temp->left = node;
	temp->parent = parent;
	node->parent = temp;
	if (parent == NULL)
	{
		tree->root = temp;
	}
	else
	{
		if (temp->parent->left == node) temp->parent->left = temp;
		else temp->parent->right = temp;
	}
	recalc(node);
}

void rotate_left(Node* node,Tree*tree)
{
	Node*temp = node->left;
	Node*parent = node->parent;
	node->left = temp->right;
	if (node->left) node->left->parent = node;
	temp->right = node;
	temp->parent = parent;
	node->parent = temp;
	if (!parent)
	{
		tree->root = temp;
	}
	else if (parent->left == node) parent->left = temp;
	else parent->right = temp;
	recalc(node);
}
void rotate_left_right(Node*node, Tree*tree)
{
	Node*temp;
	Node*temp_2;
	Node*parent;
	temp = node->left;
	temp_2 = temp->right;
	parent = node->parent;
	temp->right = temp_2->left;
	if (temp->right) temp->right->parent = temp;
	node->left = temp_2->right;
	temp_2->left = temp;
	temp_2->right = node;
	node->parent = temp_2;
	temp->parent = temp_2;
	temp_2->parent = parent;
	if (!parent)
	{
		tree->root = temp_2;
	}
	else
	{
		if (parent->left == node) parent->left = temp_2;
		else parent->right = temp_2;
	}
	recalc(temp);
	recalc(node);
}
void rotate_right_left(Node*node, Tree*tree)
{
	Node*temp;
	Node*temp_2;
	Node*parent;
	temp = node->right;
	temp_2 = temp->left;
	parent = node->parent;
	temp->left = temp_2->right;
	if (temp->left)	temp->left->parent = temp;
	node->right = temp_2->left;
	if (node->right)	node->right->parent = node;
	temp_2->left = node;
	temp_2->right = temp;
	node->parent = temp_2;
	temp->parent = temp_2;
	temp_2->parent = parent;
	if (!parent)
	{
		tree->root = temp_2;
	}
	else
	{
		if (parent->left == node)	parent->left = temp_2;
		else parent->right = temp_2;
	}
	recalc(node);
	recalc(temp);
}
int getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}


Node * search(Node* node, int key)
{
	if (!node)
		return NULL;

	if (key < node->key)
		return search(node->left, key);
	else if (key > node->key)
		return search(node->right, key);
	else
		return node;
}

void insert(Tree*tree, int value)
{
	Node*temp;
	if (!tree->root) {
		tree->root = (Node*)malloc(sizeof(Node));
		tree->root->height = 1;
		tree->root->left = NULL;
		tree->root->right = NULL;
		tree->root->key = value;
		tree->root->parent = NULL;
		return;
	}
	else {
		temp = tree->root;
		while (1 == 1) {
			if (value >= temp->key) {
				if (temp->right) {
					temp = temp->right;

				}
				else {
					temp->right = (Node*)malloc(sizeof(Node));
					temp->right->left = NULL;
					temp->right->right = NULL;
					temp->right->parent = temp;
					temp->right->key = value;
					temp->right->height = 1;

					recalc(temp);
					
					break;
				}
			}
			else {
				if (temp->left) {
					temp = temp->left;
				}
				else {
					temp->left = (Node*)malloc(sizeof(Node));
					temp->left->left = NULL;
					temp->left->right = NULL;
					temp->left->parent = temp;
					temp->left->key = value;
					temp->left->height = 1;
					recalc(temp);
					
					break;
				}
			}

		}
	}
	int balance = 0;
	
	while (temp) {
		balance= getBalance(temp);

		if (balance < -1 && getBalance(temp->right)>0)
		{
			rotate_right_left(temp, tree);
			
			temp = temp->parent;
		}
		else if(balance<-1)
		{
			rotate_right(temp, tree);
			temp = temp->parent;
		}
		if (balance > 1 && getBalance(temp->left) < 0)
		{
			rotate_left_right(temp, tree);
			temp = temp->parent;
		}
		else if(balance>1)
		{
			rotate_left(temp, tree);
			temp = temp->parent;
		}
		temp = temp->parent;
	}
}

Node* find_min(Node* node)
{
	if (node->left != NULL)
		return find_min(node->left);
	else
		return node;
}

Node* findSuccessor(Node*node)
{
	Node*temp = node;
	Node*result;
	if (node->right)
		return find_min(node->right);
	else {
		result = temp->parent;
		while ((result!=NULL)&&node == result->right)
		{
			node = result;
			result = result->parent;
		}
		return result;
	}
}

int remove_node(Tree*tree, int key)
{
	Node*node = search(tree->root, key);
	if (!node)
		return 1;

	Node*temp,*temp_2;
	if (!node->right || !node->left)
	{
		temp = node->left ? node->left : node->right;
		if (!temp)
		{
			if (node == tree->root)
			{
				free(tree->root);
				tree->root = NULL;
			}
			else
			{
				temp = node->parent;
				if (temp->left == node)temp->left = NULL;
				else temp->right = NULL;
				recalc(temp);
			}

		}
		else
		{
			*node = *temp;
			free(temp);
			recalc(node);
		}
	}
	else
	{
		temp = findSuccessor(node);
		node->key = temp->key;
		
		temp_2 = temp->parent;
		if (temp == temp_2->right)
		{
			free(temp_2->right);
			temp_2->right = NULL;
		}
		else
		{
			free(temp_2->left);
			temp_2->left = NULL;

		}
	
		recalc(temp_2);
		temp = temp_2;
	}

	int balance = 0;
	while (temp) {
		balance = getBalance(temp);
	

		if (balance < -1 && getBalance(temp->left)>1)
		{
			rotate_right_left(temp, tree);

			temp = temp->parent;
		}
		else if (balance<-1)
		{
			rotate_right(temp, tree);
			temp = temp->parent;
		}
		if (balance > 1 && getBalance(temp->left)<0)
		{
			rotate_left_right(temp, tree);
			temp = temp->parent;
		}
		else 	if (balance>1)
		{
			rotate_left(temp, tree);
			temp = temp->parent;
		}
		temp = temp->parent;
	}


	return 0;
}

void VLR(Node*root)
{
	if (root) {
		printf("%d->", root->key);
		VLR(root->left);
		VLR(root->right);
	}
}

void LVR(Node*root)
{
	if (root) {
		LVR(root->left);
		printf("%d->", root->key);
		LVR(root->right);
	}
}
void LRV(Node*root)
{
	if (root) {
		LRV(root->left);
		LRV(root->right);
		printf("%d->", root->key);
	}
}
void remove_tree(Tree*tree,Node*root)
{
	if (root)
	{
		remove_tree(tree, root->left);
		remove_tree(tree, root->right);
		remove_node(tree, root->key);
	}
}




int print(Node *tree, int is_l, int offset, int depth, char s[20][255])
{
	char b[20];
	int w = 5;

	if (!tree) return 0;

	sprintf(b, "(%03d)", tree->key);

	int l = print(tree->left, 1, offset, depth + 1, s);
	int r = print(tree->right, 0, offset + l + w, depth + 1, s);


	for (int i = 0; i < w; i++)
		s[2 * depth][offset + l + i] = b[i];

	if (depth && is_l) {

		for (int i = 0; i < w + r; i++)
			s[2 * depth - 1][offset + l + w / 2 + i] = '-';

		s[2 * depth - 1][offset + l + w / 2] = '|';
		s[2 * depth - 1][offset + l + w + r + w / 2] = '-';

	}
	else if (depth && !is_l) {

		for (int i = 0; i < l + w; i++)
			s[2 * depth - 1][offset - w / 2 + i] = '-';

		s[2 * depth - 1][offset + l + w / 2] = '|';
		
	}


	return l + w + r;
}

void print_tree(Node *tree)
{
	char s[20][255];
	for (int i = 0; i < 20; i++)
		sprintf(s[i], "%80s", " ");

	print(tree, 0, 0, 0, s);

	for (int i = 0; i < 20; i++)
		printf("%s\n", s[i]);
}






int main()
{
	int choice = 0;
	int temp = 0;
	Tree*tree = (Tree*)malloc(sizeof(Tree));
	tree->root = NULL;
	do
	{


		system("cls");
		printf("1.Dodaj wezel do drzewa\n");
		printf("2.Usun wezel\n");
		printf("3.Pokaz drzewo\n");
		printf("4.VLR\n");
		printf("5.LVR\n");
		printf("6.LRV\n");
		printf("7.Usun drzewo\n");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			system("cls");
			printf("Podaj wartosc: ");
			scanf("%d", &temp);
			insert(tree, temp);
			break;
		case 2:
			system("cls");
			printf("Podaj wartosc: ");
			scanf("%d", &temp);
			if (remove_node(tree, temp))
				printf("\nWezel nie istnieje!");
			else
				printf("\nPoprawnie usunieto.\n");
			system("pause");
			break;
		case 3:
			system("cls");
			print_tree(tree->root);
			printf("\n");
			system("pause");
			break;
		case 4:
			system("cls");
			VLR(tree->root);
			printf("\n");
			system("pause");
			break;
		case 5:
			system("cls");
			LVR(tree->root);
			printf("\n");
			system("pause");
			break;
		case 6:
			system("cls");
			LRV(tree->root);
			printf("\n");
			system("pause");
			break;
		case 7:
			remove_tree(tree, tree->root);
			break;
		}
	} while (choice > 0 && choice < 8);

	return 0;
}