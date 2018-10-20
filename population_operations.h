#ifndef POPULATION_OPERATIONS_H

#define POPULATION_OPERATIONS_H

#define N 100

#define P 100

#define G 300

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

void print_generation(individual *generation, fitness_info *current_fitness_info);

void calculate_individual_fitness(individual *individual);

void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info);

void tournament_selection(individual *population, individual *offspring);

void crossover(individual *offspring);

void mutate(individual *offspring);

void calculate_population_fitness(individual *population, fitness_info *current_fitness_info);

void print_individual(individual *individual);

void plot_graph(int *x, int *y, int len);

#endif /* POPULATION_OPERATIONS */