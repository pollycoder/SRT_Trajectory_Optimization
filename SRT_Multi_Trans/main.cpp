//
//  main.cpp
//  Lambert (self-trans)
//
//  Created by 周懿 on 2022/1/14.
//

#include <iostream>
#include <cmath>

#include "Constant.h"
#include "Ephemeris.h"
#include "Lambert.hpp"
#include "PSO_DE_NLOPT.hpp"

using namespace std;
//注意：兰伯特求解器的输入为两次位置矢量，输出为轨道的半长轴、离心率和两个速度


//规定变轨次数
const int TransNum=3;

//设置星历
string dir_Earth("/Users/polly/Desktop/SRT/Ephemeris/Earth_1.txt");
string dir_Mars("/Users/polly/Desktop/SRT/Ephemeris/Mars_1.txt");

//题目规定数据
const int five_years=5*365*24*3600;
ephemeris Earth(dir_Earth,24*3600);//读取地球星历得到地球的位置和速度
ephemeris Mars(dir_Mars,24*3600);//读取火星星历得到火星的位置和速度
const double Au = 149597870700;//日地距离

//定义所需变量
double **r;//通过二级索引定义位矢
double ***v;//通过三级索引定义每次变轨前后的速度矢量
double total_t;//定义总时间
double *delta_t;//定义每次变轨所用的时间
double *a;//使用一级索引定义每次变轨行进轨道的半长轴
double *e;//使用一级索引定义每次变轨行进轨道的离心率


//设置一个初始化所有变量的函数
void init()
{
    int i,j;
    r=new double*[TransNum+1];
    v=new double**[TransNum+1];
    delta_t=new double[TransNum];
    a=new double[TransNum];
    e=new double[TransNum];
    
    for(i=0;i<TransNum+1;i++)
    {
        r[i]=new double[3];
        v[i]=new double*[2];
        for(j=0;j<2;j++)
        {
            v[i][j]=new double[3];
        }
    }
}


//设置一个求速度变量的函数
double delta_velocity(double ***v)
{
    double result=0;
    int turn,i;
    for(turn=0;turn<TransNum+1;turn++)
    {
        double temp=0;
        for(i=0;i<3;i++)
        {
            temp+=pow(v[turn][0][i]-v[turn][1][i],2);
        }
        result+=sqrt(temp);
    }
    return result;
}


//构建一个通用求解器
void LpSolver(const std::vector<double>& X)
{
    int i;
    //得到出发时地球的位置和速度，前者用于固定一个出发点，后者用于计算出发时的速度变量
    Earth.get_position(X[0]*five_years);
    Earth.get_velocity(X[0]*five_years);
    //求出变轨总时间
    total_t=0;
    for(i=0;i<TransNum+1;i++)//X[0]*五年是出发时的时刻（因为不是t=0时立刻出发),这里消耗了X0,X1,X2,X3,X4
    {
        total_t+=X[i]*five_years;
    }
    //得到到达时火星的位置和速度，前者用于固定一个到达点，后者用于计算到达时的速度变量
    Mars.get_position(total_t);
    Mars.get_velocity(total_t);
    //初始化出发点和出发加速前速度
    r[0][0]=Earth.r[0];
    r[0][1]=Earth.r[1];
    r[0][2]=Earth.r[2];
    v[0][0][0]=Earth.v[0];
    v[0][0][1]=Earth.v[1];
    v[0][0][2]=Earth.v[2];
    //得到终点和终点的末速度
    r[TransNum][0]=Mars.r[0];
    r[TransNum][1]=Mars.r[1];
    r[TransNum][2]=Mars.r[2];
    v[TransNum][1][0]=Mars.v[0];
    v[TransNum][1][1]=Mars.v[1];
    v[TransNum][1][2]=Mars.v[2];
   
    if(TransNum>1)
    {
        for(i=1;i<TransNum;i++)//这里消耗了X5,X6,X7,X8,X9,X10,X11,X12,X13
        {
            r[i][0]=X[TransNum-2+3*i]*1.6*Au;
            r[i][1]=X[TransNum-2+3*i+1]*1.6*Au;
            r[i][2]=X[TransNum-2+3*i+2]*1.6*Au;
        }
    }
    
    
    
    //接入lambert求解器,遍历求解
    for(i=0;i<TransNum;i++)
    {
        delta_t[i]=X[i+1]*five_years;
        double *v1 = v[i][1];
        double *v2 = v[i+1][0];
        const double *R1 = r[i];
        const double *R2 = r[i+1];
        int flag=1;
        int way=0; int N = 0; int branch = 0; int Maxiter=60; double tol= 1.0e-11;
        const double unith[3] = {0,0,1};
        lambert(&v1[0],&v2[0], a[i], e[i],&R1[0], &R2[0],delta_t[i], &unith[0], flag, mu,  way, N,  branch, Maxiter, tol );
        
    }
    
}


double obj_func(const std::vector<double>& X, std::vector<double>& grad, void* f_data)
{
    double* para = (double*)f_data; //类型强制转换
    //利用惩罚函数限制在五年之内
    int i;
    double sum=0;
    for(i=0;i<TransNum+1;i++)
    {
        sum+=X[i];
    }
    if(sum>1)
    {
        return 1E20*pow(sum,2);
    }
    
    LpSolver(X);
    
    
    
    double obj =delta_velocity(v);
    
    return obj;
}


int main()
{
    std::vector<double> X = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    double f=1000.0;
    init();
    PSO(obj_func,nullptr, X, f, 4*TransNum-2 ,1000,10000,1000);
    for(int i=0;i<TransNum;i++)
    {
        for(int j=0;j<2;j++)
        {
            for(int k=0;k<3;k++)
            {
                cout<<v[i][j][k]<<endl;
            }
        }
    }
    return 0;
}
