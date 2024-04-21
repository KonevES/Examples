#include<iostream>

class Column;

class Matrix
{
	friend class Column;
public:
	Matrix();
	Matrix(int);
	Matrix(int, const int*);
	Matrix(const Matrix&);
	~Matrix();
	Matrix operator * (const Matrix&) const;
	Matrix operator + (const Matrix&) const;
	Matrix operator - (const Matrix&) const;
	Matrix operator *(const int&) const;
	Matrix& operator = (const Matrix&);
	bool operator ==(const Matrix&) const;
	bool operator !=(const Matrix&) const;
	Matrix operator ~ () const;
	Matrix operator () (int, int) const;
	int* operator[](int) const;
	Column operator () (int);
	void Output_matrix();
    void Input_matrix(int);
private:
	int dimention;
	int** matrixValues;
};

class Column
{

public:
	Column (const int n, Matrix *matrix)
	 {
		id_of_column=n;
		base_matrix=matrix;
		size=matrix->dimention;
	}

	int& operator[](int i) const
	{
		return base_matrix->matrixValues[i][id_of_column];
	}

	int Lenght() {
		return size;
	}
private:
	int id_of_column;
	int size;
	Matrix *base_matrix;
};

Matrix::Matrix()
{
	matrixValues = nullptr;
	dimention = 0;
}

Matrix::Matrix(int n): dimention(n)
{
	matrixValues = new int* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new int[dimention];
		for (int j = 0; j < dimention; j++)
		{
			matrixValues[i][j] = (i==j) ? 1 : 0;
		}
	}
}

Matrix::Matrix(int n, const int* array): dimention(n)
{
	matrixValues = new int* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new int[dimention];
		for (int j = 0; j < dimention; j++)
		{
			matrixValues[i][j] = (i==j) ? array[i] : 0;
		}
	}
}

Matrix::Matrix(const Matrix& matrix)
{
	dimention = matrix.dimention;
	matrixValues = new int* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new int[dimention];
		for (int j = 0; j < dimention; j++)
		{
			matrixValues[i][j] = matrix.matrixValues[i][j];
		}
	}
}

Matrix::~Matrix()
{
	if (matrixValues != nullptr) {
		for (int i = 0; i < dimention; i++)
		{
			delete [] matrixValues[i];
		}
		delete [] matrixValues;
		matrixValues = nullptr;
		dimention = 0;
	}
}

Matrix Matrix::operator*(const Matrix& matrix) const // const в конце, что-бы метод можно было вызывать от константных объектов. (с=a+b, a,b - константные)
{
	if (dimention != matrix.dimention)  throw std::length_error ("Maybe you won't use different sized matrices?");
	Matrix result(dimention);
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			result.matrixValues[i][j] = 0; //избавляемся от мусора. При изначально объявлении у нас была единичная матрица
			for (int k = 0; k < dimention; k++)
			{
				result.matrixValues[i][j] += matrixValues[i][k] * matrix.matrixValues[k][j];
			}
		}
	}
	return result;
}

Matrix Matrix::operator+(const Matrix& matrix) const
{
	if (dimention != matrix.dimention)  throw std::length_error ("Maybe you won't use different sized matrices?");
	Matrix result(dimention);
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			result.matrixValues[i][j]=matrix.matrixValues[i][j]+matrixValues[i][j]; //избавляться от мусора не нужно, т.к сразу присваиваем
		}
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
	if (dimention != matrix.dimention)  throw std::length_error ("Maybe you won't use different sized matrices?");
	Matrix result(dimention);
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			result.matrixValues[i][j]=matrixValues[i][j]-matrix.matrixValues[i][j];
		}
	}
	return result;
}

Matrix Matrix::operator*(const int& k) const
{
	Matrix result(*this); //конструктор копирования рабочей матрицы
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			result.matrixValues[i][j] *= k;
		}
	}
	return result;
}

Matrix& Matrix::operator=(const Matrix& matrix)
{
	if (this == &matrix) return *this;

	if (matrixValues!=nullptr)
	{
		for (int i=0; i < dimention; i++)
		{
			delete [] matrixValues[i];
		}
		delete [] matrixValues;
	}

	dimention = matrix.dimention;
	matrixValues = new int* [dimention];
	for (int i=0; i<dimention; i++)
	{
		matrixValues[i]=new int[dimention];
		for (int j=0; j<dimention; j++)
		{
			matrixValues[i][j]=matrix.matrixValues[i][j];
		}
	}
	return *this;
}

bool Matrix::operator==(const Matrix& matrix) const
{
	if (dimention != matrix.dimention) return false;
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			if (matrixValues[i][j]!=matrix.matrixValues[i][j]) return false;
		}
	}
	return true;
}

bool Matrix::operator !=(const Matrix& matrix) const
{
	return !(*this == matrix);
}

Matrix Matrix::operator~() const
{
	Matrix result(dimention);
	for (int i = 0; i < dimention; i++)
	{
		for (int j = 0; j < dimention; j++)
		{
			result.matrixValues[i][j] = matrixValues[j][i];
		}
	}
	return result;
}

Matrix Matrix::operator()(int n, int m) const
{
	Matrix minor(dimention - 1);
	for (int i = 0; i < minor.dimention; i++)
	{
		for (int j = 0; j < minor.dimention; j++)
		{
			if (i < n && j < m) {
				minor.matrixValues[i][j] = matrixValues[i][j];
			}
			if (i >= n && j < m) {
				minor.matrixValues[i][j] = matrixValues[i + 1][j];
			}
			if (i < n && j >= m) {
				minor.matrixValues[i][j] = matrixValues[i][j + 1];
			}
			if (i >= n && j >= m) {
				minor.matrixValues[i][j] = matrixValues[i + 1][j + 1];
			}
		}
	}
	return minor;
}

int* Matrix::operator[](int n) const
{
	return matrixValues[n];
}


Column Matrix::operator()(int n)
{
	Column col(n,this);
	return col;
}

void Matrix::Input_matrix(int n)
{
	for (int i=0; i<dimention; i++)
	{
		for (int j=0; j<dimention; j++)
		{
			std::cin>>matrixValues[i][j];
		}
	}
}

void Matrix::Output_matrix()
{
	for (int i=0; i<dimention; i++)
	{
		for (int j=0; j<dimention; j++)
		{
			std::cout<<matrixValues[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}

int main() {
	int n,k;
	std::cin>>n;
	std::cin>>k;
	std::string str1="123",str2="234",str3;
	str3=str1+str2;
	str2=str3;
    std::cout<<str1<<" "<<str2<<" "<<str3;

	/*
	Matrix A(n),B(n),C(n),D(n),K(n),Ans;
	A.Input_matrix(n);
	B.Input_matrix(n);
	C.Input_matrix(n);
	D.Input_matrix(n);
	Ans=(A+B*(~C)+(K*k))*(~D);
	Ans.Output_matrix();
	std::cout<<std::endl;

    A.Output_matrix();
    std::cout<<std::endl;
    B.Output_matrix();
    std::cout<<std::endl;
    std::cout<<(A==B);
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout<<(A==A);
    std::cout<<std::endl;
    std::cout<<std::endl;
    C=A+B;
    C.Output_matrix();
    std::cout<<std::endl;
    A.Input_matrix(n);
    std::cout<<std::endl;
    A[0][0]=5;
    A[1][0]=2;
    A(1)[0]=4;
    A.Output_matrix();
    int n1,n2;
    std::cin>>n;
    std::cin>>n1;
    std::cin>>n2;
    Matrix F(n),Minorr;
    F.Input_matrix(n);
    std::cout<<std::endl;
    Minorr=F(n1,n2);
    Minorr.Output_matrix();
    */
	return 0;
}
