314CD Cristescu Diana-Andreea

Explicatii functii:

get_input : - citeste cu getchar pana cand intalneste un caracter
              care exista in stringul dat(ex:numbers, letters)
char_to_int : - transforma caracterele in numere folosind codul
                 ascii
get_number_input : -folosind get_input cu stringul numbers si 
                    char_to_input ia prima cifra pe care o 
                    intalneste dupa care o salveaza si transforma 
                    intr-un int, ia urmatorul caracter si testeaza 
                    daca e numar sau nu si returneaza ori numarul
                    format din cele doua caractere ori doar din primul
points : - folosind lista de puncte returneaza cate puncte valoreaza 
            caracterul primit
put_word : - primeste coordonatele(x, y), directia(d) si un pointer 
                la word_pts(un string in care voi salva cuvantul 
                pus in game_board, punctele si numarul de casute 1
                sau 2 din bonus_board pe care le ocupa cuvntul)

                format word_pts:
                ww...wwppxy    zona cu w = cuvant
                               zona cu p(2 char-uri) = punctaj
                               zona cu x(1 char) = nr de casute cu 1
                                                   din bonus
                               zona cu y(1 char) = nr de casute cu 2
                                                   din bonus 
            - functia foloseste get_input cu stringul letters ca sa 
                ia litere din stdin si le pune in variabila input 
            - exista un while care merge doar atata timp cat input 
              contine litere, iar in interiorul lui se ia urmatoarea 
              litera la sfarsit cu getchar,iar la inceput se copiaza 
              fiecare litera in word_pts cu strncat 
            - in while se calculeaza punctajul cu ajutorul functiei points
                si se testeaza daca pts_1(variabila care retine cifra unitii 
                punctajului) devine mai mare decat 9, caz in care se
                incrementeaza pts_2(variabila care retine cifra zecilor
                punctajului) si se face pts_1 %= 10 ca sa scap de cifra
                zecilor
            - tot aici se numara de cate ori cuvantul este scris intr-o 
            casuta cu bonus(xx_bonus - retine de cate ori se suprapune cu 
            o casuta 1 in bonus_board; yy_bonus - retine de cate ori se 
            suprapune cu o casuta 2 in bonus_board)
            - se scrie litera in game_board la coordonatele corecte in functie
            de directie
            - dupa while se concateneaza la word_pts in ordinea descrisa mai sus
            pts_1 pts_2 xx_bonus si yy_bonus
get_pts : - ia din word_pts partea descrisa mai sus cu pp(punctajul) si o
             transforma in int
check_XX : !a nu se incurca cu check_xx(care face acelasi lucru doar ca primeste 
            cuvantul fara alte numere la coada)
            - ia word_pts si copiaza doar partea cu cuvantul in var word 
            - testeaza daca XX se afla in word si returneaza 1 daca da si
             0 daca nu
check_YY : !la fel ca la check_XX exista si check_yy 
            - ia word_pts si copiaza ultimele 2 caractere din cuvant in sufix
            - testeaza daca sufix coincide cu YY cu ajutorul strcmp si 
            returneaza 1 daca da si 0 daca nu
bonus_XX : - returneaza din word_pts numarul de dati in care cuv s-a 
            suprapus cu o casuta 1 in bonus_board
bonus_YY : - la fel ca la bonus_XX doar ca pentru casutele cu 2
add_played_word : - o functie care adauga cuvantul deabea adaugat in
                    game_board intr-un vector de cuvinte deja 
                    jucate(words_played) folosind partea cu cuv din word_pts
check_tile : - testeaza daca pozitia de la coord x, y din game_board e ocupata
                sau nu sau daca x sau y au trecut de marginea matricii
check_space_horizontally : - verifica cu ajutorul lui check_tile daca exista 
                                spatiu pe orizontala pentru cuvant
check_space_vertically : - exact ca la functia anterioara doar ca pe verticala
put_played_word : - pune cuvantul ales de program in game_board
is_not_played : - verifica daca cuvantul pe care vrem sa il punem in game_board
                    a fost deja jucat cu ajutorul vect words_played
play_word : - foloseste functiile de mai sus ca sa aleaga si sa puna un cuvant
                in game_board 
            - mai intai trece prin vect words dupa care pt fiecare cuv
            verifica daca exista spatiu pentru el incepand din stanga sus 
            - compara daca prima litera din cuv este aceeasi cu cea la care 
            casuta a ajuns si daca cuv a fost sau nu deja folosit
            - daca are spatiu pe orizontala il pune asa, iar daca nu si are
            spatiu pe verticala il pune asa
            - o sa incerce cuvinte noi pana il gaseste pe primul, moment in 
            care se iese din functie prin return 
check_xx : - la fel ca la check_XX doar ca nu primeste word_pts ci direct cuv
check_yy : - analog check_xx doar ca cu check_YY 
calculate_points : - calculeaza punctajul similar cu ce am facut si mai sus doar 
                    ca primeste cuvantul intreg, nu caracter cu caracter
play_word_with_points : - similar cu play_word doar ca in loc sa puna cuv 
                            in game_board cand il gaseste salveaza punctajul
                            sau(best_score) coordonatele sale(best_x, best_y),
                            directia sa(best_d) si cuvantul in sine(best_word)
                            pentru a putea compara mai tarziu in functie de 
                            parametrii dati in enuntul temei cu urmatoarele cuv
                            iar daca acestia sunt indepliniti se inlocuiesc var 
                            salvate cu cele noi pana trece prin toate posibilitatile
                            si o gaseste pe cea mai buna
                        - odata ce a gasit cuv bun il pune in game_board
                        si returneaza best_score

Explicatii main:

-> N, X, Y, D, XX, YY sunt acelasi lucru ca si in enuntul temei
-> input = inputul de la tastatura caracter cu caracter
-> se ia input si in functie de ce e(0, 1, 2, 3, 4, 5) se rezolva 
    task-urile temei
-> la 0 se printeaza game_board
-> la 1 se iau de la stdin N, X, Y, D in ordinea corecta si se 
    pun pe rand cuvintele date de checker(put_word) si se afiseaza
    game_board
-> la 2 fac acelasi lucru ca si la 1 doar ca fara print_board si
    calculez scorul fiecarui player dupa care il afisez 
-> la 3 fac ce fac la 2 doar ca mai citesc de la stdin si XX si YY
     si calculez punctajul si in functie de bonus_board
-> la 4 fac ce fac si la 1 doar ca fara punctaj si folosesc 
    add_player_word pentru a calcula si pune ultimul cuvant si
    pe parcurs adaug cuvintele jucate in words_played
-> la 5 fac ce fac si la 4 doar ca mai calculez punctajul cu bonusuri
    ca si la 3 si folosesc noul play_word_with_points pentru a alege 
    si pune cuvantul optim si pentru a recalcula punctajul lui player2
    -> in functie de cine castiga afisez board-ul sau "Fail!"




