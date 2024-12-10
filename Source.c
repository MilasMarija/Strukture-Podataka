#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAKS_LINIJA 1024

#define USPJEH 0
#define GRESKA_OTVARANJE_DATOTEKE -1
#define GRESKA_ALOKACIJA -2
#define GRESKA_UNOS -3

struct artikl;
typedef struct artikl* PozicijaArtikl;
typedef struct artikl {
    char naziv[20];
    int kolicina;
    int cijena;
    PozicijaArtikl sljedeci;
} Artikl;

struct racun;
typedef struct racun* PozicijaRacun;
typedef struct racun {
    char datum[11];
    Artikl glavaArtikala;
    PozicijaRacun sljedeci;
} Racun;

int dodajArtikl(PozicijaArtikl glava, PozicijaArtikl noviArtikl);
int dodajRacun(PozicijaRacun glava, PozicijaRacun noviRacun);
int ucitajRacun(char* imeDatoteke, PozicijaRacun glavaRacuna);
int ispisiArtikle(PozicijaArtikl trenutni);
int ispisiRacune(PozicijaRacun trenutni);
int pretraziArtiklUPodrucju(PozicijaRacun trenutniRacun, char* artikl, char* pocetniDatum, char* krajnjiDatum, int* prihod, int* ukupno);
int obrisiArtikle(PozicijaArtikl glavaArtikala);
int obrisiRacune(PozicijaRacun glavaRacuna);

int main()
{
    int prihod = 0, ukupno = 0;
    char trazeniArtikl[100] = { 0 };
    char pocetniDatum[20] = { 0 }, krajnjiDatum[20] = { 0 };
    char trenutniRacun[MAKS_LINIJA] = { 0 };
    FILE* datoteka = NULL;
    Racun glavaRacuna = {
        .datum = {0},
          .glavaArtikala = { .naziv = {0}, .kolicina = 0, .cijena = 0, .sljedeci = NULL },
        .sljedeci = NULL
    };

    datoteka = fopen("racuni.txt", "r");
    if (datoteka == NULL) {
        printf("Greška pri otvaranju datoteke.\n");
        return GRESKA_OTVARANJE_DATOTEKE;
    }

    while (!feof(datoteka)) {
        fgets(trenutniRacun, MAKS_LINIJA, datoteka);
        trenutniRacun[strcspn(trenutniRacun, "\n")] = 0; // Uklanja '\n'
        printf("%s\n", trenutniRacun);
        ucitajRacun(trenutniRacun, &glavaRacuna);
    }

    ispisiRacune(glavaRacuna.sljedeci);

    printf("\nUnesite naziv artikla: ");
    if (scanf("%s", trazeniArtikl) != 1) return GRESKA_UNOS;
    printf("Unesite poèetni datum (YYYY-MM-DD): ");
    if (scanf("%s", pocetniDatum) != 1) return GRESKA_UNOS;
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    if (scanf("%s", krajnjiDatum) != 1) return GRESKA_UNOS;

    pretraziArtiklUPodrucju(&glavaRacuna, trazeniArtikl, pocetniDatum, krajnjiDatum, &prihod, &ukupno);
    printf("Prihod od %d komada artikla '%s': %d\n", ukupno, trazeniArtikl, prihod);

    obrisiRacune(&glavaRacuna);
    fclose(datoteka);

    return 0;
}

int dodajArtikl(PozicijaArtikl glava, PozicijaArtikl noviArtikl)
{
    PozicijaArtikl trenutni = glava;

    while (trenutni->sljedeci != NULL && strcmp(trenutni->sljedeci->naziv, noviArtikl->naziv) < 0)
        trenutni = trenutni->sljedeci;

    noviArtikl->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = noviArtikl;

    return 0;
}

int dodajRacun(PozicijaRacun glava, PozicijaRacun noviRacun)
{
    PozicijaRacun trenutni = glava;

    while (trenutni->sljedeci != NULL && strcmp(trenutni->sljedeci->datum, noviRacun->datum) < 0)
        trenutni = trenutni->sljedeci;

    noviRacun->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = noviRacun;

    return 0;
}

int ucitajRacun(char* imeDatoteke, PozicijaRacun glavaRacuna)
{
    char datum[20] = { 0 };
    FILE* datoteka = NULL;
    Racun* noviRacun = NULL;
    Artikl* noviArtikl = NULL;

    datoteka = fopen(imeDatoteke, "r");
    if (datoteka == NULL) {
        printf("Greška pri otvaranju datoteke.\n");
        return GRESKA_OTVARANJE_DATOTEKE;
    }

    noviRacun = (Racun*)malloc(sizeof(Racun));
    if (noviRacun == NULL) {
        printf("Greška pri alokaciji memorije.\n");
        return GRESKA_ALOKACIJA;
    }

    fgets(datum, 20, datoteka);
    datum[strcspn(datum, "\n")] = 0;
    strcpy(noviRacun->datum, datum);
    noviRacun->glavaArtikala.sljedeci = NULL;

    while (!feof(datoteka)) {
        noviArtikl = (Artikl*)malloc(sizeof(Artikl));
        if (noviArtikl == NULL) {
            printf("Greška pri alokaciji memorije.\n");
            return GRESKA_ALOKACIJA;
        }

        if (fscanf(datoteka, "%[^,], %d, %d ", noviArtikl->naziv, &noviArtikl->kolicina, &noviArtikl->cijena) != 3) {
            free(noviArtikl);
            break;
        }

        dodajArtikl(&noviRacun->glavaArtikala, noviArtikl);
    }

    dodajRacun(glavaRacuna, noviRacun);
    fclose(datoteka);

    return 0;
}

int ispisiArtikle(PozicijaArtikl trenutni)
{
    while (trenutni != NULL) {
        printf("\t%s, %d, %d\n", trenutni->naziv, trenutni->kolicina, trenutni->cijena);
        trenutni = trenutni->sljedeci;
    }

    return 0;
}

int ispisiRacune(PozicijaRacun trenutni)
{
    while (trenutni != NULL) {
        printf("Raèun od datuma %s:\n", trenutni->datum);
        ispisiArtikle(trenutni->glavaArtikala.sljedeci);
        trenutni = trenutni->sljedeci;
    }

    return 0;
}

int pretraziArtiklUPodrucju(PozicijaRacun trenutniRacun, char* artikl, char* pocetniDatum, char* krajnjiDatum, int* prihod, int* ukupno)
{
    PozicijaArtikl trenutniArtikl = NULL;

    while (trenutniRacun != NULL && strcmp(trenutniRacun->datum, pocetniDatum) < 0)
        trenutniRacun = trenutniRacun->sljedeci;

    while (trenutniRacun != NULL && strcmp(trenutniRacun->datum, krajnjiDatum) <= 0) {
        trenutniArtikl = trenutniRacun->glavaArtikala.sljedeci;
        while (trenutniArtikl != NULL) {
            if (strcmp(trenutniArtikl->naziv, artikl) == 0) {
                *prihod += trenutniArtikl->kolicina * trenutniArtikl->cijena;
                *ukupno += trenutniArtikl->kolicina;
            }
            trenutniArtikl = trenutniArtikl->sljedeci;
        }
        trenutniRacun = trenutniRacun->sljedeci;
    }

    return 0;
}

int obrisiArtikle(PozicijaArtikl glavaArtikala)
{
    PozicijaArtikl temp = NULL;

    while (glavaArtikala->sljedeci != NULL) {
        temp = glavaArtikala->sljedeci;
        glavaArtikala->sljedeci = temp->sljedeci;
        free(temp);
    }

    return 0;
}

int obrisiRacune(PozicijaRacun glavaRacuna)
{
    PozicijaRacun temp = NULL;

    while (glavaRacuna->sljedeci != NULL) {
        temp = glavaRacuna->sljedeci;
        glavaRacuna->sljedeci = temp->sljedeci;
        obrisiArtikle(&temp->glavaArtikala);
        free(temp);
    }

    return 0;
}

