#include <vector>
#include <iostream>

using namespace std;

class Matrix
{
public:
    Matrix() = delete;
    Matrix(int, int);
    Matrix(const Matrix &);
    ~Matrix();

    pair<int, int> getSize();
    int Col();
    int Row();
    Matrix transpose();   //转置
    double determinant(); //行列式
    Matrix inversion();   //求逆

    double &operator()(int x, int y);
    Matrix &operator=(const Matrix &);
    Matrix &operator+=(const Matrix &);
    Matrix &operator-=(const Matrix &);
    Matrix &operator*=(const Matrix &);
    Matrix &operator*=(double);
    Matrix &operator/=(double);

    friend Matrix operator+(const Matrix &, const Matrix &);
    friend Matrix operator-(const Matrix &, const Matrix &);
    friend Matrix operator*(const Matrix &, const Matrix &);
    friend Matrix operator*(const Matrix &, double);
    friend Matrix operator*(double, const Matrix &);
    friend Matrix operator/(const Matrix &, int);
    friend bool operator==(const Matrix &, const Matrix &);
    friend bool operator>(const Matrix &, const Matrix &);
    friend bool operator<(const Matrix &, const Matrix &);

    friend ostream &operator<<(ostream &, const Matrix &);
    friend istream &operator>>(istream &, Matrix &);

private:
    int col;
    int row;
    vector<vector<double>> mat;
};