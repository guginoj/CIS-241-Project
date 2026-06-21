#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_YEARS 15 // Limiting years for yearly statistics to hold


// Structure to carry year statistics
typedef struct {
    int year;
    double ratio_sum;
    int count;
} YearStat;

// Function Prototypes
void print_yearly_stats(YearStat stats[], int size);
void print_financial_advice(float pcRatio);
int find_year_index(YearStat stats[], int size, int year);


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
    YearStat stats[MAX_YEARS];
    int year_count = 0;

    fgets(line, sizeof(line), file); // Skip header line

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%19[^,],%lf,%d,%d,%d", date, &ratio, &put, &call, &options);

        printf("Date: %8s, Ratio: %.2f, Put: %-7d, Call: %-7d, Options: %-8d", date, ratio, put, call, options);

        print_financial_advice(ratio);

        // Find the current year of the line
        int month, day, year;
        sscanf(date, "%d/%d/%d", &month, &day, &year);
        year += 2000;

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

        // Yearly statistics calculations
        int index = find_year_index(stats, year_count, year);

        if (index == -1) {
            if (year_count < MAX_YEARS) {
                stats[year_count].year = year;
                stats[year_count].ratio_sum = ratio;
                stats[year_count].count = 1;
                year_count++;
            }
        } else {
            stats[index].ratio_sum += ratio;
            stats[index].count++;
        }
    }

    ratio_average = ratio_sum / count;
    call_average = call_sum / count;
    put_average = put_sum / count;
    options_average = options_sum / count;

    stddev_ratio = sqrt((ratio_square_sum / count) - (ratio_average * ratio_average));


    printf("\nAverage Ratio: %.2f, Average Put: %.2f, Average Call: %.2f, Average Options: %.2f\n",
           ratio_average, put_average, call_average, options_average);
    printf("Max Ratio: %.2f, Min Ratio: %.2f\n", max_ratio, min_ratio);
    printf("Standard Deviation of Ratio: %.2f\n", stddev_ratio);

    // Display yearly statistics
    print_yearly_stats(stats, year_count);

    fclose(file);
    return 0;
}


// Function to find year index
int find_year_index(YearStat stats[], int size, int year) {
    for (int i = 0; i < size; i++) {
        if (stats[i].year == year)
            return i;
    }
    return -1;
}

// Function to printout formatted yearly statistics
void print_yearly_stats(YearStat stats[], int size) {
    printf("\n===== YEARLY ANALYSIS =====\n");

    for (int i = 0; i < size; i++) {
        double avg = stats[i].ratio_sum / stats[i].count;

        printf("Year %d -> Avg Ratio: %.3f | ", stats[i].year, avg);

        if (avg > 1.0)
            printf("Bearish\n");
        else
            printf("Bullish\n");
    }
}

void print_financial_advice (float pcRatio){
    static int prevSntmnt = 1; // 0 for Bearish, 1 for neither, 2 for Bullish

    if(prevSntmnt == 1 && (pcRatio > 0.6 && pcRatio < 0.8)){
        printf(" Hold\n");
    }
    else if(prevSntmnt == 1 && pcRatio < 0.6){
        printf(" Buy\n");
    }
    else if(prevSntmnt == 1 && pcRatio > 0.8){
        printf(" Sell\n");
    }
    else if(prevSntmnt == 0 && (pcRatio > 0.6 && pcRatio < 0.8)){
        printf(" Buy\n");
    }
    else if(prevSntmnt == 0 && pcRatio < 0.6){
        printf(" Buy\n");
    }
    else if(prevSntmnt == 0 && pcRatio > 0.8){
        printf(" Hold\n");
    }
    else if(prevSntmnt == 2 && (pcRatio > 0.6 && pcRatio < 0.8)){
        printf(" Sell\n");
    }
    else if(prevSntmnt == 2 && pcRatio < 0.6){
        printf(" Hold\n");
    }
    else if(prevSntmnt == 2 && pcRatio > 0.8){
        printf(" Sell\n");
    }

    if(pcRatio < 0.6){
        prevSntmnt = 2;
    }
    else if(pcRatio > 0.8){
        prevSntmnt = 0;
    }
    else{
        prevSntmnt = 1;
    }
}
