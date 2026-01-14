# PQHuffman
## Implementacja kodowania Huffmana bazująca na kolejce priorytetowej w C++ 

### Instrukcja obsługi
### 1. Kodowanie 
### W pliku nazwanym "key.txt" umieszczamy tekst, z którego chcemy stworzyć drzewo do kodowania Huffmana, a w pliku "to_encode.txt" wpisujemy tekst do zakodowania. Po uruchomieniu programu wybieramy opcję 1. 

### 2. Dekodowanie
### W "to_decode.txt" powinien się znaleźć tekst do odkodowania, a w pliku "dictionary.txt" - słownik zawierający kody umożliwiające odtworzenie drzewa kodowania Huffmana oraz dekodowanie tekstu. Po uruchomieniu programu wybieramy opcję 2.

Przykładowy plik "dictionary.txt":
```
\s 111
A 0
K 1100
L 1010
M 1011
O 100
T 1101
```

WAŻNE: Znaki białe to odpowiadające im symbole (np. spacja = \s)
