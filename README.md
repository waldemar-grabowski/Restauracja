# Karczma "Pod Złamanym Toporem" - System Zamówień

Aplikacja konsolowa do zarządzania zamówieniami w karczmie fantasy.
Projekt na pierwszy semestr studiów informatycznych.

## Wymagania

- Windows 10/11
- Visual Studio 2022 (lub nowszy)
- CMake 3.16+

## Kompilacja

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Uruchomienie

```bash
cd build/Release
restauracja.exe
```

## Funkcje aplikacji

### 1. Podanie danych wstępnych
- Wyświetlanie danych restauracji (nazwa, adres, właściciel)
- Podanie imienia klienta
- Wybór: na miejscu lub dowóz
- Na miejscu → numer stolika (1-15)
- Dowóz → adres + godzina dostawy (z kontrolą zakresu)

### 2. Wybór dań z menu
- Menu wczytywane z pliku `menu.txt`
- Każda pozycja ma: nazwę, cenę, składniki
- Można zamówić dowolną liczbę porcji (1-10)
- Można usunąć pozycję z zamówienia
- Suma jest aktualizowana po każdej zmianie
- Minimum 1 danie do złożenia zamówienia
- Można wyjść w dowolnym momencie

### 3. Podsumowanie zamówienia
- Wyświetlenie czasu dostawy/przygotowania
- Zapis rachunku do pliku `.txt`
- Zamknięcie aplikacji po naciśnięciu Enter

## Format pliku menu.txt

```
ID:Nazwa:Cena:Składniki
```

Przykład:
```
1:Pierog z miesem:12.50:ciasto, mieso wolowe, cebula, przyprawy
2:Zupa grzybowa:8.00:grzyby lesne, smietana, ziemniaki
```

## Struktura projektu

```
Restauracja/
├── main.cpp        # Kod źródłowy aplikacji
├── menu.txt        # Plik z menu karczmy
├── CMakeLists.txt  # Konfiguracja CMake
├── README.md       # Ten plik
└── build/          # Folder z kompilacją
```

## Autor

Projekt studencki - 1 semestr informatyki.
