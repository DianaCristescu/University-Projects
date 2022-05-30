#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_LEN 256


typedef int Item;
#include "Queue.h"



void printArray(int *arr, int len){
  for(int i=0; i < len; i++)  
    printf("%d ", arr[i]);
}

int arrayMaximum(const int* arr, int len){
	int max = arr[0];
	for(int i = 1; i < len; i++){
		if(arr[i] > max) 
			max = arr[i];
	}
	return max;
}

int getNumberOfDigits(int num){
	int digits = 0;
	while(num){
		num = num/10;
		digits++;
	}
	return digits;
}

void radixSort(int *arr, int len){

	// TODO: Cerinta 4 - Implementation must use queues
	// 1. Initialize queues for each digit, i.e. our buckets

	// 2. Compute number of iterations to sort the array:
	// 2a. Compute array maximum
	// 2b. Compute maximum number of digits


	// Radix sort
	// 3. Iterate over the maximum number of digits
	// 3a. Place the number in a bucket according to its digit 
	// 3b. Re-construct the array by using each bucket in order

	//4. Destroy queues

	int n = getNumberOfDigits(arrayMaximum(arr, len));
	int x = 0;
	AQ q0 = createQueue();
	AQ q1 = createQueue();
	AQ q2 = createQueue();
	AQ q3 = createQueue();
	AQ q4 = createQueue();
	AQ q5 = createQueue();
	AQ q6 = createQueue();
	AQ q7 = createQueue();
	AQ q8 = createQueue();
	AQ q9 = createQueue();

	for(int i=1; i<=n; i++) {
		for(int j=0; j<len; j++) {
			x = arr[j];
			for(int k=i; k>1; k--) {
				x /= 10;
			}
			x %= 10;
			switch(x) {
				case 0:
					enqueue(q0, arr[j]);
					break;
				case 1:
					enqueue(q1, arr[j]);
					break;
				case 2:
					enqueue(q2, arr[j]);
					break;
				case 3:
					enqueue(q3, arr[j]);
					break;
				case 4:
					enqueue(q4, arr[j]);
					break;
				case 5:
					enqueue(q5, arr[j]);
					break;
				case 6:
					enqueue(q6, arr[j]);
					break;
				case 7:
					enqueue(q7, arr[j]);
					break;
				case 8:
					enqueue(q8, arr[j]);
					break;
				case 9:
					enqueue(q9, arr[j]);
					break;
			}
		}
		int j=0;
		while(!isQueueEmpty(q0)) {
			arr[j] = front(q0);
			dequeue(q0);
			j++;
		}
		while(!isQueueEmpty(q1)) {
			arr[j] = front(q1);
			dequeue(q1);
			j++;
		}
		while(!isQueueEmpty(q2)) {
			arr[j] = front(q2);
			dequeue(q2);
			j++;
		}
		while(!isQueueEmpty(q3)) {
			arr[j] = front(q3);
			dequeue(q3);
			j++;
		}
		while(!isQueueEmpty(q4)) {
			arr[j] = front(q4);
			dequeue(q4);
			j++;
		}
		while(!isQueueEmpty(q5)) {
			arr[j] = front(q5);
			dequeue(q5);
			j++;
		}
		while(!isQueueEmpty(q6)) {
			arr[j] = front(q6);
			dequeue(q6);
			j++;
		}
		while(!isQueueEmpty(q7)) {
			arr[j] = front(q7);
			dequeue(q7);
			j++;
		}
		while(!isQueueEmpty(q8)) {
			arr[j] = front(q8);
			dequeue(q8);
			j++;
		}
		while(!isQueueEmpty(q9)) {
			arr[j] = front(q9);
			dequeue(q9);
			j++;
		}
	}

}

int main(void){
	

    int len;
    char fileBuffer[MAX_INPUT_LEN];
    FILE* inputFile = fopen("input-radix-sort.csv","r");
    if(inputFile == NULL) return 1;

    int capacity = MAX_INPUT_LEN;
    int *arr = (int*) malloc(capacity*sizeof(int));

    int count = 0;
    while(fgets(fileBuffer, MAX_INPUT_LEN, inputFile) != NULL){
      fileBuffer[strcspn(fileBuffer, "\r\n")] = 0;
      len = strlen(fileBuffer);
      if(len == 0) break;

      char *token = strtok(fileBuffer,",\n");
      if(token == NULL) return 1;
      len = 0;
	  arr[len++] = atoi(token);
      while(token != NULL){
      	if(len == capacity){
      		capacity <<= 1;
      		arr = realloc(arr, capacity*sizeof(int));
      	}
    	
      	token = strtok(NULL,",\r\n");
      	if(token != NULL)
      		arr[len++] = atoi(token);     	
      }

      count++;
      printf("Input array %d:\t", count);
      printArray(arr, len);
      printf("\n");
      radixSort(arr, len);
      printf("Sorted array %d:\t", count);
      printArray(arr, len);
      printf("\n");
      printf("\n");
  	}

	free(arr);
	fclose(inputFile);
	return 0;
}
