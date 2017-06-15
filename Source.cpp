#include <iostream>
#include <string>
#include<sstream>
#include<fstream>
#include <iomanip>     // for setw
#include <vector>
#include <algorithm>   // for sort

using namespace std;


void PrintMat(const vector<vector<int>> mat)
{
	for (int j=0; j < mat.size(); j++)
	{   
		for (int k=0; k < mat[0].size(); k++)
		{   
			cout << setw(4) << mat[j][k];
		}
		cout << endl;
	}
}


int main()
{
	string fileName;
	int filterSize;

	vector<vector<int>> mat;

	cout<<"This program performs median filtering operation on an input matrix given in a text file"<< endl;

	cout<<"Enter the filter size (must be an positive odd integer)"<< endl;
	cin>>filterSize;

	while (filterSize < 0 || filterSize % 2 == 0)
	{
		cout<<"Error. Enter the filter size (must be a positive odd integer)";
		cin>>filterSize;	
	}

	cout<<"Enter the input file name"<< endl;
	cin>>fileName;

	ifstream input;
	input.open(fileName);

	while(input.fail())
	{
		cout<< "Error:Could not open the file "<< fileName<< endl;

		cout<<"Enter the input file name"<< endl;
		cin>>fileName;

		ifstream input;
		input.open(fileName);
	}

	string line;
	while(!input.eof())
	{
		getline(input, line);
		stringstream ss(line);

		string num;
		vector<int> temp;

		while(ss >> num)
		{
			for(int i = 0; i < num.size(); ++i)   // looking every char in the taken string
			{
				if (num[i] < '0' || '9' < num[i] )  // looking for invalid characters
				{
					cout<<"Error: Input file contains invalid characters"<<endl;
					return 0;
				}
			}

			int a = atoi (num.c_str());  // turning taken string into integer

			if (a <0 || a > 255)   // wrong pixel value error
			{
				cout<<"Pixel with value: " <<num <<" is not in range [0-255]"<<endl;
				return 0;
			}

			temp.push_back(a);		
		}	

		mat.push_back(temp);
	}   // matrix has taken correctly

	for (int q=0; q<mat.size()-1; q++)  // checking whether the matrix is in the matrix form
	{
		if (mat[q].size() != mat[q+1].size())
		{
			cout<<"Input file is not matrix format"<<endl;
			return 0;
		}
	}

	int a = (filterSize-1)/2;    // # of index that it is needed to start from left and upper corner to write submatrix
	int matrow = mat.size();
	int matcol = mat[0].size();

	vector<vector<int>> filteredMatrix (mat.size(), vector<int>(mat[0].size()));  //a matrix with rows the same as mat and columns the same as mat, all zero

	int j,k;

	for (int j=0; j < mat.size(); j++)
	{   
		for (int k=0; k < mat[0].size(); k++)
		{
			vector <int> submatrix;

			// checking boundries from left and upper corners
			
			if (k>=a && j>=a)  
			{				
				for (int m=0; m<min (filterSize, filterSize-(j+a+1-matrow)); m++)
				{
					for (int n=0; n<min(filterSize, filterSize-(k+a+1-matcol)); n++)
					{
						submatrix.push_back(mat[j-a+m][k-a+n]);								
					}
				}
			}

			if (k>=a && j<a)
			{
				for (int m=0; m<filterSize-a+j; m++)
				{
					for (int n=0; n<min(filterSize, filterSize-(a+k+1-matcol)); n++)
					{
						submatrix.push_back(mat[m][k-a+n]);
					}
				}
			}

			if (k<a && j>=a)
			{
				for (int m=0; m<min (filterSize, filterSize-(a+j+1-matrow)); m++)
				{
					for (int n=0; n<filterSize-a+k; n++)
					{
						submatrix.push_back(mat[j-a+m][n]);
					}
				}
			}

			if (k<a && j<a)
			{
				for (int m=0; m<filterSize-a+j; m++)
				{
					for (int n=0; n<filterSize-a+k; n++)
					{
						submatrix.push_back(mat[m][n]);
					}
				}
			}

			sort (submatrix.begin(), submatrix.end());  // sorting the taken submatrix

			int median;

			if (submatrix.size() % 2 == 1)  // for odd number of elements
			{
				median = submatrix[submatrix.size()/2];
				filteredMatrix[j][k] = median;
			}

			else    // for even number of elements, averager
			{
				median = (submatrix[submatrix.size()/2] + submatrix[submatrix.size()/2-1])/2;   //it is rounded down
				filteredMatrix[j][k] = median;
			}
		}
	}

	PrintMat (mat);
	cout<<"\n";
	PrintMat (filteredMatrix);
	cout<<"\n";

	cin.ignore();
	cin.get();
	return 0;  

}
