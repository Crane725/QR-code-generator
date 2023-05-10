#ifndef GRAPHPRINTER_H_INCLUDED
#define GRAPHPRINTER_H_INCLUDED
#include <iostream>
#include "stdio.h"
#include "stdlib.h"

typedef unsigned char u8;

#define Version 6
#define ECLevel 2  //����ȼ�ΪM
#define MaxDataNum 27*4
#define CorCodeNum 16*4
#define ReminderBits 7	//ʣ��λ7λ
using namespace std;

    int  member_position[256]={0};//ֵ
    int  member_negation[256]={0};//ϵ��
    u8 data[MaxDataNum-2]={0};
    u8 siginificantData[MaxDataNum-2]={0};//��Ч����
    u8 DataCodewords[MaxDataNum];//ģʽ+�ֽ���+��Ч����+������ʶ��+����0+�����ֽ�
    u8 ECorCodewords[CorCodeNum];
    u8 mode=0x40;//����λ��Ч
    u8 numOfByte;//�ֽ�����
    u8 PaddingByte1=236;//11101100 00010001
    u8 PaddingByte2=17;
    u8 end_indicator=0;//����λ��Ч
    u8 replenishZero=0;//�����㣬Ĭ��û��
    u8 numOfPaddingBytes;//�����ֽ���Ŀ
    char picture[41][41]={0};//��ά��ͼ��
    char datBits[4*27*8]={0};//�����������
    char ecBits[4*16*8]={0};//�����������

class Graph
{
private:
    bool **graph;
    int x;
    int y;
    int ver;
public:
    Graph(int v)
    {
        ver = v;
        x=17+4*v;y=17+4*v;
        graph = new bool*[x];
        for(int i=0;i<x;i++)
        {
            graph[i] = new bool[y];
        }
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                graph[i][j]=false;
            }
        }
    }
    ~Graph()
    {
        for(int i=0;i<x;i++)
        {
            delete []graph[i];
        }
        delete []graph;
    }
    void dataGet();
    int* getGeneratorPolynomial(int length);
    int* getEcPolynomial(u8* dat_codewords,int dat_len,int numOfec);
    u8 negationToValue(int negation);
    void datCodeToBits();
    void ecCodeToBits();
    void member_init();
    void insertEC();
    void insertDat(u8 *data);
    void dataWriter();
    void turn(int i,int j);
    void addFormat();
    void Mask(int m);
    bool isForbiddenArea(int i,int j);
    void creatForbiddenArea();
    void showGraph();
};


#endif // GRAPHPRINTER_H_INCLUDED
