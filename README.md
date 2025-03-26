# Systemy_Operacyjne_SK

### Instrukcje uruchomienia projektu
#### Budowanie i uruchomienie ze Ÿróde³
Windows (Visual Studio 2022):
1. Otwórz projekt w Visual Studio.
2. W ustawieniach projektu dodaj argument uruchomieniowy (liczbê filozofów).
3. Skompiluj i uruchom program.

#### Uruchamianie z gotowego pliku exe
Pobierz i uruchom gotowy plik binarny .exe z wiersza poleceñ wraz z argumentem (liczb¹ filozofów).
```
.\ProblemJedzacychFilozofow.exe [liczba filozofow]
```
Np.
```
.\ProblemJedzacychFilozofow.exe 7
```
### Opis problemu
Problem jedz¹cych filozofów to klasyczne zagadnienie synchronizacji w programowaniu wielow¹tkowym.
Podana iloœæ filozofów siedzi przy okr¹g³ym stole, miêdzy nimi znajduj¹ siê widelce. Ka¿dy filozof na zmianê myœli i je.
Aby móg³ jeœæ, musi podnieœæ dwa widelce - lewy i prawy. Poniewa¿ ka¿dy widelec jest wspó³dzielony przez dwóch filozofów,
mo¿e dojœæ do zakleszczenia (deadlock) – sytuacji, w której ka¿dy filozof trzyma jeden widelec i czeka na drugi, blokuj¹c siê nawzajem.

### Rozwi¹zanie problemu
Aby unikn¹æ zakleszczenia, zastosowano mutexy (std\::mutex) i mechanizm (std\::lock()) do blokowania obu widelców jednoczeœnie.
Dziêki temu ¿aden filozof nie pozostanie w stanie, w którym trzyma jeden widelec i czeka na drugi.

### W¹tki
W programie dla ka¿dego filozofa powstaje osobny w¹tek, 
który w niekoñcz¹cej siê pêtli wykonuje kolejno zajêcia filozofa: myœlenie, czekanie na widelce, jedzenie.
Myœlenie - filozof czeka przez losowy czas.
Czekanie na widelce - filozof czeka, a¿ oba widelce bêd¹ dostêpne i bêdzie móg³ je zablokowaæ.
Jedzenie - po udanym podniesieniu obu widelców je przez pewien czas, a nastêpnie odk³ada widelce (odblokowuje mutexy).

### Sekcje krytyczne
Sekcj¹ krytyczn¹ w programie jest moment podniesienia dwóch widelców przez filozofa.
Ze wzglêdu na to, ¿e widelce s¹ wspó³dzielone miêdzy parami w¹tków, 
ich liczba jest równa liczbie w¹tków oraz w¹tki potrzebuj¹ dwóch widelców do jedzenia musimy zapobiec sytuacji,
w której ka¿dy w¹tek zabiera jeden widelec i czeka na drugi, którego nigdy nie otrzyma.
#### Rozwi¹zanie sekcji krytycznej
Aby unikn¹æ podanej sytuacji zastosowano std\::mutex (mutual exclusion) dla widelców, 
tak aby ¿aden filozof nie móg³ zabraæ pojedynczego widelca, a tylko dwa gdy oba s¹ dostêpne.
W tym celu zastosowano std\::lock(leftFork, rightFork), aby zablokowaæ oba mutexy lub ¿adnego, 
oraz std\::lock_guard\<std\::mutex> i std\::adopt_lock, aby automatycznie zwolniæ je po u¿yciu.