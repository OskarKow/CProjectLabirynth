/*
* kod szablonu z przedmiotu Podstawy Programowania
* do wykorzystania jedynie w ramach tego przedmiotu
* autor: K.M. Ocetkiewicz
*/
#include <time.h>
#ifndef CONIO2_H
#define CONIO2_H

#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED		4
#define MAGENTA		5
#define BROWN		6
#define LIGHTGRAY	7
#define DARKGRAY	8
#define LIGHTBLUE	9
#define LIGHTGREEN	10
#define LIGHTCYAN	11
#define LIGHTRED	12
#define LIGHTMAGENTA	13
#define YELLOW		14
#define WHITE		15

#define _NOCURSOR	0
#define _SOLIDCURSOR	1
#define _NORMALCURSOR	2

#ifndef EOF
#define EOF		-1
#endif

//#define TEST 1

#define LASTMODE	-1
#define C80		3
#define C4350		8
#define FULLSCREEN	0x8000

#define POZYCJA_STEROWANIA_X 30
#define WIDOK_2D_POZYCJAX 49
#define WIDOK_2D_POZYCJAY 3
#define ILOSC_KOLOROW 16
#define STRZALKA_GORA 0x48
#define STRZALKA_DOL 0x50
#define STRZALKA_LEWO 0x4b
#define STRZALKA_PRAWO 0x4d

#define ZNAK_STRZALKA_LEWO 17
#define ZNAK_STRZALKA_PRAWO 16
#define ZNAK_STRZALKA_GORA 30
#define ZNAK_STRZALKA_DOL 31

//widok pespektywiczny
#define GLEBOKOSC_RYSOWANIA 7
#define POLA_NA_LEWO 0
#define POLA_PRZED 1
#define POLA_NA_PRAWO 2
#define SZEROKOSC_KOLUMNY 3 //kolumna widziana z boku w widoku perspektywicznym
#define SZEROKOSC_WIDOKU_3D 46
#define WYSOKOSC_WIDOKU_3D 15
#define POZYCJA_3D_X 1
#define POZYCJA_3D_Y 1
#define PARAMETR_SZEROKOSCI_OTWARCIA 6 //im wiekszy tym mniejsza szerokosc otwarcia


//edytor
#define POZYCJA_POMOCY_EDYTORA_X 1
#define POZYCJA_POMOCY_EDYTORA_Y 1
#define MAX_SZEROKOSC 30		//limit 
#define MAX_DLUGOSC 23			// dla edytora
#define POZYCJA_EDYTORA_X 1
#define POZYCJA_EDYTORA_Y 1

extern int _wscroll;		// 1 = enable scroll
							// 0 = disable scroll

struct text_info {
	unsigned char curx;
	unsigned char cury;
	unsigned short attribute;
	unsigned short normattr;
	unsigned char screenwidth;
	unsigned char screenheight;
};

#ifdef __cplusplus
extern "C" {
#endif
	void gotoxy(int x, int y);
	int wherex(void);
	int wherey(void);
	void _setcursortype(int cur_t);

	void textattr(int newattr);
	void textbackground(int newcolor);
	void textcolor(int newcolor);

	void clreol(void);
	void clrscr(void);
	void delline(void);
	void insline(void);

	int getch(void);
	int getche(void);
	int kbhit(void);
	int ungetch(int ch);

	int cputs(const char *str);
	int putch(int c);

	int gettext(int left, int top, int right, int bottom, void *dest);
	int movetext(int left, int top, int right, int bottom, int destleft, int desttop);
	int puttext(int left, int top, int right, int bottom, void *source);

	void gettextinfo(struct text_info *info);

	void textmode(int mode);

	// bonus functions
	void settitle(const char *title);
#ifdef __cplusplus
}
#endif

#endif

enum kierunek_t
{
	GORA,
	PRAWO,
	DOL,
	LEWO
};

struct wspolrzedne_t
{
	int x;
	int y;
};

struct labirynt_t
{
	char *plansza;
	wspolrzedne_t wejscie;
	wspolrzedne_t wyjscie;
	int rozmiar_x;
	int rozmiar_y;
};

struct gracz_t
{
	kierunek_t kierunek;
	wspolrzedne_t polozenie;
	unsigned int ilosc_ruchow;
};

struct korytarz_t
{
	int dlugosc;
	wspolrzedne_t pozycja_startowa;
	wspolrzedne_t skrzyzowanie_docelowe;
};

struct skrzyzowanie_t
{
	wspolrzedne_t pozycja;
	int najkrotsza_droga;
	int ilosc_sasiadujacych;
	korytarz_t *korytarze[4]; //jesz nie wiem
};

void Graj(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia); // Glowna funkcja zawierajaca cala gre.

void Wyswietl_stan_gry(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia);
void Wykonaj_ruch(labirynt_t labirynt, gracz_t *gracz, int wprowadzenie);

bool Pole_wolne(labirynt_t labirynt, gracz_t gracz, kierunek_t ktore);
void Ustaw_wartosci_poczatkowe(labirynt_t *labirynt, gracz_t *gracz, clock_t *czas_rozpoczecia);
void Czekaj(int czas_ms);
void Wyswietl_labirynt(labirynt_t labirynt, int pozycja_x, int pozycja_y);
int Znajdz_X_Wejscia(labirynt_t labirynt);
int Znajdz_X_Wyjscia(labirynt_t labirynt);
void Pobierz_rozmiar(char *nazwa_pliku, labirynt_t *labirynt);
void Pobierz_labirynt(char *nazwa_pliku, labirynt_t *labirynt);
void Wyswietl_sterowanie();
void Zmien_kierunek(gracz_t *gracz, int wprowadzenie);
void Rusz(labirynt_t labirynt, gracz_t *gracz, int wprowadzenie);
bool Poprawny_format(labirynt_t labirynt);
void Wyswietl_licznik(clock_t czas_rozpoczecia, gracz_t gracz);
void Resetuj_stan_gry(clock_t *czas_rozpoczecia, labirynt_t *labirynt, gracz_t *gracz);
void Zmien_stan_drzwi(labirynt_t *labirynt, gracz_t gracz, clock_t czas_rozpoczecia);
void Wyswietl_znak_strzalki(gracz_t gracz);

int Pole_przed_Y(gracz_t gracz);
int Pole_przed_X(gracz_t gracz);
int Pole_na_lewo_X(gracz_t gracz);
int Pole_na_lewo_Y(gracz_t gracz);
int Pole_na_prawo_X(gracz_t gracz);
int Pole_na_prawo_Y(gracz_t gracz);
void Ustaw_widok_3d(gracz_t gracz, labirynt_t labirynt);
void Wyswietl_widok_3d(char mapa[][GLEBOKOSC_RYSOWANIA], int ile_w_przod);
void Wyswietl_animacje_otwierania(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia);
void Wyswietl_animacje_zamykania(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia);

void Stworz_nowy();
void Otworz_edytor();

bool Poprawny_format_nazwy(char *napis, int rozmiar);

char *Pobierz_nazwe_pliku(int *rozmiar_nazwy);
int Dlugosc(char *napis);

int Ilosc_sasiadujacych(wspolrzedne_t wspolrzedne, labirynt_t labirynt);
