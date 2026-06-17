#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    FILE *file = fopen("SPY241Project.txt", "r");

    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    char line[100];

    char date[20];
    double ratio, ratio_average, call_average, put_average, options_average;
    double ratio_sum = 0, call_sum = 0, put_sum = 0, options_sum = 0, max_ratio = 0, min_ratio = 2, stddev_ratio = 0, ratio_square_sum = 0;
    int put, call, options, count = 0;

    fgets(line, sizeof(line), file); // Skip header line

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%19[^,],%lf,%d,%d,%d", date, &ratio, &put, &call, &options);

        printf("Date: %s, Ratio: %.2f, Put: %d, Call: %d, Options: %d\n", date, ratio, put, call, options);
        

        if (ratio > max_ratio) {
            max_ratio = ratio;
        }
        if (ratio < min_ratio) {
            min_ratio = ratio;
        }

        ratio_square_sum += ratio * ratio;
        count++;
        ratio_sum += ratio;
        call_sum += call;
        put_sum += put;
        options_sum += options;
    }


    ratio_average = ratio_sum / count;
    call_average = call_sum / count;
    put_average = put_sum / count;
    options_average = options_sum / count;

    stddev_ratio = sqrt((ratio_square_sum / count) - (ratio_average * ratio_average));


    printf("Average Ratio: %.2f, Average Put: %.2f, Average Call: %.2f, Average Options: %.2f\n",
           ratio_average, put_average, call_average, options_average);
    printf("Max Ratio: %.2f, Min Ratio: %.2f\n", max_ratio, min_ratio);
    printf("Standard Deviation of Ratio: %.2f\n", stddev_ratio);
    
    fclose(file);
    return 0;
}