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
//char input[81]= {0, 0, 0, 0, 0, 0, 9, 0, 7, 5, 0, 0, 9, 7, 8, 1, 3, 0, 0, 0, 0, 0, 0, 2, 0, 8, 5, 9, 2, 0, 0, 1, 0, 8, 0, 3, 0, 0, 0, 3, 8, 5, 0, 0, 0, 6, 0, 8, 0, 4, 0, 0, 5, 1, 3, 4, 0, 6, 0, 0, 0, 0, 0, 0, 5, 2, 4, 9, 7, 0, 0, 6, 1, 0, 6, 0, 0, 0, 0, 0, 0};
//char input[81] = {0, 0, 5, 3, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 4, 0, 9, 0, 7, 0, 0, 2, 0, 0, 0, 0, 2, 0, 7, 0, 0, 0, 0, 0, 1, 3, 6, 4, 0, 0, 0, 9, 8, 5, 5, 0, 0, 0, 0, 0, 7, 0, 6, 0, 0, 0, 0, 4, 0, 0, 9, 0, 7, 0, 1, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 8, 1, 0, 0};
char input[81];
//= {6, 7, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 6, 0, 0, 0, 0, 3, 0, 3, 0, 0, 7, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 6, 0, 0, 2, 0, 4, 0, 0, 0, 0, 3, 0, 8, 0, 3, 0, 0, 0, 0, 0, 0, 5, 7};
char selection[81][10];
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

int check(int pos){
    int i = pos / 9;
    int j = pos % 9;
    int k ,r,p;
	char around_x[3],around_y[3];
    for (p = 0; p < 9; p++){
        if(j != p && input[i*9+p] == input[i*9 + j])
            return 0;
        if(i!= p && input[i*9+j] == input[p*9+j])
            return 0;
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
            if(around_x[k]!=i || around_y[r]!=j){
                if(input[around_x[k]*9+around_y[r]] == input[i*9 + j])
                    return 0;
            }
            
        }
    }
    return 1;
}

int sudoku(){
	int i;
	count++;
	char min_pos;
	if(num == 0) return 1;
	for(i = 0; i < 81; i++){
		if(filled[i] == 0){
            min_pos = i;
            break;
        }
	}
	
	for (i = 1; i < 10; i++){
		if(selection[min_pos][i]!=0){
			input[min_pos] = i;
            if(check(min_pos)){
                filled[min_pos] = 1;
                num--;
                if(sudoku() == 1){
                    return 1;
                }
                else {
                    input[min_pos] = 0;
                    num++;
                    filled[min_pos] = 0;
			}
            }
            input[min_pos] = 0;
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
