#include<iostream>
#include<cmath>
class Column;

class Matrix
{
	friend class Column;
public:
	Matrix();
	Matrix(int);
	Matrix(int, const double*);
	Matrix(const Matrix&);
	~Matrix();
	Matrix operator * (const Matrix&) const;
	Matrix operator + (const Matrix&) const;
	Matrix operator - (const Matrix&) const;
	Matrix operator *(const double&) const;
	Matrix& operator = (const Matrix&);
	bool operator ==(const Matrix&) const;
	bool operator !=(const Matrix&) const;
	Matrix operator ~ () const;
	Matrix operator () (int, int) const;
	double* operator[](int) const;
	Column operator () (int);
	void Output_matrix();
    void Input_matrix(int);
private:
	int dimention;
	double** matrixValues;
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

	double& operator[](int i) const
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
	matrixValues = new double* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new double[dimention];
		for (int j = 0; j < dimention; j++)
		{
			matrixValues[i][j] = (i==j) ? 1 : 0;
		}
	}
}

Matrix::Matrix(int n, const double* array): dimention(n)
{
	matrixValues = new double* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new double[dimention];
		for (int j = 0; j < dimention; j++)
		{
			matrixValues[i][j] = (i==j) ? array[i] : 0;
		}
	}
}

Matrix::Matrix(const Matrix& matrix)
{
	dimention = matrix.dimention;
	matrixValues = new double* [dimention]; //Выделяем память под одномерный массив указателей на куче
	for (int i = 0; i < dimention; i++)
	{
		matrixValues[i] = new double[dimention];
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

Matrix Matrix::operator*(const double& k) const
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
	matrixValues = new double* [dimention];
	for (int i=0; i<dimention; i++)
	{
		matrixValues[i]=new double[dimention];
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
	--n;
	--m;
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

double* Matrix::operator[](int n) const
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
	Matrix A(n), C(n), Identity_matrix(n);
	A.Input_matrix(n);
	double *x_k,*x_k_wave,*x_k_next;
	x_k=(double*) malloc(n*sizeof(double ));
	x_k_wave=(double*) malloc(n*sizeof(double ));
	x_k_next=(double*) malloc(n*sizeof(double ));
	for (int i=0; i<n; i++){
		x_k[i]=1;
		x_k_wave[i]=0;
		x_k_next[i]=0;
	}

	double spectrum_edge_1=0;
	for (int i=0; i<1000; i++){
		for (int j=0; j<n; j++){
			for (int k=0; k<n; k++){
				x_k_wave[j]+=A[j][k]*x_k[k];
			}
		}
        double x_k_wave_norm=0;
        for (int j=0; j<n; j++){
            x_k_wave_norm+=x_k_wave[j]*x_k_wave[j];
        }
        x_k_wave_norm=sqrt(x_k_wave_norm);
		for (int j=0; j<n; j++){
			x_k_next[j]=x_k_wave[j]/x_k_wave_norm;
		}

		spectrum_edge_1=0;
		for (int j=0; j<n; j++){
			spectrum_edge_1+=x_k_wave[j]*x_k[j];
		}

		for (int j=0; j<n; j++){
			x_k[j]=x_k_next[j];
		}

		for (int j=0; j<n; j++){
            x_k_wave[j]=0;
		}
	}

    C=(Identity_matrix*spectrum_edge_1-A);

    double spectrum_edge_2=0;
	for (int i=0; i<1000; i++){
		for (int j=0; j<n; j++){
			for (int k=0; k<n; k++){
				x_k_wave[j]+=C[j][k]*x_k[k];
			}
		}
        double x_k_wave_norm=0;
        for (int j=0; j<n; j++){
            x_k_wave_norm+=x_k_wave[j]*x_k_wave[j];
        }
        x_k_wave_norm=sqrt(x_k_wave_norm);
		for (int j=0; j<n; j++){
			x_k_next[j]=x_k_wave[j]/x_k_wave_norm;
		}

		spectrum_edge_2=0;
		for (int j=0; j<n; j++){
			spectrum_edge_2+=x_k_wave[j]*x_k[j];
		}

		for (int j=0; j<n; j++){
			x_k[j]=x_k_next[j];
		}

		for (int j=0; j<n; j++){
            x_k_wave[j]=0;
		}
	}
	std::cout<<spectrum_edge_1<<std::endl;
	std::cout<<spectrum_edge_2<<std::endl;
    std::cout<<std::min(spectrum_edge_1,spectrum_edge_1-spectrum_edge_2)<<" "<<std::max(spectrum_edge_1,spectrum_edge_1-spectrum_edge_2);
	return 0;
}
