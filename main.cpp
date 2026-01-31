#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>

using namespace std;

#ifdef _WIN32
#define NO_NCURSES
#endif

#ifndef NO_NCURSES
#include <ncurses.h>
#endif

#define MAX_DAN 10

struct Danie {
    int id;
    char nazwa[50];
    double cena;
    char skladniki[100];
};

struct Danie menu[MAX_DAN];
int liczba_dan = 0;
double suma_rachunku = 0.0;
int stolik = 0;  // globalny dla edycji

void load_menu(const char* plik) {
    FILE* f = fopen(plik, "r");
    if (!f) {
        printf("Blad otwarcia %s!\n", plik);
        return;
    }

    liczba_dan = 0;
    while (liczba_dan < MAX_DAN && 
        fscanf(f, "%d:%49[^:]:%lf:%99[^\n]", 
        &menu[liczba_dan].id,
        menu[liczba_dan].nazwa,
        &menu[liczba_dan].cena,
        menu[liczba_dan].skladniki) == 4) {
        liczba_dan++;
    }
    fclose(f);
}

int welcome_screen();
string delivery();
int table();
int take_order();
void edit_order();
void handle_order_choice(int choice);

#ifndef NO_NCURSES
// ===== NCURSES VERSION =====
int welcome_screen(){
    int h = 10, w = 50;
    int starty = (LINES - h) / 2; 
    int startx = (COLS  - w) / 2;

    WINDOW *win = newwin(h, w, starty, startx);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Witamy w: Karczma 'Pod Zlamanym Toporem'");
    mvwprintw(win, 2, 2, "Adres: Trakt Krolewski 7");
    mvwprintw(win, 3, 2, "Wlasciciel: Brodaty Krasnolud");
    mvwprintw(win, 5, 2, "Wcisnij 1: zamowic na miejscu");
    mvwprintw(win, 6, 2, "Wcisnij 2: zamowic na wynos");
    mvwprintw(win, 8, 2, "Twoj wybor (1/2): ");
    wrefresh(win);

    int del;
    echo();
    mvwscanw(win, 8, 19, "%d", &del);
    noecho();
    delwin(win);
    return del;
}

string delivery(){
    int h = 10, w = 50;
    int starty = (LINES - h) / 2; 
    int startx = (COLS  - w) / 2;

    WINDOW *win = newwin(h, w, starty, startx);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Prosze podac swoj adres ponizej");
    mvwprintw(win, 2, 2, ":");

    char cust_adres[100] = {0};
    echo();
    mvwgetnstr(win, 2, 3, cust_adres, 99);
    noecho();
    delwin(win);
    return string(cust_adres); 
}

int table(){
    int h = 10, w = 50;
    int starty = (LINES - h) / 2; 
    int startx = (COLS  - w) / 2;
    
    WINDOW *win = newwin(h, w, starty, startx);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Prosze podac numer stolika: ");
    mvwprintw(win, 2, 2, ":");

    int tab;
    echo();
    mvwscanw(win, 2, 3, "%d", &tab);
    noecho();
    delwin(win);
    return tab;
}

int take_order() {
    int h = 12, w = 60;
    int starty = (LINES - h) / 2; 
    int startx = (COLS - w) / 2;
    WINDOW *win = newwin(h, w, starty, startx);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "=== MENU KARCZMY ===");
    for (int i = 0; i < liczba_dan; i++) {
        mvwprintw(win, 2 + i, 2, "%d) %s (%.2f zl)", 
                  menu[i].id, menu[i].nazwa, menu[i].cena);
    }
    mvwprintw(win, 10, 2, "9) Edytuj  0) Dalej (Suma: %.2f zl)", suma_rachunku);
    wrefresh(win);

    int meal;
    echo();
    mvwscanw(win, 11, 2, "%d", &meal);
    noecho();
    delwin(win);
    return meal;
}

void edit_order() {
    int h = 15, w = 70;
    int starty = (LINES - h) / 2; 
    int startx = (COLS - w) / 2;
    WINDOW *win = newwin(h, w, starty, startx);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "=== EDYCJA ZAMOWIENIA ===");
    mvwprintw(win, 2, 2, "Aktualna suma: %.2f zl", suma_rachunku);
    mvwprintw(win, 4, 2, "1) Usun ostatnie danie");
    mvwprintw(win, 5, 2, "2) Zmien stolik (jezeli na miejscu)");
    mvwprintw(win, 6, 2, "3) Wyczysc zamowienie");
    mvwprintw(win, 8, 2, "0) Wroc do menu");
    mvwprintw(win, 9, 2, "Wybierz (0-3): ");
    wrefresh(win);

    int opcja;
    echo();
    mvwscanw(win, 9, 19, "%d", &opcja);
    noecho();
    
    if (opcja == 1) {
        if (liczba_dan > 0) {
            suma_rachunku -= menu[liczba_dan - 1].cena;
            mvwprintw(win, 11, 2, "Usunieto ostatnie!");
        } else {
            mvwprintw(win, 11, 2, "Puste zamowienie!");
        }
    } else if (opcja == 2) {
        stolik = table();
        mvwprintw(win, 11, 2, "Nowy stolik zapisany!");
    } else if (opcja == 3) {
        suma_rachunku = 0.0;
        mvwprintw(win, 11, 2, "Wyczyszczono wszystko!");
    }
    
    wrefresh(win);
    mvwprintw(win, 13, 2, "Nacisnij klawisz...");
    wrefresh(win);
    getch();
    delwin(win);
}

void handle_order_choice(int choice) {
    for (int i = 0; i < liczba_dan; i++) {
        if (menu[i].id == choice) {
            suma_rachunku += menu[i].cena;
            return;
        }
    }
    if (choice == 9) {
        edit_order();
    }
}

#else
// ===== CONSOLE FALLBACK VERSION (Windows) =====
int welcome_screen(){
    printf("\n=== Karczma 'Pod Zlamanym Toporem' ===\n");
    printf("Adres: Trakt Krolewski 7\n");
    printf("Wlasciciel: Brodaty Krasnolud\n\n");
    printf("1: zamowic na miejscu\n");
    printf("2: zamowic na wynos\n\n");
    printf("Twoj wybor (1/2): ");
    int d = 0; 
    scanf("%d", &d);
    getchar(); // consume newline
    return d;
}

string delivery(){
    char buf[200] = {0};
    printf("Prosze podac swoj adres: ");
    fgets(buf, sizeof(buf), stdin);
    string s(buf);
    if (!s.empty() && s.back() == '\n') s.pop_back();
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return s;
}

int table(){
    printf("Prosze podac numer stolika: ");
    int t = 0; 
    scanf("%d", &t);
    getchar(); // consume newline
    return t;
}

int take_order() {
    printf("\n=== MENU KARCZMY ===\n");
    for (int i = 0; i < liczba_dan; i++) {
        printf("%d) %s (%.2f zl)\n", menu[i].id, menu[i].nazwa, menu[i].cena);
    }
    printf("9) Edytuj  0) Dalej (Suma: %.2f zl)\n\n", suma_rachunku);
    printf("Wybor: ");
    int meal = 0; 
    scanf("%d", &meal);
    getchar(); // consume newline
    return meal;
}

void edit_order() {
    printf("\n=== EDYCJA ZAMOWIENIA ===\n");
    printf("Aktualna suma: %.2f zl\n", suma_rachunku);
    printf("1) Usun ostatnie danie\n");
    printf("2) Zmien stolik (jezeli na miejscu)\n");
    printf("3) Wyczysc zamowienie\n");
    printf("0) Wroc do menu\n\n");
    printf("Wybierz (0-3): ");
    int opcja = 0; 
    scanf("%d", &opcja);
    getchar();
    
    if (opcja == 1) {
        if (liczba_dan > 0) suma_rachunku -= menu[liczba_dan - 1].cena;
        printf("Usunieto ostatnie!\n");
    } else if (opcja == 2) {
        stolik = table();
        printf("Nowy stolik zapisany!\n");
    } else if (opcja == 3) {
        suma_rachunku = 0.0;
        printf("Wyczyszczono wszystko!\n");
    }
    printf("Nacisnij Enter...\n");
    getchar();
}

void handle_order_choice(int choice) {
    for (int i = 0; i < liczba_dan; i++) {
        if (menu[i].id == choice) {
            suma_rachunku += menu[i].cena;
            return;
        }
    }
    if (choice == 9) {
        edit_order();
    }
}
#endif

int main() {
#ifndef NO_NCURSES
    initscr();
    cbreak();
    noecho();
#endif

    load_menu("menu.txt");

    int wybor = welcome_screen();
    string adres;
    
    if (wybor == 2) {
        adres = delivery();
    } else {
        stolik = table();
    }

    int meal;
    do {
        meal = take_order();
        handle_order_choice(meal);
    } while (meal != 0);

#ifndef NO_NCURSES
    endwin();
#endif
    
    printf("\n=== RACHUNEK ===\n");
    printf("Suma: %.2f zl\n", suma_rachunku);
    if (wybor == 2) {
        printf("Dostawa na: %s\n", adres.c_str());
    } else {
        printf("Stolik nr: %d\n", stolik);
    }
    printf("Nacisnij Enter...\n");
    getchar();
    
    return 0;
}
