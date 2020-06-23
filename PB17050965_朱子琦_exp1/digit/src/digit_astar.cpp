#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 5
#define MAX_SIZE 10000000
#define INFTY 10000
#define SCALE1 4
#define SCALE2 2
#define INPUT_PATH "../input/2.txt"
#define OUTPUT_PATH "../output/2.txt"

FILE *input_file = fopen(INPUT_PATH, "r");
FILE *output_file = fopen(OUTPUT_PATH, "w+");

struct point{
    char x;
    char y;
};

struct mapmsg{
    char map[SIZE*SIZE];
    int parent;
    char move_num;
    char direction;
    point zero[2];
    int g_value;
    int h_value;
    int f_value;
    int hash;
};

int x[22] = {0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4};
int y[22] = {0, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2};
int max = 0;
char finish = 0;
int position = 0,cur_pos,final_pos;
time_t start_time = time(0),mid_time,end_time;
int d_limit,next_d_limit = INFTY;
long counter;
struct mapmsg* maplist= (struct mapmsg *)malloc(MAX_SIZE * sizeof(struct mapmsg));

void printmap(int pos){
    printf("===map %d===\n", pos);
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            printf("%d\t", maplist[pos].map[i * SIZE + j]);
        }
        printf("\n");
    }
}

void get_result(int pos){
    if(pos == 0){
        printmap(pos);
        return;
    }
    else {
        get_result(maplist[pos].parent);
        printf("%d %c \n", maplist[pos].move_num, maplist[pos].direction);
        printmap(pos);
    }
}

void get_result2(int pos){
    if(pos == 0){
        return;
    }
    else {
        get_result2(maplist[pos].parent);
        fprintf(output_file,"(%d,%c);", maplist[pos].move_num, maplist[pos].direction);
    }
}

int measure(char map[SIZE*SIZE]){
    int value = 0;
    int i = 0, j = 0;
    int t1, t2;
    int num;
    for( i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++){
            num = map[i*SIZE+j];
            if(num == 7){
                if (map[(i+1)*SIZE+j] == 7){
                    t1 = i - x[num];
                    t2 = j - y[num];
                    t1 = (t1 >= 0) ? t1 : -t1;
                    t2 = (t2 >= 0) ? t2 : -t2;
                    value += SCALE1 * (t1 + t2);
                    continue;
                }
                else {
                    continue;
                }
            }
            if(num == 0){
                continue;
            }
            t1 = i - x[num];
            t2 = j - y[num];
            t1 = (t1 >= 0) ? t1 : -t1;
            t2 = (t2 >= 0) ? t2 : -t2;
            value += SCALE2 * (t1 + t2);
        }
    }
    return value;
}

int check(char *map,int hash){
    int i,j;
    int flag;
    for (i = 0; i < position; i++){
        flag = 1;
        if (hash == maplist[i].hash){
            for (j = 0; j < 25; j++){
                if(map[j]!=maplist[i].map[j]){
                    flag = 0;
                    break;
                }
            }
            if(flag == 1)
                return 1;
        }
    }
    return 0;
}

void move(int select, char direction){
    char new_map[25];
    char temp;
    mapmsg cur_map = maplist[cur_pos];
    char base[2] = {cur_map.zero[0].x * SIZE + cur_map.zero[0].y, cur_map.zero[1].x * SIZE + cur_map.zero[1].y};
    int hash = 0;
    int h_value;
    memcpy(new_map, cur_map.map, sizeof(new_map));
    
    if(select == 2){
        temp = 7;
        new_map[base[0]] = temp;
        new_map[base[1]] = temp;
        switch(direction){
            case 'u':{
                new_map[base[0] + SIZE] = 0;
                new_map[base[1] + SIZE + SIZE] = 0;
                break;
            }
            case 'd':{
                new_map[base[0] - SIZE] = 0;
                new_map[base[1] - SIZE - SIZE] = 0;
                break;
            }
            case 'l':{
                new_map[base[0] + 2] = 0;
                new_map[base[1] + 1] = 0;
                break;
            }
            case 'r':{
                new_map[base[0] - 2] = 0;
                new_map[base[1] - 1] = 0;
                break;
            }
            default:{
                printf("error!\n");
                break;
            }
        }
    }
    else {
        switch(direction){
            case 'u':{
                temp = new_map[base[select] + SIZE];
                new_map[base[select]] = temp;
                new_map[base[select] + SIZE] = 0;
                break;
            }
            case 'd':{
                temp = new_map[base[select] - SIZE];
                new_map[base[select]] = temp;
                new_map[base[select] - SIZE] = 0;
                break;
            }
            case 'l':{
                temp = new_map[base[select] + 1];
                new_map[base[select]] = temp;
                new_map[base[select] + 1] = 0;
                break;
            }
            case 'r':{
                temp = new_map[base[select] - 1];
                new_map[base[select]] = temp;
                new_map[base[select] - 1] = 0;
                break;
            }
        }
    }
    for (int i = 0; i < 25; i++){
        for(int j = 0; j <= i; j++){
            hash += new_map[i];
        }
    }
    if (check(new_map, hash) == 1)
        return;

    position++;
    memcpy(maplist[position].map, new_map, sizeof(new_map));
    
    
    maplist[position].zero[0].x = cur_map.zero[0].x;
    maplist[position].zero[1].x = cur_map.zero[1].x;
    maplist[position].zero[0].y = cur_map.zero[0].y;
    maplist[position].zero[1].y = cur_map.zero[1].y;

    maplist[position].move_num = temp;
    maplist[position].parent = cur_pos;
    maplist[position].hash = hash;
    
        
    maplist[position].direction = direction;
    if(select == 2){
        switch(direction){
            case 'u':{
                maplist[position].zero[0].x = cur_map.zero[0].x + 1;
                maplist[position].zero[1].x = cur_map.zero[1].x + 2;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[0].x < x[temp] ? SCALE1 : -SCALE1);
                break;
            }
            case 'd':{
                maplist[position].zero[0].x = cur_map.zero[0].x - 1;
                maplist[position].zero[1].x = cur_map.zero[1].x - 2;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[0].x > x[temp] ? SCALE1 : -SCALE1);
                break;
            }
            case 'l':{
                maplist[position].zero[0].y = cur_map.zero[0].y + 2;
                maplist[position].zero[1].y = cur_map.zero[1].y + 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[1].y < y[temp] ? SCALE1 : -SCALE1);
                break;
            }
            case 'r':{
                maplist[position].zero[0].y = cur_map.zero[0].y - 2;
                maplist[position].zero[1].y = cur_map.zero[1].y - 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[1].y > y[temp] ? SCALE1 : -SCALE1);
                break;
            }
            default:{
                printf("error!\n");
                break;
            }
        }
    }
    else {
        switch(direction){
            case 'u':{
                maplist[position].zero[select].x = cur_map.zero[select].x + 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[select].x < x[temp] ? SCALE2 : -SCALE2);
                break;
            }
            case 'd':{
                maplist[position].zero[select].x = cur_map.zero[select].x - 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[select].x > x[temp] ? SCALE2 : -SCALE2);
                break;
            }
            case 'l':{
                maplist[position].zero[select].y = cur_map.zero[select].y + 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[select].y < y[temp] ? SCALE2 : -SCALE2);
                break;
            }
            case 'r':{
                maplist[position].zero[select].y = cur_map.zero[select].y - 1;
                maplist[position].h_value = cur_map.h_value + (cur_map.zero[select].y > y[temp] ? SCALE2 : -SCALE2);
                break;
            }
            default:{
                printf("error!\n");
                break;
            }
        }
    }
    maplist[position].g_value = cur_map.g_value + 1;
    maplist[position].f_value = maplist[position].h_value + maplist[position].g_value;
    if(maplist[position].zero[0].x>=maplist[position].zero[1].x && maplist[position].zero[0].y>=maplist[position].zero[1].y){
        temp = maplist[position].zero[0].x;
        maplist[position].zero[0].x = maplist[position].zero[1].x;
        maplist[position].zero[1].x = temp;
        temp = maplist[position].zero[0].y;
        maplist[position].zero[0].y = maplist[position].zero[1].y;
        maplist[position].zero[1].y = temp;
    }
    if(maplist[position].h_value == 0){
        finish = 1;
        final_pos = position;
    }
}


void search(){
    int i;
    int flag[4] = {0, 0, 0, 0};
    mapmsg cur_map = maplist[cur_pos];
    char base[2] = {cur_map.zero[0].x * SIZE + cur_map.zero[0].y, cur_map.zero[1].x * SIZE + cur_map.zero[1].y};
    for (i = 0; i < 2; i++){
        if ((cur_map.zero[i].x > 0) && cur_map.map[base[i] - SIZE] != 0)
        {
            if (cur_map.map[base[i] - SIZE] != 7) move(i, 'd');
            else flag[0]++;
        }
        if ((cur_map.zero[i].x < 4) && cur_map.map[base[i] + SIZE] != 0)
        {
            if (cur_map.map[base[i] + SIZE] != 7) move(i, 'u');
            else flag[1]++;
        }
        if ((cur_map.zero[i].y > 0) && cur_map.map[base[i] - 1] != 0)
        {
            if (cur_map.map[base[i] - 1] != 7) move(i, 'r');
            else flag[2]++;
        }
        if ((cur_map.zero[i].y < 4) && cur_map.map[base[i] + 1] != 0)
        {
            if (cur_map.map[base[i] + 1] != 7) move(i, 'l');
            else flag[3]++;
        }
    }
    if(flag[0] == 2)
        move(2, 'd');
    if(flag[1] == 2)
        move(2, 'u');
    if(flag[2] == 2)
        move(2, 'r');
    if(flag[3] == 2)
        move(2, 'l');
    maplist[cur_pos].f_value = INFTY; //set invalid
}

void astar(){
    int i,min;
    int k = 1;
    while(finish != 1) {
        for (i = 0, min = INFTY; i <= position; i++){
            if(maplist[i].f_value < min){
                min = maplist[i].f_value;
                cur_pos = i;
            }
        }
        search();
        k++;
    }
    get_result(final_pos);
    get_result2(final_pos);
}
int main(){
    char input[25];
    int i;
    for (i = 0; i < 5; i++){
        fscanf(input_file, "%d,%d,%d,%d,%d\n", &input[5*i],&input[5*i+1],&input[5*i+2],&input[5*i+3],&input[5*i+4]);
    }
    for (i = 0; i < 25; i++){
        maplist[0].map[i] = input[i];
    }
    for (i = 0; i < 25; i++){
        if(maplist[0].map[i] == 0){
            maplist[0].zero[0].x = i / SIZE;
            maplist[0].zero[0].y = i % SIZE;
            //printf("set 1:%d %d\n", maplist[0].x_1, maplist[0].y_1);
            break;
        }
    }
    maplist[0].hash = 0;
    for (int i = 0; i < 25; i++){
        for(int j = 0; j <= i; j++){
            maplist[0].hash += input[i];
        }
    }
    for (i++; i < 25; i++){
        if(maplist[0].map[i] == 0){
            maplist[0].zero[1].x = i / SIZE;
            maplist[0].zero[1].y = i % SIZE;
            //printf("set 2:%d %d\n", maplist[0].x_2, maplist[0].y_2);
            break;
        }
    }
    maplist[0].h_value = measure(maplist[0].map);
    maplist[0].g_value = 0;
    maplist[0].f_value = maplist[0].h_value + maplist[0].g_value;
    printf("h_value: %d\n", maplist[0].h_value);
    astar();
    end_time = time(0);
    printf("\ntime: %lds\n\n\n", end_time - start_time);
    return 0;
}