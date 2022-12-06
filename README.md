# Řešení domácích úloh Kasiopea 2022

Moje řešení domácích úloh C - I [soutěže Kasiopea 2022](https://kasiopea.matfyz.cz/archiv/2022/doma/). Všechny úlohy
jsou řešeny v jazyce C++.

(Solution of tasks C - I of [Kasiopea 2022 competition](https://kasiopea.matfyz.cz/archiv/2022/doma/). Assignments are
in the Czech language.)

## Úlohy

Zadání úloh najdete v [archivu soutěže](https://kasiopea.matfyz.cz/archiv/2022/doma/). Úlohy A a B si zkuste vyřešit
sami :).

### [C: Cesta do přírody](taskC.cpp)

Jednoduché řešení spočívající v užití Hashovací tabulky.

### [D: Lesy](taskD.cpp)

Ukládám les do Hashovací tabulky, neboť pro těžkou verzi úlohy se vstup nevleze do 2D pole. Používám algoritmus DFS k
procházení grafu a hledání obdélníků vymezujících lesy.

### [E: Piktogramy](taskE.cpp)

Využívám dynamické programování. Všechny možné sumy piktogramů si za běhu ukládám do Hashovací tabulky a zároveň
vyřazuji ty větší než K. Nakonec bude počet kombinací uložen pod klíčem K.

### [F: Oprava silnic](taskF.cpp)

Nejdříve odstraním problémová města, zkontroluji, zda je graf pořád souvislý, přičtu vždy nejlevnější hranu pro každé
problémové město a na zbývajícím grafu (bez problémových měst) použiji algoritmus Kruskala k nalezení minimální kostry
stromu.

### [G: Bakterie](taskG.cpp)

Nejdříve si uložím souřadnice čtverců vzniklých po K krocích. Potom používám Line Sweep algoritmus, který prochází zdola
nahoru všechny čtverce a postupně sčítá jejich obsahy. Využívám taky segmentový strom na rychlé sčítání intervalů.

### [H: Sušenky](taskH.cpp)

Používám zde dynamické programování. Postupně ukládám sumy S možných rozložení sušenek pod klíčem masky, která
reprezentuje, který org dostane sušenku. Výsledek se nachází pod klíčem se samými jedničkami, tedy 2^K - 1.

### [I: Věrnostní kartičky](taskI.cpp)

V těžké verzi pomocí upraveného algoritmu Dijskra a DFS najdu všechny možné nejkratší cesty od 1 do N. V lehké verzi do
stejné provedu kombinací BFS a efektivnějšího DFS. Potom není-li řešení triviální (všechny hotely řetězce K lze dát do
nenavštívených měst, neležících n nalezených cestách), zkouíšm všechny možné kombinace přiřazení hotelů K na nejkratší
cesty.

Algoritmus je dle mě osobně neelegantní a líbí se mi více autorské řešení.