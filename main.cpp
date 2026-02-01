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
#define MAKS_ZAMOWIEN 50

// Struktura dania w menu
struct Danie {
    int id;
    char nazwa[50];
    double cena;
    char skladniki[100];
};

// Struktura pozycji w zamowieniu
struct Pozycja {
    int id_dania;
    char nazwa[50];
    double cena;
    int ilosc;
};

// Zmienne globalne
struct Danie menu[MAKS_DAN];
struct Pozycja zamowienie[MAKS_ZAMOWIEN];
int liczba_dan = 0;
int liczba_pozycji = 0;
double suma_rachunku = 0.0;

// Dane klienta
char imie_klienta[50];
int typ_zamowienia = 0;  // 1 = na miejscu, 2 = dowoz
int numer_stolika = 0;
char adres_dostawy[100];
int godzina_dostawy = 0;
int minuta_dostawy = 0;

// Godziny otwarcia karczmy
const int GODZINA_OTWARCIA = 10;
const int GODZINA_ZAMKNIECIA = 22;

#ifdef _WIN32
void ustawKonsole() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}
#endif

// Funkcja do czyszczenia bufora wejscia
void czyscBufor() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funkcja do wczytania liczby calkowitej z kontrola bledow
int wczytajLiczbe(int min, int max) {
    int liczba;
    int wynik;
    
    while (1) {
        wynik = scanf("%d", &liczba);
        czyscBufor();
        
        if (wynik != 1) {
            printf("Blad! Podaj liczbe: ");
            continue;
        }
        
        if (liczba < min || liczba > max) {
            printf("Blad! Podaj liczbe od %d do %d: ", min, max);
            continue;
        }
        
        return liczba;
    }
}

// Wczytanie menu z pliku
int wczytajMenu(const char* nazwa_pliku) {
    FILE* plik = fopen(nazwa_pliku, "r");
    
    if (plik == NULL) {
        printf("BLAD: Nie mozna otworzyc pliku %s!\n", nazwa_pliku);
        return 0;
    }
    
    liczba_dan = 0;
    char linia[256];
    
    while (fgets(linia, sizeof(linia), plik) != NULL && liczba_dan < MAKS_DAN) {
        // Format: ID:Nazwa:Cena:Skladniki
        int wynik = sscanf(linia, "%d:%49[^:]:%lf:%99[^\n]",
            &menu[liczba_dan].id,
            menu[liczba_dan].nazwa,
            &menu[liczba_dan].cena,
            menu[liczba_dan].skladniki);
        
        if (wynik == 4) {
            liczba_dan++;
        }
    }
    
    fclose(plik);
    
    if (liczba_dan == 0) {
        printf("BLAD: Plik menu jest pusty lub ma zly format!\n");
        return 0;
    }
    
    return 1;
}

// Wyswietlenie danych restauracji
void wyswietlDaneRestauracji() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     KARCZMA 'POD ZLAMANYM TOPOREM'                        ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║  Adres:      Trakt Krolewski 7, Miasto Smocza Skala       ║\n");
    printf("║  Wlasciciel: Grumli Brodaty Krasnolud                     ║\n");
    printf("║  Godziny:    10:00 - 22:00                                ║\n");
    printf("║  Specjalnosc: Kuchnia krasnoludzka i elficka              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

// Pobranie imienia klienta
void pobierzImie() {
    printf("Podaj swoje imie: ");
    fgets(imie_klienta, sizeof(imie_klienta), stdin);
    
    // Usun znak nowej linii
    size_t len = strlen(imie_klienta);
    if (len > 0 && imie_klienta[len-1] == '\n') {
        imie_klienta[len-1] = '\0';
    }
    
    // Sprawdz czy nie puste
    if (strlen(imie_klienta) == 0) {
        strcpy(imie_klienta, "Nieznajomy");
    }
    
    printf("\nWitaj, %s! Cieszymy sie, ze odwiedzasz nasza karczmę!\n\n", imie_klienta);
}

// Wybor typu zamowienia
void wybierzTypZamowienia() {
    printf("Jak chcesz zlozyc zamowienie?\n");
    printf("1 - Na miejscu (w karczmie)\n");
    printf("2 - Dowoz (do domu)\n");
    printf("0 - Wyjscie z aplikacji\n");
    printf("\nTwoj wybor: ");
    
    typ_zamowienia = wczytajLiczbe(0, 2);
    
    if (typ_zamowienia == 0) {
        printf("\nDo zobaczenia nastepnym razem!\n");
        exit(0);
    }
}

// Pobranie numeru stolika
void pobierzStolik() {
    printf("\nDostepne stoliki: 1-15\n");
    printf("Podaj numer stolika: ");
    numer_stolika = wczytajLiczbe(1, 15);
    printf("Stolik nr %d zostal zarezerwowany dla Ciebie.\n", numer_stolika);
}

// Pobranie adresu i godziny dostawy
void pobierzDaneDostawy() {
    printf("\nPodaj adres dostawy: ");
    fgets(adres_dostawy, sizeof(adres_dostawy), stdin);
    
    size_t len = strlen(adres_dostawy);
    if (len > 0 && adres_dostawy[len-1] == '\n') {
        adres_dostawy[len-1] = '\0';
    }
    
    if (strlen(adres_dostawy) == 0) {
        printf("BLAD: Adres nie moze byc pusty!\n");
        pobierzDaneDostawy();
        return;
    }
    
    // Pobierz aktualny czas
    time_t teraz = time(NULL);
    struct tm* czas = localtime(&teraz);
    int aktualna_godzina = czas->tm_hour;
    int aktualna_minuta = czas->tm_min;
    
    printf("\nAktualna godzina: %02d:%02d\n", aktualna_godzina, aktualna_minuta);
    printf("Dostawy realizujemy w godzinach %d:00 - %d:00\n", GODZINA_OTWARCIA, GODZINA_ZAMKNIECIA);
    printf("Minimalny czas dostawy: 30 minut od teraz\n\n");
    
    // Oblicz minimalna godzine dostawy
    int min_godzina = aktualna_godzina;
    int min_minuta = aktualna_minuta + 30;
    if (min_minuta >= 60) {
        min_minuta -= 60;
        min_godzina++;
    }
    
    // Sprawdz czy jeszcze mozna zamowic
    if (min_godzina >= GODZINA_ZAMKNIECIA) {
        printf("BLAD: Niestety, jest juz za pozno na zamowienie dostawy.\n");
        printf("Zapraszamy jutro od %d:00!\n", GODZINA_OTWARCIA);
        exit(0);
    }
    
    if (min_godzina < GODZINA_OTWARCIA) {
        min_godzina = GODZINA_OTWARCIA;
        min_minuta = 0;
    }
    
    printf("Podaj godzine dostawy (%d-%d): ", min_godzina, GODZINA_ZAMKNIECIA - 1);
    godzina_dostawy = wczytajLiczbe(min_godzina, GODZINA_ZAMKNIECIA - 1);
    
    printf("Podaj minute dostawy (0-59): ");
    
    // Dla minimalnej godziny sprawdz minuty
    if (godzina_dostawy == min_godzina) {
        minuta_dostawy = wczytajLiczbe(min_minuta, 59);
    } else {
        minuta_dostawy = wczytajLiczbe(0, 59);
    }
    
    printf("\nDostawa na adres: %s\n", adres_dostawy);
    printf("Planowana godzina: %02d:%02d\n", godzina_dostawy, minuta_dostawy);
}

// Wyswietlenie menu
void wyswietlMenu() {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    MENU KARCZMY                           ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < liczba_dan; i++) {
        printf("║ %d. %-25s %8.2f zl                ║\n", 
               menu[i].id, menu[i].nazwa, menu[i].cena);
        printf("║    Skladniki: %-43s║\n", menu[i].skladniki);
        printf("╠════════════════════════════════════════════════════════════╣\n");
    }
    
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

// Wyswietlenie aktualnego zamowienia
void wyswietlZamowienie() {
    if (liczba_pozycji == 0) {
        printf("\n[Twoje zamowienie jest puste]\n");
        return;
    }
    
    printf("\n--- TWOJE ZAMOWIENIE ---\n");
    for (int i = 0; i < liczba_pozycji; i++) {
        printf("%d. %s x%d = %.2f zl\n", 
               i + 1,
               zamowienie[i].nazwa, 
               zamowienie[i].ilosc, 
               zamowienie[i].cena * zamowienie[i].ilosc);
    }
    printf("------------------------\n");
    printf("SUMA: %.2f zl\n", suma_rachunku);
    printf("------------------------\n");
}

// Dodanie dania do zamowienia
void dodajDanie() {
    wyswietlMenu();
    
    printf("\nPodaj numer dania (1-%d) lub 0 aby wrocic: ", liczba_dan);
    int wybor = wczytajLiczbe(0, liczba_dan);
    
    if (wybor == 0) return;
    
    // Znajdz danie
    int indeks = -1;
    for (int i = 0; i < liczba_dan; i++) {
        if (menu[i].id == wybor) {
            indeks = i;
            break;
        }
    }
    
    if (indeks == -1) {
        printf("Nie znaleziono dania o numerze %d!\n", wybor);
        return;
    }
    
    printf("Ile porcji '%s'? (1-10): ", menu[indeks].nazwa);
    int ilosc = wczytajLiczbe(1, 10);
    
    // Sprawdz czy danie juz jest w zamowieniu
    for (int i = 0; i < liczba_pozycji; i++) {
        if (zamowienie[i].id_dania == menu[indeks].id) {
            zamowienie[i].ilosc += ilosc;
            suma_rachunku += menu[indeks].cena * ilosc;
            printf("Dodano %d porcji '%s'. Teraz masz %d porcji tego dania.\n", 
                   ilosc, menu[indeks].nazwa, zamowienie[i].ilosc);
            return;
        }
    }
    
    // Dodaj nowa pozycje
    if (liczba_pozycji < MAKS_ZAMOWIEN) {
        zamowienie[liczba_pozycji].id_dania = menu[indeks].id;
        strcpy(zamowienie[liczba_pozycji].nazwa, menu[indeks].nazwa);
        zamowienie[liczba_pozycji].cena = menu[indeks].cena;
        zamowienie[liczba_pozycji].ilosc = ilosc;
        liczba_pozycji++;
        suma_rachunku += menu[indeks].cena * ilosc;
        
        printf("Dodano: %s x%d (%.2f zl)\n", 
               menu[indeks].nazwa, ilosc, menu[indeks].cena * ilosc);
    } else {
        printf("Zamowienie jest pelne!\n");
    }
}

// Usuniecie pozycji z zamowienia
void usunPozycje() {
    if (liczba_pozycji == 0) {
        printf("\nZamowienie jest puste - nie ma czego usuwac!\n");
        return;
    }
    
    wyswietlZamowienie();
    
    printf("\nKtora pozycje usunac? (1-%d) lub 0 aby wrocic: ", liczba_pozycji);
    int wybor = wczytajLiczbe(0, liczba_pozycji);
    
    if (wybor == 0) return;
    
    int indeks = wybor - 1;
    
    // Odejmij od sumy
    suma_rachunku -= zamowienie[indeks].cena * zamowienie[indeks].ilosc;
    
    printf("Usunieto: %s\n", zamowienie[indeks].nazwa);
    
    // Przesun pozostale pozycje
    for (int i = indeks; i < liczba_pozycji - 1; i++) {
        zamowienie[i] = zamowienie[i + 1];
    }
    liczba_pozycji--;
}

// Glowne menu zamawiania
int menuZamawiania() {
    while (1) {
        printf("\n========================================\n");
        printf("Co chcesz zrobic?\n");
        printf("1 - Dodaj danie do zamowienia\n");
        printf("2 - Usun pozycje z zamowienia\n");
        printf("3 - Zobacz aktualne zamowienie\n");
        printf("4 - Potwierdz zamowienie\n");
        printf("0 - Anuluj i wyjdz\n");
        printf("========================================\n");
        printf("Wybor: ");
        
        int wybor = wczytajLiczbe(0, 4);
        
        switch (wybor) {
            case 0:
                printf("\nZamowienie anulowane. Do zobaczenia!\n");
                return 0;
            case 1:
                dodajDanie();
                break;
            case 2:
                usunPozycje();
                break;
            case 3:
                wyswietlZamowienie();
                break;
            case 4:
                if (liczba_pozycji == 0) {
                    printf("\nMusisz zamowic co najmniej 1 danie!\n");
                } else {
                    return 1;  // Potwierdzone
                }
                break;
        }
    }
}

// Zapisanie rachunku do pliku
void zapiszRachunek() {
    // Utworz nazwe pliku z czasem
    time_t teraz = time(NULL);
    struct tm* czas = localtime(&teraz);
    
    char nazwa_pliku[100];
    sprintf(nazwa_pliku, "rachunek_%04d%02d%02d_%02d%02d%02d.txt",
            czas->tm_year + 1900, czas->tm_mon + 1, czas->tm_mday,
            czas->tm_hour, czas->tm_min, czas->tm_sec);
    
    FILE* plik = fopen(nazwa_pliku, "w");
    
    if (plik == NULL) {
        printf("BLAD: Nie mozna zapisac rachunku do pliku!\n");
        return;
    }
    
    fprintf(plik, "╔════════════════════════════════════════════════════════════╗\n");
    fprintf(plik, "║           KARCZMA 'POD ZLAMANYM TOPOREM'                  ║\n");
    fprintf(plik, "║                    RACHUNEK                               ║\n");
    fprintf(plik, "╚════════════════════════════════════════════════════════════╝\n\n");
    
    fprintf(plik, "Data: %04d-%02d-%02d %02d:%02d\n",
            czas->tm_year + 1900, czas->tm_mon + 1, czas->tm_mday,
            czas->tm_hour, czas->tm_min);
    fprintf(plik, "Klient: %s\n", imie_klienta);
    
    if (typ_zamowienia == 1) {
        fprintf(plik, "Typ: Na miejscu, stolik nr %d\n", numer_stolika);
    } else {
        fprintf(plik, "Typ: Dowoz\n");
        fprintf(plik, "Adres: %s\n", adres_dostawy);
        fprintf(plik, "Godzina dostawy: %02d:%02d\n", godzina_dostawy, minuta_dostawy);
    }
    
    fprintf(plik, "\n--- ZAMOWIONE DANIA ---\n");
    
    for (int i = 0; i < liczba_pozycji; i++) {
        fprintf(plik, "%d. %-25s x%d  %8.2f zl\n",
                i + 1,
                zamowienie[i].nazwa,
                zamowienie[i].ilosc,
                zamowienie[i].cena * zamowienie[i].ilosc);
    }
    
    fprintf(plik, "\n---------------------------\n");
    fprintf(plik, "SUMA DO ZAPLATY: %.2f zl\n", suma_rachunku);
    fprintf(plik, "---------------------------\n");
    
    fprintf(plik, "\nDziekujemy za zamowienie!\n");
    fprintf(plik, "Zapraszamy ponownie!\n");
    
    fclose(plik);
    
    printf("\nRachunek zapisano do pliku: %s\n", nazwa_pliku);
}

// Wyswietlenie podsumowania
void wyswietlPodsumowanie() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              PODSUMOWANIE ZAMOWIENIA                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Klient: %s\n", imie_klienta);
    
    if (typ_zamowienia == 1) {
        printf("Zamowienie: Na miejscu, stolik nr %d\n", numer_stolika);
        printf("Szacowany czas przygotowania: 15-25 minut\n");
    } else {
        printf("Zamowienie: Dowoz\n");
        printf("Adres dostawy: %s\n", adres_dostawy);
        printf("Planowana godzina dostawy: %02d:%02d\n", godzina_dostawy, minuta_dostawy);
    }
    
    printf("\n--- ZAMOWIONE DANIA ---\n");
    for (int i = 0; i < liczba_pozycji; i++) {
        printf("%d. %s x%d = %.2f zl\n",
               i + 1,
               zamowienie[i].nazwa,
               zamowienie[i].ilosc,
               zamowienie[i].cena * zamowienie[i].ilosc);
    }
    
    printf("\n========================================\n");
    printf("   SUMA DO ZAPLATY: %.2f zl\n", suma_rachunku);
    printf("========================================\n");
    
    printf("\nDziekujemy za zamowienie, %s!\n", imie_klienta);
    printf("Smacznego!\n");
}

// Funkcja glowna
int main() {
#ifdef _WIN32
    ustawKonsole();
#endif

    // Wczytaj menu z pliku
    if (!wczytajMenu("menu.txt")) {
        printf("Nacisnij Enter aby zakonczyc...\n");
        getchar();
        return 1;
    }
    
    // Wyswietl dane restauracji
    wyswietlDaneRestauracji();
    
    // Pobierz imie klienta
    pobierzImie();
    
    // Wybor typu zamowienia
    wybierzTypZamowienia();
    
    // Pobierz dane w zaleznosci od typu
    if (typ_zamowienia == 1) {
        pobierzStolik();
    } else {
        pobierzDaneDostawy();
    }
    
    // Menu zamawiania
    int potwierdzone = menuZamawiania();
    
    if (potwierdzone) {
        // Zapisz rachunek do pliku
        zapiszRachunek();
        
        // Wyswietl podsumowanie
        wyswietlPodsumowanie();
    }
    
    // Czekaj na klawisz
    printf("\nNacisnij Enter aby zakonczyc...\n");
    getchar();
    
    return 0;
}
