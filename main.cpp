#include "graphPrinter.cpp"
using namespace std;

int main()
{
    int v = 6;
    Graph myGraph1(v);
    myGraph1.member_init();
    myGraph1.creatForbiddenArea();//���ɶ�λʶ����
    myGraph1.dataGet();//�����ַ�������u8��ʽ�����루��ǰ��׺��
    myGraph1.insertDat(data);//��dataת��Ϊ��ǰ��׺��u8��ʽ����datBits
    myGraph1.insertEC();//����datCode����u8��ʽ�ľ�����ecCode
    myGraph1.datCodeToBits();
    myGraph1.ecCodeToBits();//��ecInbyteתΪ�����Ƹ�ʽ
    myGraph1.dataWriter();//��������������ͼ����
    myGraph1.addFormat();//����ʽ��Ƕ��ͼ����
    myGraph1.Mask(3);//�����������
    myGraph1.showGraph();//չʾ���ս��
    return 0;
}
