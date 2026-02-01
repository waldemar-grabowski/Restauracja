# INSTRUKCJA OBSŁUGI
## Karczma "Pod Złamanym Toporem" - System Zamówień

---

## 1. URUCHOMIENIE APLIKACJI

1. Otwórz folder `build/Release`
2. Uruchom plik `restauracja.exe`
3. Aplikacja wyświetli ekran powitalny z danymi karczmy

---

## 2. PODANIE DANYCH

### Krok 1: Podaj swoje imię
- Wpisz swoje imię i naciśnij Enter
- Jeśli zostawisz puste, otrzymasz imię "Nieznajomy"

### Krok 2: Wybierz typ zamówienia
```
1 - Na miejscu (w karczmie)
2 - Dowóz (do domu)
0 - Wyjście z aplikacji
```

### Krok 3a: Jeśli wybrałeś "Na miejscu"
- Podaj numer stolika (1-15)

### Krok 3b: Jeśli wybrałeś "Dowóz"
- Podaj adres dostawy
- Podaj godzinę dostawy (w godzinach otwarcia 10:00-22:00)
- Podaj minuty dostawy
- Aplikacja sprawdza czy godzina jest prawidłowa!

---

## 3. ZAMAWIANIE DĄŃ

Po podaniu danych pojawi się menu:
```
1 - Dodaj danie do zamówienia
2 - Usuń pozycję z zamówienia
3 - Zobacz aktualne zamówienie
4 - Potwierdź zamówienie
0 - Anuluj i wyjdź
```

### Dodawanie dania (opcja 1)
1. Wybierz numer dania z wyświetlonego menu
2. Podaj ile porcji chcesz (1-10)
3. Danie zostanie dodane, suma zaktualizowana

### Usuwanie pozycji (opcja 2)
1. Aplikacja pokaże Twoje aktualne zamówienie
2. Wybierz numer pozycji do usunięcia
3. Pozycja zostanie usunięta, suma zaktualizowana

### Podgląd zamówienia (opcja 3)
- Wyświetla wszystkie zamówione dania
- Pokazuje ilość porcji każdego dania
- Pokazuje aktualną sumę do zapłaty

### Potwierdzenie (opcja 4)
- Musisz mieć co najmniej 1 danie w zamówieniu!
- Zamówienie zostanie zapisane do pliku

---

## 4. PODSUMOWANIE

Po potwierdzeniu zamówienia:
1. Rachunek zapisywany jest do pliku `.txt` (np. `rachunek_20240115_143022.txt`)
2. Wyświetlane jest podsumowanie z czasem dostawy/przygotowania
3. Naciśnij Enter aby zakończyć

---

## 5. KONTROLA BŁĘDÓW

Aplikacja sprawdza:
- ✓ Czy podano liczbę (nie litery)
- ✓ Czy liczba jest w dozwolonym zakresie
- ✓ Czy godzina dostawy mieści się w godzinach otwarcia
- ✓ Czy minęło minimum 30 minut od teraz do dostawy
- ✓ Czy zamówiono co najmniej 1 danie
- ✓ Czy plik menu istnieje

---

## 6. PLIKI

| Plik | Opis |
|------|------|
| `restauracja.exe` | Aplikacja |
| `menu.txt` | Menu karczmy (musi być obok .exe) |
| `rachunek_*.txt` | Zapisane rachunki |

---

## 7. ROZWIĄZYWANIE PROBLEMÓW

### "Nie można otworzyć pliku menu"
- Sprawdź czy `menu.txt` jest w tym samym folderze co `restauracja.exe`

### "Jest już za późno na zamówienie"
- Karczma działa 10:00-22:00, spróbuj jutro

### Polskie znaki się nie wyświetlają
- Uruchom program w Windows Terminal lub PowerShell
- Czcionka konsoli musi obsługiwać UTF-8

---

*Karczma "Pod Złamanym Toporem" - Smacznego!*
