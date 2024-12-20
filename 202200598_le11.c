/*

#----------------------------------------------------------------------------------------------------------------#
# Filename        : 202200598.le11.c                                                                                                 #
# Author          : Gabriel Angelo Catimbang                                                                                                   #        
# Last Modified   : Dec. 20, 2024                                                                     #
# Description     : This code contains the modifed program for the threads lab exercise                   #
# Honor Code      : This is my own program. I have not received any        #
#                   unauthorized help in completing this work. I have not           #
#                   copied from my classmate, friend, nor any unauthorized      #
#                   resource. I am well aware of the policies stipulated              #
#                   in the handbook regarding academic dishonesty.                  #                   
#                   If proven guilty, I won't be credited any points for                  #
#                   this exercise.                                                                               #
#-------------------------------------------------------------------------------------------------------------#*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 10
#define MAX_CASES 100
#define MAX_NUMBERS 1000

typedef struct {
    int case_num;
    int n;
    double numbers[MAX_NUMBERS];
} CaseData;

CaseData cases[MAX_CASES];
int num_cases;

void* compute_range(void* arg) {
    CaseData* data = (CaseData*) arg;
    double min = data->numbers[0], max = data->numbers[0];
    for (int i = 1; i < data->n; i++) {
        if (data->numbers[i] < min) min = data->numbers[i];
        if (data->numbers[i] > max) max = data->numbers[i];
    }
    printf("I'm thread 0. RANGE = %.2f\n", max - min);
    return NULL;
}

void* compute_mean(void* arg) {
    CaseData* data = (CaseData*) arg;
    double sum = 0;
    for (int i = 0; i < data->n; i++) {
        sum += data->numbers[i];
    }
    printf("I'm thread 1. MEAN = %.2f\n", sum / data->n);
    return NULL;
}

void* compute_std(void* arg) {
    CaseData* data = (CaseData*) arg;
    double sum = 0, mean, variance = 0;
    for (int i = 0; i < data->n; i++) {
        sum += data->numbers[i];
    }
    mean = sum / data->n;
    for (int i = 0; i < data->n; i++) {
        variance += pow(data->numbers[i] - mean, 2);
    }
    variance /= data->n;
    printf("I'm thread 2. STD = %.2f\n", sqrt(variance));
    return NULL;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("le11.in", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fscanf(file, "%d", &num_cases);
    for (int i = 0; i < num_cases; i++) {
        fscanf(file, "%d", &cases[i].n);
        for (int j = 0; j < cases[i].n; j++) {
            fscanf(file, "%lf", &cases[i].numbers[j]);
        }
        cases[i].case_num = i + 1;
    }
    fclose(file);

    for (int i = 0; i < num_cases; i++) {
        printf("CASE %d:\n", cases[i].case_num);
        pthread_t threads[3];
        pthread_create(&threads[0], NULL, compute_range, (void*)&cases[i]);
        pthread_create(&threads[1], NULL, compute_mean, (void*)&cases[i]);
        pthread_create(&threads[2], NULL, compute_std, (void*)&cases[i]);
        for (int j = 0; j < 3; j++) {
            pthread_join(threads[j], NULL);
        }
    }

    return 0;
}