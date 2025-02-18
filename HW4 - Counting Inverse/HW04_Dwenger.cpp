/* Counting inverse*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <limits> 
#include <fstream>
using namespace std;


int mergeSortAndCountInv(int arr[], int l, int r);
int merge(int arr[], int l, int m, int r);

int merge(int arr[], int l, int m, int r) {
	int i, j, k, inv_count = 0;

	int n1 = m - l + 1; // number of elements in the left half
	int n2 = r - m;		// number of elements in the right half

	/* create temp arrays */
	int *L = new int[n1];
	int *R = new int[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (int i = 0; i < n1; i++) {
		L[i] = arr[l + i];
	}

	for (int j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
	}

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if(L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		} else {
			arr[k] = R[j];
			j++;
			inv_count = inv_count + (n1-i);
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there are any */
	while (j < n2) {
		arr[k] = R[j];
		k++;
		j++;
	}

	delete[] L;
	delete[] R;

	return inv_count;
}

int mergeSortAndCountInv(int arr[], int l, int r) {
	int inv_count = 0;
	if (l < r) {
		int middle = (l + r) / 2;
    inv_count = inv_count + mergeSortAndCountInv(arr, l, middle);
    inv_count = inv_count + mergeSortAndCountInv(arr, middle + 1, r);
		inv_count = inv_count + merge(arr, l, middle, r);
	}
	return inv_count;
}

/* Function to print an array */
void printArray(int A[], int size) {
	int i;
	for (i = 0; i < size; i++)
		cout << A[i] << "  ";
	cout << endl;
}

/* Driver program to test above functions */
int main() 
{ 
	int arr_size;
  ifstream file; 
    // opening file "input.txt" 
    // in (read) mode 
    // ios::in Open for input operations. 
    file.open("input.txt",ios::in); 
    // If no file is created, then 
    // show the error message. 
    if(!file) 
    { 
        cout<<"Error in creating file!!!"<< endl; 
        return 0; 
    } 
    cout<<"File opened successfully." << endl; 
    file>>arr_size;

    int arr[arr_size];

    for(int i = 0; i < arr_size; i++){
      file>>arr[i];
    }


	cout<<"Given array is \n";
	printArray(arr, arr_size); 

	cout<<"Number of inversions are "<< 
  mergeSortAndCountInv(arr, 0, arr_size - 1) << endl;

	return 0; 
} 

