#include <iostream>
#include<iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;


//define node data type for tree
struct node {
	int ldata;
	int rdata;
	struct node *left;
	struct node *right;
	int fp;
};



//Heap functions to get index of parent, left child or right child. Index starts from 0
int parent(int i){
	return (i-1)/2;
}

int left(int i){
	return (i*2) + 1;
}

int right(int i){
	return (i*2) + 2;
}



//Get data from a specific position in the file. Here position = index. Starts from 0. fid is the integer filename with .txt extension
int hgetpos(int fid, int position){

	stringstream t;
	t << fid;
	string filename;
	t >> filename;
	filename.append(".txt");

	fstream f1;
	char const *fname = filename.data();
	f1.open(fname, fstream::in | fstream::out);
	
	stringstream data("");
	string line;

	int i = -1;

	if (f1.is_open()){
		while (!f1.eof() ){
			getline (f1, line);
		     	if (i==position){
				data << line;
				break;
		      	}
			else{		
			}
			i++;
		}
		f1.close();
	}

	int result = 0;
	data >> result;
	return result;
}




//Inserts data to a specific position in the file. Here position = index. Starts from 0. fid is the integer filename with .txt extension
void hinsertpos(int fid, int record, int position){

	stringstream t;
	t << fid;
	string filename;
	t >> filename;
	filename.append(".txt");


	fstream f1;
	char const *fname = filename.data();
	f1.open(fname, fstream::in | fstream::out);
	
	stringstream data("");
	string line;

	int i = -1;

	if (f1.is_open()){
		while (!f1.eof() ){
			getline (f1, line);
			if (i==position){
				data << setw(7) << record;
				data << "\n";
		      	}
			else{
				data << line;
				data << "\n";			
			}
			i++;
		}
		f1.close();
	}


	ofstream f2(fname);

	    if(f2.is_open())   
	    {
		    f2 << data.rdbuf();
			f2.close();     
	    }
	
}




//Inserts a record in the min-heap and re-arranges heap to maintain min-heap property
void hinsert(int fid, int record){

	stringstream t;
	t << fid;
	string filename;
	t >> filename;
	filename.append(".txt");


	fstream f1;
	char const *fname = filename.data();
	f1.open(fname, fstream::in | fstream::out);

	int size = 0;

	if(f1.is_open())
	{
	    f1.seekg(0);
	    string s;
	    s.resize(7);
	    f1.read(&s[0], 7);
		stringstream temps(s);		
		temps >> size;
		f1.close();
	}

	hinsertpos(fid, record, size);	
	int i = size;
	hinsertpos(fid, ++size, -1);

	int temp;
	while(i !=0 && hgetpos(fid, parent(i)) > hgetpos(fid, i)){
		temp = hgetpos(fid, i);
		hinsertpos(fid, hgetpos(fid, parent(i)), i);
		hinsertpos(fid, temp, parent(i));
		i = parent(i);
	}
	
}




//Returns number of elements in a file with heap. filename is the integer filename with .txt extension
int hgetsize(int filename){
	return hgetpos(filename, -1);
}


//Returns minimum of the elements in a file with heap. filename is the integer filename with .txt extension
int hfindmin(int filename){
	return hgetpos(filename, 0);
}


//Returns maximum of elements in a file with heap. filename is the integer filename with .txt extension
int hfindmax(int filename){
	int heapsize = hgetpos(filename, -1);
	int max = 0;
	for(int i = 0; i < heapsize; i++){
		if(hgetpos(filename, i) > max){
			max = hgetpos(filename, i);
		}
	}

	return max;

}


//Searches a record in heap file. filename is the integer filename with .txt extension
bool hsearch(int filename, int key){

	int heapsize = hgetpos(filename, -1);	

	for(int i = 0; i < heapsize; i++){
		if(hgetpos(filename, i) == key){
			return 1;
		}
	}
	return 0;
}


//Deletes a record in heap file. filename is the integer filename with .txt extension
void hdelmin(int filename){

	int heapsize = hgetpos(filename, -1);	
	hinsertpos(filename, hgetpos(filename, heapsize-1), 0);
	hinsertpos(filename, -1, heapsize-1);
	hinsertpos(filename, --heapsize, -1);

	int i = 0;

	while(left(i) < heapsize && right(i) < heapsize){
		if((left(i) <= heapsize) && (hgetpos(filename, i) > hgetpos(filename, left(i))) && (hgetpos(filename, left(i)) < hgetpos(filename, right(i))) && (hgetpos(filename, left(i)) != -1) ){
			int temp = hgetpos(filename, i);
			hinsertpos(filename, hgetpos(filename, left(i)), i);
			hinsertpos(filename, temp, left(i));

			i = left(i);
			continue;
		}

		else if( (right(i) <= heapsize) && (hgetpos(filename, i) > hgetpos(filename, right(i))) && (hgetpos(filename, right(i)) < hgetpos(filename, left(i))) && (hgetpos(filename, right(i)) !=-1)  ){
			int temp = hgetpos(filename, i);
			hinsertpos(filename, hgetpos(filename, right(i)), i);
			hinsertpos(filename, temp, right(i));

			i = right(i);
			continue;
		}

		else{
			break;
		}

	}
}


//Creates an enpty heap file and initializes some values. fid is the integer filename. file created will be <fid>.txt
void hinit(int fid){

	stringstream t;
	t << fid;
	string filename;
	t >> filename;
	filename.append(".txt");


	fstream f1;
	char const *fname = filename.data();
	f1.open(fname, std::ios::out | std::ios::app);
	f1.close();
	f1.open(fname, fstream::in | fstream::out);
	if(!f1) { 
       		cout<<"Unable to create file."; 
   	} 

	if (f1.is_open()){
	    	f1 << setw(8) << "0\n";
		for(int i = 0; i < 32; i++){
	    		f1 << setw(7) << "-1";
				f1 << "\n";
	 	}
	    	f1.close();
	 }		

	 else cout << "Unable to open file."; 
	
}


node* dbinit(int *f, int flag){
	node *n;
	n = new node();
	n->ldata = -1;
	n->rdata = -1;
	n->left = NULL;
	n->right = NULL;
	n->fp = -1;

	if(flag==1){
		n->fp = *f;
		hinit(*f);
		(*f)++;
	}

	return n;
}




//Searches for a specific record in the database
bool dbsearch(int x, node *root){
	node *n = root;

	bool found = 0;

	while(n->fp == -1){
		if(n->ldata <= x && n->rdata >= x){
			if(n->left->ldata <= x && n->left->rdata >= x){
				n = n->left;
			}
			else{
				n = n->right;
			}
		}
		else{
			return found;
		}
	}

	if(hsearch(n->fp, x)){
		found = 1;
	}

	return found;

}



//Inserts a record into the database
void dbinsert(int x, int *f, node *root){
	
	node *n = root;

	while(n->fp == -1){
		if(x <= n->left->rdata ){
			if(x < n->ldata){
				n->ldata = x;
			}
			n = n->left;
		}
		else if(x > n->left->rdata){
			if(x > n->rdata){
				n->rdata = x;
			}
			n = n->right;
		}

	}

	int fid = n->fp;

	int size = hgetpos(fid, -1);

	if(size == 32){
		if(!hsearch(fid, x)){
			node *n1 = dbinit(f, 1);
			node *n2 = dbinit(f, 0);
			n2->fp = fid;

			n->left = n1;
			n->right = n2;
			n->fp = -1;
			if(x < n->ldata){
				n->ldata = x;
			}
			if(x > n->rdata){
				n->rdata = x;
			}

			int fid1 = n1->fp;
			int fid2 = n2->fp;


			for(int i = 0; i < 16; i++){
				int temp = hfindmin(fid2);
				hinsert(fid1, temp);
				hdelmin(fid2);
			}


			n1->ldata = hfindmin(fid1);
			n1->rdata = hfindmax(fid1);

			n2->ldata = hfindmin(fid2);
			n2->rdata = hfindmax(fid2);

			if(x < n1->rdata){
				hinsert(fid1, x);
				n1->ldata = hfindmin(fid1);
			}
			else{
				hinsert(fid2, x);
				n2->ldata = hfindmin(fid2);
				n2->rdata = hfindmax(fid2);
			}

		}

	}

	else{
		if(!hsearch(fid, x)){
			hinsert(fid, x);
			if(x < n->ldata || n->ldata==-1){
				n->ldata = x;
			}
			if(x > n->rdata || n->rdata ==-1){
				n->rdata = x;
			}
		}
	}
}




//Helper diagnostic functions from here onwards
void inorder(node *n) 
{ 
	if (n == NULL) 
		return; 
	inorder(n->left); 
	if(n->fp != -1){
		cout << n->ldata << " "; 
		cout << n->rdata << " "; 
	}
	inorder(n->right); 
} 

void inorder2(node *n) 
{ 
	if (n == NULL) 
		return; 
	inorder2(n->left); 
	if(n->fp != -1){
		cout << hfindmin(n->fp) << " "; 
		cout << hfindmax(n->fp) << " "; 
	}
	inorder2(n->right); 
}

void minvalues(node *n) 
{ 
	if (n == NULL) 
		return; 
	cout << n->ldata << " "; 
	minvalues(n->left); 
	minvalues(n->right); 
} 

void maxvalues(node *n) 
{ 
	if (n == NULL) 
		return; 
	maxvalues(n->left); 
	maxvalues(n->right);
	cout << n->rdata << " "; 
} 

void countnodes(node *root, int *count)
{
	if(root != NULL)
	{
		countnodes(root->left, count);
		(*count)++;
		countnodes(root->right, count);
	}
}

void countleaves(node *root, int *count)
{
	if(root != NULL)
	{
		countleaves(root->left, count);
		if(root->fp != -1){
			(*count)++;
		}
		countleaves(root->right, count);
	}
}

int height(node* node)  
{  
	if (node == NULL)  
		return 0;  
	else
	{ 
		int lh = height(node->left);  
		int rh = height(node->right);  
		if (lh > rh)  
			return(lh + 1);  
		else return(rh + 1);  
	}  
}  

void printbst(node *n, int tabs) 
{ 
	if (n == NULL) 
		return; 

	for(int i = 0; i < tabs; i++){
		cout << "\t";
	}
	cout << "Range = [" << n->ldata << "," << n->rdata << "], File: "; 
	if(n->fp == -1){
		cout << "None" << endl;
	}
	else{
		cout << n->fp << ".txt" << endl;
	}
	printbst(n->left, ++tabs); 
	printbst(n->right, tabs); 
} 

int main(){

	int f =  1;
	node *root = dbinit(&f, 1);

	int insertions;

	cout << "Enter number of insertions: ";
	cin >> insertions;

	int temp;

	cout << "Enter numbers seperated by newline or spaces: " << endl;

	for(int i = 0; i < insertions; i++){
		cin >> temp;
		dbinsert(temp, &f, root);
	}


	cout << endl << endl;


	cout << "+++ Inorder listing of min and max values of leaves." << endl;
	inorder(root);
	cout << endl;
	cout << endl;
	cout << "+++ Inorder listing of min and max values read from files." << endl;
	inorder2(root);
	cout << endl;
	cout << endl;
	cout << "+++ Sorted listing of min values at all nodes." << endl;
	minvalues(root);
	cout << endl;
	cout << endl;
	cout << "+++ Sorted listing of max values at all nodes." << endl;
	maxvalues(root);
	cout << endl;
	cout << endl;
	cout << "+++ Statistics of the BST." << endl;
	int count1 = 0, count2 = 0;
	countnodes(root, &count1);
	cout <<"	Number of nodes = " << count1 << endl;
	countleaves(root, &count2);
	cout <<"	Number of leaves = " << count2 << endl;
	cout <<"	Height           = " << height(root)-1 << endl;
	cout << endl;
	cout << endl;
	cout << "+++ The BST" << endl;
	printbst(root, 0);

	cout << endl;
	cout << endl;

	int s1, s2;
	cout << "Enter search key 1: ";
	cin >> s1;
	cout << "Enter search key 2: ";
	cin >> s2;

	cout << endl;

	cout << "+++ Search results" << endl;

	if(dbsearch(s1, root)){
		cout << "\t" << s1 << " : PRESENT" << endl;
	}
	else{
		cout << "\t" << s1 << " : ABSENT" << endl;
	}

	if(dbsearch(s2, root)){
		cout << "\t" << s2 << " : PRESENT" << endl;
	}
	else{
		cout << "\t" << s2 << " : ABSENT" << endl;
	}

	return 0;
}
