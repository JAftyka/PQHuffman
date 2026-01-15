# PQHuffman
## Implementacja kodowania Huffmana bazująca na kolejce priorytetowej w C++

### Opis
Program umożliwia kodowanie oraz dekodowanie tekstu przy użyciu algorytmu Huffmana.  
Drzewo kodowania budowane jest na podstawie częstotliwości znaków występujących w pliku `key.txt`.

---

## Instrukcja obsługi

### 1. Kodowanie

1. W pliku `key.txt` umieść tekst, na podstawie którego zostanie utworzone drzewo kodowania Huffmana.
2. Uruchom program i wybierz opcję **1**.
3. Podaj nazwę pliku, który chcesz zakodować.

**Wynik działania programu:**
- Tworzony jest plik `encoded.txt`, który zawiera:
  - w **pierwszej linii** – słownik kodów Huffmana,
  - w pozostałej części pliku – zakodowane dane w postaci binarnej.

---

### 2. Dekodowanie

1. Przygotuj plik zawierający:
   - w **pierwszej linii** słownik kodów Huffmana,
   - w dalszej części pliku zakodowane dane binarne.
2. Uruchom program i wybierz opcję **2**.
3. Podaj nazwę pliku do dekodowania.

**Wynik działania programu:**
- Odkodowany tekst zostaje zapisany do pliku `decoded.txt`.

---

## Format słownika

Słownik składa się z par `znak:kod` rozdzielonych średnikiem `;`

### Przykład:
```\s:111;A:0;K:1100;L:1010;M:1011;O:100;T:1101;```

---

## Znaki specjalne

Znaki białe zapisywane są przy użyciu symboli specjalnych:

- `\n` – nowa linia  
- `\s` – spacja  
- `\t` – tabulator  

---

## Uwagi

- Program rozróżnia wielkość liter zgodnie z implementacją kodowania.
- Plik `encoded.txt` zapisywany jest w trybie binarnym.
- Do poprawnego dekodowania wymagany jest niezmieniony słownik z pliku kodowanego.

---

