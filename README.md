<<<<<<< HEAD
# Restauracja
Żeby skompilować użyj komendy ``g++ -std=c++20 template.cpp -lncurses -o main``
=======
# Karczma "Pod Złamanym Toporem" - System Zamówień

Aplikacja restauracyjna z interfejsem tekstowym, obsługująca zamówienia na miejscu i dostawę.

## Wymagania

### Linux/macOS (z interfejsem ncurses)
- C++ compiler (gcc, clang)
- ncurses development library
- CMake (opcjonalnie)

### Windows (tryb konsolowy)
- C++ compiler (MSVC, gcc, clang)
- CMake (opcjonalnie)

## Instalacja zależności

### Debian/Ubuntu
```bash
sudo apt-get install build-essential cmake libncurses-dev
```

### macOS (homebrew)
```bash
brew install cmake ncurses
```

### Windows
- Zainstaluj Visual Studio Build Tools lub MinGW
- Zainstaluj CMake ze strony https://cmake.org

## Kompilacja

### Metoda 1: CMake (Rekomendowane)

#### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make
./restauracja
```

#### Windows (MSVC)
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
.\Release\restauracja.exe
```

#### Windows (MinGW)
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
make
restauracja.exe
```

### Metoda 2: Kompilacja ręczna

#### Linux/macOS (z ncurses)
```bash
g++ -std=c++11 main.cpp -o restauracja -lncurses
./restauracja
```

#### Linux/macOS (bez ncurses - tryb konsolowy)
```bash
g++ -std=c++11 -DNO_NCURSES main.cpp -o restauracja
./restauracja
```

#### Windows (MSVC)
```bash
cl /std:c++11 /D NO_NCURSES main.cpp
restauracja.exe
```

#### Windows (MinGW)
```bash
g++ -std=c++11 -DNO_NCURSES main.cpp -o restauracja.exe
restauracja.exe
```

## Uruchamianie

Upewnij się, że plik `menu.txt` znajduje się w tym samym katalogu co aplikacja:

```bash
./restauracja    # Linux/macOS
restauracja.exe  # Windows
```

## Struktura projektu

```
.
├── CMakeLists.txt      # Konfiguracja CMake dla kompilacji wieloplatformowej
├── main.cpp            # Główny kod aplikacji z warunkową kompilacją ncurses
├── menu.txt            # Plik z menu restauracji
└── README.md           # Dokumentacja
```

## Funkcjonalność

- Zamówienia na miejscu (ze wskazaniem numeru stolika)
- Zamówienia z dostawą na adres
- Edycja zamówienia (usuwanie dań, czyszczenie koszyka)
- Rachunek podsumowujący zamówienie
- Interfejs tekstowy ncurses (Linux/macOS) lub konsolowy (Windows)

## Obsługa wieloplatformowa

Aplikacja automatycznie:
- **Na Linux/macOS**: Używa biblioteki ncurses dla lepszego interfejsu tekstowego
- **Na Windows**: Przechodzi w tryb konsolowy (brak ncurses)
- **Bez ncurses na dowolnej platformie**: Uruchom z flagą `-DNO_NCURSES`
>>>>>>> 9410edf (Windows fix)
