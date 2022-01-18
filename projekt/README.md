Komunikator z użyciem kolejek IPC.
Repozytorium zawiera projekt komunikatora z wykorzystaniem kolejek IPC zaprojektowany przez Jana Krenza i Piotra Wojsznisa w ramach projektu na zajęcia z przedmiotu Programowanie Systemowe i Współbierzne

Kody zapisane w plikach inf147414s.c i inf147414k.c kompilujemy za pomocą kompilatora GNU Compiler Collection w wersji 9.3.0, używając w terminalu komendy ```gcc <nazwapliku> -o <preferowana_nazwa>```. Skompilowane programy uruchamiamy w konsoli ```./<preferowana_nazwa> <numer_kolejki>``` (liczba naturalna, najlepiej conajmniej pięciocyfrowa), najpierw server a następnie aplikacje klienta. Programy zamykamy w odwrotnej kolejności wpisując w konsolę słowo kluczowe "exit". Przez procedurę logowanie w aplikacji klienta przechodzimy zgodnie z podanymi poleceniami.


inf147414k.c:
w pliku znajduje się funkcja fork, gdzie potomek procesu macierzystego zajmuje się odbieraniem przychodzących wiadomości kolejek komunikatów IPC a proces macierzysty wysyła je

inf147414s.c:
w pliku znajduje się switch statement, które na podstawie typu otrzymanje wiadomości decyduje o podejmowanej przez siebie działalności