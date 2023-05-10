#include "graphPrinter.cpp"
using namespace std;

int main()
{
    int v = 6;
    Graph myGraph1(v);
    myGraph1.member_init();
    myGraph1.creatForbiddenArea();//生成定位识别区
    myGraph1.dataGet();//输入字符串，是u8格式的输入（无前后缀）
    myGraph1.insertDat(data);//将data转变为含前后缀的u8格式输入datBits
    myGraph1.insertEC();//利用datCode生成u8格式的纠错码ecCode
    myGraph1.datCodeToBits();
    myGraph1.ecCodeToBits();//将ecInbyte转为二进制格式
    myGraph1.dataWriter();//将二进制码填入图形中
    myGraph1.addFormat();//将格式码嵌入图形中
    myGraph1.Mask(3);//进行掩码操作
    myGraph1.showGraph();//展示最终结果
    return 0;
}
