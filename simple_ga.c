#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define N 100
 
#define P 100

#define G 10000
 
#define PROB_C 0.8
 
#define PROB_M 0.0019
 
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
void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info);
void tournament_selection(individual *population, individual *offspring);
void crossover(individual *offspring);
void mutate(individual *offspring);
void get_fitness_stats(individual *population, fitness_info *current_fitness_info);
void print_individual(individual *individual);
void plot_graph(int *x, int *y, int len);

int main(int argc, char *argv[]) {

    srand(564);

    individual population[P];

    individual offspring[P];
    
    fitness_info current_fitness_info;
    
    int number_of_generations = 0;

    int i, j;

    int x[G];

    int y[G];

    generate_random_population(population);
    
    get_fitness_stats(population, &current_fitness_info);

    printf("\nTotal:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
            current_fitness_info.total, current_fitness_info.max, current_fitness_info.average);

    x[number_of_generations] = number_of_generations;

    y[number_of_generations] = current_fitness_info.max;

    while(number_of_generations < G - 1) {
        
        number_of_generations++;

        roulette_wheel_selection(population, offspring, &current_fitness_info);

        //tournament_selection(population, offspring);

        printf("\nGeneration %d\n", number_of_generations);

        crossover(offspring);

        mutate(offspring);

        get_fitness_stats(offspring, &current_fitness_info);

        //printf("x is %d\t:\ty is %d", x[0], y[0]);

        for(i = 0; i < P; i++) {
            for(j = 0; j < N; j++) {
                printf("%d", offspring[i].gene[j]);
            }
            printf(" : fitness is %d\n", offspring[i].fitness);
        }

        printf("Total:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
                current_fitness_info.total, current_fitness_info.max, current_fitness_info.average);

        x[number_of_generations] = number_of_generations;

        y[number_of_generations] = current_fitness_info.max;

        memcpy(&population, &offspring, sizeof(offspring));        
    }

    plot_graph(x, y, G);

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

void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info) {
    
    int i, j, selection_point, running_total;

    for(i = 0; i < P; i++)
    {
        selection_point = rand() % current_fitness_info->total;

        running_total = 0;

        j = 0;

        while(running_total <= selection_point) {

            running_total += population[j].fitness;
            
            j++;
        }

        offspring[i] = population[j - 1];
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
    
    int i;

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

void plot_graph(int *x, int *y, int len) {
    
    FILE *p = popen("gnuplot -persistent", "w");

    int i;
    
    int max_fitness = 0;

    int max_generation = 0;

    fprintf(p, "set title \"Max Fitness by Generation\"\n set key left\n set xlabel \"Number of Generations\"\n set ylabel \"Max Fitness\"\n plot '-' smooth csplines\n");

    for(i = 0; i < len; i++) {
        fprintf(p, "%d %d\n", x[i], y[i]);
    }

    for(i = 0; i < len; i++) {
        if(y[i] > max_fitness) {
            max_fitness = y[i];
            max_generation = x[i];
        }
        //printf("%d %d\n", x[i], y[i]);
    }    

    printf("Max fitness of %d reached at generation %d\n", max_fitness, max_generation);

    fflush(p);

    fprintf(p, "e");

    pclose(p);
}