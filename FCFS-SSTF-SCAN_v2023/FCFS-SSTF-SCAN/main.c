#include <stdio.h>
#include <stdlib.h>
#include <locale.h>     // necessário para setlocale()
#include <time.h>       // necessário para time()
#include <unistd.h>     // necessário para usleep()

#define N           25
#define DEBUG       1
#define DEBUG_SSTF  1
#define DEBUG_SCAN  1

//
// ATENÇÃO
//
// declarar a estrutura struct timespec, caso ela não esteja declarada no arquivo time.h
//
/*
struct timespec
{
    time_t  tv_sec;     // segundos
    long    tv_nsec;    // nanossegundos
};
*/
//
// ATENÇÃO
//

struct escalonamento
{
    int distancia;
    int cilindro;
};

void seek(int []);
void SSTF(int [], int []);
void SCAN(int [], int []);

int main()
{
    int cilindros[N];
    int cilindrosSSTF[N];
    int cilindrosSCAN[N];
    int i;

    setlocale(LC_ALL, "");
    printf("\n");
    srand(time(NULL));
    for (i = 0; i < N; i++)
        cilindros[i] = rand() % 1000;

    printf("\ncilindros FCFS: ");
    for (i = 0; i < N; i++)
        printf("%03d ", cilindros[i]);
    printf("\n");

    printf("\nalgoritmo FCFS: ");
    seek(cilindros);

    SSTF(cilindros, cilindrosSSTF);

    printf("\n\n\ncilindros SSTF: ");
    for (i = 0; i < N; i++)
        printf("%03d ", cilindrosSSTF[i]);
    printf("\n");

    printf("\nalgoritmo SSTF: ");
    seek(cilindrosSSTF);

    SCAN(cilindros, cilindrosSCAN);

    printf("\n\n\ncilindros SCAN: ");
    for (i = 0; i < N; i++)
        printf("%03d ", cilindrosSCAN[i]);
    printf("\n");

    printf("\nalgoritmo SCAN: ");
    seek(cilindrosSCAN);

    printf("\n\n");
    return (0);
}

void seek(int cilindros[])
{
    struct timespec req, rem;
    time_t tempo_inicial;       // UNIX time, epoch UNIX, POSIX time
    time_t tempo_final;         // 01/01/1970 00:00:00 menos 3 horas
    clock_t CLK1;
    clock_t CLK2;
    float fDelta;
    int i;
    int deslocamento;
    int seekTOTAL;
    long miliseconds;


    // calcula o número de operações de seek
    seekTOTAL = 0;
    for (i = 0; i < N-1; i++)
    {
        if (cilindros[i+1] > cilindros[i])
            deslocamento = cilindros[i+1] - cilindros[i];
        else
            deslocamento = cilindros[i] - cilindros[i+1];

        seekTOTAL = seekTOTAL + deslocamento;
    }

    // tempo de seek = número de operações de seek x 6 ms
    miliseconds = seekTOTAL * 6;

    req.tv_sec = (int)(miliseconds / 1000);
    req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000;

    time(&tempo_inicial);
    CLK1 = clock();

    // aguarda tempo de seek
    nanosleep(&req , &rem);

    CLK2 = clock();
    time(&tempo_final);

    fDelta = ((CLK2 - CLK1) * 1000) / CLOCKS_PER_SEC;
    printf("%ld s\t", (long int)(tempo_final-tempo_inicial));
    printf("(%0.0f ms)\t", fDelta);
    printf("(%d operações de seek)\n", seekTOTAL);
}

void SSTF(int cilindros[], int cilindrosSSTF[])
{
    int cilindrosAUX[N];
    struct escalonamento ordem[N];
    int i, j, k;
    int distancia_aux, cilindro_aux;


    // trabalha com uma cópia da lista de cilindros a visitar
    for (i = 0; i < N; i++)
        cilindrosAUX[i] = cilindros[i];

    k = 0;

    // assume o braço do disco posicionado no 1º cilindro a visitar
    cilindrosSSTF[k] = cilindrosAUX[k];

    while (k < (N-1))
    {
        if (DEBUG_SSTF)
        {
            printf("\n\nSSTF: ");
            for (i = 0; i <= k; i++)
                printf("%d, ", cilindrosSSTF[i]);
            printf("\n");
        }

        // calcula as distâncias entre o cilindro atual e os demais cilindros
        for (i = k; i < N-1; i++)
        {
            if (cilindrosAUX[i+1] > cilindrosAUX[k])
                ordem[i].distancia = cilindrosAUX[i+1] - cilindrosAUX[k];
            else
                ordem[i].distancia = cilindrosAUX[k] - cilindrosAUX[i+1];

            ordem[i].cilindro = cilindrosAUX[i+1];
        }

        if (DEBUG_SSTF)
        {
            printf("distância:cilindro = ");
            for (i = k; i < N-1; i++)
                printf("%03d:%03d, ", ordem[i].distancia, ordem[i].cilindro);
        }

        // ordena a lista de distâncias em ordem crescente
        for (i = k; i < N-2; i++)
            for (j = k; j < (N-2)-(i-k); j++)
                if (ordem[j].distancia > ordem[j+1].distancia)
                    {
                        distancia_aux = ordem[j].distancia;
                        ordem[j].distancia = ordem[j+1].distancia;
                        ordem[j+1].distancia = distancia_aux;

                        cilindro_aux = ordem[j].cilindro;
                        ordem[j].cilindro = ordem[j+1].cilindro;
                        ordem[j+1].cilindro = cilindro_aux;
                    }

        if (DEBUG_SSTF)
        {
            printf("\ndistâncias em ordem= ");
            for (i = k; i < N-1; i++)
                printf("%03d:%03d, ", ordem[i].distancia, ordem[i].cilindro);
        }

        // re-ordena a lista de cilindros a visitar
        for (i = k; i < N; i++)
            cilindrosAUX[i+1] = ordem[i].cilindro;

        // próximo cilindro a visitar é o cilindro mais próximo
        k++;
        cilindrosSSTF[k] = cilindrosAUX[k];
    }
}

void SCAN(int cilindros[], int cilindrosSCAN[])
{
    int cilindrosUP;
    int cilindrosDOWN;
    int i, j;
    int cilindrosAUX[N];
    int cilindro_aux;

    cilindrosUP = 0;
    cilindrosDOWN = 0;

    // assume o braço do disco posicionado no 1º cilindro a visitar
    cilindrosSCAN[0] = cilindros[0];

    // algoritmo SCAN com braço inicialmente para cima
    // (movimentando-se para cilindros maiores)
    // gera duas sublistas de cilindros a visitar
    // (cilindros maiores e cilindros menores)
    for (i = 1; i < N; i++)
    {
        if (cilindros[i] >= cilindrosSCAN[0])
        {
            cilindrosUP++;
            cilindrosSCAN[cilindrosUP] = cilindros[i];
        }
        else
        {
            cilindrosAUX[cilindrosDOWN] = cilindros[i];
            cilindrosDOWN++;
        }
    }

    if (DEBUG_SCAN)
    {
        printf("\n\n\nSCAN:UP= %d, DOWN= %d\n", cilindrosUP, cilindrosDOWN);
        printf("\nSCAN (UP):\t\t");
        for (i = 0; i <= cilindrosUP; i++)
            printf("%03d ", cilindrosSCAN[i]);
        printf("\n");
    }

    // coloca os cilindros UP a visitar em ordem crescente
    for (i = 0; i < cilindrosUP; i++)
        for (j = 0; j < (cilindrosUP)-i; j++)
            if (cilindrosSCAN[j] > cilindrosSCAN[j+1])
                {
                    cilindro_aux = cilindrosSCAN[j];
                    cilindrosSCAN[j] = cilindrosSCAN[j+1];
                    cilindrosSCAN[j+1] = cilindro_aux;
                }

    if (DEBUG_SCAN)
    {
        printf("\nSCAN (UP) em ordem:\t");
        for (i = 0; i <= cilindrosUP; i++)
            printf("%03d ", cilindrosSCAN[i]);
        printf("\n");
    }

    // coloca os cilindros DOWN a visitar em ordem decrescente
    for (i = 0; i < cilindrosDOWN-1; i++)
        for (j = 0; j < (cilindrosDOWN-1)-i; j++)
            if (cilindrosAUX[j] < cilindrosAUX[j+1])
                {
                    cilindro_aux = cilindrosAUX[j];
                    cilindrosAUX[j] = cilindrosAUX[j+1];
                    cilindrosAUX[j+1] = cilindro_aux;
                }

    if (DEBUG_SCAN)
    {
        printf("\nSCAN (DOWN) em ordem:\t");
        for (i = 0; i < cilindrosDOWN; i++)
            printf("%03d ", cilindrosAUX[i]);
        printf("\n");
    }

    // após visitar os cilindros maiores, inverte o sentido
    // do movimento e visita os cilindros menores
    j = 0;
    for (i = cilindrosUP+1; i < N; i++)
    {
        cilindrosSCAN[i] = cilindrosAUX[j];
        j++;
    }
}
