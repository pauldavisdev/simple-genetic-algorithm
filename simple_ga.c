#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define N 100
 
#define P 100
 
#define PROB_C 0.7
 
#define PROB_M 0.01
 
typedef struct {
    int gene[N];
    int fitness;
} individual;

typedef struct {
    int total;
    int max;
    float average;
} fitness_info;
 
void generate_random_population(individual *population);
void print_generation(individual * generation);
void calculate_fitness(individual *individual);
void tournament_selection(individual *population, individual *offspring);
void crossover(individual *offspring);
void mutate(individual *offspring);
void print_individual(individual *individual);
void get_fitness_stats(individual *population, fitness_info *current_fitness_info);
 
int main() {

    srand(123);

    individual population[P];
    individual offspring[P];
    fitness_info current_fitness_info;
    int number_of_generations = 0;

    generate_random_population(population);
    
    get_fitness_stats(population, &current_fitness_info);

    printf("\nTotal:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
            current_fitness_info.total, current_fitness_info.max, current_fitness_info.average);

    while(current_fitness_info.max < N) {
        number_of_generations++;

        tournament_selection(population, offspring);

        crossover(offspring);

        mutate(offspring);

        get_fitness_stats(offspring, &current_fitness_info);

        printf("\nGeneration %d\nTotal:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
                number_of_generations ,current_fitness_info.total, current_fitness_info.max, current_fitness_info.average);

        memcpy(&population, &offspring, sizeof(offspring));

        
    }

    printf("\nGoal reached after %d generations\n", number_of_generations);

    return 0;
}

void generate_random_population(individual *population) {

    int i, j;

    for(i = 0; i < P; i++) {
        printf("%d\t:\t", i + 1);
        for(j = 0; j < N; j++) {
            population[i].gene[j] = rand() % 2;
            printf("%d", population[i].gene[j]);
        }

        calculate_fitness(&population[i]);

        printf("\t:\tFitness is %d\n", population[i].fitness);
    }
}

void tournament_selection(individual *population, individual *offspring) {
    
    int i, parent_1, parent_2;

    for(i = 0; i < P; i++) {
        parent_1 = rand() % P;
        parent_2 = rand() % P;

        if(population[parent_1].fitness >= population[parent_2].fitness) {
            offspring[i] = population[parent_1];
        } else {
            offspring[i] = population[parent_2];
        }
    }

}

void crossover(individual *offspring) {

    float random;

    int i, j, k, crossover_point;
    
    int l = 0;

    int random_offspring;

    individual parent_1, parent_2, temp;

    for(i = 0; i < P; i++) {
        random = (float)rand() / RAND_MAX;
        if(random <= PROB_C) {

            crossover_point = rand() % N;
            random_offspring = rand() % P;
            parent_1 = offspring[i];
            parent_2 = offspring[random_offspring];
            temp = parent_1;

            // printf("\n\ncrossover point is %d\n", crossover_point);

            // printf("\nOld parent 1: ");
            // print_individual(&parent_1);
            // printf("\nOld parent 2: ");
            // print_individual(&parent_2);

            for(j = crossover_point; j < N; j++) {
                parent_1.gene[j] = parent_2.gene[j];
            }

            for(k = crossover_point; k < N; k++) {
                parent_2.gene[k] = temp.gene[k];
            }

            calculate_fitness(&parent_1);
            calculate_fitness(&parent_2);

            // printf("\nNew parent 1: ");
            // print_individual(&parent_1);
            // printf("\nNew parent 2: ");
            // print_individual(&parent_2);

            l++;
           
        }
    }
    printf("crossover applied %d times\n", l);
}

void mutate(individual *offspring) {
    float random;
    int i, j;
    int k = 0;
    for(i = 0; i < P; i++) {
        for(j = 0; j < N; j++) {
            random = (float)rand() / RAND_MAX;
            if(random <= PROB_M) {
                offspring[i].gene[j] = 1 - offspring[i].gene[j];
                k++;
            }
        }
        calculate_fitness(&offspring[i]);
        
    }
    printf("%d bits mutated\n", k);
    
}

void calculate_fitness(individual *individual) {
    
    int i = 0;

    int fitness = 0;

    for(i = 0; i < N; i++) {
        if(individual->gene[i] == 1) {
            fitness++;
        }
    }
    individual->fitness = fitness;
}

void print_individual(individual *individual) {

    int i;

    for(i = 0; i < N; i++) {
        printf("%d", individual->gene[i]);
    }
    printf("\n");
}

void get_fitness_stats(individual *population, fitness_info *current_fitness_info) {

    int i;

    current_fitness_info->total = 0;
    current_fitness_info->max = 0;
    current_fitness_info->average = 0;

    for(i = 0; i < P; i++) {
        current_fitness_info->total += population[i].fitness;
        if(population[i].fitness > current_fitness_info->max) {
            current_fitness_info->max = population[i].fitness;
        }
    }

    current_fitness_info->average = (float)current_fitness_info->total / P;
}