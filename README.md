# Systemy_Operacyjne_SK

### Instrukcje uruchomienia projektu
#### Budowanie i uruchomienie ze źródeł
Windows (Visual Studio 2022):
1. Otwórz projekt w Visual Studio.
2. W ustawieniach projektu dodaj argument uruchomieniowy (liczbę filozofów).
3. Skompiluj i uruchom program.

#### Kompilacja programu
Do kompilacji można też użyć komendy.
```
g++ -std=c++17 -pthread ProblemJedzacychFilozofow.cpp -o ProblemJedzacychFilozofow
```

#### Uruchamianie z gotowego pliku exe
Pobierz i uruchom gotowy plik binarny .exe z wiersza poleceń wraz z argumentem (liczbą filozofów).
```
.\ProblemJedzacychFilozofow.exe [liczba filozofow]
```
Np.
```
.\ProblemJedzacychFilozofow.exe 7
```
### Opis problemu
Problem jedzących filozofów to klasyczne zagadnienie synchronizacji w programowaniu wielowątkowym.
Podana ilość filozofów siedzi przy okrągłym stole, między nimi znajdują się widelce. Każdy filozof na zmianę myśli i je.
Aby mógł jeść, musi podnieść dwa widelce - lewy i prawy. Ponieważ każdy widelec jest współdzielony przez dwóch filozofów,
może dojść do zakleszczenia (deadlock) – sytuacji, w której każdy filozof trzyma jeden widelec i czeka na drugi, blokując się nawzajem.

### Rozwiązanie problemu
Aby uniknąć zakleszczenia, zastosowano mutexy (std\::mutex) i mechanizm (std\::lock()) do blokowania obu widelców jednocześnie.
Dzięki temu żaden filozof nie pozostanie w stanie, w którym trzyma jeden widelec i czeka na drugi.

### Wątki
W programie dla każdego filozofa powstaje osobny wątek, 
który w niekończącej się pętli wykonuje kolejno zajęcia filozofa: myślenie, czekanie na widelce, jedzenie.
Myślenie - filozof czeka przez losowy czas.
Czekanie na widelce - filozof czeka, aż oba widelce będą dostępne i będzie mógł je zablokować.
Jedzenie - po udanym podniesieniu obu widelców je przez pewien czas, a następnie odkłada widelce (odblokowuje mutexy).

### Sekcje krytyczne
Sekcją krytyczną w programie jest moment podniesienia dwóch widelców przez filozofa.
Ze względu na to, że widelce są współdzielone między parami wątków, 
ich liczba jest równa liczbie wątków oraz wątki potrzebują dwóch widelców do jedzenia musimy zapobiec sytuacji,
w której każdy wątek zabiera jeden widelec i czeka na drugi, którego nigdy nie otrzyma.
#### Rozwiązanie sekcji krytycznej
Aby uniknąć podanej sytuacji zastosowano std\::mutex (mutual exclusion) dla widelców, 
tak aby żaden filozof nie mógł zabrać pojedynczego widelca, a tylko dwa gdy oba są dostępne.
W tym celu zastosowano std\::lock(leftFork, rightFork), aby zablokować oba mutexy lub żadnego, 
oraz std\::lock_guard\<std\::mutex> i std\::adopt_lock, aby automatycznie zwolnić je po użyciu.
