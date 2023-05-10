#include "graphPrinter.h"
using namespace std;
void Graph::turn(int i,int j)
{
    if(graph[i][j]==true) graph[i][j]=false;
    else graph[i][j]=true;
}

void Graph::dataGet()
{
    printf("Input the information to convert : ");
    scanf("%s",data);
}

void Graph::insertDat(u8 *data)
{
    cout<<"start insertDat"<<endl;
	int datlen=0;
	//��������
	while(*data){//data��ָ�����ݵ��˽�����
		siginificantData[datlen]=*data;
		data++;
		datlen++;
	}
	numOfByte=datlen;//���ݳ���
	numOfPaddingBytes=MaxDataNum-numOfByte-2;
	//�����е�������������������
	//ģʽ+�ֽ��� 	����λ+����λ   >>���ȼ����ڡ�+���ȼ�;
	DataCodewords[0]= mode+(numOfByte>>4);
	DataCodewords[1]=(numOfByte<<4)+(siginificantData[0]>>4);
	//����+������־��+������
	for(int i=0;i<numOfByte-1;i++){
		DataCodewords[i+2]=(siginificantData[i]<<4)+(siginificantData[i+1]>>4);
	}
	DataCodewords[numOfByte+1]=siginificantData[numOfByte-1]<<4;
	//���벹���ֽ�
	for(int i=1;i<=numOfPaddingBytes;i++){
		if(i%2){//������
			DataCodewords[numOfByte+1+i]=PaddingByte1;
		}else{
			DataCodewords[numOfByte+1+i]=PaddingByte2;
		}
	}
	cout<<"end insertDat"<<endl;
}

void Graph::member_init(){
	int _operator=1;
	member_negation[0] = -1;
	for(int i=0;i<256;i++){
		member_position[i]=_operator;
		member_negation[_operator]=i;
		_operator<<=1;
		if(_operator>255){
			_operator^=285;
		}
	}
	member_negation[1]=0;//��һ����_operatorȡ������1
}

int* Graph::getGeneratorPolynomial(int length){//lengthΪa�Ĵ���
    cout<<"start generator "<<endl;
	int *member_generator_polynomial =(int *)calloc(2,sizeof(int));
	*member_generator_polynomial=0,*(member_generator_polynomial+1)=0;
	for(int temp=2;temp<length+1;temp++){//ѭ���������ڼ�������ʽ���
		int* temp_polynomial=(int *)calloc(temp,sizeof(int));
		int* result_polynomial=(int *)calloc(temp+1,sizeof(int));
		for(int i=0;i<temp;i++){//a��ָ����˵Ľ��
			*(temp_polynomial+i)=*(member_generator_polynomial+i)+temp-1;
			if (temp_polynomial[i] > 255){
				temp_polynomial[i] %= 255;
			}
		}
		for(int i=0;i<temp+1;i++){//x��˵Ľ��������a��˵Ľ�����
			if(i==0){
				*(result_polynomial+i)=0;
			}else if(temp==i){
				*(result_polynomial+i)=*(temp_polynomial+i-1);
			}else{
				int first=member_position[*(temp_polynomial+i-1)];//����a��ָ��
				int last=member_position[*(member_generator_polynomial+i)];//����x
				*(result_polynomial+i)=member_negation[first^last];
			}
		}
		free(temp_polynomial);
		free(member_generator_polynomial);
		member_generator_polynomial=(int *)calloc(temp+1,sizeof(int));
		for(int i=0;i<temp+1;i++){
			*(member_generator_polynomial+i)=*(result_polynomial+i);
		}
		free(result_polynomial);
	}
	cout<<"end generator "<<endl;
	return member_generator_polynomial;
}

int* Graph::getEcPolynomial(u8* dat_codewords,int dat_len,int numOfec)
{
    cout<<"start getEcPolynomial"<<endl;
	int maxlen=(dat_len>numOfec+1?dat_len:numOfec+1)+numOfec;
	int* ec_polynomial=(int*)calloc(numOfec,sizeof(int));
	int* generator_polynomial=getGeneratorPolynomial(numOfec);
	int* ec_codewords_temp=(int*)calloc(maxlen,sizeof(int));
	for(int i=0;i<maxlen;i++){//�������ݵ�ϵ������Ϊ�㣬Ϊ����ֵΪ1�ˣ����������ɶ���ʽ��ϵ�����������ı�����
		ec_codewords_temp[i]=-1;
	}
	for(int i=0;i<dat_len;i++){
		ec_codewords_temp[i]=member_negation[dat_codewords[i]];
	}
	for(int temp=0;temp<dat_len;temp++){//����numofex�γ���
		int differ=ec_codewords_temp[temp];//��Ϣ����ʽ����������ɶ���ʽ
		int generator_negation;//���ɶ���ʽ��ϵ��������Ϣ����ʽ����ϵ��
		for(int i=0;i<numOfec+1;i++){//���ɶ���ʽҪ���еĲ���
			generator_negation=generator_polynomial[i]+differ;
			if(generator_negation>255){
				generator_negation%=255;
			}
			int value=member_position[generator_negation]^member_position[ec_codewords_temp[i+temp]];
			ec_codewords_temp[i+temp]=member_negation[value];
		}
	}
	for(int i=0;i<numOfec;i++){
		ec_polynomial[i]=ec_codewords_temp[i+dat_len];
	}
	free(generator_polynomial);
	free(ec_codewords_temp);
	cout<<"end getEcPolynomial"<<endl;
	return ec_polynomial;
}

u8 Graph::negationToValue(int negation)
{
	return (u8)member_position[negation];
}

void Graph::insertEC()
{
    cout<<"start insertEC"<<endl;
	int lenOfDat=MaxDataNum/4;
	int lenOfEc=CorCodeNum/4;
	for(int i=0;i<4;i++){
		int* ecCodes=getEcPolynomial(&DataCodewords[i*lenOfDat],lenOfDat,lenOfEc);
		for(int j=0;j<lenOfEc;j++){
			ECorCodewords[i*lenOfEc+j]=negationToValue(ecCodes[j]);//��2��ָ����Ϊֵ
		}
		free(ecCodes);
	}
	cout<<"end insertEC"<<endl;
}

bool Graph::isForbiddenArea(int i,int j)
{
    if((i<=8&&j<=8)||(i<=8&&j>=y-8)||(i>=x-8&&j<=8)) return true;
    if(i==6||j==6) return true;
    if(ver!=1)
    {
        if(i>=x-9&&i<=x-5&&j>=y-9&&j<=y-5) return true;
    }
    return false;
}

void Graph::creatForbiddenArea()
{
    for(int i=0;i<7;i++)
    {
        graph[i][0] = true;
        graph[i][y-1] = true;
        graph[i][y-7] = true;
        graph[i][6] = true;
        graph[x-1-i][0] = true;
        graph[x-1-i][6] = true;
    }
    for(int j=0;j<7;j++)
    {
        graph[0][j] = true;
        graph[x-1][j] = true;
        graph[x-7][j] = true;
        graph[6][j] = true;
        graph[0][y-1-j] = true;
        graph[6][y-1-j] = true;
    }
    for(int i=2;i<=4;i++)
    {
        for(int j=2;j<=4;j++)
        {
            graph[i][j] = true;
            graph[i][y-1-j] = true;
            graph[x-1-i][j] = true;
        }
    }
    for(int i=7;i<x-7;i++)
    {
        if(i%2==1)
        {
            graph[i][6]=false;
            graph[6][i]=false;
        }
        else
        {
            graph[i][6]=true;
            graph[6][i]=true;
        }
    }
    graph[x-8][8] = true;
    if(ver!=1)
    {
        for(int i=x-9;i<x-4;i++)
        {
            graph[x-9][i] = true;
            graph[x-5][i] = true;
            graph[i][y-9] = true;
            graph[i][y-5] = true;
        }
        graph[x-7][y-7] = true;
    }
}

void Graph::datCodeToBits(){
	int position=0;
	cout<<"start datCodeBits ";
	for(int i=0;i<27;i++){
		for(int one=0;one<4;one++){
			u8 dat=DataCodewords[i+one*27];
			for(int j=0;j<8;j++){
				if(dat&(1<<7-j)){
					datBits[position++]=1;
					cout<<"1";
				}else{
					datBits[position++]=0;
					cout<<"0";
				}
			}
		}
	}
	cout<<" end datCodeBits ";
}

void Graph::ecCodeToBits()
{
    cout<<"start ecCodeToBits"<<endl;
	int position=0;
	for(int i=0;i<16;i++){
		for(int one=0;one<4;one++){
			u8 ec=ECorCodewords[i+one*16];
			for(int j=0;j<8;j++){
				if(ec&(1<<7-j)){
					ecBits[position++]=1;
					cout<<'1';
				}else{
					ecBits[position++]=0;
					cout<<'0';
				}
			}
		}
	}
	cout<<" end ecCodeToBits"<<endl;
}

void Graph::dataWriter()
{
    int process=0;
    int property=1;
    int upordown=1;
    int cood[2] = {40,40};
    while(process<1376)
    {
        if(isForbiddenArea(cood[0],cood[1])==false)//��ǰ�ǽ�����������������г�
        {
            if(process<864&&datBits[process]==1) graph[cood[0]][cood[1]]=true;
            else if(process>=864&&ecBits[process-864]==1) graph[cood[0]][cood[1]]=true;
            process++;
        }
        //�����ǲ����ǲ��ǽ�����Ҫ����·������
        if((property==0)&&((upordown==1&&cood[0]!=0)||(upordown==-1&&cood[0]!=40)))//���漰��Ե����������
        {
            if(property==1)
            {
                cood[1]--;
                property=0;
            }
            if(property==0)
            {
                if(upordown==1)
                {
                    cood[0]--;cood[1]++;
                }
                if(upordown==-1)
                {
                    cood[0]++;cood[1]++;
                }
                property=1;
            }
        }
        else if(property==1)//���в��漰���е��ж����������У����Ƿ��ڱ�Ե�޹�
        {
            cood[1]--;
            property=0;
        }
        else//���ֱ�ԵӰ����Ҫ��ͷ
        {
            cood[1]--;
            property=1;
            upordown*=-1;
        }
    }

}

void Graph::addFormat()
{
    graph[40][8]=1;graph[8][0]=1;
    graph[39][8]=0;graph[8][1]=0;
    graph[38][8]=1;graph[8][2]=1;
    graph[37][8]=1;graph[8][3]=1;
    graph[36][8]=0;graph[8][4]=0;
    graph[35][8]=1;graph[8][5]=1;
    graph[34][8]=1;graph[8][7]=1;
    graph[8][8]=0;graph[8][8]=0;
    graph[7][8]=1;graph[8][34]=1;
    graph[5][8]=0;graph[8][35]=0;
    graph[4][8]=0;graph[8][36]=0;
    graph[3][8]=1;graph[8][37]=1;
    graph[2][8]=0;graph[8][38]=0;
    graph[1][8]=1;graph[8][39]=1;
    graph[0][8]=1;graph[8][40]=1;
//    graph[40][8]=1;graph[8][0]=1;
//    graph[39][8]=0;graph[8][1]=0;
//    graph[38][8]=0;graph[8][2]=0;
//    graph[37][8]=0;graph[8][3]=0;
//    graph[36][8]=0;graph[8][4]=0;
//    graph[35][8]=0;graph[8][5]=0;
//    graph[34][8]=0;graph[8][7]=0;
//    graph[8][8]=1;graph[8][33]=1;
//    graph[7][8]=1;graph[8][34]=1;
//    graph[5][8]=0;graph[8][35]=0;
//    graph[4][8]=0;graph[8][36]=0;
//    graph[3][8]=1;graph[8][37]=1;
//    graph[2][8]=1;graph[8][38]=1;
//    graph[1][8]=1;graph[8][39]=1;
//    graph[0][8]=0;graph[8][40]=0;
}

void Graph::Mask(int m)
{
    if(m==0)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if((i+j)%2==0) turn(i,j);
                }
            }
        }
    }
    else if(m==1)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if(i%2==0) turn(i,j);
                }
            }
        }
    }
    else if(m==2)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if(j%3==0) turn(i,j);
                }
            }
        }
    }
    else if(m==3)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if((i+j)%3==0) turn(i,j);
                }
            }
        }
    }
    else if(m==4)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if((i/2+j/3)%2==0) turn(i,j);
                }
            }
        }
    }
    else if(m==5)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if((i*j)%2+(i*j)%3==0) turn(i,j);
                }
            }
        }
    }
    else if(m==6)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if(((i*j)%2+(i*j)%3)%2==0) turn(i,j);
                }
            }
        }
    }
    else if(m==7)
    {
        for(int i=0;i<x;i++)
        {
            for(int j=0;j<y;j++)
            {
                if(isForbiddenArea(i,j)==false)
                {
                    if(((i+j)%2+(i*j)%3)%2==0) turn(i,j);
                }
            }
        }
    }
}

void Graph::showGraph()
{
    for(int i=0;i<x;i++)
    {
        for(int j=0;j<y;j++)
        {
            if(graph[i][j]==true) cout<<"\u2588 ";
            else cout<<"  ";
            if(j==y-1) cout<<""<<endl;
        }
    }
}
