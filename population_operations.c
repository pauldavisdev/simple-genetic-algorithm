#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "population_operations.h"

void generate_random_population(individual *population) {

    int i, j;

    for(i = 0; i < P; i++) {

        for(j = 0; j < N; j++) {

            population[i].gene[j] = rand() % 2;
        }
        
        calculate_individual_fitness(&population[i]);
    }
}

void print_generation(individual *generation, fitness_info *current_fitness_info) {

    int i, j;

    for(i = 0; i < P; i++) {

        printf("%d\t:\t", i + 1);

        for(j = 0; j < N; j++) {

            printf("%d", generation[i].gene[j]);
        }

        printf(" : fitness is %d\n", generation[i].fitness);
    }

    printf("Total:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
        current_fitness_info->total, current_fitness_info->max, current_fitness_info->average);
}

void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info) {
    
    int i, j, selection_point, running_total;

    for(i = 0; i < P; i++) {

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

            calculate_individual_fitness(&parent_1);

            calculate_individual_fitness(&parent_2);

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

        calculate_individual_fitness(&offspring[i]);
    }
    printf("%d bits mutated\n", k);
    
}

void calculate_individual_fitness(individual *individual) {
    
    int i;

    int fitness = 0;

    for(i = 0; i < N; i++) {

        if(individual->gene[i] == 1) {
            fitness += pow(2, i);
        }

    }

    fitness = pow(fitness, 2);

    individual->fitness = fitness;
}

void print_individual(individual *individual) {

    int i;

    for(i = 0; i < N; i++) {
        printf("%d", individual->gene[i]);
    }

    printf("\n");
}

void calculate_population_fitness(individual *population, fitness_info *current_fitness_info) {

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

    fprintf(p, "set yrange [0:66000]\nset title \"Max Fitness by Generation\"\n set key left\n set xlabel \"Number of Generations\"\n set ylabel \"Max Fitness\"\n plot '-' smooth csplines\n");

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