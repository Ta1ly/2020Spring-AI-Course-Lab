//sudoku
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_PATH "../input/sudoku02.txt"
#define OUTPUT_PATH "../output/sudoku02.txt"

FILE *input_file = fopen(INPUT_PATH, "r");
FILE *output_file = fopen(OUTPUT_PATH, "w+");
char degree[81];
char filled[81];
char num ;
char input[81];char selection[81][10];
char curselection[81][10];
char curdegree[81];
int count = 0;
void printresult(){
	int i,j;
	for (i = 0; i < 9; i++){
    	for (j = 0; j < 9; j++){
    		fprintf(output_file,"%d ",input[i*9 + j]);
		}
		fprintf(output_file,"\n");
	}
	//printf("\n");
}

void printmap(){
	int i,j;
	for (i = 0; i < 9; i++){
    	for (j = 0; j < 9; j++){
    		//printf("%d ",input[i*9 + j]);
		}
		//printf("\n");
	}
	//printf("\n");
}




void getselection(){
	int i, j ,k ,r;
	char around_x[3],around_y[3];
	char temp;
	num = 0;
    for (i = 0; i < 81; i++){
        if (input[i] == 0){
            for (j = 0; j < 10; j++){
                    selection[i][j] = j;
            }
            filled[i] = 0;
            num ++;
        }
        else{
            for (j = 0; j < 10; j++){
                    selection[i][j] = 0;
            }
            filled[i] = 1;
        }
    }
	for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            temp = input[i * 9 + j];
            for (k = 0; k < 9; k++){
                selection[i * 9 + k][temp] = 0;
                selection[k * 9 + j][temp] = 0;
            }
            r = i % 3;
            for (k = 0; k < 3; k++){
                around_x[k] = i + k - r;
            }
            r = j % 3;
            for (k = 0; k < 3; k++){
                around_y[k] = j + k - r;
            }
            for (k = 0; k < 3; k++){
                for (r = 0; r < 3; r++){
                    selection[around_x[k] * 9 + around_y[r]][temp] = 0;
                }
            }
        }
    }
}

void getdegree(){
	int i,j;
    for (i = 0; i < 81; i++){
        degree[i] = 10;
        for (j = 0; j < 10; j++){
            if(selection[i][j] == 0)
                degree[i]--;
        }
    }
}



int sudoku(){
	int i;
	count++;
	char min = 20,min_pos;
	if(num == 0) return 1;
	for(i = 0; i < 81; i++){
		if(filled[i] == 0){
			if(degree[i] == 0) return -1;
			if(degree[i] < min){
				min = degree[i];
				min_pos = i;
			}	
		}
	}
	
	for (i = 1; i < 10; i++){
		if(selection[min_pos][i]!=0){
			input[min_pos] = i;
			filled[min_pos] = 1;
			getselection();
			getdegree();
			if(sudoku() == 1){
				return 1;
			}
			else {
				input[min_pos] = 0;
				selection[min_pos][i] = 0;
				filled[min_pos] = 0;
				getselection();
				getdegree();
			}
		}
	}
}


int main(){
    int i, j, k, r;
    char temp;
	for (i = 0; i < 9; i++){
		fscanf(input_file,"%d %d %d %d %d %d %d %d %d\n",&input[i * 9 + 0],&input[i * 9 + 1],&input[i * 9 + 2],&input[i * 9 + 3],&input[i * 9 + 4],&input[i * 9 + 5],&input[i * 9 + 6],&input[i * 9 + 7],&input[i * 9 + 8]);
	}
	getselection();
    getdegree();
    for (i = 0; i < 9; i++){
    	for (j = 0; j < 9; j++){
    		//printf("%d ",degree[i*9 + j]);
		}
		//printf("\n");
	}

	//printf("\n");
	
	
	
	if(sudoku() == 1){
		printresult();
    	printf("finish\n");
	}
    else printf("sth wrong");
	printf("node check:%d", count);
}
