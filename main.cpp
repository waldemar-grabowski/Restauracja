#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAKS_DAN = 20;

// MENU
string nazwy[MAKS_DAN];
double ceny[MAKS_DAN];
int liczba_dan = 0;

// ZAMÓWIENIE
int zamowione_id[MAKS_DAN];
int zamowione_ile[MAKS_DAN];
int liczba_zamowien = 0;
double suma = 0;

// KLIENT
string imie;
string adres;
int stolik;
int typ;

// ================= DANE RESTAURACJI =================
void pokazKarczme() {
    cout << "KARCZMA POD ZLAMANYM TOPOREM\n";
    cout << "Adres: Trakt Krolewski 7, Smocza Skala\n";
    cout << "Wlasciciel: Grumli Brodaty\n";
    cout << "Godziny: 10:00 - 22:00\n\n";
}

// ================= WCZYTYWANIE MENU =================
void wczytajMenu() {
    ifstream f("menu.txt");
    if (!f) {
        cout << "Blad: Nie mozna otworzyc pliku menu.txt\n";
        return;
    }

    string linia;
    int numerLinii = 0;

    while (getline(f, linia) && liczba_dan < MAKS_DAN) {
        numerLinii++;
        size_t p1 = linia.find(':');
        size_t p2 = linia.find(':', p1 + 1);

        if (p1 == string::npos || p2 == string::npos) {
            cout << "Blad w linii " << numerLinii << ": Nieprawidlowy format (brak dwukropka). Pomijam...\n";
            continue;
        }

        try {
            string nazwa = linia.substr(p1 + 1, p2 - p1 - 1);
            string cenaStr = linia.substr(p2 + 1);
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

// ================= MENU =================
void pokazMenu() {
    cout << "--- MENU ---\n";
    for (int i = 0; i < liczba_dan; i++)
        cout << i + 1 << ". " << nazwy[i] << " - " << ceny[i] << " zl\n";
}

// ================= ZAMÓWIENIE =================
void dodajDanie() {
    pokazMenu();
    int nr, ile;
    cout << "Numer dania: "; cin >> nr;
    if (nr < 1 || nr > liczba_dan) return;
    cout << "Ile porcji: "; cin >> ile;
    zamowione_id[liczba_zamowien] = nr - 1;
    zamowione_ile[liczba_zamowien] = ile;
    suma += ceny[nr - 1] * ile;
    liczba_zamowien++;
}

void pokazZamowienie() {
    if (liczba_zamowien == 0) { cout << "Brak zamowien\n"; return; }
    for (int i = 0; i < liczba_zamowien; i++)
        cout << nazwy[zamowione_id[i]] << " x" << zamowione_ile[i] << " = " << ceny[zamowione_id[i]] * zamowione_ile[i] << " zl\n";
    cout << "SUMA: " << suma << " zl\n";
}

void usunPozycje() {
    if (liczba_zamowien == 0) return;
    pokazZamowienie();
    int nr; cout << "Ktora pozycje usunac: "; cin >> nr; nr--;
    if (nr < 0 || nr >= liczba_zamowien) return;
    suma -= ceny[zamowione_id[nr]] * zamowione_ile[nr];
    for (int i = nr; i < liczba_zamowien - 1; i++) {
        zamowione_id[i] = zamowione_id[i + 1];
        zamowione_ile[i] = zamowione_ile[i + 1];
    }
    liczba_zamowien--;
}

// ================= RACHUNEK =================
void zapiszRachunek() {
    ofstream f("rachunek.txt");
    f << "RACHUNEK\n";
    f << "Klient: " << imie << "\n";
    if (typ == 1) f << "Stolik: " << stolik << "\n";
    else f << "Dostawa: " << adres << "\n";
    f << "Zamowienie:\n";
    for (int i = 0; i < liczba_zamowien; i++)
        f << nazwy[zamowione_id[i]] << " x" << zamowione_ile[i] << " = " << ceny[zamowione_id[i]] * zamowione_ile[i] << " zl\n";
    f << "SUMA: " << suma << " zl\n";
    f.close();
    cout << "Zapisano rachunek do rachunek.txt\n";
}

// ================= GODZINY DOSTAWY =================
bool godzinaOK(int godz) { return godz >= 10 && godz <= 22; }

// ================= MAIN =================
int main() {
    pokazKarczme();
    wczytajMenu();

    cout << "Podaj imie: "; cin >> imie;
    int typ;
    while (true) {
        cout << "1 - Na miejscu, 2 - Dowoz. Wybor: ";
        if (cin >> typ && (typ == 1 || typ == 2)) {
            break;
        }
        else {
            cout << "Blad! Wybierz 1 - Na miejscu, 2 - Dowoz. Wybor:\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    int przewidywanyCzas = 0;
    if (typ == 1) {
        cout << "Numer stolika: ";
        while (!(cin >> stolik)) {
            cout << "Blad! Numer stolika musi byc liczba. Sprobuj ponownie: ";

            cin.clear();
            cin.ignore(1000, '\n');
        }
        przewidywanyCzas = 20;
    }
    else {
        cout << "Adres: "; cin.ignore(); getline(cin, adres);
        int godz;
        while (true) {
            cout << "Godzina dostawy (10-22): ";

            if (cin >> godz) {
                if (godzinaOK(godz)) {
                    break;
                }
                else {
                    cout << "Nieprawidlowa godzina! Restauracja czynna 10-22.\n";
                }
            }
            else {
                cout << "To nie jest liczba! Podaj godzine cyframi.\n";

                cin.clear();
                cin.ignore(1000, '\n');
            }
        }
        przewidywanyCzas = godz;
        cout << "Dostawa zaplanowana na godzine " << przewidywanyCzas << ":00\n";
    }

    int wybor;
    do {
        cout << "1-Dodaj 2-Usun 3-Pokaz 4-Zakoncz "; cin >> wybor;
        if (wybor == 1) dodajDanie();
        if (wybor == 2) usunPozycje();
        if (wybor == 3) pokazZamowienie();
    } while (wybor != 4); // pozwala wyjść od razu

    zapiszRachunek();

    cout << "===== PODSUMOWANIE =====\n";
    pokazZamowienie();

    if (typ == 1)
        cout << "Przewidywany czas przygotowania: ok. 20 minut\n";
    else
        cout << "Przewidywana godzina dostawy: " << przewidywanyCzas << ":00\n";

    cout << "Do zaplaty: " << suma << " zl\n";
    cout << "Dziekujemy za zamowienie!\n";
    cout << "Nacisnij Enter, aby zakonczyc..."; cin.ignore(); cin.get();
}
