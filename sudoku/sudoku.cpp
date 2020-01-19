#ifndef PCH_H
#define PCH_H

// TODO: 添加要在此处预编译的标头

#endif //PCH_H

#include<fstream>
#include<iostream> 
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<math.h> 
#include<cstring>
using namespace std;

int n = 0; //要生成的数独终局个数 
char a[10][10]; //数独矩阵 
double list[10]; //用作保存生成的随机浮点数 
int vis_code[10]; //编码时标记当前序号是否被编码过 
char move1[10][5] = { "036","063" }; //移动规则 
char move2[10][5] = { "258","285","528","582","852","825" };
char move3[10][5] = { "147","174","417","471","714","741" };
char move_boss[10]; //排列组合出的一个序列 
int move_boss_int[10]; //排列组合出序列的整型版 
const double low = -30, high = 30; //维度随机数的上下界

char Print_Data[200000000];
char vis[3][10][10];
bool Found_Ans = false;
bool Is_First_Task = true;

ofstream OutputFile("sudoku.txt");
ofstream OutputFileSolveSudoku("sudoku.txt");

double Rand(double lowwer, double upper) //随机数函数 
{

	return (rand() / (double)RAND_MAX) * ((upper)-(lowwer)) + (lowwer);
}


void transform(double* temp1)  //编码 
{
	memset(vis_code, 0, sizeof(vis_code));
	//每一次找到一个维数中的最小值，从最小值开始顺序赋值i 
	int min;
	for (int i = 1; i <= 9; i++)
	{
		min = -1;
		for (int j = 1; j <= 8; j++)//找到最小的维度 
		{
			if ((min == -1 || temp1[j] < temp1[min]) && vis_code[j] == 0)
				min = j;
		}//min为当前最小维度 
		if (i != 9)
			a[1][min + 1] = i + '0'; //将当前最小维度的元件序号赋值为i 
		else
			a[1][min + 1] = i + 1 + '0';
		vis_code[min] = 1;
	}
}

void Initial()  //初始化第一行的数独 
{
	a[1][1] = 9 + '0'; //左上角的数字为我的学号编码 

	srand((unsigned)(time(NULL) + rand()));
	for (int j = 1; j <= 8; j++)
		list[j] = Rand(low, high); //生成九个随机浮点数 
	transform(list); //生成了一个数独终局第一行随机序列 
}

void To_int() //将一个字符序列转换成整数序列 
{
	for (int i = 0; i < 9; i++)
		move_boss_int[i] = move_boss[i] - '0';
}

void Print_Sudoku(int tag)
{
	int pos = 0;
	if (tag == 1) //生成数独终局的输出 
	{
		for (int i = 1; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
			{
				if (j != 9)
				{
					Print_Data[pos++] = a[i][j];
					Print_Data[pos++] = ' ';//存入空格	
				}
				else
					Print_Data[pos++] = a[i][j];
			}
			Print_Data[pos++] = '\n';//存入换行符
		}
		Print_Data[pos++] = '\n';
		OutputFile << Print_Data; //一个数独终局整体以字符串的形式输出
	}
	else if (tag == 2) //求解数独的输出 
	{
		if (Is_First_Task == false) //如果不是第一个问题，就在求解之前输出一行空行 
			OutputFileSolveSudoku << endl; //塞进去 
		else
			Is_First_Task = false;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (j != 9)
				{
					Print_Data[pos++] = a[i][j];
					Print_Data[pos++] = ' ';//存入空格	
				}
				else
					Print_Data[pos++] = a[i][j];
			}
			Print_Data[pos++] = '\n';//存入换行符
		}
		OutputFileSolveSudoku << Print_Data; //一个数独终局整体以字符串的形式输出
	}
}

void Produce_Sudoku(int DEMAND)
{
	int count = 0;
	for (int i = 0; i < 2; i++) //共可以生成72种不同的排序方式 
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				char s1[15], s2[15];
				strcpy(s1, move1[i]);
				strcpy(s2, move2[j]);
				strcpy(move_boss, strcat(strcat(s1, s2), move3[k])); //排列组合出的字符型序列 
				To_int(); //将字符型序列转换为int型 
				for (int q = 2; q <= 9; q++) //逐行生成一个数独矩阵 
				{
					for (int w = 1; w <= 9; w++)
					{
						int m = (w - move_boss_int[q - 1] + 9) % 9;
						if (m == 0) //对应最后一列的情况需要特殊处理 
							m = 9;
						a[q][w] = a[1][m];
					}
				}
				++count;
				Print_Sudoku(1);
				if (count == DEMAND)//如果满足了这一轮的需求，就退出 
					return;
			}
		}
	}
}

void Set_Vis(int r, int c, int num)
{
	vis[0][r][num] = 1;
	vis[1][c][num] = 1;
	vis[2][r / 3 * 3 + c / 3][num] = 1;
}

void Reset_Vis(int r, int c, int num)
{
	vis[0][r][num] = 0;
	vis[1][c][num] = 0;
	vis[2][r / 3 * 3 + c / 3][num] = 0;
}

bool Check_Vis(int r, int c, int num) //检查可不可以访问
{
	if (vis[0][r][num] == 0
		&& vis[1][c][num] == 0
		&& vis[2][r / 3 * 3 + c / 3][num] == 0)
		return true;
	else
		return false;
}

void Solve_Sudoku(int r, int c)
{
	while (a[r][c] != '0') //找到一个空的数独位置 
	{
		if (c < 8)
			c++;
		else //再来一轮 
		{
			c = 0;
			r++;
		}
		if (r == 9)	//找到了一个答案即9x9数独中没有0，那就是找到了一个解答 
		{
			Found_Ans = true;
			return; //走喽 
		}
	}

	bool Can_Search = false; //标记回溯算法中当前结点是否可以搜索
	for (int i = 1; i <= 9; i++)
	{
		if (Check_Vis(r, c, i))
		{
			Can_Search = true; //标记可以搜索 
			Set_Vis(r, c, i); //当前结点搜索过 
			a[r][c] = i + '0';
			Solve_Sudoku(r, c);
			if (Found_Ans) //剪枝 
				return;
			Can_Search = false;
			Reset_Vis(r, c, i);
			a[r][c] = '0';
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Please input THREE parameters!");
		return 0;
	}
	FILE* fp1 = fopen(argv[2], "r");
	if (strcmp(argv[1], "-s") == 0 && fp1 == NULL)
	{
		printf("Command illegal!\n");
		return 0;
	}
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		int len = strlen(argv[2]); //将命令行字符串转换为整数 
		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] >= '0' && argv[2][i] <= '9')
			{
				int value = argv[2][i] - '0';
				for (int j = 1; j <= len - 1; j++)
					value *= 10;
				n += value;
			}
			else
			{
				printf("Please input a legal number!\n");
				return 0;
			}
		}
		if (n > 1000000 || n <= 0)
		{
			printf("Please input a number from 1 to 1000000!\n");
			return 0;
		}
		int round; //需要生成数独终局的轮数，一轮生成72个
		if (n <= 72)
			round = 1;
		else
			round = n / 72 + 1;
		while (round)
		{
			int demand; //每一轮需要生成的矩阵数 
			Initial(); //初始化数独的第一行 
			if (round != 1) //不是最后一轮 
				demand = 72;
			else //是最后一轮 
				demand = n % 72;
			Produce_Sudoku(demand);
			--round;
		}
	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)
	{
		char buf[110];
		int count = 0;
		while (fgets(buf, 20, fp1))
		{
			if (strcmp(buf, "\n") == 0) //遇空行跳过 
				continue;
			for (int i = 0; i <= 16; i++)
			{
				if (i % 2 == 0)
				{
					a[count][i / 2] = buf[i];
					Set_Vis(count, i / 2, buf[i] - '0');
				}
			}
			count++;
			if (count == 9)
			{
				Found_Ans = false;
				Solve_Sudoku(0, 0); //回溯解数独 
				Print_Sudoku(2);
				memset(vis, 0, sizeof(vis));
				count = 0;
			}
		}
	}
	else
		printf("Command illegal!\n");
	return 0;
}

