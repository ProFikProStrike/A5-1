#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
#include <chrono>
#include <ctime>
#define T 4

using namespace std;

struct templ {
	int b[3][T];
};

vector<templ> B[10];

int b[24][3][3];

void step(int a[3][T], int b[3][T]) {
	int m;
	m = (a[0][T - 1] * a[1][T - 1]) | (a[0][T - 1] * a[2][T - 1]) | (a[1][T - 1] * a[2][T - 1]);
	for (int i = 0; i < 3; i++) {
		if (m == a[i][T - 1]) {
			for (int j = 0; j < T - 1; j++) {
				b[i][j] = a[i][j + 1];
			}
			b[i][T - 1] = 2;
		}
		else {
			for (int j = 0; j < T; j++) {
				b[i][j] = a[i][j];

			}
		}
	}
}

int intersect(int a[3][T], int b[3][T]) {
	int c;
	c = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < T; j++) {
			if ((a[i][j] == 0 && b[i][j] == 1) || (a[i][j] == 1 && b[i][j] == 0))
				return 0;
			if (a[i][j] == 2 && b[i][j] == 2)
				c += 1;
		}
	}
	return (3 * T - c);
}

int intersectN(int n, int *a, int globaliteration) {
	int c;
	c = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = T - 1; j > (T - 3 - globaliteration); j--) {//(T - 3 - globaliteration)
			int flag0 = 0;
			int flag1 = 0;
			for (int k = 0; k < n; k++) {
				flag0 |= (B[globaliteration][a[k]].b[i][j] == 0);
				flag1 |= (B[globaliteration][a[k]].b[i][j] == 1);
			}
			if (flag0 && flag1)
				return 0;
			if (!flag0 && !flag1)
				c += 1;
		}
	}
	return (3 * T - c);
}

void global_step(int i) {
	for (int k = 0; k < B[i - 1].size(); k++) {
		int x1 = -1, x2 = -1;
		if (B[i - 1][k].b[0][T - 1] == 2)
			x1 = 0;

		if (B[i - 1][k].b[1][T - 1] == 2) {
			if (x1 == -1)
				x1 = 1;
			else x2 = 1;
		}

		if (B[i - 1][k].b[2][T - 1] == 2)
			x2 = 2;
		for (int vx1 = 0; vx1 <= 1; vx1++) {
			for (int vx2 = 0; vx2 <= 1; vx2++) {
				B[i - 1][k].b[x1][T - 1] = vx1;
				B[i - 1][k].b[x2][T - 1] = vx2;
				templ tt;
				step(B[i - 1][k].b, tt.b);
				B[i].push_back(tt);
			}
		}
	}
}

int t = 0;
int g = 0;
int main()
{
	//Временные штуки 
	chrono::time_point<chrono::system_clock> start, end;
	time_t end_time = chrono::system_clock::to_time_t(end);
	////////////////

	int a[6][3][3] = {
	{ { 2,0,2 },{ 2,0,2 },{ 2,1,0 } },
	{ { 2,0,2 },{ 2,1,0 },{ 2,0,2 } },
	{ { 2,0,1 },{ 2,1,2 },{ 2,1,2 } },
	{ { 2,1,0 },{ 2,0,2 },{ 2,0,2 } },
	{ { 2,1,2 },{ 2,0,1 },{ 2,1,2 } },
	{ { 2,1,2 },{ 2,1,2 },{ 2,0,1 } },
	};

	templ tt[6];
	for (int i = 0; i < 6; i++) {
		for (int k = 0; k < 3; k++) {
			tt[i].b[k][T - 1] = a[i][k][2];
			tt[i].b[k][T - 2] = a[i][k][1];
			tt[i].b[k][T - 3] = a[i][k][0];
			for (int j = 0; j < T - 3; j++) {
				tt[i].b[k][j] = 2;
			}
		}
		B[0].push_back(tt[i]);
	}

	//Global Iteration
	for (int iter = 1; iter < 5; iter++) {
		global_step(iter);
		static int tmptmp[100] = { 0 };
		int tmp = 0;
		tmptmp[tmp] = { 0 };
		for (int i = 0; i < 100; i++) { 
			tmptmp[i] =  0 ; 
		}

		for (int i = 0; i < B[iter].size(); i++) {  
			g = intersect(B[iter][i].b, B[iter][i].b);
			tmptmp[g]++;
		}
		

		printf("SIZE B[%d] = %d\n", iter, B[iter].size());

		static int pairs[100];
	
		
		int c;
		c = 0;

		int *ones;
		ones = (int*)malloc(B[iter].size() * sizeof(int));

		start = chrono::system_clock::now(); 
		for (int n = 2; n < 100; n++) {
			int tmp = 0;
			tmptmp[tmp] = { 0 };
			printf("Iter: %d\t\tn: %d\t\tIntersection:\n", iter, n);
			for (int i = 0; i < n; i++)
				ones[i] = 1;
			for (int j = n; j < B[iter].size(); j++)
				ones[j] = 0;

			int check = 0;
			int perm_n = 0;

			do {
				int ff = 0;
				for (int k = 0; k < B[iter].size(); k++)
					if (ones[k] == 1) {
						pairs[ff] = k;
						ff++;
					}
				perm_n++;

				if (iter == 2 && perm_n % 1000000 == 0) 
					printf("\t\t%d  --   %d %d (%d)\n", iter, n, perm_n, check);

				tmp = intersectN(n, pairs, iter);

				if (tmp != 0)
					check++;

				if (n % 2 == 0)
					tmptmp[tmp]--;
				else tmptmp[tmp]++;

			} while (prev_permutation(ones, ones + B[iter].size()));

			if (check == 0)
				break;
		}
		end = chrono::system_clock::now();
		int elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start).count();
		cout << "Time: " << elapsed_seconds << "s\n";
		printf("Intersection: \n");
		for (int i = 1; i < 64; i++) {
			if (tmptmp[i] != 0) 
				cout << tmptmp[i] << " * " << "2^" << 64 - i << endl;
		}

	}
	scanf("\n");
	return 0;
}
