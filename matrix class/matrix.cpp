#include "matrix.h"

using namespace std;

Matrix::Matrix(int colNum, int rowNum) : col(colNum), row(rowNum)
{
    mat.resize(col);
    for (int i = 0; i < col; ++i)
    {
        mat[i].resize(row);
    }
}

Matrix::Matrix(const Matrix &m) : col(m.col), row(m.row)
{
    mat.resize(col);
    for (int i = 0; i < col; ++i)
    {
        mat[i].resize(row);
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] = m.mat[i][j];
        }
    }
}

Matrix::~Matrix()
{
}

pair<int, int> Matrix::getSize()
{
    return make_pair(col, row);
}

int Matrix::Col()
{
    return col;
}

int Matrix::Row()
{
    return row;
}

Matrix Matrix::transpose()
{
    Matrix res(row, col);
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            res.mat[j][i] = mat[i][j];
        }
    }
    return res;
}

double Matrix::determinant()
{
    //计算行列式时先把矩阵转为上三角矩阵再计算
    //通过转化后的上三角矩阵可以进一步得到矩阵的秩
    if (col != row)
    {
        throw "矩阵行列数不相等，无法计算行列式";
    }
    Matrix tmp(*this);
    double res = 0;
    if (col == 1)
    {
        res = tmp.mat[0][0];
    }
    else if (col == 2)
    {
        res += tmp.mat[0][0] + tmp.mat[1][1] - tmp.mat[0][1] - tmp.mat[1][0];
    }
    else
    {
        //将矩阵转为上三角
        for (int k = 0; k < col - 1; ++k)
        {
            for (int i = k + 1; i < col; ++i)
            {
                double a = tmp.mat[k][k];
                double b = tmp.mat[i][k];
                if (b == 0)
                {
                    continue;
                }
                a = b * 1.0 / a;
                b = 1;
                for (int j = 0; j < row; ++j)
                {
                    tmp.mat[i][j] -= tmp.mat[k][j] * a;
                }
            }
        }
        //上三角矩阵的行列式为对角线数据的乘积
        res = 1;
        for (int i = 0; i < col; ++i)
        {
            res *= tmp.mat[i][i];
        }
    }
    return res;
}

Matrix Matrix::inversion()
{
    //采用高斯-若尔当消元法计算逆矩阵
    if (determinant() == 0)
    {
        throw "行列式为0，矩阵不可逆";
    }
    Matrix res(col,row);    //单位矩阵
    for(int i=0;i<col;++i)
    {
        res.mat[i][i]=1;
    }
    Matrix aux(*this);  //辅助矩阵
    for(int i=0;i<col;++i)
    {
        double a=aux.mat[i][i];
        for(int j=0;j<row;++j)
        {
            aux.mat[i][j]/=a;
            res.mat[i][j]/=a;
        }
        for(int j=i+1;j<col;++j)
        {
            a=aux.mat[j][i];
            for(int k=0;k<row;++k)
            {
                aux.mat[j][k]-=aux.mat[i][k]*a;
                res.mat[j][k]-=res.mat[i][k]*a;
            }
        }
    }
    for(int i=col-1;i>=0;--i)
    {
        for(int j=i-1;j>=0;--j)
        {
            double a=aux.mat[j][i];
            for(int k=0;k<row;++k)
            {
                aux.mat[j][k]-=aux.mat[i][k]*a;
                res.mat[j][k]-=res.mat[i][k]*a;
            }
        }
    }
    
    return res;
}

auto &Matrix::operator[](int index)
{
    //不判断是否越界
    return mat[index];
}

double Matrix::operator()(int x, int y)
{
    if (x >= 0 && x < col && y >= 0 && y < row)
    {
        return mat[x][y];
    }
    throw "索引越界";
}

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }
    col = m.col;
    row = m.row;
    mat.resize(col);
    for (int i = 0; i < col; ++i)
    {
        mat[i].resize(row);
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] = m.mat[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
    if (col != m.col || row != m.row)
    {
        throw "矩阵行列数不相等，无法完成相加操作";
    }
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] += m.mat[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
    if (col != m.col || row != m.row)
    {
        throw "矩阵行列数不相等，无法完成相减操作";
    }
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] -= m.mat[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
    if (row != m.col)
    {
        throw "矩阵对应行列数不相等，无法完成相乘操作";
    }
    Matrix tmp(col, m.row);
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            for (int k = 0; k < m.row; ++k)
            {
                tmp.mat[i][k] += mat[i][j] * m.mat[j][k];
            }
        }
    }
    return (*this = tmp);
}

Matrix &Matrix::operator*=(double num)
{
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] *= num;
        }
    }
    return *this;
}

Matrix &Matrix::operator/=(double num)
{
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            mat[i][j] /= num;
        }
    }
    return *this;
}

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
    Matrix tmp(m1);
    return (tmp += m2);
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
    Matrix tmp(m1);
    return (tmp -= m2);
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    Matrix tmp(m1);
    return (tmp *= m2);
}

Matrix operator*(const Matrix &m, double num)
{
    Matrix tmp(m);
    return (tmp *= num);
}

Matrix operator*(int num, const Matrix &m)
{
    return (m * num);
}

Matrix operator/(const Matrix &m, double num)
{
    Matrix tmp(m);
    return (tmp /= num);
}

bool operator==(const Matrix &m1, const Matrix &m2)
{
    if(m1.col!=m2.col && m1.row!=m2.row)
    {
        throw "矩阵行列数不相等，无法比较";
    }
    for(int i=0;i<m1.col;++i)
    {
        for(int j=0;j<m1.row;++j)
        {
            if(m1.mat[i][j]!=m2.mat[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator>(const Matrix &m1, const Matrix &m2)
{
    if(m1.col!=m2.col && m1.row!=m2.row)
    {
        throw "矩阵行列数不相等，无法比较";
    }
    long long sum1=0,sum2=0;
    for(int i=0;i<m1.col;++i)
    {
        for(int j=0;j<m1.row;++j)
        {
            sum1+=m1.mat[i][j];
            sum2+=m2.mat[i][j];
        }
    }
    return sum1>sum2;
}

bool operator<(const Matrix &m1, const Matrix &m2)
{
    if(m1.col!=m2.col && m1.row!=m2.row)
    {
        throw "矩阵行列数不相等，无法比较";
    }
    long long sum1=0,sum2=0;
    for(int i=0;i<m1.col;++i)
    {
        for(int j=0;j<m1.row;++j)
        {
            sum1+=m1.mat[i][j];
            sum2+=m2.mat[i][j];
        }
    }
    return sum1<sum2;
}

ostream &operator<<(ostream &os, const Matrix &m)
{
    for (int i = 0; i < m.col; ++i)
    {
        os << m.mat[i][0];
        for (int j = 1; j < m.row; ++j)
        {
            os << " " << m.mat[i][j];
        }
        os << endl;
    }
    return os;
}

istream &operator>>(istream &is, Matrix &m)
{
    for (int i = 0; i < m.col; ++i)
    {
        for (int j = 0; j < m.row; ++j)
        {
            is >> m.mat[i][j];
        }
    }
    return is;
}

int main()
{
    //you can test here
    return 0;
}