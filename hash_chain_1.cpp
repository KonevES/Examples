#include <iostream>
#include <fstream>
#include <string>
#include<vector>

using namespace std;

ifstream fin("input.txt");

ofstream fout("output.txt");

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
    Matrix const operator * (const Matrix&) const;
    Matrix const operator + (const Matrix&) const;
    Matrix const operator - (const Matrix&) const;
    Matrix const operator *(const int&) const;
    Matrix& operator = (const Matrix&);
    bool const operator ==(const Matrix&) const;
    bool const operator !=(const Matrix&) const;
    Matrix const operator ~ () const;
    Matrix const operator () (int, int) const;
    int* const operator [] (int) const;
    Column const operator () (int) const;
    int ElementSum() {
        int sum = 0;
        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                sum += MatrixBody[i][j];
            }
        }
        return sum;
    }
    friend std::ostream& operator << (std::ostream& os, const Matrix& p);
    friend std::ifstream& operator>> (std::ifstream& ifstream1, Matrix& s);
private:
    int** MatrixBody;
    int Size;
};

class Column
{
public:
    Column(int id, const Matrix* matrix) {
        ColumnId = id;
        MatrixFrom = matrix;
        Size = matrix->Size;
    }
    int& operator[](int i) {
        return MatrixFrom->MatrixBody[i][ColumnId];
    }
    int Lenght() {
        return Size;
    }
    friend std::ostream& operator << (std::ostream& os, Column p);
private:
    const Matrix* MatrixFrom;
    int ColumnId;
    int Size;
};

Matrix::Matrix()
{
    MatrixBody = nullptr;
    Size = 0;
}

Matrix::Matrix(int n)
{
    Size = n;
    MatrixBody = new int* [Size];
    for (int i = 0; i < Size; i++)
    {
        MatrixBody[i] = new int[Size];
        for (int j = 0; j < Size; j++)
        {
            MatrixBody[i][j] = 0;
        }
        MatrixBody[i][i] = 1;
    }
}

Matrix::Matrix(int n, const int* array)
{
    Size = n;
    MatrixBody = new int* [Size];
    for (int i = 0; i < Size; i++)
    {
        MatrixBody[i] = new int[Size];
        for (int j = 0; j < Size; j++)
        {
            MatrixBody[i][j] = 0;
        }
        MatrixBody[i][i] = array[i];
    }
}

Matrix::Matrix(const Matrix& matrix)
{
    Size = matrix.Size;
    MatrixBody = new int* [Size];
    for (int i = 0; i < Size; i++)
    {
        MatrixBody[i] = new int[Size];
        for (int j = 0; j < Size; j++)
        {
            MatrixBody[i][j] = matrix.MatrixBody[i][j];
        }
    }
}

Matrix::~Matrix()
{
    if (MatrixBody != nullptr) {
        for (int i = 0; i < Size; i++)
        {
            delete[]MatrixBody[i];
        }
        delete[]MatrixBody;
        MatrixBody = nullptr;
        Size = 0;
    }
}

Matrix const Matrix::operator*(const Matrix& matrix) const
{
    if (Size != matrix.Size) { throw; }
    Matrix toReturn(Size);
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            toReturn.MatrixBody[i][j] = 0;
            for (int k = 0; k < Size; k++)
            {
                toReturn.MatrixBody[i][j] += MatrixBody[i][k] * matrix.MatrixBody[k][j];
            }
        }
    }
    return toReturn;
}

Matrix const Matrix::operator+(const Matrix& matrix) const
{
    if (Size != matrix.Size) { throw; }
    Matrix toReturn(Size);
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            toReturn.MatrixBody[i][j] = MatrixBody[i][j] + matrix.MatrixBody[i][j];
        }
    }
    return toReturn;
}

Matrix const Matrix::operator-(const Matrix& matrix) const
{
    if (Size != matrix.Size) { throw; }
    Matrix toReturn(Size);
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            toReturn.MatrixBody[i][j] = MatrixBody[i][j] - matrix.MatrixBody[i][j];
        }
    }
    return toReturn;
}

Matrix const Matrix::operator*(const int& k) const
{
    Matrix toReturn(*this);
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            toReturn.MatrixBody[i][j] *= k;
        }
    }
    return toReturn;
}

Matrix& Matrix::operator=(const Matrix& matrix)
{
    if (this == &matrix) {
        return *this;
    }
    if (MatrixBody != nullptr) {
        for (int i = 0; i < Size; i++)
        {
            delete[] MatrixBody[i];
        }
        delete[]MatrixBody;
    }
    Size = matrix.Size;
    MatrixBody = new int* [Size];
    for (int i = 0; i < Size; i++)
    {
        MatrixBody[i] = new int[Size];
        for (int j = 0; j < Size; j++)
        {
            MatrixBody[i][j] = matrix.MatrixBody[i][j];
        }
    }
    return *this;
}

bool const Matrix::operator==(const Matrix& matrix) const
{
    if (Size != matrix.Size) { throw; }
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            if (MatrixBody[i][j] != matrix.MatrixBody[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool const Matrix::operator!=(const Matrix& matrix) const
{
    if (Size != matrix.Size) { throw; }
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            if (MatrixBody[i][j] != matrix.MatrixBody[i][j]) {
                return true;
            }
        }
    }
    return false;
}

Matrix const Matrix::operator~() const
{
    Matrix toReturn(Size);
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            toReturn.MatrixBody[i][j] = MatrixBody[j][i];
        }
    }
    return toReturn;
}

Matrix const Matrix::operator()(int n, int m) const
{
    n--;
    m--;
    Matrix minor(Size - 1);
    for (int i = 0; i < minor.Size; i++)
    {
        for (int j = 0; j < minor.Size; j++)
        {
            if (i < n && j < m) {
                minor.MatrixBody[i][j] = MatrixBody[i][j];
            }
            if (i >= n && j < m) {
                minor.MatrixBody[i][j] = MatrixBody[i + 1][j];
            }
            if (i < n && j >= m) {
                minor.MatrixBody[i][j] = MatrixBody[i][j + 1];
            }
            if (i >= n && j >= m) {
                minor.MatrixBody[i][j] = MatrixBody[i + 1][j + 1];
            }
        }
    }
    return minor;
}

int* const Matrix::operator[](int i) const
{
    return MatrixBody[i];
}

Column const Matrix::operator()(int i) const
{
    Column a(i, this);
    return a;
}

std::ostream& operator<<(std::ostream& os, const Matrix& s)
{
    for (int i = 0; i < s.Size; i++)
    {
        for (int j = 0; j < s.Size; j++)
        {
            os << s.MatrixBody[i][j];
            os << " ";
        }
        os << '\n';
    }
    return os;
}

std::ifstream& operator>>(std::ifstream& ifstream1, Matrix& s)
{
    for (int i = 0; i < s.Size; i++)
    {
        for (int j = 0; j < s.Size; j++)
        {
            ifstream1 >> s.MatrixBody[i][j];
        }
    }
    return ifstream1;
}

std::ostream& operator<<(std::ostream& os, Column p)
{
    for (int i = 0; i < p.Size; i++)
    {
        os << p[i] << " ";
    }
    return os;
}

namespace std
{
    template <>
    struct hash<Matrix>
    {
        size_t operator()(Matrix&);
    };
}

size_t std::hash<Matrix>::operator()(Matrix& matrix)
{
    return matrix.ElementSum();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename KeyType, typename DataType>
class LinkedHashList
{
private:
    KeyType KeyValue;
    DataType DataValue;
    LinkedHashList* NextLeaf;
public:
    LinkedHashList(KeyType Key, DataType Value) : KeyValue(Key), DataValue(Value), NextLeaf(nullptr) {}

    LinkedHashList(KeyType Key, DataType Value, LinkedHashList* Next) : KeyValue(Key), DataValue(Value), NextLeaf(Next) {}

    KeyType GetKey()
    {
        return KeyValue;
    }
    DataType GetValue()
    {
        return DataValue;
    }
    DataType& GetValueToChange()
    {
        return DataValue;
    }
    void SetValue(const DataType& Value)
    {
        this->DataValue = Value;
    }
    LinkedHashList<KeyType, DataType>* GetNext()
    {
        return NextLeaf;
    }
    void SetNext(LinkedHashList<KeyType, DataType>* Next)
    {
        this->NextLeaf = Next;
    }
};

template <typename DataType>
class TableArray {
private:
    DataType* ArrayBody;
    int Lenght;
public:
    TableArray() : Lenght(0), ArrayBody(nullptr) {}

    TableArray(int Size) : Lenght(Size), ArrayBody(new DataType[Lenght])
    {
        for (int i = 0; i != Lenght; ++i) ArrayBody[i] = nullptr;
    }

    TableArray(int Size, const DataType& Value) : Lenght(Size), ArrayBody(new DataType[Lenght])
    {
        for (int i = 0; i != Lenght; ++i) ArrayBody[i] = Value;
    }

    TableArray(const TableArray<DataType>& Another)
    {
        if (ArrayBody != nullptr) { delete[] ArrayBody; }
        Lenght = Another.Lenght;
        ArrayBody = new DataType[Lenght];
        for (int i = 0; i != Lenght; i++) ArrayBody[i] = Another.ArrayBody[i];
    }

    ~TableArray()
    {
        if (ArrayBody != nullptr) delete[] ArrayBody;
    }

    void ChangeSize(int NewSize, DataType FillingValue)
    {
        DataType* newArr = new DataType[NewSize];
        for (int i = 0; i != NewSize; i++) newArr[i] = (i < (Lenght < NewSize ? Lenght : NewSize) ? ArrayBody[i] : FillingValue);
        if (ArrayBody != nullptr) delete[] ArrayBody;
        ArrayBody = newArr;
        Lenght = NewSize;
    }
    int GetSize()
    {
        return Lenght;
    }
    DataType& operator[](int I)
    {
        return ArrayBody[I];
    }
    TableArray<DataType>& operator=(TableArray<DataType>& Another)
    {
        if (this == &Another)return *this;
        else if (ArrayBody != nullptr) delete[] ArrayBody;
        Lenght = Another.Lenght;
        ArrayBody = new DataType[Lenght];
        for (int i = 0; i < Lenght; i++) ArrayBody[i] = Another.ArrayBody[i];
        return *this;
    }

    typedef DataType* iterator;
    iterator begin() {
        return ArrayBody;
    }
    iterator end() {
        return ArrayBody + Lenght;
    }
};

template <typename KeyType, typename DataType>
class Iterator {
private:
    LinkedHashList<KeyType, DataType>* Current;
    LinkedHashList<KeyType, DataType>* Previous;
    typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator ArrayStart;
    typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator ArrayEnd;
public:
    Iterator
    (
        LinkedHashList<KeyType, DataType>* Cur,
        typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator ArrItStart,
        typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator ArrayItEnd
    )
    {
        Current = Cur;
        ArrayStart = ArrItStart;
        ArrayEnd = ArrayItEnd;
        Previous = nullptr;
    }

    Iterator(const Iterator& Other) : Current(Other.Current), ArrayStart(Other.ArrayStart), ArrayEnd(Other.ArrayEnd), Previous(Other.Previous) {}

    Iterator& operator=(const Iterator& Other) {
        Previous = Other.Previous;
        Current = Other.Current;
        ArrayStart = Other.ArrayStart;
        ArrayEnd = Other.ArrayEnd;
        return *this;
    }

    LinkedHashList<KeyType, DataType>*& operator->() {
        return Current;
    }

    LinkedHashList<KeyType, DataType>& operator*() {
        return *Current;
    }

    bool operator==(const Iterator& Other) {
        return ((Current == Other.Current) && (ArrayStart == Other.ArrayStart) && (ArrayEnd == Other.ArrayEnd));
    }

    bool operator!=(const Iterator& Other) {
        return !(*this == Other);
    }

    Iterator& operator++() {
        if (Current->GetNext()) {
            Previous = Current;
            Current = Current->GetNext();
            return *this;
        }
        Previous = nullptr;
        ArrayStart++;
        while (ArrayStart != ArrayEnd) {
            if (*ArrayStart) {
                Current = *ArrayStart;
                return *this;
            }
            ArrayStart++;
        }
        Current = nullptr;
        return *this;
    }

    Iterator operator++(int) {
        Iterator returned = *this;
        if (Current->GetNext()) {
            Previous = Current;
            Current = Current->GetNext();
            return returned;
        }
        Previous = nullptr;
        ArrayStart++;
        while (ArrayStart != ArrayEnd) {
            if (*ArrayStart) {
                Current = *ArrayStart;
                return returned;
            }
            ArrayStart++;
        }
        Current = nullptr;
        return returned;
    }

    typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator GetArrIterator() {
        return ArrayStart;
    }

    bool HasValue() {
        return Current != nullptr;
    }

    LinkedHashList<KeyType, DataType>* GetPrev() {
        return Previous;
    }
    LinkedHashList<KeyType, DataType>* GetCur() {
        return Current;
    }
};

template <typename KeyType, typename DataType>
class HashTable {
protected:
    TableArray<LinkedHashList<KeyType, DataType>*> Table;
    int TableSize;
    int BeingUsed;
    std::hash<KeyType> HashFunction;
    double FillPercent = 0.75;

public:
    HashTable(int Size) : TableSize(Size) {
        Table.ChangeSize(Size, nullptr);
        BeingUsed = 0;
    }

    HashTable<KeyType, DataType>& operator=(HashTable<KeyType, DataType>& other) {
        TableSize = other.TableSize;
        BeingUsed = other.BeingUsed;
        Table = other.Table;
        return *this;
    }

    virtual ~HashTable()
    {
        for (auto list : Table) {
            if (!list) continue;
            auto ptr = list;
            while (ptr) {
                auto delPtr = ptr;
                ptr = ptr->GetNext();
                delete delPtr;
            }
        }
    }

    Iterator<KeyType, DataType> end() {
        return Iterator<KeyType, DataType>(nullptr, Table.end(), Table.end());
    }

    Iterator<KeyType, DataType> begin() {
        for (typename TableArray<LinkedHashList<KeyType, DataType>*>::iterator ArrayIterator = Table.begin(); ArrayIterator != Table.end(); ArrayIterator++)
        {
            if (*ArrayIterator) return Iterator<KeyType, DataType>(*ArrayIterator, ArrayIterator, Table.end());
        }
        return end();
    }

    Iterator<KeyType, DataType> Find(KeyType Key) {
        int index = GetIndex(Key);
        if (!Table[index]) return end();
        Iterator<KeyType, DataType> required(Table[index], Table.begin() + index, Table.end());
        auto tableIterator = required.GetArrIterator();
        while (true)
        {
            if (required.GetArrIterator() != tableIterator) return end();
            if (required->GetKey() == Key) return required;
            required++;
        }
    }

    void virtual Insert(KeyType Key, DataType Val) {
        Iterator<KeyType, DataType> found = Find(Key);
        if (found.HasValue()) found->SetValue(Val);
        else if (!found.HasValue()) {
            int index = GetIndex(Key);
            CheckResize();
            if (!Table[index]) Table[index] = new LinkedHashList<KeyType, DataType>(Key, Val);
            else
            {
                LinkedHashList<KeyType, DataType>* ptr = new LinkedHashList<KeyType, DataType>(Key, Val);
                ptr->SetNext(Table[index]);
                Table[index] = ptr;
            }
            BeingUsed++;
        }
    }

    void Remove(KeyType Key) {
        int index = GetIndex(Key);
        Iterator<KeyType, DataType> found = Find(Key);
        if (!found.HasValue()) return;
        if (found.GetPrev()) found.GetPrev()->SetNext(found->GetNext());
        else *(found.GetArrIterator()) = found->GetNext();
        BeingUsed--;
        delete found.GetCur();
    }

    DataType& operator[](KeyType Key) {
        Iterator<KeyType, DataType> found = Find(Key);
        if (found.HasValue()) {
            return found->GetValueToChange();
        }
        else if (!found.HasValue())
        {
            int index = GetIndex(Key);
            CheckResize();
            if (!Table[index])
            {
                DataType a = NULL;
                Table[index] = new LinkedHashList<KeyType, DataType>(Key, a);
                BeingUsed++;
                return Table[index]->GetValueToChange();
            }
            else {
                DataType a = NULL;
                LinkedHashList<KeyType, DataType>* ptr = new LinkedHashList<KeyType, DataType>(Key, a);
                ptr->SetNext(Table[index]);
                Table[index] = ptr;
                BeingUsed++;
                return Table[index]->GetValueToChange();
            }
        }
    }

    int GetUsed() {
        return BeingUsed;
    }

protected:
    int GetIndex(KeyType key) {
        return HashFunction(key) % TableSize;
    }

    void RehashList(LinkedHashList<KeyType, DataType>* ptr, HashTable<KeyType, DataType>& newTable) {
        LinkedHashList<KeyType, DataType>* delTmp;
        while (ptr) {
            delTmp = ptr;
            newTable.Insert(ptr->GetKey(), ptr->GetValue());
            ptr = ptr->GetNext();
            delete delTmp;
        }
    }

    void CheckResize() {
        if ((double)BeingUsed + 1.0 >= (double)TableSize * FillPercent) {
            HashTable<KeyType, DataType> newHashTable(this->TableSize * 2);
            LinkedHashList<KeyType, DataType>* tmp;
            for (int i = 0; i < TableSize; ++i) {
                if (!Table[i]) continue;
                tmp = Table[i];
                Table[i] = nullptr;
                RehashList(tmp, newHashTable);
            }
            *this = newHashTable;
        }
    }
};

template <typename KeyType, typename DataType>
class MultiHashMap : public HashTable<KeyType, DataType>
{
public:
    MultiHashMap(int Size) :HashTable<KeyType, DataType>(Size) {}
    ~MultiHashMap() {}

    void Insert(KeyType Key, DataType Val) override {
        int index = this->GetIndex(Key);
        this->CheckResize();
        if (!this->Table[index]) this->Table[index] = new LinkedHashList<KeyType, DataType>(Key, Val);
        else
        {
            LinkedHashList<KeyType, DataType>* ptr = new LinkedHashList<KeyType, DataType>(Key, Val);
            ptr->SetNext(this->Table[index]);
            this->Table[index] = ptr;
        }
        this->BeingUsed++;
    }
    void RemoveAll(KeyType Key) {
        while (true)
        {
            int index = this->GetIndex(Key);
            Iterator<KeyType, DataType> found = this->Find(Key);
            if (!found.HasValue()) return;
            if (found.GetPrev()) found.GetPrev()->SetNext(found->GetNext());
            else *(found.GetArrIterator()) = found->GetNext();
            this->BeingUsed--;
            delete found.GetCur();
        }
    }
    vector<DataType> ReturnAll(KeyType Key)
    {
        vector<DataType> toReturn;
        int index = this->GetIndex(Key);
        LinkedHashList<KeyType, DataType>* ptr = this->Table[index];
        while (ptr) {
            if (ptr->GetKey() == Key) {
                toReturn.push_back(ptr->GetValue());
            }
            ptr = ptr->GetNext();
        }
        return toReturn;
    }
    int EnumOfSames(KeyType Key) {
        vector<DataType> vec = ReturnAll(Key);
        return vec.size();
    }
};


template <typename KeyType, typename DataType>
void MainWork() {
    HashTable<KeyType, DataType> hashTable(32);
    int n;
    fin >> n;
    char c;
    KeyType key;
    DataType value;
    for (int i = 0; i < n; i++) {
        fin >> c;
        if (c == 'A') {
            fin >> key >> value;
            hashTable.Insert(key, value);
        }
        else if (c == 'R') {
            fin >> key;
            hashTable.Remove(key);
        }
    }
    HashTable<DataType, int> hashTable1(32);
    for (auto Leafes : hashTable) hashTable1.Insert(Leafes.GetValue(), 0);
    fout << hashTable.GetUsed() << " " << hashTable1.GetUsed();
}


void ChooseFunction(char first, char second)
{
    if (first == 'I') {
        if (second == 'I') MainWork<int, int>();
        if (second == 'D') MainWork<int, double>();
        if (second == 'S') MainWork<int, string>();
    }
    else if (first == 'D') {
        if (second == 'I') MainWork<double, int>();
        if (second == 'D') MainWork<double, double>();
        if (second == 'S') MainWork<double, string>();
    }
    else if (first == 'S') {
        if (second == 'I') MainWork<string, int>();
        if (second == 'D') MainWork<string, double>();
        if (second == 'S') MainWork<string, string>();
    }
}

int main()
{
    char first, second;
    fin >> first >> second;
    ChooseFunction(first, second);
    fin.close();
    fout.close();
    return 0;
}
