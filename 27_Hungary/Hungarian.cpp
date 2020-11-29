#include "stdio.h"
#include <iostream>

/*
7
1 1
1 2
2 2
2 3
3 1
3 2
4 3
*/

using namespace std;

const int maxn = 510;

int line[maxn][maxn], used[maxn], nxt[maxn];
int t, n, m, u, v;

bool Find(int x)
{
	for (int i = 1; i < m; i++)
	{
		if (line[x][i] && !used[i])
		{
			used[i] = 1;
			if (nxt[i] == 0 || Find(nxt[i]))
			{
				nxt[i] = x;
				return true;
			}
		}
	}
	return false;
}

int match()
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		memset(used, 0, sizeof(used));
		if (Find(i)) sum++;
	}
	return sum;
}

void main()
{
	ios::sync_with_stdio(false);
	cin >> t;
	memset(nxt, 0, sizeof(nxt));
	memset(line, 0, sizeof(line));
	n = m = 4;

	while (t--)
	{
		cin >> u >> v;
		line[u][v] = 1;
	}

	cout << match() << endl;

	system("pause");

}