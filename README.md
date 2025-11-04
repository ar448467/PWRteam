# Zadanie rekrutacyjne — Software

Wszystkie obliczenia geometryczne w tym projekcie wykonujemy z uwzględnieniem błędu numerycznego epsilon = 1e−9, 
co pozwala uniknąć błędów związanych z niedokładnością typu double przy porównywaniu wartości.

---

## Zadanie 1 — Otoczka wypukła

Pierwszym etapem jest znalezienie otoczki wypukłej zbioru punktów, czyli najmniejszego wielokąta wypukłego, który zawiera wszystkie punkty wejściowe.

Do rozwiązania zastosowałem algorytm Andrew (Monotone Chain), będący prostszym wariantem algorytmu Grahama.

Zalety tego podejścia:
- prosta implementacja
- łatwe uwzględnienie przypadków brzegowych (punkty współliniowe)
- czas działania O(n log n)
- złożoność pamięciowa O(n)

Opis działania:
1. Punkty są sortowane leksykograficznie po współrzędnych (x, y)
2. Następnie budowana jest dolna i górna część otoczki
3. W trakcie dodawania punktów usuwane są te, które powodowałyby, że otoczka przestaje być wypukła (sprawdzamy znak iloczynu wektorowego)

---

## Zadanie 2 — Najwęższy pas 

W tym zadaniu kluczowe jest obserwacja, że jeżeli pas obejmuje wszystkie punkty otoczki, to obejmuje również cały zbiór punktów.

Algorytm wykorzystuje metodę rotating calipers.
W każdym kroku obracamy prostą styczną do kolejnej krawędzi otoczki i szukamy punktu antypodycznego, który maksymalizuje odległość od tej krawędzi.

Punkty antypodyczne to takie dwa punkty na otoczce, dla których można poprowadzić równoległe proste styczne do otoczki w tych punktach.
W ten sposób dla każdej orientacji krawędzi znajdujemy maksymalną odległość między parami prostych równoległych.
Minimalna z tych wartości to szerokość najwęższego pasa.

Złożoność obliczeniowa: O(t), gdzie t to liczba punktów na otoczce wypukłej

---

## Zadanie 3 — Najbliższa para punktów


Wykorzystałem tutaj metodę zamiatania.
Punkty są sortowane po współrzędnej x, a następnie przesuwamy pionową prostą wzdłuż osi x.
W zrównoważonym drzewie binarnym (w tym przypadku set) trzymamy punkty po lewej stronie prostej, które znajdują się wystarczająco blisko bieżącego punktu (w odległości nie większej niż aktualnie znana minimalna odległość).

W każdym kroku sprawdzamy tylko punkty z wąskiego poziomego paska wokół bieżącego punktu – w praktyce znajduje się tam co najwyżej 5 punktów, więc liczba porównań jest stała.

Złożoność czasowa: O(n log n)
Złożoność pamięciowa: O(n)

---

## Informacje dodatkowe

Końcowe rozwiązanie znajduje się na branchu master w pliku zadanie2.cpp.
