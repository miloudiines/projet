#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 10

// Structure pour les arguments des threads
typedef struct {
    int thread_id;
    int n1, m1, n2, m2;
    int* matrix_B;
    int* matrix_C;
    int* matrix_A;
    int* buffer_T;
} ThreadArgs;

// Fonction producteur
void* producer(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;

    int row = thread_args->thread_id;

    // Calculer la ligne de la matrice résultante A
    for (int col = 0; col < thread_args->m2; ++col) {
        int result = 0;
        for (int k = 0; k < thread_args->m1; ++k) {
            result += thread_args->matrix_B[row * thread_args->m1 + k] * 
                      thread_args->matrix_C[k * thread_args->m2 + col];
        }
        // Placer le résultat dans le tampon T
        thread_args->buffer_T[row * thread_args->m2 + col] = result;
    }

    pthread_exit(NULL);
}

// Fonction consommateur
void* consumer(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;

    // Copier les résultats du tampon T dans la matrice résultante A
    for (int i = 0; i < thread_args->n1; ++i) {
        for (int j = 0; j < thread_args->m2; ++j) {
            thread_args->matrix_A[i * thread_args->m2 + j] = 
                thread_args->buffer_T[i * thread_args->m2 + j];
        }
    }

    pthread_exit(NULL);
}

int main() {
    // Dimensions des matrices
    int n1 = 3, m1 = 3;
    int n2 = 3, m2 = 3;

    // Vérifier que le produit de matrices est possible
    if (m1 != n2) {
        printf("Le produit de matrices n'est pas possible avec les dimensions fournies.\n");
        return 1;
    }

    // Initialisation des matrices avec des valeurs aléatoires
    int* matrix_B = (int*)malloc(n1 * m1 * sizeof(int));
    int* matrix_C = (int*)malloc(n2 * m2 * sizeof(int));
    int* matrix_A = (int*)malloc(n1 * m2 * sizeof(int));

    for (int i = 0; i < n1 * m1; ++i) {
        matrix_B[i] = rand() % 10; // Valeurs aléatoires entre 0 et 9
    }

    for (int i = 0; i < n2 * m2; ++i) {
        matrix_C[i] = rand() % 10; // Valeurs aléatoires entre 0 et 9
    }

    // Tampon pour les résultats intermédiaires
    int* buffer_T = (int*)malloc(n1 * m2 * sizeof(int));

    // Création des threads producteurs
    pthread_t producer_threads[MAX_THREADS];
    ThreadArgs producer_args[MAX_THREADS];

    for (int i = 0; i < n1; ++i) {
        producer_args[i].thread_id = i;
        producer_args[i].n1 = n1;
        producer_args[i].m1 = m1;
        producer_args[i].n2 = n2;
        producer_args[i].m2 = m2;
        producer_args[i].matrix_B = matrix_B;
        producer_args[i].matrix_C = matrix_C;
        producer_args[i].matrix_A = matrix_A;
        producer_args[i].buffer_T = buffer_T;

        pthread_create(&producer_threads[i], NULL, producer, (void*)&producer_args[i]);
    }

    // Création du thread consommateur
    pthread_t consumer_thread;
    ThreadArgs consumer_args;
    consumer_args.n1 = n1;
    consumer_args.m2 = m2;
    consumer_args.buffer_T = buffer_T;
    consumer_args.matrix_A = matrix_A;

    pthread_create(&consumer_thread, NULL, consumer, (void*)&consumer_args);

    // Attente de la fin des threads producteurs
    for (int i = 0; i < n1; ++i) {
        pthread_join(producer_threads[i], NULL);
    }

    // Attente de la fin du thread consommateur
    pthread_join(consumer_thread, NULL);

    // Affichage des matrices
    printf("Matrix B:\n");
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            printf("%d ", matrix_B[i * m1 + j]);
        }
        printf("\n");
    }

    printf("\nMatrix C:\n");
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            printf("%d ", matrix_C[i * m2 + j]);
        }
        printf("\n");
    }

    printf("\nMatrix A (result):\n");
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            printf("%d ", matrix_A[i * m2 + j]);
        }
        printf("\n");
    }

    // Libération de la mémoire
    free(matrix_B);
    free(matrix_C);
    free(matrix_A);
    free(buffer_T);

    return 0;
}
