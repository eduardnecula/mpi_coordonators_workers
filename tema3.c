#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COORDONATOR_0 0
#define COORDONATOR_1 1
#define COORDONATOR_2 2

// #define CLUSTER_0 "/home/student/Desktop/9.tema_3_apd/tema3/tests/test1/cluster0.txt"
// #define CLUSTER_1 "/home/student/Desktop/9.tema_3_apd/tema3/tests/test1/cluster1.txt"
// #define CLUSTER_2 "/home/student/Desktop/9.tema_3_apd/tema3/tests/test1/cluster2.txt"

#define CLUSTER_0 "cluster0.txt"
#define CLUSTER_1 "cluster1.txt"
#define CLUSTER_2 "cluster2.txt"

// calculare minim
int min(int a, int b) {
    if (a < b) return a;
    return b;
}

/**
 * Functia de afisare a topologie
 * Vrea:
 * rankul workerului
 * si 3 perechi,
 * formate din [lungimea workerului, numarul de workeri]
 */ 
void afisareTopologie(int rank, int nrRank0, int *vectorRank0, int nrRank1, int *vectorRank1,
            int nrRank2, int* vectorRank2) {

        char  buf[200];
        char  newBuf[10000] = "";
        snprintf(buf, sizeof(buf), "%d -> 0:", rank);
        strcat(newBuf, buf);

        for (int i = 0; i < nrRank0; i++) {
            if (i == 0 && nrRank0 != 1) {
                snprintf(buf, sizeof(buf), "%d,", vectorRank0[i]);
                strcat(newBuf, buf);
            } else if (nrRank0 == 1) {
                snprintf(buf, sizeof(buf), "%d", vectorRank0[i]);
                strcat(newBuf, buf);
            } else if (i == (nrRank0 - 1)) {
                snprintf(buf, sizeof(buf), "%d", vectorRank0[i]);
                strcat(newBuf, buf);
            } else {
                snprintf(buf, sizeof(buf), "%d,", vectorRank0[i]);
                strcat(newBuf, buf);
            }
        }

        strcat(newBuf, " 1:");
        for (int i = 0; i < nrRank1; i++) {
            if (i == 0 && nrRank1 != 1) {
                snprintf(buf, sizeof(buf), "%d,", vectorRank1[i]);
                strcat(newBuf, buf);
            } else if (nrRank1 == 1) {
                snprintf(buf, sizeof(buf), "%d", vectorRank1[i]);
                strcat(newBuf, buf);
            } else if (i == (nrRank1 - 1)) {
                snprintf(buf, sizeof(buf), "%d", vectorRank1[i]);
                strcat(newBuf, buf);
            } else {
                snprintf(buf, sizeof(buf), "%d,", vectorRank1[i]);
                strcat(newBuf, buf);
            }
        }

        strcat(newBuf, " 2:");
        for (int i = 0; i < nrRank2; i++) {
            if (i == 0 && nrRank2 != 1) {
                snprintf(buf, sizeof(buf), "%d,", vectorRank2[i]);
                strcat(newBuf, buf);
            } else if (nrRank2 == 1) {
                snprintf(buf, sizeof(buf), "%d", vectorRank2[i]);
                strcat(newBuf, buf);
            } else if (i == (nrRank2 - 1)) {
                snprintf(buf, sizeof(buf), "%d", vectorRank2[i]);
                strcat(newBuf, buf);
            } else {
                snprintf(buf, sizeof(buf), "%d,", vectorRank2[i]);
                strcat(newBuf, buf);
            }
        }

        strcat(newBuf, "\n");
        printf("%s", newBuf);
}

int main(int argc, char *argv[]) {

    int proceseCoordonator[100];
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    // vectorul de dublat
    int elementeDuble[60000];

    // fisierele din care o sa citesc
    FILE *cluster0, *cluster1, *cluster2;

    // status
    MPI_Status status;
    
    // identificator coordonatorul workerului
    int idCoordonator;

    // citire din fisier
    // Coordonatorul 0
    if (rank == COORDONATOR_0) {
        // dimensiune vector de dublat
        int dimensiuneVector = atoi(argv[1]);

        cluster0 = fopen(CLUSTER_0, "r");
        if (cluster0 == NULL) {
            printf("eroare citire fisier cluster 0");
            return -1;
        }
        // nr de procese de la rank 0
        int nrProcese = 0;
        fscanf(cluster0, "%d", &nrProcese);

        int procese[nrProcese];
        for (int i = 0; i < nrProcese; i++) {
            fscanf(cluster0, "%d", &procese[i]);
        }

        // rank-ul coordonatorului
        // trimit rangul workerilor mei
        int clasterMaster = 0;
        for (int i = 0; i < nrProcese; i++) {
            // data trimisa, nr elemente, tip, destinatie, tag, comunicator
            MPI_Send(&clasterMaster, 1, MPI_INT, procese[i], 100, MPI_COMM_WORLD);
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // rank 0 primeste dimensiunea de la rank 1
        int dimRank1 = 0;
        MPI_Recv(&dimRank1,
                1, // nr de elemente
                MPI_INT, // tip
                1, // de la cine primesc
                100, // tag
                MPI_COMM_WORLD,
                &status);

        // rank 0 primeste acum vectorul de la rank 1
        int vectorRank1[dimRank1];
        MPI_Recv(&vectorRank1,
                dimRank1,
                MPI_INT,
                1,
                100,
                MPI_COMM_WORLD,
                &status);

        // rank 0 primeste dimensiunea de la rank 2
        int dimRank2 = 0;
        MPI_Recv(&dimRank2,
                1, // nr elemente primite
                MPI_INT,
                2, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
        
        // rank 0 primeste acum vectorul de la rank 2
        int vectorRank2[dimRank2];
        MPI_Recv(&vectorRank2,
                dimRank2,
                MPI_INT,
                2,
                100,
                MPI_COMM_WORLD,
                &status);

        // afisare topologie
        afisareTopologie(rank, nrProcese, procese, dimRank1, vectorRank1,
                            dimRank2, vectorRank2);

        // rank 0 trimite coordonatorului 1 valoarea dimensiunii lui
        MPI_Send(&nrProcese, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    1, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        printf("M(%d,%d)\n", rank, 1);

        // rank 0 trimite coordonatorului 1 elementele lui
        MPI_Send(&procese, // data trimisa
                    nrProcese, // nr elemente
                    MPI_INT, // tip
                    1, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        printf("M(%d,%d)\n", rank, 1);

        // rank 0 trimite coordonatorului 2 valoarea dimesiunii lui
        MPI_Send(&nrProcese, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    2, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        printf("M(%d,%d)\n", rank, 2);
        
        // rank 0 trimite coordonatorului 2 elementele lui
        MPI_Send(&procese, // data trimisa
                    nrProcese, // nr elemente
                    MPI_INT, // tip
                    2, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        printf("M(%d,%d)\n", rank, 2);

        // dimensiunea rangului 0
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&nrProcese, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 0
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&procese, // data trimisa
                    nrProcese, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 1
        // dimensiunea rangului 1
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&dimRank1, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit elementele rank 1
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&vectorRank1, // data trimisa
                    dimRank1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 2
        // dimensiunea rangului 2
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&dimRank2, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit elementele rank 1
        for (int i = 0; i < nrProcese; i++) {
            // trimit mai intai dimensiunile de la Coordonator 0
            MPI_Send(&vectorRank2, // data trimisa
                    dimRank2, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // inceput Etapa 2 - calcule
        for (int i = 0; i < dimensiuneVector; i++) {
            elementeDuble[i] = i;
        }

        // impartite vector de workeri
        // numarul total de workeri
        int nrTotalWorkeri = 0;
        nrTotalWorkeri += nrProcese + dimRank1 + dimRank2;

        // trimit un mesaj coordinatorilor cu dimensiunea vectorului
        // pentru coordonator 1
        MPI_Send(&dimensiuneVector, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    1, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        // pentru logare in terminal
        printf("M(%d,%d)\n", rank, 1);

        // pentru coordonator 1
        MPI_Send(&dimensiuneVector, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    2, // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
        // pentru logare in terminal
        printf("M(%d,%d)\n", rank, 2);
        
        // impartire calcule
        int N = dimensiuneVector; // nr elemente
        int P = nrTotalWorkeri; // nr de workeri
        int nr1 = nrProcese;
        int nr2 = dimRank1;
        int nr3 = dimRank2;
        for (int i = 0; i < nrTotalWorkeri; i++) {
            int start = i * (double)N / P;
            int end = min((i + 1) * (double)N / P, N);
            // trimit workerilor de la COO 0
            if (nr1 > 0) {
                // trimit si dimensiunea vectorului
                MPI_Send(&dimensiuneVector, // data trimisa
                        1, // nr elemente
                        MPI_INT, // tip
                        procese[i], // destinatie
                        100, // tag
                        MPI_COMM_WORLD); // comunicator
                // pentru logare in terminal
                printf("M(%d,%d)\n", rank, procese[i]);

                // trimit mai intai dimensiunile de la Coordonator 0
                    // trimit start - de unde sa inceapa vectorul
                    MPI_Send(&start, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            procese[i], // destinatie
                            100, // tag
                            MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, procese[i]);

                    // trimit end - de unde sa termine vectorul
                    MPI_Send(&end, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            procese[i], // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, procese[i]);

                    // trimit si vectorul de modificat
                    MPI_Send(&elementeDuble, // data trimisa
                            dimensiuneVector, // nr elemente
                            MPI_INT, // tip
                            procese[i], // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, procese[i]);

                    // primesc vectorul
                    MPI_Recv(&elementeDuble,
                            dimensiuneVector,
                            MPI_INT,
                            procese[i],
                            100,
                            MPI_COMM_WORLD,
                            &status);
                nr1--;
                 // trimis ms lui COO 1
            } else if (nr2 > 0) {
                // trimit start - de unde sa inceapa vectorul
                    MPI_Send(&start, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            1, // destinatie
                            100, // tag
                            MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 1);

                    // trimit end - de unde sa termine vectorul
                    MPI_Send(&end, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            1, // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 1);

                    // trimit si vectorul de modificat
                    MPI_Send(&elementeDuble, // data trimisa
                            dimensiuneVector, // nr elemente
                            MPI_INT, // tip
                            1, // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 1);

                    // primesc vectorul
                    MPI_Recv(&elementeDuble,
                            dimensiuneVector,
                            MPI_INT,
                            1,
                            100,
                            MPI_COMM_WORLD,
                            &status);
                nr2--;
            } else

            // trimis ms lui COO 2
            if (nr3 > 0) {
                // trimit start - de unde sa inceapa vectorul
                    MPI_Send(&start, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            2, // destinatie
                            100, // tag
                            MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 1);

                    // trimit end - de unde sa termine vectorul
                    MPI_Send(&end, // data trimisa
                            1, // nr elemente
                            MPI_INT, // tip
                            2, // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 2);

                    // trimit si vectorul de modificat
                    MPI_Send(&elementeDuble, // data trimisa
                            dimensiuneVector, // nr elemente
                            MPI_INT, // tip
                            2, // destinatie
                            100, // tag
                        MPI_COMM_WORLD); // comunicator
                    // pentru logare in terminal
                    printf("M(%d,%d)\n", rank, 2);

                    // primesc vectorul
                    MPI_Recv(&elementeDuble,
                            dimensiuneVector,
                            MPI_INT,
                            2,
                            100,
                            MPI_COMM_WORLD,
                            &status);
                nr3--;
            }
        }        

        // convertire vector in string pentru afisare
        char  buf[200];
        char  newBuf[30000] = "";
        strcat(newBuf, "Rezultat: ");
        for (int i = 0; i < dimensiuneVector; i++) {
            snprintf(buf, sizeof(buf), "%d ", elementeDuble[i]);
            strcat(newBuf, buf);
        }
        strcat(newBuf, "\n");
        printf("%s", newBuf);        
  
        fclose(cluster0);
     } else if (rank == COORDONATOR_1) {
        int dimensiuneVector = 0;
        // citire fisier
        cluster1 = fopen(CLUSTER_1, "r");
        if (cluster1 == NULL) {
            printf("eroare citire fisier cluster 1");
            return -1;
        }

        int nrProcese = 0;
        fscanf(cluster1, "%d", &nrProcese);

        int procese[nrProcese];
        for (int i = 0; i < nrProcese; i++) {
            fscanf(cluster1, "%d", &procese[i]);
        }

        // trimit mesaj workerilor cine e sefu
        int clasterMaster = 1;
        for (int i = 0; i < nrProcese; i++) {
            // data trimisa, nr elemente, tip, destinatie, tag, comunicator
            MPI_Send(&clasterMaster,
                    1,
                    MPI_INT,
                    procese[i],
                    100,
                    MPI_COMM_WORLD);
            // pentru logare terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // rank 1 ii da dimensiunea lui rank 0
        MPI_Send(&nrProcese,
                1, // cate procese
                MPI_INT,
                0, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        // log terminal
        printf("M(%d,%d)\n", rank, 0);

        // rank 1 acum ii trimite vectorul de elemente lui rank 0
        MPI_Send(&procese,
                nrProcese,
                MPI_INT,
                0, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, 0);

        // rank 1 ii da dimensiunea lui rank 2
        MPI_Send(&nrProcese,
                1, // cate procese
                MPI_INT,
                2, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        // log terminal
        printf("M(%d,%d)\n", rank, 2);

        // rank 1 acum ii trimite vectorul de elemente lui rank 2
        MPI_Send(&procese,
                nrProcese,
                MPI_INT,
                2, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, 2);

        // trebuie sa stiu de ce valoare are lungime are coordonatorul 0
        int dimRank0 = 0;
        MPI_Recv(&dimRank0,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // trebuie sa stiu elementele coordonatorului 0
        int vectorRank0[dimRank0];
        MPI_Recv(&vectorRank0,
                dimRank0, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // trebuie sa stiu ce valoare are coordonatorul 2
        int dimRank2 = 0;
        MPI_Recv(&dimRank2,
                1, // nr elemente primite
                MPI_INT,
                2, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // trebuie sa stiu elementele coordonatorului 2
        int vectorRank2[dimRank2];
        MPI_Recv(&vectorRank2,
                dimRank2, // nr elemente primite
                MPI_INT,
                2, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // afisare topologie
        afisareTopologie(rank, dimRank0, vectorRank0, nrProcese, procese,
                dimRank2, vectorRank2);

        // trimit dimensiunea lui rank 0
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&dimRank0, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }
        
        // trimit rank 0
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&vectorRank0,
                dimRank0,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
                printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 1
        // trimit dimensiunea lui rank 1
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&nrProcese, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }
        
        // trimit rank 1
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&procese,
                nrProcese,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 2
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&dimRank2, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }
        
        // trimit rank 0
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&vectorRank2,
                dimRank2,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // primesc dimensiune vector
            MPI_Recv(&dimensiuneVector,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // primesc mesaj de la coordonator 0, pe care le voi trimite workerilor
        for (int i = 0; i < nrProcese; i++) {
            // trimit dimensiune vector
            MPI_Send(&dimensiuneVector,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            int start = 0;
            int end = 0;
            // primesc start si end
            MPI_Recv(&start,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            MPI_Recv(&end,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

            // primesc si vectorul
            MPI_Recv(&elementeDuble,
                dimensiuneVector, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            // voi trimite start si end
            MPI_Send(&start,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            MPI_Send(&end,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            // trimit si vectorul
            MPI_Send(&elementeDuble,
                    dimensiuneVector,
                    MPI_INT,
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            // primesc vectorul de la workeri
            MPI_Recv(&elementeDuble,
                dimensiuneVector, // nr elemente primite
                MPI_INT,
                procese[i], // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            // trimit vectorul la coordonator 0
            MPI_Send(&elementeDuble,
                    dimensiuneVector,
                    MPI_INT,
                    0, // destinatie
                    100, // tag
                    MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, 0);
        }

        fclose(cluster1);
    } else if (rank == COORDONATOR_2) {
        int dimensiuneVector = 0;
         // citire fisier
        cluster2 = fopen(CLUSTER_2, "r");
        if (cluster2 == NULL) {
            printf("eroare citire fisier cluster 2");
            return -1;
        }
        int nrProcese = 0;
        fscanf(cluster2, "%d", &nrProcese);

        int procese[nrProcese];
        for (int i = 0; i < nrProcese; i++) {
            fscanf(cluster2, "%d", &procese[i]);
        }
        
        int clasterMaster = 2;
        for (int i = 0; i < nrProcese; i++) {
            // data trimisa, nr elemente, tip, destinatie, tag, comunicator
            MPI_Send(&clasterMaster, 1, MPI_INT, procese[i], 100, MPI_COMM_WORLD);
            // logater terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // rank 2 ii da dimensiunea lui rank 0
        MPI_Send(&nrProcese, // data trimisa
                1, // nr elemente
                MPI_INT, // tip
                0, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        // logare terminal
        printf("M(%d,%d)\n", rank, 0);

        // rank 2 acum ii trimite vectorul de elemente lui rank 0
        //for (int i = 0; i < nrProcese; i++) {
        MPI_Send(&procese,
                    nrProcese,
                    MPI_INT,
                    0, // destinatie
                    100, // tag
                    MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, 0);

        // rank 2 ii da dimensiunea lui rank 1
        MPI_Send(&nrProcese,
                1, // cate procese
                MPI_INT,
                1, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        // log terminal
        printf("M(%d,%d)\n", rank, 1);

        // rank 2 acum ii trimite vectorul de elemente lui rank 1
        MPI_Send(&procese,
                nrProcese,
                MPI_INT,
                1, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, 1);

        // rank 2 primeste dimensiunea de la rank 0
        int dimRank0 = 0;
        MPI_Recv(&dimRank0,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
        
        // rank 2 primeste acum vectorul de la rank 0
        int vectorRank0[dimRank0];
        MPI_Recv(&vectorRank0,
                dimRank0,
                MPI_INT,
                0,
                100,
                MPI_COMM_WORLD,
                &status);
        

        // rank 2 primeste dimensiunea de la rank 1
        int dimRank1 = 0;
        MPI_Recv(&dimRank1,
                1, // nr elemente primite
                MPI_INT,
                1, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
        
        // rank 2 primeste acum vectorul de la rank 1
        int vectorRank1[dimRank1];
        MPI_Recv(&vectorRank1,
                dimRank1,
                MPI_INT,
                1,
                100,
                MPI_COMM_WORLD,
                &status);

        // afisare topologie
        afisareTopologie(rank, dimRank0, vectorRank0, dimRank1, vectorRank1,
                            nrProcese, procese);       

        // dimensiunea lui rank 0
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&dimRank0, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 0
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&vectorRank0,
                dimRank0,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 1
        // dimensiunea lui rank 1
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&dimRank1, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit elementele din rank 1
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&vectorRank1,
                dimRank1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit rank 2
        // dimensiunea lui rank 2
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&nrProcese, // data trimisa
                    1, // nr elemente
                    MPI_INT, // tip
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD); // comunicator
            // pentru logare in terminal
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // trimit elementele din rank 2
        for (int i = 0; i < nrProcese; i++) {
            MPI_Send(&procese,
                nrProcese,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
        }

        // primesc dimensiune vector
            MPI_Recv(&dimensiuneVector,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // primesc mesaj de la coordonator 0, pe care le voi trimite workerilor
        for (int i = 0; i < nrProcese; i++) {
            // trimit dimensiune vector
            MPI_Send(&dimensiuneVector,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            int start = 0;
            int end = 0;
            // primesc start si end
            MPI_Recv(&start,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            MPI_Recv(&end,
                1, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            MPI_Recv(&elementeDuble,
                dimensiuneVector, // nr elemente primite
                MPI_INT,
                0, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            // voi trimite start si end
            MPI_Send(&start,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[0]);

            MPI_Send(&end,
                1,
                MPI_INT,
                procese[i], // destinatie
                100, // tag
                MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);
            
            // trimit si vectorul
            MPI_Send(&elementeDuble,
                    dimensiuneVector,
                    MPI_INT,
                    procese[i], // destinatie
                    100, // tag
                    MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, procese[i]);

            // primesc vectorul de la workeri
            MPI_Recv(&elementeDuble,
                dimensiuneVector, // nr elemente primite
                MPI_INT,
                procese[i], // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
            
            // trimit vectorul la coordonator 0
            MPI_Send(&elementeDuble,
                    dimensiuneVector,
                    MPI_INT,
                    0, // destinatie
                    100, // tag
                    MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, 0);
        }

        fclose(cluster2);
    } else {
        // dimensiune vector
        int dimensiuneVector = 0;
        // rang trimis
        int clasterMaster;

        // ca sa stiu sa primesc numarul coordonatorului
        MPI_Recv(&clasterMaster, // identificatorul clasterului
                1, // nr de elemente
                MPI_INT, // tipul
                MPI_ANY_SOURCE, // de unde primesc
                100, // tag
                MPI_COMM_WORLD,
                &status);

        // primesc numarul de procese de la rank 0
        int nrRank0;
        MPI_Recv(&nrRank0,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
        //printf("nr procese : %d\n", nrProceseCoordonator);

        // primesc rank 0
        int vectorRank0[20];
        MPI_Recv(&vectorRank0,
                nrRank0, // nr de elemente
                MPI_INT, // tipul
                clasterMaster, // de unde primesc
                100, // tag
                MPI_COMM_WORLD,
                &status);

        // primesc numarul de procese de la rank 1
        int nrRank1;
        MPI_Recv(&nrRank1,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
                
        
        // primesc rank 2
        int vectorRank1[20];
        MPI_Recv(&vectorRank1,
                nrRank1, // nr de elemente
                MPI_INT, // tipul
                clasterMaster, // de unde primesc
                100, // tag
                MPI_COMM_WORLD,
                &status);

        // primesc numarul de procese de la rank 2
        int nrRank2;
        MPI_Recv(&nrRank2,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // primesc rank 1
        int vectorRank2[20];
        MPI_Recv(&vectorRank2,
                nrRank2, // nr de elemente
                MPI_INT, // tipul
                clasterMaster, // de unde primesc
                100, // tag
                MPI_COMM_WORLD,
                &status);
        
        // afisarea topologiei
        afisareTopologie(rank, nrRank0, vectorRank0, nrRank1, vectorRank1,
                        nrRank2, vectorRank2);

        // primesc dimensiunea vectorului
        MPI_Recv(&dimensiuneVector,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        // primesc start si end pe care il voi calcula
        int start = 0;
        int end = 0;
        MPI_Recv(&start,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        MPI_Recv(&end,
                1, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);
        
        // primesc vectorul pe care il voi calcula

        MPI_Recv(&elementeDuble,
                dimensiuneVector, // nr elemente primite
                MPI_INT,
                clasterMaster, // de la cine primeste
                100,
                MPI_COMM_WORLD,
                &status);

        for (int i = start; i < end; i++) {
            elementeDuble[i] = i * 2;
        }
        
        // trimit vectorul inapoi la fiecare coordonator
        // ce il va da intr-un final la coordonator 0
        MPI_Send(&elementeDuble,
                dimensiuneVector,
                MPI_INT,
                clasterMaster, // destinatie
                100, // tag
                MPI_COMM_WORLD);
        printf("M(%d,%d)\n", rank, clasterMaster);
    }

    MPI_Finalize();
    return 0;
}
