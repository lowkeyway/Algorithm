#include<iostream>
using namespace std;
const int maxn = 10, INF = 0x3f3f3f3f;
int L[maxn / 2 + 2], R[maxn / 2 + 2];
int countNum = 0;

void merge(int a[], int n, int left, int mid, int right)
{
	countNum++;
	int n1 = mid - left, n2 = right - mid;
	for (int i = 0; i < n1; i++)
		L[i] = a[left + i];
	for (int i = 0; i < n2; i++)
		R[i] = a[mid + i];
	L[n1] = R[n2] = INF;
	int i = 0, j = 0;
	for (int k = left; k < right; k++)
	{
		if (L[i] <= R[j])
			a[k] = L[i++];
		else
			a[k] = R[j++];
	}
}
void mergesort(int a[], int n, int left, int right)
{
	if (left + 1 < right)
	{
		int mid = (left + right) / 2;
		mergesort(a, n, left, mid);
		mergesort(a, n, mid, right);
		merge(a, n, left, mid, right);
	}
}
int mergeSortMain()
{
	int a[maxn], n;
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> a[i];
	mergesort(a, n, 0, n);
	cout << "countNum: " << countNum << endl;
	for (int i = 0; i < n; i++)
	{
		if (i)
			cout << " ";
		cout << a[i];
	}
	cout << endl;

	system("pause");
	return 0;
}