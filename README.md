# Systemy_Operacyjne_SK

### Instrukcje uruchomienia projektu
#### Budowanie i uruchomienie ze �r�de�
Windows (Visual Studio 2022):
1. Otw�rz projekt w Visual Studio.
2. W ustawieniach projektu dodaj argument uruchomieniowy (liczb� filozof�w).
3. Skompiluj i uruchom program.

#### Uruchamianie z gotowego pliku exe
Pobierz i uruchom gotowy plik binarny .exe z wiersza polece� wraz z argumentem (liczb� filozof�w).
```
.\ProblemJedzacychFilozofow.exe [liczba filozofow]
```
Np.
```
.\ProblemJedzacychFilozofow.exe 7
```
### Opis problemu
Problem jedz�cych filozof�w to klasyczne zagadnienie synchronizacji w programowaniu wielow�tkowym.
Podana ilo�� filozof�w siedzi przy okr�g�ym stole, mi�dzy nimi znajduj� si� widelce. Ka�dy filozof na zmian� my�li i je.
Aby m�g� je��, musi podnie�� dwa widelce - lewy i prawy. Poniewa� ka�dy widelec jest wsp�dzielony przez dw�ch filozof�w,
mo�e doj�� do zakleszczenia (deadlock) � sytuacji, w kt�rej ka�dy filozof trzyma jeden widelec i czeka na drugi, blokuj�c si� nawzajem.

### Rozwi�zanie problemu
Aby unikn�� zakleszczenia, zastosowano mutexy (std\::mutex) i mechanizm (std\::lock()) do blokowania obu widelc�w jednocze�nie.
Dzi�ki temu �aden filozof nie pozostanie w stanie, w kt�rym trzyma jeden widelec i czeka na drugi.

### W�tki
W programie dla ka�dego filozofa powstaje osobny w�tek, 
kt�ry w nieko�cz�cej si� p�tli wykonuje kolejno zaj�cia filozofa: my�lenie, czekanie na widelce, jedzenie.
My�lenie - filozof czeka przez losowy czas.
Czekanie na widelce - filozof czeka, a� oba widelce b�d� dost�pne i b�dzie m�g� je zablokowa�.
Jedzenie - po udanym podniesieniu obu widelc�w je przez pewien czas, a nast�pnie odk�ada widelce (odblokowuje mutexy).

### Sekcje krytyczne
Sekcj� krytyczn� w programie jest moment podniesienia dw�ch widelc�w przez filozofa.
Ze wzgl�du na to, �e widelce s� wsp�dzielone mi�dzy parami w�tk�w, 
ich liczba jest r�wna liczbie w�tk�w oraz w�tki potrzebuj� dw�ch widelc�w do jedzenia musimy zapobiec sytuacji,
w kt�rej ka�dy w�tek zabiera jeden widelec i czeka na drugi, kt�rego nigdy nie otrzyma.
#### Rozwi�zanie sekcji krytycznej
Aby unikn�� podanej sytuacji zastosowano std\::mutex (mutual exclusion) dla widelc�w, 
tak aby �aden filozof nie m�g� zabra� pojedynczego widelca, a tylko dwa gdy oba s� dost�pne.
W tym celu zastosowano std\::lock(leftFork, rightFork), aby zablokowa� oba mutexy lub �adnego, 
oraz std\::lock_guard\<std\::mutex> i std\::adopt_lock, aby automatycznie zwolni� je po u�yciu.