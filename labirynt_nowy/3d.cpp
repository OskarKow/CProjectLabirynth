#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include "conio2.h"
#include <windows.h>

using namespace std;


int Pole_przed_Y(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return gracz.polozenie.y - 1;
		break;
	}
	case DOL: {
		return gracz.polozenie.y + 1;
		break;
	}
	case LEWO: {
		return gracz.polozenie.y;
		break;
	}
	case PRAWO: {
		return gracz.polozenie.y;
		break;
	}
	default: return -1;
	}
	return -1;
}

int Pole_przed_X(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return  gracz.polozenie.x;
		break;
	}
	case DOL: {
		return gracz.polozenie.x;
		break;
	}
	case LEWO: {
		return gracz.polozenie.x - 1;
		break;
	}
	case PRAWO: {
		return gracz.polozenie.x + 1;
		break;
	}
	default: return -1;
	}
	return -1;
}

int Pole_na_lewo_X(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return  gracz.polozenie.x - 1;
		break;
	}
	case DOL: {
		return gracz.polozenie.x + 1;
		break;
	}
	case LEWO: {
		return gracz.polozenie.x;
		break;
	}
	case PRAWO: {
		return gracz.polozenie.x;
		break;
	}
	default: return -1;
	}
}

int Pole_na_lewo_Y(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return  gracz.polozenie.y;
		break;
	}
	case DOL: {
		return gracz.polozenie.y;
		break;
	}
	case LEWO: {
		return gracz.polozenie.y + 1;
		break;
	}
	case PRAWO: {
		return gracz.polozenie.y - 1;
		break;
	}
	default: return -1;
	}
}
int Pole_na_prawo_X(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return  gracz.polozenie.x + 1;
		break;
	}
	case DOL: {
		return gracz.polozenie.x - 1;
		break;
	}
	case LEWO: {
		return gracz.polozenie.x;
		break;
	}
	case PRAWO: {
return gracz.polozenie.x;
break;
	}
	default: return -1;
	}
}
int Pole_na_prawo_Y(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		return  gracz.polozenie.y;
		break;
	}
	case DOL: {
		return gracz.polozenie.y;
		break;
	}
	case LEWO: {
		return gracz.polozenie.y - 1;
		break;
	}
	case PRAWO: {
		return gracz.polozenie.y + 1;
		break;
	}
	default: return -1;
	}
}

void Rysuj_klatke_animacji(int pozycja_brzegu_drzwi)
{
	bool rysuj_pionowe = false, rysuj_kropki = false;
	for (int i = 0; i < SZEROKOSC_WIDOKU_3D; i++)
	{
		gotoxy(POZYCJA_3D_X + i, POZYCJA_3D_Y);
		if (i == 0 || i == SZEROKOSC_WIDOKU_3D - 1) rysuj_pionowe = true;
		if (i == pozycja_brzegu_drzwi || i == SZEROKOSC_WIDOKU_3D - pozycja_brzegu_drzwi - 2) rysuj_pionowe = true;
		if (i > pozycja_brzegu_drzwi && i < SZEROKOSC_WIDOKU_3D - pozycja_brzegu_drzwi - 2) rysuj_kropki = true;
		//rysowanie kolumny
		for (int j = 0; j < (GLEBOKOSC_RYSOWANIA * 2) + 2; j++)
		{
			if (j == 0)
			{
				if (i != 0 && i != SZEROKOSC_WIDOKU_3D - 1)cout << "_";
				gotoxy(POZYCJA_3D_X + i, POZYCJA_3D_Y + j + 1);
				continue;
			}
			if (j == (GLEBOKOSC_RYSOWANIA * 2) + 1)
			{
				gotoxy(POZYCJA_3D_X + i, POZYCJA_3D_Y + j - 1);
				if (i != 0 && i != SZEROKOSC_WIDOKU_3D - 1)cout << "_";
			}
			if (rysuj_pionowe) cout << "|";
			if (rysuj_kropki) cout << ".";
			if (!rysuj_kropki && !rysuj_pionowe) cout << " ";
			gotoxy(POZYCJA_3D_X + i, POZYCJA_3D_Y + j);
		}
		rysuj_pionowe = false;
		rysuj_kropki = false;
	}
}

void Wyswietl_animacje_otwierania(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia)
{
	gotoxy(POZYCJA_3D_X, POZYCJA_3D_Y);
	int pozycja_brzegu_drzwi = ((SZEROKOSC_WIDOKU_3D - 2) / 2) - 2; // drugi: (SZEROKOSC_3D - 2 ) - pozycja_brzegu
	while (pozycja_brzegu_drzwi > PARAMETR_SZEROKOSCI_OTWARCIA)
	{
		clrscr();
		Rysuj_klatke_animacji(pozycja_brzegu_drzwi);
		// Wyswietlamy stan gry bez widoku 3d
		Wyswietl_labirynt(labirynt, WIDOK_2D_POZYCJAX, WIDOK_2D_POZYCJAY);
		Wyswietl_licznik(czas_rozpoczecia, gracz);
		gotoxy(gracz.polozenie.x + WIDOK_2D_POZYCJAX - 1, gracz.polozenie.y + WIDOK_2D_POZYCJAY - 1);
		textcolor(GREEN);
		Wyswietl_znak_strzalki(gracz);
		textcolor(LIGHTGRAY);
		Czekaj(90);
		pozycja_brzegu_drzwi--;
	}
}

void Wyswietl_animacje_zamykania(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia)
{
	gotoxy(POZYCJA_3D_X, POZYCJA_3D_Y);
	int pozycja_brzegu_drzwi = PARAMETR_SZEROKOSCI_OTWARCIA + 1;
	while (pozycja_brzegu_drzwi < ((SZEROKOSC_WIDOKU_3D - 2) / 2) - 1)
	{
		clrscr();
		Rysuj_klatke_animacji(pozycja_brzegu_drzwi);
		// Wyswietlamy stan gry bez widoku 3d
		Wyswietl_labirynt(labirynt, WIDOK_2D_POZYCJAX, WIDOK_2D_POZYCJAY);
		Wyswietl_licznik(czas_rozpoczecia, gracz);
		gotoxy(gracz.polozenie.x + WIDOK_2D_POZYCJAX - 1, gracz.polozenie.y + WIDOK_2D_POZYCJAY - 1);
		textcolor(GREEN);
		Wyswietl_znak_strzalki(gracz);
		textcolor(LIGHTGRAY);
		Czekaj(90);
		pozycja_brzegu_drzwi++;
	}
}

void Rysuj_wypelnienie_scian(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], char mapa[][GLEBOKOSC_RYSOWANIA], int ile_w_przod)
{

	int indeks_x = 1;
	for (int i = 0; i < ile_w_przod * 2; i++, indeks_x += SZEROKOSC_KOLUMNY)
	{
		if (i == ile_w_przod) indeks_x = 1; //przechodzimy do rysowania drugiej strony, przypisujemy wartosc domyslna dla indeksu
		if (i < ile_w_przod) //lewa strona
		{
			if (mapa[POLA_NA_LEWO][i] == ' ' || mapa[POLA_NA_LEWO][i] == 'D' || mapa[POLA_NA_LEWO][i] == 'O') continue;
			for (int j = i + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - i; j++)
			{
				tab_docelowa[j][indeks_x] = tab_docelowa[j][indeks_x + 1] = mapa[POLA_NA_LEWO][i];
			}
		}
		else { //prawa strona
			if (mapa[POLA_NA_PRAWO][i % ile_w_przod] == ' ' || mapa[POLA_NA_PRAWO][i % ile_w_przod] == 'D' || 
				mapa[POLA_NA_PRAWO][i % ile_w_przod] == 'O') continue;
			for (int j = (i % ile_w_przod) + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - (i % ile_w_przod); j++)
			{
				tab_docelowa[j][SZEROKOSC_WIDOKU_3D - indeks_x - 1] = mapa[POLA_NA_PRAWO][i % ile_w_przod];
				tab_docelowa[j][SZEROKOSC_WIDOKU_3D - indeks_x - 2] = mapa[POLA_NA_PRAWO][i % ile_w_przod];
			}
		}
	}
}

static void Rysuj_kolumny(char tab_docelowa[][46], int ile_w_przod, char mapa[][GLEBOKOSC_RYSOWANIA]) //TODO j dla drugiej strony planszy ;] mozna zrobic 2 petle tez
{
	//rysujemy (ile_w_przod+1)*2 kolumn
	int wsp_x_poczatku = 0, ilosc_kolumn;
	int dlugosc_kolumny, y_poczatkowe;
	if (ile_w_przod == GLEBOKOSC_RYSOWANIA) ilosc_kolumn = GLEBOKOSC_RYSOWANIA * 2;
	else ilosc_kolumn = (ile_w_przod + 1) * 2;
	//poruszamy sie po wszystkich kolumnach - i=numer kolumny
	for (int i = 0; i < ilosc_kolumn; i++)
	{
		//obsluga przypadkow gdy pierwsze pole na lewo/prawo jest puste
		if (i == 0 && mapa[POLA_NA_LEWO][0] == ' ')
		{
			wsp_x_poczatku += SZEROKOSC_KOLUMNY;
			continue;
		}
		if (i == ilosc_kolumn / 2)
		{
			wsp_x_poczatku = SZEROKOSC_WIDOKU_3D - 1;//przejscie do rysowania drugiej strony widoku
			if (mapa[POLA_NA_PRAWO][0] == ' ')
			{
				wsp_x_poczatku -= SZEROKOSC_KOLUMNY;
				continue;
			}
		}
		y_poczatkowe = (i % (ilosc_kolumn / 2)) + 1;
		dlugosc_kolumny = (GLEBOKOSC_RYSOWANIA * 2) - ((i % (ilosc_kolumn / 2)) * 2);
		for (int j = y_poczatkowe; j < y_poczatkowe + dlugosc_kolumny; j++)
		{
			tab_docelowa[j][wsp_x_poczatku] = '|';
		}
		if (i < ilosc_kolumn / 2) wsp_x_poczatku += SZEROKOSC_KOLUMNY;
		else wsp_x_poczatku -= SZEROKOSC_KOLUMNY;
	}
}

static void Rysuj_drzwi_otwarte(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], int ile_w_przod)
{
	int x_poczatek = (ile_w_przod + 1) * SZEROKOSC_KOLUMNY;
	int indeks_y = ile_w_przod;
	for (int i = x_poczatek; i < SZEROKOSC_WIDOKU_3D - ((ile_w_przod + 1) * SZEROKOSC_KOLUMNY) + 2; i++)
	{
		tab_docelowa[indeks_y][i] = tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y][i] = '_';
		if (ile_w_przod < GLEBOKOSC_RYSOWANIA - 1)
		{
			if (i == x_poczatek + PARAMETR_SZEROKOSCI_OTWARCIA - 1 || i == SZEROKOSC_WIDOKU_3D - x_poczatek - PARAMETR_SZEROKOSCI_OTWARCIA + 1)
			{
				for (int j = indeks_y + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - indeks_y; j++)
				{
					tab_docelowa[j][i - 1] = '|';
				}
			}
			if( i > x_poczatek + PARAMETR_SZEROKOSCI_OTWARCIA - 1 && i < SZEROKOSC_WIDOKU_3D - x_poczatek - PARAMETR_SZEROKOSCI_OTWARCIA + 1){
				for (int j = indeks_y + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - indeks_y; j++)
				{
					tab_docelowa[j][i - 1] = '.';
				}
			}
		}
	}
}

static void Rysuj_drzwi_zamkniete(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], int ile_w_przod)
{
	int x_poczatek = (ile_w_przod + 1) * SZEROKOSC_KOLUMNY;
	int szerokosc_sciany = (SZEROKOSC_WIDOKU_3D - ((ile_w_przod + 1) * SZEROKOSC_KOLUMNY)) - x_poczatek;
	int indeks_srodka = ((szerokosc_sciany / 2) - 1) + x_poczatek;
	int indeks_y = ile_w_przod;
	for (int i = x_poczatek; i < SZEROKOSC_WIDOKU_3D - ((ile_w_przod + 1) * SZEROKOSC_KOLUMNY) + 2; i++)
	{
		tab_docelowa[indeks_y][i] = tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y][i] = '_';
		//rysowanie srodka drzwi
		if (i == indeks_srodka - 1 && ile_w_przod < GLEBOKOSC_RYSOWANIA - 1)
		{
			for (int j = indeks_y + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - indeks_y; j++)
			{
				tab_docelowa[j][i - 1] = tab_docelowa[j][i + 1] = '|';
			}
		}
		if (i == indeks_srodka && ile_w_przod < GLEBOKOSC_RYSOWANIA - 1)
		{
			for (int j = indeks_y + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - indeks_y; j++)
			{
				tab_docelowa[j][i - 1] = '.';
			}
		}
	}
}

static void Rysuj_sciane_przed_graczem(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], int ile_w_przod, char mapa[][GLEBOKOSC_RYSOWANIA])
{
	int x_poczatek = (ile_w_przod) * SZEROKOSC_KOLUMNY;
	int indeks_y = ile_w_przod;
	for (int i = x_poczatek + 1; i < SZEROKOSC_WIDOKU_3D - ((ile_w_przod + 1) * SZEROKOSC_KOLUMNY) + 2; i++)
	{
		tab_docelowa[indeks_y][i] = tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y][i] = '_';
		//rysowanie wypelnienia
		for (int j = indeks_y + 1; j < (GLEBOKOSC_RYSOWANIA * 2) - indeks_y; j++)
		{
			tab_docelowa[j][i] = mapa[POLA_PRZED][ile_w_przod];
		}
	}
}

static void Rysuj_podstawy(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], int ile_w_przod, char mapa[][GLEBOKOSC_RYSOWANIA])
{
	int nr_sciany_lewa, nr_sciany_prawa;
	int indeks_y = 0, i_temp;
	for (int i = 1; i < SZEROKOSC_WIDOKU_3D - 1; i++)
	{
		if (ile_w_przod < GLEBOKOSC_RYSOWANIA && i == (ile_w_przod + 1) * SZEROKOSC_KOLUMNY) // gdy gracz widzi przed soba sciane
		{
			i_temp = i;
			i = (SZEROKOSC_WIDOKU_3D - 1) - i_temp; //przesuniecie indeksu za narysowana sciane
			continue;
		}
		if (i % 3 > 0)
		{
			//Przechodzimy dalej i nic nie robimy gdy pole na lewo/prawo gracza jest/puste/drzwi/drzwi otwarte. Jeœli nie- rysujemy podstawy
			nr_sciany_lewa = (i - 1) / SZEROKOSC_KOLUMNY;
			nr_sciany_prawa = GLEBOKOSC_RYSOWANIA - 1 - (((i - 1) % (SZEROKOSC_WIDOKU_3D / 2)) / SZEROKOSC_KOLUMNY);
			if (i <= SZEROKOSC_WIDOKU_3D / 2 && mapa[POLA_NA_LEWO][nr_sciany_lewa] == ' ')continue;
			if (i > SZEROKOSC_WIDOKU_3D / 2 && mapa[POLA_NA_PRAWO][nr_sciany_prawa] == ' ') continue;
			tab_docelowa[indeks_y][i] = tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y][i] = '_';
		}	
		else {
			if (i <= SZEROKOSC_WIDOKU_3D / 2) //lewa strona
			{
				indeks_y++;
				if (mapa[POLA_NA_LEWO][(i - 1) / SZEROKOSC_KOLUMNY] != ' ')
				{
					tab_docelowa[indeks_y][i] = '\\';
					tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y + 1][i] = '/';
				}
			}
			else { //prawa strona
				if (mapa[POLA_NA_PRAWO][GLEBOKOSC_RYSOWANIA - 1 - (((i - 1) % (SZEROKOSC_WIDOKU_3D / 2)) / SZEROKOSC_KOLUMNY)] != ' ')
				{
					tab_docelowa[indeks_y][i] = '/';
					tab_docelowa[(GLEBOKOSC_RYSOWANIA * 2) - indeks_y + 1][i] = '\\';
				}
				indeks_y--;
			}
		}
	}
}

void Rysuj_horyzont(char tab_docelowa[][SZEROKOSC_WIDOKU_3D], int ile_w_przod, char mapa[][GLEBOKOSC_RYSOWANIA])
{
	int dlugosc = 2, x_poczatkowe = (SZEROKOSC_WIDOKU_3D / 2) - 1;
	if (mapa[POLA_NA_LEWO][GLEBOKOSC_RYSOWANIA - 1] == ' ')
	{
		dlugosc += SZEROKOSC_KOLUMNY;
		x_poczatkowe -= SZEROKOSC_KOLUMNY;
	}
	if (mapa[POLA_NA_PRAWO][GLEBOKOSC_RYSOWANIA - 1] == ' ') dlugosc += SZEROKOSC_KOLUMNY;

	for (int i = 0; i < dlugosc; i++)
	{
		tab_docelowa[GLEBOKOSC_RYSOWANIA][x_poczatkowe +  i] = '_';
	}
}

void Wyswietl_widok_3d(char mapa[][GLEBOKOSC_RYSOWANIA], int ile_w_przod)
{
	char widok[WYSOKOSC_WIDOKU_3D][SZEROKOSC_WIDOKU_3D];
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 46; j++) widok[i][j] = ' ';
	gotoxy(POZYCJA_3D_X, POZYCJA_3D_Y);
	Rysuj_kolumny(widok, ile_w_przod, mapa);
	Rysuj_podstawy(widok, ile_w_przod, mapa);
	Rysuj_wypelnienie_scian(widok, mapa, ile_w_przod);
	if(mapa[POLA_PRZED][ile_w_przod] != 'D' && mapa[POLA_PRZED][ile_w_przod] != 'O') Rysuj_sciane_przed_graczem(widok, ile_w_przod, mapa);
	if (mapa[POLA_PRZED][ile_w_przod] == 'D')Rysuj_drzwi_zamkniete(widok, ile_w_przod);
	if (mapa[POLA_PRZED][ile_w_przod] == 'O')Rysuj_drzwi_otwarte(widok, ile_w_przod);
	if (ile_w_przod == GLEBOKOSC_RYSOWANIA) Rysuj_horyzont(widok, ile_w_przod, mapa);

	for (int i = 0; i < WYSOKOSC_WIDOKU_3D; i++)
	{
		for (int j = 0; j < SZEROKOSC_WIDOKU_3D; j++)
		{
			
			cout << widok[i][j];
		}
		gotoxy(POZYCJA_3D_X, POZYCJA_3D_Y + 1 + i);
	}
}

//Funkcja wykonuje czynnosci przygotowujace, potrzebne do wyrysowania widoku perpektywicznego,
//a nastepnie wywoluje funkcje wyswietlajaca
void Ustaw_widok_3d(gracz_t gracz, labirynt_t labirynt)//do zrobienia gdy patrzymy na wejscie/wyjscie
{
	wspolrzedne_t wsp_gracza_kopia = { gracz.polozenie.x - 1,gracz.polozenie.y - 1 };
	gracz_t gracz_kopia = { gracz.kierunek, wsp_gracza_kopia, gracz.ilosc_ruchow };
	char teren_do_rysowania[3][GLEBOKOSC_RYSOWANIA] = { 0 };
	bool rysuj_boczne = true;
	int ile_w_przod = 0; //ilosc wolnych pol prosto przed graczem(korytarz)
	wspolrzedne_t pole_przed, pole_lewo, pole_prawo;

	//Ustawiamy "mape 2d" terenu przed graczem ktory bedzie wyswietlany w rzucie perspektywicznym:
	//Poruszamy sie po polach przed graczem i zapisujemy ich stan(oraz pol na lewo i prawo) do tablicy
	for (int i = 0; i < GLEBOKOSC_RYSOWANIA; i++)
	{
		gracz_kopia.polozenie.x = Pole_przed_X(gracz_kopia);
		gracz_kopia.polozenie.y = Pole_przed_Y(gracz_kopia);
		pole_przed = gracz_kopia.polozenie;
		teren_do_rysowania[POLA_PRZED][i] = labirynt.plansza[(pole_przed.y * labirynt.rozmiar_x) + pole_przed.x];
		if (teren_do_rysowania[POLA_PRZED][i] != ' ') break;
		else {
			ile_w_przod++;
			pole_lewo.x = Pole_na_lewo_X(gracz_kopia);
			pole_lewo.y = Pole_na_lewo_Y(gracz_kopia);
			teren_do_rysowania[POLA_NA_LEWO][i] = labirynt.plansza[(pole_lewo.y * labirynt.rozmiar_x) + pole_lewo.x];
			pole_prawo.x = Pole_na_prawo_X(gracz_kopia);
			pole_prawo.y = Pole_na_prawo_Y(gracz_kopia);
			teren_do_rysowania[POLA_NA_PRAWO][i] = labirynt.plansza[(pole_prawo.y * labirynt.rozmiar_x) + pole_prawo.x];
		}
	}
	Wyswietl_widok_3d(teren_do_rysowania, ile_w_przod);
}


//FUNKCJE ZWIAZANE ZE ZNAJDOWANIEM NAJKROTSZEJ DROGI

int Ilosc_sasiadujacych(wspolrzedne_t wspolrzedne, labirynt_t labirynt);//zwraca ilosc dostepnych drog ze wspolrzednych podanych jako argument
void Przejdz_korytarz(korytarz_t *korytarz, labirynt_t labirynt, skrzyzowanie_t *skrzyzowanie_wyjsciowe); //przechodzi korytarz i ustawia wartosci
void Zainicjuj_szukanie_sciezki(labirynt_t labirynt, gracz_t gracz);



int Ilosc_sasiadujacych(wspolrzedne_t wspolrzedne, labirynt_t labirynt)
{
	int ilosc_pustych = 0;
	wspolrzedne_t kopia = { wspolrzedne.x - 1,wspolrzedne.y - 1 }; //do indeksow tablicy
	if (labirynt.plansza[(kopia.y - 1) * labirynt.rozmiar_x + kopia.x] == ' ') ilosc_pustych++;
	if (labirynt.plansza[kopia.y * labirynt.rozmiar_x + kopia.x + 1] == ' ') ilosc_pustych++;
	if (labirynt.plansza[(kopia.y + 1) * labirynt.rozmiar_x + kopia.x] == ' ') ilosc_pustych++;
	if (labirynt.plansza[kopia.y * labirynt.rozmiar_x + kopia.x - 1] == ' ') ilosc_pustych++;
	return ilosc_pustych;
}

void Zainicjuj_szukanie_sciezki(labirynt_t labirynt, gracz_t gracz)
{
	skrzyzowanie_t startowe;
	startowe.pozycja = gracz.polozenie;
	startowe.najkrotsza_droga = 0;
	startowe.ilosc_sasiadujacych = Ilosc_sasiadujacych(startowe.pozycja, labirynt);
}

//Zakladamy, ze wartosc dlugosci korytarza z argumentu wynosi 0
void Przejdz_korytarz(korytarz_t *korytarz, labirynt_t labirynt, skrzyzowanie_t *skrzyzowanie_wyjsciowe)
{
	wspolrzedne_t pole_poprzednie = (*skrzyzowanie_wyjsciowe).pozycja; //aby poruszac sie stale w jednym kierunku
	wspolrzedne_t aktualne = (*korytarz).pozycja_startowa;
	wspolrzedne_t kopia;
	while (Ilosc_sasiadujacych(aktualne, labirynt) < 3)
	{
		(*korytarz).dlugosc++;
		kopia = { aktualne.x - 1,aktualne.y - 1 };
		/*if (labirynt.plansza[(kopia.y - 1) * labirynt.rozmiar_x + kopia.x] == ' ' && 
			pole_poprzednie.x != aktualne.x && pole_poprzednie.y != aktualne.y)*/
	}
	/*if (labirynt.plansza[(kopia.y - 1) * labirynt.rozmiar_x + kopia.x] == ' ') ilosc_pustych++;
	if (labirynt.plansza[kopia.y * labirynt.rozmiar_x + kopia.x + 1] == ' ') ilosc_pustych++;
	if (labirynt.plansza[(kopia.y + 1) * labirynt.rozmiar_x + kopia.x] == ' ') ilosc_pustych++;
	if (labirynt.plansza[kopia.y * labirynt.rozmiar_x + kopia.x - 1] == ' ') ilosc_pustych++;*/

}

