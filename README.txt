================================================================================

	TEACA BOGDAN

	Brick Breaker Game

================================================================================

	I. IMPLEMENTAREA TEMEI

	Fisierele in care am lucrat pentru aceasta tema se afla la:
- "Source/Laboratoare/Tema1/Tema1.cpp"
- "Source/Laboratoare/Tema1/Tema1.h"
- "Source/Laboratoare/Tema1/Object2D.cpp"
- "Source/Laboratoare/Tema1/Object2D.h"
- "Source/Laboratoare/Tema1/Transform2D.h"

	Pentru realizarea acestei teme am creat cele 3 functii pentru
transformari 2D: Translate(), Scale() si Rotate() care le-am implementat in
fisierul "Transform2D.h".

	In fisierul "Object2D.cpp" am implementat doua functii:
- CreateRectangle() creaza o mesha dreptunghi prin care reprezint grafic
elementele din joc precum platforma jucatorului, caramizile, powerup-urile
in cadere si zidurile de pe marginea scenei.
- CreateCircle() creaza o mesha cerc prin care reprezint grafic bilele de joc.
	Detalii pentru implementarea acestor doua functii le-am scris in
comentariile acestora.

	In fisierul "Tema1.h" am scris definitiile structurilor de tip "Brick",
"Ball" si "Powerup", care contin proprieti atasate acestor entitati din joc
(coordonatele x, y, viteze, durata). De asemenea, in acest fisier am definit
variabile pe care le-am folosit in fisierul principal al temei ("Tema1.cpp")
cum ar fi variabile pentru dimensiunile scenei, vietile jucatorului,
dimensiunile caramizilor, bilei si powerup-urilor. Am scris cate un comentariu
care clarifica scopul pentru fiecare variabila din acest fisier.

	In fisierul "Tema1.cpp", in functia Init() am initializat camera,
am alocat memorie pentru caramizile din joc si am initializat meshele pentru
elemente din joc (caramizi, powerup-uri, bile, ziduri, etc), dupa care am
initializat primul nivel din joc folosind functia "InitLevel()".

	In functia InitLevel(), am resetat toate variabilele ce tin de
initializarea unui nivel: am setat bila sa stea pe platforma la inceputul
jocului, am resetat lista de bile si powerup-uri, am resetat powerup-urile care
au o durata de timp, am adaugat bila initiala in lista de bile, dupa care
am initializat matricea de caramizi asociata nivelului curent (fiecare nivel
are un alt pattern de caramizi).

	In functia Update() am modularizat fiecare parte din joc la care
fac update la fiecare frame, creand functiile:
- Update_Walls() care translateaza cele 3 ziduri pe marginea din stanga,
dreapta si sus a scenei
- Update_Lives_Icons() care translateaza mesh-ele cu vietile jucatorului in
scena in coltul din stanga jos
- Update_Bricks() care translateaza caramizile care au ramas in nivel si
afiseaza animatia de micsorare a caramizilor care au fost distruse
- Update_Paddle() care translateaza platforma la pozitia sa pe ecran
- Update_Powerups() care translateaza si roteste powerup-urile in cadere
si gestioneaza logica powerup-urilor atunci cand cad sub scena sau sunt
prinse de platforma
- Update_Balls() care translateaza bilele din joc si gestioneaza coliziunile
acestora cu platforma, caramizile si zidurile din joc
- Update_Level_Clear_Check() care verifica daca au mai ramas caramizi, iar
daca nu au mai ramas, atunci sa se faca trecerea la nivelul urmator

	In functia OnMouseMove() am implementat logica pentru miscarea
platformei atunci cand jucatorul misca mouse-ul. Mijlocul platformei se
va afla la coordonata x a cursorului. De asemenea am limitat miscarea
platformei in extremele scenei astfel incat sa elimin cazurile limita in
care jucatorul ar lansa bila direct intr-unul din zidurile de pe margine.

	In functia OnMouseBtnPress() am facut ca atunci cand jucatorul apasa
pe butonul de la mouse, platforma sa lanseze bila.

	Pentru detectarea coliziunilor din joc am implementat functia
"CircleRectangleCollision()" care verifica daca are loc o coliziune intre
un cerc (cum ar fi bila) si un dreptunghi (cum ar fi o caramida sau platforma).
Valoarea returnata de aceasta functie semnifica:
- 0, daca nu are loc nicio coliziune
- 1, daca are loc o coliziune, iar aceasta este in partea stanga sau dreapta
a dreptunghiului
- 2, daca are loc o coliziune, iar aceasta este in partea de jos sau sus
a dreptunghiului
- 3, daca are loc o coliziune, iar aceasta este intr-un colt al dreptunghiului
	Dupa ce am detectat coliziunea, pentru a detecta si in ce parte a
dreptunghiului are loc coliziunea, am verificat in care sectiune marginala
intra cercul, verificand daca cercul se afla intr-o banda ingusta de latime
"collision_const". Astfel imi dau seama in ce directie trebuie sa reflect
directia de miscare a bilei la coliziunea cu o caramida. Pentru detectarea
coliziunii dintre powerup-uri si platforma si detectarea coliziunii dintre
bila si platforma am folosit tot aceasta functie, insa in cazul coliziunii
dintre bila si platforma am reflectat bila folosind functia "arccos" in felul
urmator: am facut diferenta dintre centrul bilei si centrul platformei, astfel
incat sa obtin un numar negativ daca bila se afla in stanga centrului
platformei, un numar pozitiv daca bila se afla in dreapta centrului platformei
si valoarea 0 daca bila se afla fix in centrul platformei. Aceasta diferenta am
impartit-o la jumatate din latimea platformei astfel incat sa obtin un numar
intre -1 si 1 pentru a fi compatibil cu argumentul functiei arccos care primeste
valori din intervalul -1 si 1. Arccos din rezultatul operatiilor de mai sus
imi da unghiul unde trebuie reflectata bila. Dupa aceste operatii am calculat
noua directie a vitezei bilei inmultind viteza efectiva a bilei (speed_modul)
cu cosinusul sau sinusul din rezultatul lui arccosinusul anterior pentru a
ajunge la vitezele pe componenta x si componenta y a vitezei bilei. Acest calcul
se afla in functia "Update_Balls()". Mai multe detalii am scris in comentariile
de la calculul aflat in codul sursa.

	Pentru miscarea bilei in scena am updatat la fiecare frame pozitia
bilelor, adunand de fiecare data la pozitii viteza bilelor (asemanator cu
legea miscarii de la fizica).

	Dupa ce o caramida este distrusa, exista o sansa sa apara un powerup
care sa poata sa fie prins de platforma jucatorului ajutandu-l la castigarea
mai rapida a nivelului.

	Am implementat toate functionalitatile obligatorii ale temei:
- desenare scena
- control platforma cu mouse-ul
- detectie coliziuni bila-platforma, bila-pereti, platforma-powerups
- detectie coliziuni bila-caramizi
- deplasare bila in scena la lansare
- deplasare prin reflexie la coliziunea cu platforma
- deplasare prin reflexie la coliziunea cu peretii si caramizile
- animatie disparitie caramizi si animatie rotire si cadere powerups
- gestiune pierdere vieti
- implementarea unui powerup la alegere

	De asemenea, am implementat si alte functionalitati pentru punctaj bonus,
care sunt descrise in sectiunea urmatoare din readme:

================================================================================

	II. BONUSURI

	Pentru punctaj bonus, am implementat urmatoarele functionalitati:

1. Am implementat mai multe nivele (3 nivele), fiecare nivel cu pattern-ul sau
de caramizi (caramizile sunt asezate altfel in fiecare nivel). Pentru trecerea
de la un nivel la altul trebuie ca jucatorul sa sparga toate caramizile dintr-un
nivel.

2. Unele caramizi sunt mai rezistente decat altele: caramizile rosii trebuie
lovite o singura data pentru a fi sparte, caramizile colorate in albastru deschis
trebuie lovite de 2 ori, iar caramizile albastre trebuie lovite de 3 ori.

3. In timp, viteza bilelor din scena creste pana la o viteza limita, astfel
jocul devine din ce in ce mai dificil.

4. Am implementat powerup pentru extinderea platformei. Astfel, jucatorul poate
prinde mai usor bila. Acest powerup are o durata limita dupa care platforma
revine la dimensiunea originala.

5. Am implementat powerup pentru split-uirea fiecarei bile din scena in doua
bile. Chiar daca una din aceste bile split-uite nu este prinsa de platforma,
jucatorul nu pierde o viata atata timp cat mai ramane in scena macar o bila.

6. Am implementat powerup pentru incetinirea bilelor din joc (deoarece viteza
bilelor creste in timp, acest powerup este util deoarece ajuta jucatorul in
prinderea bilelor cu platforma).

7. Am implementat powerup care acorda jucatorului inca o viata.

8. Am implementat powerup care face bilele sa nu se mai reflecte dupa ce lovesc
caramizi, ci sa treaca prin ele, distrugandu-le. Acest powerup are o durata limita
dupa care bilele revin la normal.

================================================================================