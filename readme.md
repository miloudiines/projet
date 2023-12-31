/* miloudi ines 
groupe 01 */

Q1: Quelles sont les structures de données à utiliser ?

matrix_B: Tableau unidimensionnel pour la matrice B.
matrix_C: Tableau unidimensionnel pour la matrice C.
matrix_A: Tableau unidimensionnel pour la matrice résultante A.
buffer_T: Tableau unidimensionnel pour le tampon intermédiaire T.

Q2: Comment allez-vous protéger l'accès à ces données?

pthread_mutex_t mutex_matrices: Mutex pour protéger l'accès aux matrices B, C et A.
pthread_mutex_t mutex_buffer_T: Mutex pour protéger l'accès au tampon T.

Q3: Quels sont les risques?
Risque de course critique : Accès simultané non synchronisé aux données partagées.
Risque de blocage (Deadlock) : Possibilité de blocage si les mutex ne sont pas correctement gérés.
Risque de consistance des données : Incohérence des données entre les threads en cas de synchronisation inadéquate.
Risque de performance : Mauvaise gestion des verrous pouvant entraîner des problèmes de performance.
