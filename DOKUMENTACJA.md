# DOKUMENTACJA PROJEKTU
## Karczma "Pod Złamanym Toporem" - System Zamówień

---

## USER STORIES (Historie użytkownika)

### User Story 1: Zamówienie na miejscu
**Jako** gość karczmy  
**Chcę** złożyć zamówienie na miejscu  
**Aby** móc zjeść posiłek przy stoliku

**Kryteria akceptacji:**
- Mogę podać swoje imię
- Mogę wybrać numer stolika (1-15)
- Widzę pełne menu z cenami i składnikami
- Mogę zamówić wiele dań i wybrać ilość porcji
- Widzę sumę zamówienia po każdej zmianie
- Otrzymuję informację o czasie przygotowania (15-25 min)

---

### User Story 2: Zamówienie z dostawą
**Jako** klient zamawiający dowóz  
**Chcę** złożyć zamówienie z dostawą do domu  
**Aby** zjeść posiłek bez wychodzenia z domu

**Kryteria akceptacji:**
- Mogę podać swoje imię
- Mogę podać adres dostawy
- Mogę wybrać godzinę dostawy w godzinach otwarcia (10:00-22:00)
- System sprawdza czy godzina jest co najmniej 30 minut od teraz
- Widzę potwierdzenie adresu i godziny dostawy
- Otrzymuję rachunek zapisany do pliku

---

### User Story 3: Modyfikacja zamówienia
**Jako** klient  
**Chcę** móc modyfikować swoje zamówienie przed potwierdzeniem  
**Aby** poprawić ewentualne pomyłki

**Kryteria akceptacji:**
- Mogę dodawać dania do zamówienia
- Mogę usuwać pozycje z zamówienia
- Mogę zobaczyć aktualne zamówienie w każdej chwili
- Suma jest automatycznie aktualizowana
- Mogę anulować zamówienie i wyjść
- Muszę zamówić minimum 1 danie aby potwierdzić

---

## SCHEMAT DZIAŁANIA APLIKACJI

```
┌─────────────────────────────────────┐
│           START                     │
└─────────────────┬───────────────────┘
                  ▼
┌─────────────────────────────────────┐
│    Wczytaj menu z pliku             │
│    (menu.txt)                       │
└─────────────────┬───────────────────┘
                  ▼
         ┌────────┴────────┐
         │ Plik istnieje?  │
         └────────┬────────┘
           NIE │    │ TAK
               ▼    ▼
        ┌──────┐  ┌─────────────────────────────┐
        │ BŁĄD │  │ Wyświetl dane restauracji   │
        │ KONIEC │ └─────────────┬───────────────┘
        └──────┘               ▼
                  ┌─────────────────────────────┐
                  │ Pobierz imię klienta        │
                  └─────────────┬───────────────┘
                                ▼
                  ┌─────────────────────────────┐
                  │ Wybór typu zamówienia       │
                  │ 1-Na miejscu 2-Dowóz 0-Wyjdź│
                  └─────────────┬───────────────┘
                                ▼
              ┌─────────────────┼─────────────────┐
              │                 │                 │
         [1]  ▼            [2]  ▼            [0]  ▼
    ┌─────────────┐    ┌─────────────┐    ┌─────────┐
    │Podaj stolik │    │Podaj adres  │    │ KONIEC  │
    │  (1-15)     │    │Podaj godzinę│    └─────────┘
    └──────┬──────┘    │(sprawdzanie)│
           │           └──────┬──────┘
           └────────┬─────────┘
                    ▼
       ┌────────────────────────────┐
       │     MENU ZAMAWIANIA        │
       │  1-Dodaj  2-Usuń  3-Pokaż  │
       │  4-Potwierdź  0-Anuluj     │
       └────────────┬───────────────┘
                    ▼
      ┌─────────────┼─────────────────────┐
      │             │             │       │
 [1]  ▼        [2]  ▼        [3]  ▼  [4]  ▼
┌──────────┐ ┌──────────┐ ┌──────────┐    │
│Wyświetl  │ │Wyświetl  │ │Wyświetl  │    │
│menu      │ │zamówienie│ │zamówienie│    │
│Wybierz   │ │Wybierz   │ └────┬─────┘    │
│danie     │ │pozycję   │      │          │
│Podaj     │ │Usuń      │ ◄────┘          │
│ilość     │ └────┬─────┘                 │
│Dodaj     │      │                       ▼
└────┬─────┘      │            ┌──────────────────┐
     │            │            │ Czy jest min.    │
     └──────┬─────┴────────────┤ 1 danie?         │
            │                  └────────┬─────────┘
            ▼                    NIE │    │ TAK
   [Powrót do menu]                 ▼    ▼
                              ┌──────┐ ┌─────────────┐
                              │Błąd! │ │Zapisz       │
                              │Powrót│ │rachunek     │
                              └──────┘ │do pliku .txt│
                                       └──────┬──────┘
                                              ▼
                                       ┌─────────────┐
                                       │Wyświetl     │
                                       │podsumowanie │
                                       └──────┬──────┘
                                              ▼
                                       ┌─────────────┐
                                       │Czekaj na    │
                                       │Enter        │
                                       └──────┬──────┘
                                              ▼
                                       ┌─────────────┐
                                       │   KONIEC    │
                                       └─────────────┘
```

---

## STRUKTURA DANYCH

### Struktura Danie (menu)
```c
struct Danie {
    int id;           // Numer dania (1, 2, 3...)
    char nazwa[50];   // Nazwa dania
    double cena;      // Cena za porcję
    char skladniki[100]; // Lista składników
};
```

### Struktura Pozycja (zamówienie)
```c
struct Pozycja {
    int id_dania;     // Numer dania z menu
    char nazwa[50];   // Nazwa dania
    double cena;      // Cena jednostkowa
    int ilosc;        // Ilość porcji
};
```

---

## KONTROLA BŁĘDÓW

| Sytuacja | Reakcja aplikacji |
|----------|-------------------|
| Plik menu nie istnieje | Komunikat błędu, zakończenie |
| Puste imię | Zastąpienie przez "Nieznajomy" |
| Nieprawidłowa liczba | Prośba o ponowne wpisanie |
| Liczba poza zakresem | Prośba o ponowne wpisanie |
| Godzina poza zakresem | Prośba o ponowne wpisanie |
| Za późno na dostawę | Komunikat, zakończenie |
| Pusty adres | Prośba o ponowne wpisanie |
| Puste zamówienie | Nie pozwala potwierdzić |

---

## PLIKI APLIKACJI

| Plik | Typ | Opis |
|------|-----|------|
| `main.cpp` | Wejściowy | Kod źródłowy aplikacji |
| `menu.txt` | Wejściowy | Menu karczmy (ID:Nazwa:Cena:Składniki) |
| `rachunek_*.txt` | Wyjściowy | Zapisany rachunek klienta |

---

*Dokumentacja projektu - Karczma "Pod Złamanym Toporem"*
