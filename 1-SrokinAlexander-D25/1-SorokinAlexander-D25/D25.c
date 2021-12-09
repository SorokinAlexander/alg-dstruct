#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

const char nl = '\n';
int  U, B, K;
int* weights;
int* prices;
int all_sum;
int all_weight;


int Read(const char* file_name) {
    FILE* file = fopen(file_name, "r");

    fscanf(file, "%d %d %d\n", &U, &B, &K);

    weights = (int*)malloc(sizeof(int) * U);
    if (weights == NULL) {
        printf("No memory allocated!");
        return 0;
    }

    prices = (int*)malloc(sizeof(int) * U);
    if (prices == NULL) {
        printf("No memory allocated!");
        return 0;
    }

    for (int i = 0; i < U; i++) {
        fscanf(file, "%d", &weights[i]);
    }
    for (int i = 0; i < U; i++) {
        fscanf(file, "%d", &prices[i]);
    }
    fclose(file);
}

int Min(int* a) {
    int id = -1;
    int min = 1000000;
    for (int i = 0; i < U; i++)
        if (a[i] < min && a[i] != 0) {
            id = i;
            min = a[i];
        }
    return id;
}

int Sum(int  U, int B, int K) {
    all_sum = 0;
    all_weight = 0;
    int min_weight = 100000;

    for (int i = 0; i < U; i++) {
        all_sum += prices[i];
        all_weight += weights[i];
        if (min_weight > weights[i])
            min_weight = weights[i];
    }

    if ((all_sum >= K) && all_weight <= B) {
        FILE* values;

        char name[] = "output.txt";
        values = fopen(name, "w");
        for (int i = 1; i <= U; i++)
            fprintf(values, "%d ", i);

        fclose(values);
    }

    if (min_weight > B || all_sum < K) {
        char name[] = "output.txt";
        FILE* values = fopen(name, "w");
        fprintf(values, "%d ", 0);

        fclose(values);
    }

    int cur_weight = all_weight;
    int cur_sum = all_sum;
    int flag = 1;

    while (flag != -1 && flag != -2) {
        int min_price = Min(prices);
        cur_weight -= weights[min_price];
        cur_sum -= prices[min_price];
        weights[min_price] = 0;
        prices[min_price] = 0;

        if (cur_weight - B <= 0) {
            flag = -2;
            if (cur_sum >= K) {
                flag = -1;
            }
        }
    }

    if (flag == -1) {
        char name[] = "output.txt";
        FILE* values = fopen(name, "w");
        for (int i = 0; i < U; i++) {
            if (weights[i] > 0)
                fprintf(values, "%d ", i + 1);
        }
        fclose(values);
        return 0;
    }
    else if (flag == -2) {
        char name[] = "output.txt";
        FILE* values = fopen(name, "w");
        fprintf(values, "%d ", 0);
        fclose(values);
    }
    return 1;
}

int main() {
    Read("input.txt");
    Sum(U, B, K);
}