#include <iostream>
#include <cmath>

using namespace std;

const double PI = 3.1415926;

//高斯坐标正算
//参数:经度 纬度
//输入的是度数：10°30′30″->10.508333
pair<double, double> BL2XY(double longitude, double latitude)
{
    double x, y;
    //北京54坐标系参数
    double a = 6378245.0;    //长半轴
    double b = 6356863.0188; //短半轴
    // //西安80坐标系参数
    // double a=6378140.0;       //长半轴
    // double b=6356755.2882;  //短半轴

    double e1 = sqrt(a * a - b * b) / a; //第一偏心率
    double e2 = sqrt(a * a - b * b) / b; //第二偏心率

    // int zoneWidth=3;    //3°带
    // int zoneNumber=int((longitude-1.5)/zoneWidth);
    // double L0=3*zoneNumber*PI/180;
    int zoneWidth = 6; //6°带
    int zoneNumber = int(longitude / zoneWidth);
    double L0 = (6 * zoneNumber - 3) * PI / 180;

    double L = longitude * PI / 180 - L0;
    double B = latitude * PI / 180;

    double N = a / sqrt(1 - pow(e1 * sin(B), 2));
    double t = tan(B);
    double n = pow(e2 * cos(B), 2);
    double m = cos(B) * L * PI / 180;

    double m0 = a * (1 - e1 * e1);
    double m2 = 3 / 2 * e1 * e1 * m0;
    double m4 = 5 / 4 * e1 * e1 * m2;
    double m6 = 7 / 6 * e1 * e1 * m4;
    double m8 = 9 / 8 * e1 * e1 * m6;

    double a0 = m0 + m2 / 2 + 3 / 8 * m4 + 5 / 16 * m6 + 35 / 128 * m8;
    double a2 = m2 / 2 + m4 / 2 + 15 / 32 * m6 + 7 / 16 * m8;
    double a4 = m4 / 8 + 3 / 16 * m6 + 7 / 32 * m8;
    double a6 = m6 / 32 + m8 / 16;
    double a8 = m8 / 128;

    double X = a0 * B - a2 * sin(2 * B) / 2 + a4 * sin(4 * B) / 4 - a6 * sin(6 * B) / 6 + a8 * sin(8 * B) / 8;

    x = X + N * t * ((1 / 2 + (1 / 24 * (5 - t * t + 9 * n + 4 * n) + 1 / 720 * (61 - 58 * t * t + t * t * t * t) * m * m) * m * m) * m * m);
    y = 500000 + N * ((1 + (1 / 6 * (1 - t * t + n) + 1 / 120 * (5 - 18 * t * t + t * t * t * t + 14 * n - 58 * n * t * t) * m * m) * m * m) * m);

    // x=1000000*zoneNumber+500000+x;
    // y=1000000*zoneNumber+500000+y;

    return make_pair(x, y);
}

pair<double, double> XY2BL(double X, double Y, double L0)
{
    double B;
    double L;

    //北京54坐标系参数
    double a = 6378245.0;    //长半轴
    double b = 6356863.0188; //短半轴
    // //西安80坐标系参数
    // double a=6378140.0;       //长半轴
    // double b=6356755.2882;  //短半轴

    double e1 = sqrt(a * a - b * b) / a; //第一偏心率
    double e2 = sqrt(a * a - b * b) / b; //第二偏心率
    double ee = e1 * e1 / (1 - e1 * e1);
    
    int zoneWidth = 6; //6°带
    int zoneNumber = int(X / 1000000);
    L0 = L0 * PI / 180;
    double X0 = zoneNumber * 1000000 + 500000;
    double Y0 = 0;
    double x = X - X0;
    double y = Y - Y0;

    double M = y;
    double u = M / (a * (1 - e1 * e1 / 4 - 3 * e1 * e1 / 64 - 5 * pow(e1, 6) / 256));
    double Bf = u + (3 * ee / 2 - 27 * ee * ee * ee / 32) * sin(2 * u) + (21 * ee * ee / 16 - 55 * ee * ee * ee * ee / 32) * sin(4 * u) + (151 * ee * ee * ee / 96) * sin(6 * u) + (1097 * ee * ee * ee * ee / 512) * sin(8 * u);

    double C = e2 * cos(Bf) * cos(Bf);
    double T = tan(Bf) * tan(Bf);
    double NN = a / sqrt(1.0 - e1 * sin(Bf) * sin(Bf));
    double R = a * (1 - e1) / sqrt((1 - e1 * sin(Bf) * sin(Bf)) * (1 - e1 * sin(Bf) * sin(Bf)) * (1 - e1 * sin(Bf) * sin(Bf)));
    double D = x / NN;

    L = L0 + (D - (1 + 2 * T + C) * D * D * D / 6 + (5 - 2 * C + 28 * T - 3 * C * C + 8 * ee + 24 * T * T) * D * D * D * D * D / 120) / cos(Bf);
    B = Bf - (NN * tan(Bf) / R) * (D * D / 2 - (5 + 3 * T + 10 * C - 4 * C * C - 9 * ee) * D * D * D * D / 24 + (61 + 90 * T + 298 * C + 45 * T * T - 256 * ee - 3 * C * C) * D * D * D * D * D * D / 720);

    L = L / PI * 180;
    B = B / PI * 180;

    return make_pair(L, B);
}

int main()
{
    auto pair=XY2BL(434760.7611718801, 4438512.040474475, 117.0);
    // auto pair = BL2XY(114.333, 30.500);
    cout << pair.first << " " << pair.second << endl;

    return 0;
}