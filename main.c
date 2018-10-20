#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "population_operations.h"

int main(int argc, char *argv[]) {

    srand(564);

    individual population[P];

    individual offspring[P];
    
    fitness_info current_fitness_info;
    
    int number_of_generations = 0;

    int x[G];

    int y[G];

    generate_random_population(population);
    
    while(number_of_generations < G) {
        
        calculate_population_fitness(population, &current_fitness_info);

        print_generation(population, &current_fitness_info);

        x[number_of_generations] = number_of_generations;

        y[number_of_generations] = current_fitness_info.max;

        number_of_generations++;

        // roulette_wheel_selection(population, offspring, &current_fitness_info);

        tournament_selection(population, offspring);

        printf("\nGeneration %d\n", number_of_generations);

        crossover(offspring);

        mutate(offspring);

        memcpy(&population, &offspring, sizeof(offspring));        
    }

    plot_graph(x, y, G);

    return 0;
}