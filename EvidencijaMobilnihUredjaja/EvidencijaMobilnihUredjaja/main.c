#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 50
#define NAZIV_DATOTEKE "telefoni.txt"

typedef struct Telefon {
    char NazivModelaTelefona[MAX_NAZIV];
    char NazivOperativnogSistema[MAX_NAZIV];
    int GodinaProizvodnjeModelaTelefona;
    struct Telefon *sledeci;
} Telefon;

void ocistiUlaz() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void ukloniNoviRed(char tekst[]) {
    tekst[strcspn(tekst, "\n")] = '\0';
}

Telefon* kreirajTelefon(char model[], char sistem[], int godina) {
    Telefon *novi = (Telefon*)malloc(sizeof(Telefon));

    if (novi == NULL) {
        printf("Greska: memorija nije uspesno zauzeta.\n");
        return NULL;
    }

    strcpy(novi->NazivModelaTelefona, model);
    strcpy(novi->NazivOperativnogSistema, sistem);
    novi->GodinaProizvodnjeModelaTelefona = godina;
    novi->sledeci = NULL;

    return novi;
}

void dodajNaKrajListe(Telefon **glava, Telefon *novi) {
    if (novi == NULL) {
        return;
    }

    if (*glava == NULL) {
        *glava = novi;
    } else {
        Telefon *trenutni = *glava;

        while (trenutni->sledeci != NULL) {
            trenutni = trenutni->sledeci;
        }

        trenutni->sledeci = novi;
    }
}

void dodajTelefon(Telefon **glava) {
    char model[MAX_NAZIV];
    char sistem[MAX_NAZIV];
    int godina;

    printf("\nUnesite naziv modela telefona: ");
    fgets(model, MAX_NAZIV, stdin);
    ukloniNoviRed(model);

    printf("Unesite naziv operativnog sistema: ");
    fgets(sistem, MAX_NAZIV, stdin);
    ukloniNoviRed(sistem);

    printf("Unesite godinu proizvodnje modela telefona: ");

    if (scanf("%d", &godina) != 1) {
        printf("Neispravan unos godine.\n");
        ocistiUlaz();
        return;
    }

    ocistiUlaz();

    if (godina < 1990 || godina > 2100) {
        printf("Godina nije u dozvoljenom opsegu.\n");
        return;
    }

    Telefon *novi = kreirajTelefon(model, sistem, godina);
    dodajNaKrajListe(glava, novi);

    printf("Telefon je uspesno dodat u listu.\n");
}

void prikaziTelefone(Telefon *glava) {
    if (glava == NULL) {
        printf("\nLista telefona je prazna.\n");
        return;
    }

    Telefon *trenutni = glava;
    int redniBroj = 1;

    printf("\n--- Lista mobilnih uredjaja ---\n");

    while (trenutni != NULL) {
        printf("\nTelefon %d\n", redniBroj);
        printf("Model telefona: %s\n", trenutni->NazivModelaTelefona);
        printf("Operativni sistem: %s\n", trenutni->NazivOperativnogSistema);
        printf("Godina proizvodnje: %d\n", trenutni->GodinaProizvodnjeModelaTelefona);

        trenutni = trenutni->sledeci;
        redniBroj++;
    }
}

void pretraziPoOperativnomSistemu(Telefon *glava) {
    char trazeniSistem[MAX_NAZIV];
    int pronadjen = 0;

    if (glava == NULL) {
        printf("\nLista telefona je prazna.\n");
        return;
    }

    printf("\nUnesite naziv operativnog sistema za pretragu: ");
    fgets(trazeniSistem, MAX_NAZIV, stdin);
    ukloniNoviRed(trazeniSistem);

    Telefon *trenutni = glava;

    printf("\n--- Rezultati pretrage ---\n");

    while (trenutni != NULL) {
        if (strcmp(trenutni->NazivOperativnogSistema, trazeniSistem) == 0) {
            printf("\nModel telefona: %s\n", trenutni->NazivModelaTelefona);
            printf("Operativni sistem: %s\n", trenutni->NazivOperativnogSistema);
            printf("Godina proizvodnje: %d\n", trenutni->GodinaProizvodnjeModelaTelefona);
            pronadjen = 1;
        }

        trenutni = trenutni->sledeci;
    }

    if (pronadjen == 0) {
        printf("Nije pronadjen nijedan telefon sa trazenim operativnim sistemom.\n");
    }
}

void sacuvajUDatoteku(Telefon *glava) {
    FILE *datoteka = fopen(NAZIV_DATOTEKE, "w");

    if (datoteka == NULL) {
        printf("\nGreska pri otvaranju datoteke za upis.\n");
        return;
    }

    Telefon *trenutni = glava;

    while (trenutni != NULL) {
        fprintf(datoteka, "%s;%s;%d\n",
                trenutni->NazivModelaTelefona,
                trenutni->NazivOperativnogSistema,
                trenutni->GodinaProizvodnjeModelaTelefona);

        trenutni = trenutni->sledeci;
    }

    fclose(datoteka);

    printf("\nPodaci su uspesno sacuvani u datoteku %s.\n", NAZIV_DATOTEKE);
}

void oslobodiListu(Telefon **glava) {
    Telefon *trenutni = *glava;

    while (trenutni != NULL) {
        Telefon *zaBrisanje = trenutni;
        trenutni = trenutni->sledeci;
        free(zaBrisanje);
    }

    *glava = NULL;
}

void ucitajIzDatoteke(Telefon **glava) {
    FILE *datoteka = fopen(NAZIV_DATOTEKE, "r");

    if (datoteka == NULL) {
        printf("\nDatoteka %s ne postoji ili ne moze da se otvori.\n", NAZIV_DATOTEKE);
        return;
    }

    oslobodiListu(glava);

    char linija[200];

    while (fgets(linija, sizeof(linija), datoteka) != NULL) {
        char *model = strtok(linija, ";");
        char *sistem = strtok(NULL, ";");
        char *godinaTekst = strtok(NULL, "\n");

        if (model != NULL && sistem != NULL && godinaTekst != NULL) {
            int godina = atoi(godinaTekst);
            Telefon *novi = kreirajTelefon(model, sistem, godina);
            dodajNaKrajListe(glava, novi);
        }
    }

    fclose(datoteka);

    printf("\nPodaci su uspesno ucitani iz datoteke %s.\n", NAZIV_DATOTEKE);
}

void prikaziMeni() {
    printf("\n=====================================\n");
    printf(" Evidencija mobilnih uredjaja\n");
    printf("=====================================\n");
    printf("1. Dodaj telefon u listu\n");
    printf("2. Prikazi sve telefone iz liste\n");
    printf("3. Pretrazi telefone po operativnom sistemu\n");
    printf("4. Sacuvaj podatke u datoteku\n");
    printf("5. Ucitaj podatke iz datoteke\n");
    printf("0. Izlaz iz programa\n");
    printf("Izaberite opciju: ");
}

int main() {
    Telefon *glava = NULL;
    int izbor;

    do {
        prikaziMeni();

        if (scanf("%d", &izbor) != 1) {
            printf("\nNeispravan unos. Pokusajte ponovo.\n");
            ocistiUlaz();
            continue;
        }

        ocistiUlaz();

        if (izbor == 1) {
            dodajTelefon(&glava);
        } else if (izbor == 2) {
            prikaziTelefone(glava);
        } else if (izbor == 3) {
            pretraziPoOperativnomSistemu(glava);
        } else if (izbor == 4) {
            sacuvajUDatoteku(glava);
        } else if (izbor == 5) {
            ucitajIzDatoteke(&glava);
        } else if (izbor == 0) {
            printf("\nIzlaz iz programa.\n");
        } else {
            printf("\nNepostojeca opcija. Pokusajte ponovo.\n");
        }
    /*obrada izabrane opcije */
    } while (izbor != 0);

    oslobodiListu(&glava);

    return 0;
}
