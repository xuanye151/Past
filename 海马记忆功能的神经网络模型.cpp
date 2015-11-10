//#include<iostream.h>
#include<math.h>
#include <stdlib.h>          
#include <conio.h>     
#include <time.h>  
#include<fstream.h>
#include<string.h>


#define num_A     49
#define num_B     49
#define num_C     49
#define num_CA1   32
#define num_CA3   32

double ka=0.05;
double kb=0.05;
double k1=0.03;
double k3=0.03; 
double K=1;
double kp=0.1;

double A[num_A]={
	1,0,0,0,0,0,0,
    0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
};
double B[num_B]={
	0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
};
double C[num_C];
double CA1[num_CA1];
double CA3[num_CA3];

double weight_AC[num_A][num_C];
double weight_BC[num_B][num_C];
double weight_A3[num_A][num_CA3];
double weight_B3[num_B][num_CA3];
double weight_33[num_CA3][num_CA3];
double weight_31[num_CA3][num_CA1];
double weight_1C[num_CA1][num_C];
double weight_1A[num_CA1][num_A];
double weight_1B[num_CA1][num_B];

double threshold_CA3[num_CA3];
double threshold_33[num_CA3];
double threshold_31[num_CA1];
double threshold_C[num_C];
double threshold_A[num_A];
double threshold_B[num_B];

//---------------------------------------------------------------------------------//
double function(double u)
{
	double f;
	if(u>=0)  f=1;
	if(u<0)   f=0;
	return f;
}
/*
double function(double u)     //【功能函数】
{

	double a=60;     //曲线平滑度
	double f;
	double x;
	double b;
	x=-u/a;
    b=exp(x);
	f=1/(1+b);
	return f;
}
*/
int ini()    //【初始化】
{
	for(int i=0;i<num_CA3;i++)      threshold_CA3[i]=1;
	for(int ia=0;ia<num_CA3;ia++)   threshold_33[ia]=1;
    for(int ib=0;ib<num_CA1;ib++)   threshold_31[ib]=1;
	for(int j=0;j<num_C;j++)        threshold_C[j]=1;
	for(int k=0;k<num_A;k++)        threshold_A[k]=1;
	for(int x=0;x<num_B;x++)        threshold_B[x]=1;
	return 0;
}
//--------------------------------------------------------------------------------//
int output_CA3()  //【CA3区输出】
{
	double sum=0;
	for(int i=0;i<num_CA3;i++)      //接收A区输入
	{
		for(int j=0;j<num_A;j++)
		{
			sum+=A[j]*weight_A3[j][i];
		}
        CA3[i]=sum;
		sum=0;
	}

    for(int ia=0;ia<num_CA3;ia++)   //接收B区输入并结合进入功能函数
	{
		for(int ja=0;ja<num_B;ja++)
		{
			sum+=B[ja]*weight_B3[ja][ia];
		}
        CA3[ia]+=sum;
		CA3[ia]=function(CA3[ia]-threshold_CA3[ia]);
		sum=0;
	}
	 
	for(int ib=0;ib<num_CA3;ib++)   //CA3区自联想
	{
		for(int jb=0;jb<num_CA3;jb++)
		{
			sum+=CA3[jb]*weight_33[ib][jb];
		}
		sum=sum-threshold_33[ib];
		CA3[ib]=function(sum);
		sum=0;
	}
	return 0;
}

int output_CA1()   //【CA1区输出】
{
	double sum=0;
	for(int i=0;i<num_CA1;i++)
	{
		for(int j=0;j<num_CA3;j++)
		{
			sum+=CA3[j]*weight_31[j][i];
		}
		CA1[i]=function(sum-threshold_31[i]);
		sum=0;
    }
	return 0;
}

int output_C()   //【C区输出】
{
	double sum=0;
	for(int i=0;i<num_C;i++)
	{
		for(int j=0;j<num_A;j++)
		{
			sum+=A[j]*weight_AC[j][i];
		}
		for(int k=0;k<num_B;k++)
		{
			sum+=B[k]*weight_BC[k][i];
		}
		for(int x=0;x<num_CA1;x++)
		{
			sum+=CA1[x]*weight_1C[x][i];
		}
		C[i]=function(sum-threshold_C[i]);
		sum=0;
	}
	return 0;
}

int output_A()   //【A区输出】
{
	double sum=0;
	for(int i=0;i<num_A;i++)
	{
		for(int j=0;j<num_C;j++)
		{
			sum+=C[j]*weight_AC[i][j];
		}
		for(int k=0;k<num_CA1;k++)
		{
			sum+=CA1[k]*weight_1A[k][i];
		}
		A[i]=function(sum-threshold_A[i]);
		sum=0;
	}
	return 0;
}

int output_B()   //【B区输出】
{
	double sum=0;
	for(int i=0;i<num_B;i++)
	{
		for(int j=0;j<num_C;j++)
		{
			sum+=C[j]*weight_BC[i][j];
		}
		for(int k=0;k<num_CA1;k++)
		{
			sum+=CA1[k]*weight_1B[k][i];
		}
		B[i]=function(sum-threshold_B[i]);
		sum=0;
	}
	return 0;
}


//-------------------------------------------------------------------------------------//
int update_ac()
{
	for(int i=0;i<num_A;i++)
	{
		for(int j=0;j<num_C;j++)
		{
			weight_AC[i][j]+=ka*A[i]*C[j];
		}
	}
	return 0;
}

int update_bc()
{
	for(int i=0;i<num_B;i++)
	{
		for(int j=0;j<num_C;j++)
		{
			weight_BC[i][j]+=kb*B[i]*C[j];
		}
	}
	return 0;
}

int update_a3()
{
	for(int i=0;i<num_A;i++)
	{
		for(int j=0;j<num_CA3;j++)
		{
			weight_A3[i][j]+=K*A[i]*CA3[j]-kp*A[i]-k3;
		}
	}
	return 0;
}

int update_b3()
{
	for(int i=0;i<num_B;i++)
	{
		for(int j=0;j<num_CA3;j++)
		{
			weight_B3[i][j]+=K*B[i]*CA3[j]-kp*B[i]-k3;
		}
	}
	return 0;
}

int update_33()
{
	for(int i=0;i<num_CA3;i++)
	{
		for(int j=0;j<num_CA3;j++)
		{
			weight_33[i][j]+=K*CA3[i]*CA3[j]-k3;
		}
	}
	return 0;
}

int update_31()
{
	for(int i=0;i<num_CA3;i++)
	{
		for(int j=0;j<num_CA1;j++)
		{
			weight_31[i][j]+=K*CA3[i]*CA1[j]-kp*CA3[i]-k1;
		}
	}
	return 0;
}

int update_1c()
{
	for(int i=0;i<num_CA1;i++)
	{
		for(int j=0;j<num_C;j++)
		{
			weight_1C[i][j]+=K*CA1[i]*C[j]-k1;
		}
	}
	return 0;
}

int update_1a()
{
	for(int i=0;i<num_CA1;i++)
	{
		for(int j=0;j<num_A;j++)
		{
			weight_1A[i][j]+=K*CA1[i]*A[j]-k1;
		}
	}
	return 0;
}

int update_1b()
{
	for(int i=0;i<num_CA1;i++)
	{
		for(int j=0;j<num_B;j++)
		{
			weight_1B[i][j]+=K*CA1[i]*B[j]-k1;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------------------//

int save_weight()  //【二进制保存权重】
{
	cout<<"保存权重中....."<<endl;

	ofstream outf("d:\\Weight权重库\\海马记忆模型权重\\weight_AC.txt",ios::binary);
	if(!outf)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outf.write((char*)&weight_AC,sizeof(weight_AC));
	outf.close();

	ofstream outfs("d:\\Weight权重库\\海马记忆模型权重\\weight_BC.txt",ios::binary);
	if(!outfs)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfs.write((char*)&weight_BC,sizeof(weight_BC));
	outfs.close();

	ofstream outfa("d:\\Weight权重库\\海马记忆模型权重\\weight_A3.txt",ios::binary);
	if(!outfa)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfa.write((char*)&weight_A3,sizeof(weight_A3));
	outfa.close();

	ofstream outfb("d:\\Weight权重库\\海马记忆模型权重\\weight_B3.txt",ios::binary);
	if(!outfb)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfb.write((char*)&weight_B3,sizeof(weight_B3));
	outfb.close();

	ofstream outfc("d:\\Weight权重库\\海马记忆模型权重\\weight_33.txt",ios::binary);
	if(!outfc)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfc.write((char*)&weight_33,sizeof(weight_33));
	outfc.close();

    ofstream outfd("d:\\Weight权重库\\海马记忆模型权重\\weight_31.txt",ios::binary);
	if(!outfd)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfd.write((char*)&weight_31,sizeof(weight_31));
	outfd.close();

	ofstream outfe("d:\\Weight权重库\\海马记忆模型权重\\weight_1C.txt",ios::binary);
	if(!outfe)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfe.write((char*)&weight_1C,sizeof(weight_1C));
	outfe.close();

	ofstream outff("d:\\Weight权重库\\海马记忆模型权重\\weight_1A.txt",ios::binary);
	if(!outff)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outff.write((char*)&weight_1A,sizeof(weight_1A));
	outff.close();

	ofstream outfg("d:\\Weight权重库\\海马记忆模型权重\\weight_1B.txt",ios::binary);
	if(!outfg)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	outfg.write((char*)&weight_1B,sizeof(weight_1B));
	outfg.close();

	cout<<"保存完毕!"<<endl;
	return 0;
}

int read_weight()  //【二进制读取权重】
{
	cout<<"获取权重中......."<<endl;

	ifstream inf("d:\\Weight权重库\\海马记忆模型权重\\weight_AC.txt",ios::binary);
	if(!inf)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	inf.read((char*)&weight_AC,sizeof(weight_AC));
	inf.close();

	ifstream infs("d:\\Weight权重库\\海马记忆模型权重\\weight_BC.txt",ios::binary);
	if(!infs)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infs.read((char*)&weight_BC,sizeof(weight_BC));
	infs.close();

	ifstream infa("d:\\Weight权重库\\海马记忆模型权重\\weight_A3.txt",ios::binary);
	if(!infa)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infa.read((char*)&weight_A3,sizeof(weight_A3));
	infa.close();

	ifstream infb("d:\\Weight权重库\\海马记忆模型权重\\weight_B3.txt",ios::binary);
	if(!infb)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infb.read((char*)&weight_B3,sizeof(weight_B3));
	infb.close();

	ifstream infc("d:\\Weight权重库\\海马记忆模型权重\\weight_33.txt",ios::binary);
	if(!infc)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infc.read((char*)&weight_33,sizeof(weight_33));
	infc.close();
    
	ifstream infd("d:\\Weight权重库\\海马记忆模型权重\\weight_31.txt",ios::binary);
	if(!infd)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infd.read((char*)&weight_31,sizeof(weight_31));
	infd.close();

	ifstream infe("d:\\Weight权重库\\海马记忆模型权重\\weight_1C.txt",ios::binary);
	if(!infe)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infe.read((char*)&weight_1C,sizeof(weight_1C));
	infe.close();

	ifstream inff("d:\\Weight权重库\\海马记忆模型权重\\weight_1A.txt",ios::binary);
	if(!inff)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	inff.read((char*)&weight_1A,sizeof(weight_1A));
	inff.close();

	ifstream infg("d:\\Weight权重库\\海马记忆模型权重\\weight_1B.txt",ios::binary);
	if(!infg)
	{
		cout<<"错误"<<endl;
		return 1;
	}
	infg.read((char*)&weight_1B,sizeof(weight_1B));
	infg.close();

	cout<<"获取完毕!"<<endl;
	return 0;
}
//------------------------------------------------------------------------------------//
int output()     //【连接】
{
	output_CA3();        //【CA3区输出】
	output_CA1();        //【CA1区输出】
    output_C();          //【C区输出】
//  output_A();          //【A区输出】
//	output_B();          //【B区输出】
	return 0;
}

int update()    //【更新权重】
{
	update_ac();
	update_bc();
	update_a3();
	update_b3();
	update_33();
	update_31();
	update_1c();
	update_1a();
	update_1b();
	return 0;
}

int remember()
{
	ini();          //【初始化阈值】
	read_weight();  //【读取权重】
	for(int i=0;i<5;i++)
	{
        output();       //【连接】
        update();       //【更新权重】
	}
	output_A();
    output_B();
	for(int j=0;j<num_A;j++) 
	{
		if(A[j]>=0.5)   cout<<"1"; 
		if(A[j]<0.5)    cout<<"0";
		if((j+1)%7==0)  cout<<endl;
	}
	cout<<endl;
	for(int k=0;k<num_B;k++)  
	{
		if(B[k]>=0.5) cout<<"1"; 
		if(B[k]<0.5)  cout<<"0";
        if((k+1)%7==0)  cout<<endl;
	}
	save_weight();  //【保存】
	return 0;
}
int exam()
{
    ini();          //【初始化阈值】
	read_weight();  //【读取权重】
	output();       //【连接】
	output_A();
    output_B();
	for(int j=0;j<num_A;j++) 
	{
		if(A[j]>=0.5)   cout<<"1"; 
		if(A[j]<0.5)    cout<<"0";
		if((j+1)%7==0)  cout<<endl;
	}
	cout<<endl;
	for(int k=0;k<num_B;k++)  
	{
		if(B[k]>=0.5) cout<<"1"; 
		if(B[k]<0.5)  cout<<"0";
        if((k+1)%7==0)  cout<<endl;
	}
	return 0;
}
//------------------------------------------------------------------------------------//

int main()
{
//	remember();
	exam();
	return 0;
}