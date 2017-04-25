#include <iostream>
#include "conio2.h"
#include <time.h>
#include <string.h>

using namespace std;

int main()
{
	int zn, attr = LIGHTGRAY;
	char *plansza = NULL;
	clock_t czas_rozpoczecia = NULL;
	unsigned int ilosc_ruchow = 0;
	struct wspolrzedne_t wsp_gracza = { 1, 1 };
	struct gracz_t gracz = { GORA, wsp_gracza, 0 };
	struct wspolrzedne_t wejscie_labiryntu = { 1, 1 }, wyjscie_labiryntu = { 1, 1 };
	struct labirynt_t labirynt = { NULL, wejscie_labiryntu, wyjscie_labiryntu, 0, 0 };
	settitle("Oskar Kowalewski 160359");
	textbackground(BLACK);
	_setcursortype(_NOCURSOR);
	do {
		clrscr();
		textcolor(attr);
		zn = getch();
		if (zn == 'e') Otworz_edytor();
		if (zn == 'h') Wyswietl_sterowanie();
		if (zn == 'i')
		{
			Pobierz_rozmiar("domyslny.txt", &labirynt);
			labirynt.plansza = (char *)malloc(labirynt.rozmiar_y * labirynt.rozmiar_x);
			Pobierz_labirynt("domyslny.txt", &labirynt);
			if (Poprawny_format(labirynt) == false) break;
			Ustaw_wartosci_poczatkowe(&labirynt, &gracz, &czas_rozpoczecia);
			Graj(labirynt, gracz, czas_rozpoczecia);
		}
		if (zn == 'o')
		{
			char *nazwa_pliku = NULL;
			int rozmiar;
			nazwa_pliku = Pobierz_nazwe_pliku(&rozmiar);
			Pobierz_rozmiar(nazwa_pliku, &labirynt);
			labirynt.plansza = (char *)malloc(labirynt.rozmiar_y * labirynt.rozmiar_x);
			Pobierz_labirynt(nazwa_pliku, &labirynt);
			if (Poprawny_format(labirynt) == false) break;
			Ustaw_wartosci_poczatkowe(&labirynt, &gracz, &czas_rozpoczecia);
			Graj(labirynt, gracz, czas_rozpoczecia);
		}
	} while (zn != 'q');
	getch();
	return 0;
}