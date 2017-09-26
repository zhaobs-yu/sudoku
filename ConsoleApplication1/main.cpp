#include<iostream> 
#include <fstream>
#include <cassert>
#include <string>
#include <stdlib.h> 
#include <time.h>  
#include <sstream> 

using namespace std;

class block {
	public:
		int id;
		int row;
		int column;
		int area;
		int numbers[10];
		int poss;
		block() {};
};

class row {
	public:
		int id;
		int blockids[9];
		int need[10];
		int count[10];
};

class column {
	public:
		int id;
		int blockids[9];
		int need[10];
		int count[10];
};

class area {
	public:
		int id;
		int blockids[9];
		int need[10];
		int count[10];
};

class table {
	public:
		block blocks[81];
		row rows[9];
		column columns[9];
		area areas[9];
		table() {};
};

ofstream fileout;
int tryi = 0;
int tryn[2];
int error = 0;
int seeds[27] = {1,3,5,7,18,20,22,24,26,28,30,32,34,45,47,49,51,53,55,57,59,61,72,74,76,78,80};
table tabletry[200];
int tablesin[1000000][81];
int tablenumber = 0;


table* fill(table *table1, int i, int n) { //第i个格子填n
	table1->blocks[i].numbers[0] = n;
	int j;
	for (j = 1; j < 10; j++) {
		table1->blocks[i].numbers[j] = 0;
	}
	table1->blocks[i].poss = 0;
	table1->rows[table1->blocks[i].row].need[n] = 0;
	for (j = 0; j < 9; j++) {
		if (table1->blocks[table1->rows[table1->blocks[i].row].blockids[j]].numbers[n] ==1) {
			table1->blocks[table1->rows[table1->blocks[i].row].blockids[j]].numbers[n] = 0;
			if (table1->blocks[table1->rows[table1->blocks[i].row].blockids[j]].poss > 0) {
				table1->blocks[table1->rows[table1->blocks[i].row].blockids[j]].poss--;
			}
		}
	}

	table1->columns[table1->blocks[i].column].need[n] = 0;
	for (j = 0; j < 9; j++) {
		if (table1->blocks[table1->columns[table1->blocks[i].column].blockids[j]].numbers[n] ==1) {
			table1->blocks[table1->columns[table1->blocks[i].column].blockids[j]].numbers[n] = 0;
			if (table1->blocks[table1->columns[table1->blocks[i].column].blockids[j]].poss > 0) {
				table1->blocks[table1->columns[table1->blocks[i].column].blockids[j]].poss--;
			}
		}
	}

	table1->areas[table1->blocks[i].area].need[n] = 0;
	for (j = 0; j < 9; j++) {
		if (table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].numbers[n] ==1) {
			table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].numbers[n] = 0;
			if (table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].poss > 0 &&
				table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].row != table1->blocks[i].row&&
				table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].column != table1->blocks[i].column) {
				table1->blocks[table1->areas[table1->blocks[i].area].blockids[j]].poss--;
			}
		}
	}
	return table1;
}

int scanblock(table *table1) {
	int i, j;
	for (i = 0; i < 81; i++) {
		if (table1->blocks[i].poss == 1) {
			for (j = 1; j < 10; j++) {
				if (table1->blocks[i].numbers[j] == 1) {
					//cout << "第" << i/ 9 + 1 << "行 " << i% 9 + 1 << "列填" << j << "\n";
					fill(table1, i, j);
					return 1;
				}
			}
		}
		else if (table1->blocks[i].poss == 0 && table1->blocks[i].numbers[0] == 0) {
			//cout << "第" << i / 9 + 1 << "行 " << i % 9 + 1 << "列矛盾\n";
			return -1;
		}
	}
	return 0;
}

int scan(table *table1) {
	int i, j,k;
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			if (table1->rows[i].count[j] == 1) {
				for (k = 0; k < 9; k++) {
					if (table1->blocks[table1->rows[i].blockids[k]].numbers[j] == 1) {
						//cout << "第" << table1->rows[i].blockids[k] / 9+1<< "行 " << table1->rows[i].blockids[k] % 9+1 << "列填" << j << "\n";
						fill(table1, table1->rows[i].blockids[k], j);
						return 1;
					}
				}
			}
		}
	}
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			if (table1->columns[i].count[j] == 1) {
				for (k = 0; k < 9; k++) {
					if (table1->blocks[table1->columns[i].blockids[k]].numbers[j] == 1) {
						//cout << "第" << table1->columns[i].blockids[k] / 9+1 << "行 " << table1->columns[i].blockids[k] % 9+1 << "列填" << j << "\n";
						fill(table1, table1->columns[i].blockids[k], j);
						return 1;
					}
				}
			}
		}
	}
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			if (table1->areas[i].count[j] == 1) {
				for (k = 0; k < 9; k++) {
					if (table1->blocks[table1->areas[i].blockids[k]].numbers[j] == 1) {
						//cout << "第" << table1->areas[i].blockids[k] /9+1<<"行 "<< table1->areas[i].blockids[k] %9+1<<"列填" << j << "\n";
						fill(table1, table1->areas[i].blockids[k], j);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void getcount(table *table1) {
	int i, j,k;
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			table1->rows[i].count[j] = 0;
			table1->columns[i].count[j] = 0;
			table1->areas[i].count[j] = 0;
			for (k = 0; k < 9; k++) {
				if (table1->blocks[table1->rows[i].blockids[k]].numbers[j]== 1) {
					table1->rows[i].count[j]++;
				}
				if (table1->blocks[table1->columns[i].blockids[k]].numbers[j] == 1) {
					table1->columns[i].count[j]++;
				}
				if (table1->blocks[table1->areas[i].blockids[k]].numbers[j] == 1) {
					table1->areas[i].count[j]++;
				}
			}
		}
	}
}

void inittable(table *table1) {
	int i,j;
	//初始化所有格子
	for (i = 0; i < 81; i++) {
		table1->blocks[i].id = i;
		table1->blocks[i].row = i / 9;
		table1->blocks[i].column = i %9;
		table1->blocks[i].area = i / 9 / 3 * 3 + (i % 9) /3;
		table1->blocks[i].numbers[0] = 0;
		for (j = 1; j <10; j++) {
			table1->blocks[i].numbers[j] = 1;
		}
		table1->blocks[i].poss = 9;
	}
	//初始化行信息
	for (i = 0; i < 9; i++) {
		table1->rows[i].id = i;
		for (j = 0; j < 9; j++) {
			table1->rows[i].blockids[j] = i * 9 + j;
			table1->rows[i].need[j+1] = 1;
			table1->rows[i].count[j+1] = 9;
		}
	}
	//初始化列信息
	for (i = 0; i < 9; i++) {
		table1->columns[i].id = i;
		for (j = 0; j < 9; j++) {
			table1->columns[i].blockids[j] = j * 9 + i;
			table1->columns[i].need[j + 1] = 1;
			table1->columns[i].count[j + 1] = 9;
		}
	}
	//初始化块信息
	for (i = 0; i < 9; i++) {
		table1->areas[i].id = i;
		for (j = 0; j < 9; j++) {
			table1->areas[i].blockids[j] = (i / 3 * 3 + j / 3) * 9 + (i % 3 * 3 + j % 3);
			table1->areas[i].need[j + 1] = 1;
			table1->areas[i].count[j + 1] = 9;
		}
	}
}

void printtable(table *table1) {
	int i, j;
	cout << '\n';
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			cout << table1->blocks[i * 9 + j].numbers[0];
			if (j == 8) {
				cout << '\n';
			}
			else {
				cout << ' ';
			}
		}
	}
}

void writetable(table *table1) {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fileout << table1->blocks[i * 9 + j].numbers[0];
			if (j == 8) {
				fileout << '\n';
			}
			else {
				fileout << ' ';
			}
		}
	}
	fileout << '\n';
}

void printposs(table *table1) {
	cout << "poss:\n";
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			cout << table1->blocks[i * 9 + j].poss;
			if (j == 8) {
				cout << '\n';
			}
			else {
				cout << ' ';
			}
		}
	}
}

void printcount(table *table1) {
	cout << "count:\n";
	cout << "row:\n";
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			cout << table1->rows[i].count[j];
			if (j == 9) {
				cout << '\n';
			}
			else {
				cout << ' ';
			}
		}
	}
	cout << "column:\n";
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			cout << table1->columns[i].count[j];
			if (j == 9) {
				cout << '\n';
			}
			else {
				cout << ' ';
			}
		}
	}
	cout << "area:\n";
	for (i = 0; i < 9; i++) {
		for (j = 1; j < 10; j++) {
			cout << table1->areas[i].count[j];
			if (j == 9) {
				cout << '\n';
			}
			else {
				cout << ' ';
			}
		}
	}
}

int readtables(string s) {
	ifstream infile;
	infile.open(s, std::ios::out);
	if (!infile)
	{
		return -1;
	}
	char c;
	int i=0,j=0;
	while (!infile.eof())
	{
		infile >> c;
		int n = c - 48;
		tablesin[i][j] = n;
		j++;
		if (j >= 81) {
			i++;
			tablenumber++;
			j = 0;
		}
	}
	infile.close();
	return 0;
}

void readfile(table *table1) {
	ifstream file;
	file.open("E:/puzzle3.txt" , std::ios::out);
	char c;
	int i;
	for (i = 0; i < 81; i++)
	{
		file>> c;
		int n = c - 48;
		//cout << c << endl;
		table1->blocks[i].numbers[0] = n;
		if (c > 48) {
			fill(table1, i, n);
		}
	}
	file.close();
}

void readtable(table *table1,int k) {
	int i;
	for (i = 0; i < 81; i++)
	{
		int n = tablesin[k][i];
		table1->blocks[i].numbers[0] = n;
		if (n > 0) {
			fill(table1, i, n);
		}
	}
}

int work1(table *table1) {
	int changed1 = 0;
	int changed2 = 0;
	do {
		do {
			changed1 = scanblock(table1);
			if (changed1 == -1) {
				return -1;
			}
			//cout << "changed:" << changed << "\n";
			getcount(table1);
		} while (changed1 == 1);
		do {
			changed2 = scan(table1);
			if (changed2 == 1) {
				changed1 = 1;
			}
			//cout << "changed:" << changed << "\n";
			getcount(table1);
		} while (changed2 == 1);
	} while (changed1 == 1 || changed2 == 1);
	return 0;
}

int check(table *table1) {
	int i;
	int over=1;
	int isnext=0;
	for (i = 0; i < 81; i++) {
		if (table1->blocks[i].poss >0) {
			over = 0;
			break;
		}
	}
	if (over == 1) {
		return 1;
	}
	else {
		for (i = 0; i < 81; i++) {
			if (table1->blocks[i].poss == 2) {
				tryi = i;
				break;
			}
		}
		for (i = 1; i < 10; i++) {
			if (table1->blocks[tryi].numbers[i] == 1) {
				if (isnext == 0) {
					tryn[0] = i;
					isnext = 1;
				}
				else {
					tryn[1] = i;
					break;
				}
			}
		}
	}
	return 0;
}

int solve(table *table1) {
	int point = 0;
	//printtable(table1);
	//printposs(table1);
	getcount(table1);
	//printcount(table1);
	int exit = 0;
	exit = work1(table1);
	//printtable(table1);
	//printposs(table1);
	//printcount(table1);
	check(table1);
	tabletry[point] = *table1;
	point++;
	tabletry[point] = *fill(table1, tryi, tryn[0]);
	point--;
	tabletry[point] = *fill(&tabletry[point], tryi, tryn[1]);
	point++;
	if (exit == 0) {
		while (1) {
			//cout << "begin try" << tryi / 9 + 1 << "行" << tryi % 9 + 1 << "\n";
			error = work1(&tabletry[point]);
			//printtable(&tabletry[point]);
			//printposs(&tabletry[point]);
			//printcount(&tabletry[point]);
			if (error == -1) {
				//cout << "try another";
				point--;
				//cout << point << endl;
				if (point < 0) {
					//cout << "无解" << endl;
					return -1;
				}
			}
			else {
				if (check(&tabletry[point]) == 1) {
					//cout << "over" << endl;
					//printtable(&tabletry[point]);
					writetable(&tabletry[point]);
					break;
				}
				else {
					point++;
					tabletry[point] = tabletry[point - 1];
					point++;
					tabletry[point] = *fill(&tabletry[point - 2], tryi, tryn[0]);
					point--;
					tabletry[point] = *fill(&tabletry[point], tryi, tryn[1]);
					point++;
				}
			}
		}
	}
	else {
		//cout << "无解" << endl;
		return -1;
	}
	return 0;
}

int make1() {
	table *table0 = new table();
	inittable(table0);
	fill(table0, 0, 2);
	int i;
	int n;
	srand(rand());
	for (i = 0; i < 27; i++) {
		while (1) {
			n = rand() % 9 + 1;
			if (table0->blocks[seeds[i]].numbers[n] == 1) {
				fill(table0, seeds[i], n);
				break;
			}
		}
	}
	//printtable(table0);
	int o= solve(table0);
	delete table0;
	return o;
}

void makemany(int n) {
	int k = 0;
	while (1) {
		if (make1() == 0) {
			k++;
			//cout << k << endl;
			if (k >= n) {
				//cout << k << endl;
				fileout.close();
				break;
			}
		}
	}
}

bool isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

int main(int argc, char * argv[]) {
	srand((unsigned)time(NULL));
	clock_t start_time = clock();
	fileout.open("sudoku.txt");
	table *table1 = new table();
	int i;
	if (argc == 3) {
		cout << "参数个数正确" << endl;
		if (strcmp(argv[1],"-c")==0&&isNum(argv[2]) ){
			int a;
			sscanf_s(argv[2], "%d", &a);
			if (a <=0) {
				cout << "输入了非正数" << endl;
			}
			else {
				cout << "生成" <<a<<"个" <<endl;
				makemany(a);
			}
		}
		else if (strcmp(argv[1], "-s") == 0) {
			if (readtables(argv[2]) == -1) {
				cout << "文件不存在" << endl;
			}
			else {
				cout << "解" << tablenumber << "个" << endl;
				for (i = 0; i < tablenumber; i++) {
					inittable(table1);
					readtable(table1, i);
					//printtable(table1);
					solve(table1);
				}
			}
		}
		else {
			cout << "输入有误" << endl;
		}
	}
	else {
		cout << "参数个数有误" << endl;
	}
	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC  << "s" << endl;//输出运行时间
	//system("pause");
	return 0;
}
