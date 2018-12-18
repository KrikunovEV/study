#include <iostream>
#include <bitset>
using namespace std;


const int AdamaraMatrix[16][16] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, -1,	1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1},
		{1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1},
		{1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1},
		{1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1},
		{1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1},
		{1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1},
		{1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1},
		{1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, -1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1},
		{1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 1},
		{1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1},
		{1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1},
		{1, -1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1},
		{1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, 1, -1, -1},
		{1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1}
};


void Encode(int* data, int** GeneratingMatrix, int m, int n, int* out);
void Decode(int* encode, int m, int n, int** BinAdamaraMatrix, int* out);
void GetBinAdamaraMatrix(int n, int** out);
void PrintMatrix(int** matrix, int n, int m);


int main()
{
	cout << "SHAAAAAAAAAAAAAAAAA-1" << endl;

	int m = 4;
	int n = 16;

	int** BinAdamaraMatrix = new int*[n];
	for (int i = 0; i < n; i++)
	{
		BinAdamaraMatrix[i] = new int[n];
	}
	GetBinAdamaraMatrix(n, BinAdamaraMatrix);




	int** GeneratingMatrix = new int*[m+1];
	for (int i = 0; i < m + 1; i++)
	{
		GeneratingMatrix[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
	{
		GeneratingMatrix[0][i] = 1;
	}

	for (int i = 0; i < n; i++) // columns
	{
		bitset<4> bits = i;

		for (int j = m; j > 0; j--) //rows
		{
			GeneratingMatrix[j][i] = bits[m - j];
		}
	}

	cout << endl << "Generating matrix: " << endl;
	PrintMatrix(GeneratingMatrix, m + 1, n);
	cout << endl;




	int* data = new int[m + 1];
	data[0] = data[3] = data[4] = 1;
	data[1] = data[2] = 0;

	cout << "Data: ";
	for (int i = 0; i < m + 1; i++)
		cout << data[i];
	cout << endl;




	int* encode = new int[n];
	for (int i = 0; i < n; i++)
		encode[i] = 0;

	Encode(data, GeneratingMatrix, m, n, encode);
	
	cout << "Encoded word: ";
	for (int i = 0; i < n; i++)
		cout << encode[i];
	cout << endl;



	// MAKE SOME ERRORS
	encode[0] = encode[0] ? 0 : 1;
	encode[10] = encode[10] ? 0 : 1;
	encode[11] = encode[11] ? 0 : 1;
	encode[15] = encode[15] ? 0 : 1;



	int* decode = new int[m + 1];

	Decode(encode, m, n, BinAdamaraMatrix, decode);

	cout << "Decoded word: ";
	for (int i = 0; i < m + 1; i++)
		cout << decode[i];
	cout << endl;

	system("pause");
	return 0;
}


void Encode(int* data, int** GeneratingMatrix, int m, int n, int* out)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m + 1; j++)
		{
			out[i] += GeneratingMatrix[j][i] * data[j];
		}
		out[i] %= 2;
	}
}


void Decode(int* encode, int m, int n, int** BinAdamaraMatrix, int* out)
{
	for (int i = 0; i < n; i++)
	{
		if (encode[i] == 0)
		{
			encode[i] = -1;
		}
	}

	int* temp = new int[n];
	for (int i = 0; i < n; i++)
	{
		temp[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			temp[i] += AdamaraMatrix[j][i] * encode[j];
		}
	}

	cout << "(encode * H) vector: ";
	for (int i = 0; i < n; i++)
		cout << temp[i] << ' ';
	cout << endl;

	int argmax = 0;
	for (int i = 0; i < n; i++)
	{
		if (temp[argmax] < temp[i])
		{
			argmax = i;
		}
	}
	cout << "Index of max element: " << argmax << endl;

	for (int i = 0; i < n; i++)
	{
		temp[i] = BinAdamaraMatrix[argmax][i];
	}
	cout << "From bin Adamara matrix: ";
	for (int i = 0; i < n; i++)
		cout << temp[i];
	cout << endl;


	out[0] = temp[0];
	int counter = 1;
	for (int i = 0; i < m; i++)
	{
		out[m - i] = out[0] + temp[counter];
		out[m - i] %= 2;
		counter *= 2;
	}
}


void GetBinAdamaraMatrix(int n, int** out)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			out[i][j] = (AdamaraMatrix[i][j] + 1) / 2;
		}
	}
}


void PrintMatrix(int** matrix, int rows, int columns)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
}