/*
    Karczma "Pod Zlamanym Toporem" - System Zamowien
    Projekt na studia - 1 semestr
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAKS_DAN 20

// Dane menu
char nazwy[MAKS_DAN][50];
double ceny[MAKS_DAN];
char skladniki[MAKS_DAN][100];
int liczba_dan = 0;

// Zamowienie
int zamowione_id[MAKS_DAN];
int zamowione_ile[MAKS_DAN];
int liczba_zamowien = 0;
double suma = 0.0;

// Dane klienta
char imie[50];
char adres[100];
int stolik = 0;
int typ = 0;  // 1=na miejscu, 2=dowoz
int godz_dost = 0, min_dost = 0;

// Czysci bufor klawiatury
void czyscBufor() {
    while (getchar() != '\n');
}

// Wczytuje liczbe z kontrola zakresu
int wczytajLiczbe(int min, int max) {
    int n;
    while (1) {
        if (scanf("%d", &n) != 1) {
            czyscBufor();
            printf("Blad! Podaj liczbe: ");
            continue;
        }
        czyscBufor();
        if (n < min || n > max) {
            printf("Podaj liczbe %d-%d: ", min, max);
            continue;
        }
        return n;
    }
}

// Wczytuje menu z pliku
void wczytajMenu() {
    ifstream f("menu.txt");
    if (!f) {
        cout << "Blad: Nie mozna otworzyc pliku menu.txt\n";
        return;
    }

    string linia;
    int numerLinii = 0; // Pomaga zidentyfikować, gdzie jest błąd

    while (getline(f, linia) && liczba_dan < MAKS_DAN) {
        numerLinii++;

        // 1. Szukamy pozycji separatorów
        size_t p1 = linia.find(':');
        size_t p2 = linia.find(':', p1 + 1);

        // Sprawdzenie, czy format linii jest poprawny (czy są minimum dwa dwukropki)
        if (p1 == string::npos || p2 == string::npos) {
            cout << "Blad w linii " << numerLinii << ": Nieprawidlowy format (brak dwukropka). Pomijam...\n";
            continue;
        }

        try {
            // 2. Wycinanie danych
            string nazwa = linia.substr(p1 + 1, p2 - p1 - 1);
            string cenaStr = linia.substr(p2 + 1);

            // Konwersja ceny na double
            // stod moze wyrzucic wyjatek, jesli po drugim dwukropku nie ma liczby
            ceny[liczba_dan] = stod(cenaStr);
            nazwy[liczba_dan] = nazwa;

            liczba_dan++;
        }
        catch (...) {
            cout << "Blad w linii " << numerLinii << ": Niepoprawny format ceny. Pomijam...\n";
            continue;
        }
    }
    f.close();
}

// Wyswietla dane karczmy
void pokazKarczme() {
    printf("\n===========================================\n");
    printf("       KARCZMA 'POD ZLAMANYM TOPOREM'\n");
    printf("===========================================\n");
    printf("Adres: Trakt Krolewski 7, Smocza Skala\n");
    printf("Wlasciciel: Grumli Brodaty\n");
    printf("Godziny: 10:00 - 22:00\n");
    printf("===========================================\n\n");
}

// Wyswietla menu
void pokazMenu() {
    printf("\n--- MENU ---\n");
    for (int i = 0; i < liczba_dan; i++) {
        printf("%d. %s - %.2f zl\n", i+1, nazwy[i], ceny[i]);
        printf("   (%s)\n", skladniki[i]);
    }
    printf("------------\n");
}

// Wyswietla zamowienie
void pokazZamowienie() {
    if (liczba_zamowien == 0) {
        printf("\nZamowienie puste.\n");
        return;
    }
    printf("\n--- TWOJE ZAMOWIENIE ---\n");
    for (int i = 0; i < liczba_zamowien; i++) {
        int id = zamowione_id[i];
        printf("%d. %s x%d = %.2f zl\n", i+1, nazwy[id], zamowione_ile[i], 
               ceny[id] * zamowione_ile[i]);
    }
    printf("SUMA: %.2f zl\n", suma);
    printf("------------------------\n");
}

// Dodaje danie
void dodajDanie() {
    pokazMenu();
    printf("Numer dania (0=anuluj): ");
    int nr = wczytajLiczbe(0, liczba_dan);
    if (nr == 0) return;
    
    printf("Ile porcji? (1-10): ");
    int ile = wczytajLiczbe(1, 10);
    
    int id = nr - 1;
    
    // Sprawdz czy juz zamowione
    for (int i = 0; i < liczba_zamowien; i++) {
        if (zamowione_id[i] == id) {
            zamowione_ile[i] += ile;
            suma += ceny[id] * ile;
            printf("Dodano %d x %s\n", ile, nazwy[id]);
            return;
        }
    }
    
    // Nowa pozycja
    zamowione_id[liczba_zamowien] = id;
    zamowione_ile[liczba_zamowien] = ile;
    liczba_zamowien++;
    suma += ceny[id] * ile;
    printf("Dodano %d x %s\n", ile, nazwy[id]);
}

// Usuwa pozycje
void usunPozycje() {
    if (liczba_zamowien == 0) {
        printf("Nie ma czego usuwac!\n");
        return;
    }
    pokazZamowienie();
    printf("Ktora pozycje usunac? (0=anuluj): ");
    int nr = wczytajLiczbe(0, liczba_zamowien);
    if (nr == 0) return;
    
    int i = nr - 1;
    suma -= ceny[zamowione_id[i]] * zamowione_ile[i];
    printf("Usunieto: %s\n", nazwy[zamowione_id[i]]);
    
    // Przesun reszte
    for (; i < liczba_zamowien - 1; i++) {
        zamowione_id[i] = zamowione_id[i+1];
        zamowione_ile[i] = zamowione_ile[i+1];
    }
    liczba_zamowien--;
}

// Zapisuje rachunek do pliku
void zapiszRachunek() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    
    char plik[50];
    sprintf(plik, "rachunek_%02d%02d_%02d%02d.txt", 
            tm->tm_mday, tm->tm_mon+1, tm->tm_hour, tm->tm_min);
    
    FILE* f = fopen(plik, "w");
    if (!f) {
        printf("Blad zapisu rachunku!\n");
        return;
    }
    
    fprintf(f, "KARCZMA 'POD ZLAMANYM TOPOREM' - RACHUNEK\n");
    fprintf(f, "Data: %02d-%02d-%d %02d:%02d\n", 
            tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_hour, tm->tm_min);
    fprintf(f, "Klient: %s\n", imie);
    
    if (typ == 1) {
        fprintf(f, "Stolik nr: %d\n", stolik);
    } else {
        fprintf(f, "Dostawa: %s, godz. %02d:%02d\n", adres, godz_dost, min_dost);
    }
    
    fprintf(f, "\nZamowienie:\n");
    for (int i = 0; i < liczba_zamowien; i++) {
        int id = zamowione_id[i];
        fprintf(f, "- %s x%d = %.2f zl\n", nazwy[id], zamowione_ile[i], 
                ceny[id] * zamowione_ile[i]);
    }
    fprintf(f, "\nSUMA: %.2f zl\n", suma);
    fprintf(f, "\nDziekujemy!\n");
    
    fclose(f);
    printf("Rachunek zapisano: %s\n", plik);
}

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    if (!wczytajMenu()) {
        printf("Nacisnij Enter...");
        getchar();
        return 1;
    }
    
    pokazKarczme();
    
    // Imie klienta
    printf("Podaj imie: ");
    fgets(imie, sizeof(imie), stdin);
    imie[strcspn(imie, "\n")] = 0;
    if (strlen(imie) == 0) strcpy(imie, "Gosc");
    
    printf("\nWitaj %s!\n", imie);
    
    // Typ zamowienia
    printf("\n1 - Na miejscu\n2 - Dowoz\n0 - Wyjscie\nWybor: ");
    typ = wczytajLiczbe(0, 2);
    if (typ == 0) return 0;
    
    // Dane dodatkowe
    if (typ == 1) {
        printf("Numer stolika (1-15): ");
        stolik = wczytajLiczbe(1, 15);
    } else {
        printf("Adres dostawy: ");
        fgets(adres, sizeof(adres), stdin);
        adres[strcspn(adres, "\n")] = 0;
        
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);
        int min_godz = tm->tm_hour + 1;  // minimum 1h od teraz
        if (min_godz < 10) min_godz = 10;
        if (min_godz > 21) {
            printf("Za pozno na dowoz! Zapraszamy jutro.\n");
            getchar();
            return 0;
        }
        
        printf("Godzina dostawy (%d-21): ", min_godz);
        godz_dost = wczytajLiczbe(min_godz, 21);
        printf("Minuta (0-59): ");
        min_dost = wczytajLiczbe(0, 59);
    }
    
    // Zamawianie
    int wybor;
    do {
        printf("\n1-Dodaj 2-Usun 3-Pokaz 4-Potwierdz 0-Anuluj: ");
        wybor = wczytajLiczbe(0, 4);
        
        switch(wybor) {
            case 1: dodajDanie(); break;
            case 2: usunPozycje(); break;
            case 3: pokazZamowienie(); break;
            case 4:
                if (liczba_zamowien == 0) {
                    printf("Zamow minimum 1 danie!\n");
                    wybor = -1;
                }
                break;
        }
    } while (wybor != 0 && wybor != 4);
    
    if (wybor == 0) {
        printf("Anulowano. Do widzenia!\n");
    } else {
        zapiszRachunek();
        
        printf("\n=== PODSUMOWANIE ===\n");
        printf("Klient: %s\n", imie);
        if (typ == 1) {
            printf("Stolik: %d\n", stolik);
            printf("Czas: ok. 20 minut\n");
        } else {
            printf("Dostawa: %s o %02d:%02d\n", adres, godz_dost, min_dost);
        }
        pokazZamowienie();
        printf("\nDziekujemy, %s! Smacznego!\n", imie);
    }
    
    printf("\nNacisnij Enter...");
    getchar();
    return 0;
}
