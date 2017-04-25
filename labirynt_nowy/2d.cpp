#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <malloc.h>
#include "conio2.h"
#include <time.h>
#include <cstdlib>

using namespace std;

void Czekaj(int czas_ms)
{
	clock_t aktualny = clock();
	while (clock() < (aktualny + czas_ms) );
}

int Dlugosc(char *napis)
{
	int dlugosc = 0;
	while (napis[dlugosc] != NULL) dlugosc++;
	return dlugosc;
}

void Pobierz_rozmiar(char *nazwa_pliku, labirynt_t *labirynt)
{	//odwolania do elementow: tab[wiersz * ilosc_kolumn + kolumna];
	FILE *plik;
	plik = fopen(nazwa_pliku, "r");
	fseek(plik, SEEK_SET, 1);
	char znak;
	int il_wierszy = 0, il_kolumn = 0;
	do
	{
		znak = getc(plik);
		if (znak == '\n') il_wierszy++;
		if (il_wierszy == 0) il_kolumn++;
	} while (znak != EOF);
	il_wierszy++;
	(*labirynt).rozmiar_y = il_wierszy;
	(*labirynt).rozmiar_x = il_kolumn;
	fclose(plik);
}

//Sprawdza czy tablica zawiera podany element - do losowania bez powtorzen
bool Zawiera_element(int tablica[], int element, int ilosc_elementow)
{
	for (int i = 0; i < ilosc_elementow; i++)
	{
		if (tablica[i] == element) return true;
	}
	return false;
}

void Przypisz_kategorie_scian(labirynt_t *labirynt, int tablica_indeksow[], int ilosc_scian)
{
	for (int i = 0; i < ilosc_scian; i++)
	{
		switch (i % 5)
		{
		case 0: {
			(*labirynt).plansza[tablica_indeksow[i]] = '#';
			break;
		}
		case 1: {
			(*labirynt).plansza[tablica_indeksow[i]] = '@';
			break;
		}
		case 2: {
			(*labirynt).plansza[tablica_indeksow[i]] = '$';
			break;
		}
		case 3: {
			(*labirynt).plansza[tablica_indeksow[i]] = '%';
			break;
		}
		case 4: {
			(*labirynt).plansza[tablica_indeksow[i]] = '&';
			break;
		}
		}
	}
}

//Funkcja losuje i przypisuje rownomiernie rozne kategorie dla scian
void Wylosuj_kategorie_scian(labirynt_t *labirynt)
{
	int *indeksy_scian, ilosc_scian = 0, wylosowany;
	indeksy_scian = (int *)malloc((sizeof(int)) * ((*labirynt).rozmiar_y * (*labirynt).rozmiar_x));
	//przypisanie do tablicy indeksow scian
	for (int i = 0; i < (*labirynt).rozmiar_y * (*labirynt).rozmiar_x; i++)
	{
		if ((*labirynt).plansza[i] != ' ' && (*labirynt).plansza[i] != 'D' && (*labirynt).plansza[i] != 'S' && (*labirynt).plansza[i] != 'M')
		{
			indeksy_scian[ilosc_scian] = i;
			ilosc_scian++;
		}
	}
	int *wylosowane = NULL; //przechowuje indeksy wylosowanych scian
	wylosowane = (int *)malloc(sizeof(int) * ilosc_scian);
	//losowanie bez powtorzen
	for (int i = 0; i < ilosc_scian; i++)
	{
		wylosowany = indeksy_scian[rand() % ilosc_scian];
		while (Zawiera_element(wylosowane, wylosowany, i))
		{
			wylosowany = indeksy_scian[rand() % ilosc_scian];
		}
		wylosowane[i] = wylosowany;
	}
	labirynt_t labirynt_kopia = { (*labirynt).plansza, (*labirynt).wejscie, (*labirynt).wyjscie, (*labirynt).rozmiar_x, (*labirynt).rozmiar_y };
	Przypisz_kategorie_scian(&labirynt_kopia, wylosowane, ilosc_scian);
	*labirynt = labirynt_kopia;
}

void Pobierz_labirynt(char *nazwa_pliku, labirynt_t *labirynt)
{
	FILE *plik;
	plik = fopen(nazwa_pliku, "r");
	fseek(plik, SEEK_SET, 1);
	char znak;
	srand(time(NULL));
	int i = 0;
	do
	{
		znak = getc(plik);
		if (znak == '\n')
			continue;
		if (znak == '0') (*labirynt).plansza[i] = ' ';
		if (znak == 'D' || znak == 'S' || znak == 'M') (*labirynt).plansza[i] = znak;
		if(znak == '1')(*labirynt).plansza[i] = '#';
		i++;
	} while (znak != EOF);
	labirynt_t labirynt_kopia = { (*labirynt).plansza, (*labirynt).wejscie, (*labirynt).wyjscie, (*labirynt).rozmiar_x, (*labirynt).rozmiar_y };
	Wylosuj_kategorie_scian(&labirynt_kopia);
	*labirynt = labirynt_kopia;
	fclose(plik);
}

//Zwraca wspolrzedne znaku w labiryncie
wspolrzedne_t Znajdz_znak(labirynt_t labirynt, char znak)
{
	wspolrzedne_t aktualne = { 0, 0 };
	for (int i = 0; i < labirynt.rozmiar_y; i++)
	{
		for (int j = 0; j < labirynt.rozmiar_x; j++)
		{
			if (labirynt.plansza[i * labirynt.rozmiar_x + j] == znak)
			{
				aktualne.x = j + 1;
				aktualne.y = i + 1;
				return aktualne;
			}
		}
	}
	return aktualne;
}

void Ustaw_wartosci_poczatkowe(labirynt_t *labirynt, gracz_t *gracz, clock_t *czas_rozpoczecia)
{
	(*labirynt).wejscie = Znajdz_znak(*labirynt, 'S');
	(*labirynt).wyjscie = Znajdz_znak(*labirynt, 'M');
	(*gracz).polozenie.y = (*labirynt).wejscie.y;
	(*gracz).polozenie.x = (*labirynt).wejscie.x;
	*czas_rozpoczecia = clock();
}

//Zwraca true gdy podany znak wystepuje na krawedziach labiryntu
bool Znak_znaleziony(labirynt_t labirynt, char znak)
{
	//sprawdzanie gornych krawedzi
	for (int i = 0; i < labirynt.rozmiar_x; i++)
	{
		if (labirynt.plansza[i] == znak || labirynt.plansza[(labirynt.rozmiar_y - 1) * labirynt.rozmiar_x + i] == znak) return true;
	}
	//sprawdzanie bocznych krawedzi
	for (int i = 0; i < labirynt.rozmiar_y; i++)
	{
		if (labirynt.plansza[i * labirynt.rozmiar_x] == znak || labirynt.plansza[i * labirynt.rozmiar_x + labirynt.rozmiar_x - 1] == znak) return true;
	}
	return false;
}

//Sprawdzanie wystepowania pustych przestrzeni i wystepowanie wejscia/wyjscia
bool Poprawny_format(labirynt_t labirynt)
{
	bool nie_zawiera_przestrzeni = true, wejscie_znalezione = false, wyjscie_znalezione = false, poprawny = false;
	for (int i = 0; i < labirynt.rozmiar_y; i++)
	{
		for (int j = 0; j < labirynt.rozmiar_x; j++)
		{
			if (labirynt.plansza[(i * labirynt.rozmiar_x) + j] == ' ' && labirynt.plansza[((i + 1) * labirynt.rozmiar_x) + j] == ' ' &&
				labirynt.plansza[((i + 1) * labirynt.rozmiar_x) + j + 1] == ' ' &&
				labirynt.plansza[(i * labirynt.rozmiar_x) + j + 1] == ' ') nie_zawiera_przestrzeni = false;
		}
	}
	if (Znak_znaleziony(labirynt, 'S')) wejscie_znalezione = true;
	if (Znak_znaleziony(labirynt, 'M')) wyjscie_znalezione = true;
	if (nie_zawiera_przestrzeni && wejscie_znalezione && wyjscie_znalezione) poprawny = true;
	if (!poprawny)
	{
		gotoxy(1, 1);
		cout << "Niepoprawny format labiryntu.\n";
	}
	return poprawny;
}

void Wyswietl_labirynt(labirynt_t labirynt, int pozycja_x, int pozycja_y)
{
	gotoxy(pozycja_x, pozycja_y);
	for (int i = 0; i < labirynt.rozmiar_y; i++)
	{
		for (int j = 0; j < labirynt.rozmiar_x; j++)
		{
			cout << labirynt.plansza[i * labirynt.rozmiar_x + j];
		}
		gotoxy(pozycja_x, pozycja_y + 1 + i);
	}
}

void Wyswietl_sterowanie()
{
	clrscr();
	gotoxy(POZYCJA_STEROWANIA_X, 1);
	cout << "i - wczytanie domyslnego labiryntu.";
	gotoxy(POZYCJA_STEROWANIA_X, 2);
	cout << "Strzalka w lewo/prawo - zmiana kierunku patrzenia.";
	gotoxy(POZYCJA_STEROWANIA_X, 3);
	cout << "Strzalka w gore - ruch do przodu.";
	gotoxy(POZYCJA_STEROWANIA_X, 4);
	cout << "Strzalka w dol - ruch do tylu.";
	gotoxy(POZYCJA_STEROWANIA_X, 5);
	cout << "d - otworz/zamknij drzwi";
	gotoxy(POZYCJA_STEROWANIA_X, 6);
	cout << "r- reset gry.";
	gotoxy(POZYCJA_STEROWANIA_X, 7);
	cout << "q - opuszczenie gry.";
	gotoxy(POZYCJA_STEROWANIA_X, 8);
	cout << "e - otwarcie edytora.";
	gotoxy(POZYCJA_STEROWANIA_X, 10);
	cout << "              ****LEGENDA****";
	gotoxy(POZYCJA_STEROWANIA_X, 11);
	cout << "#@$%& - sciany.";
	gotoxy(POZYCJA_STEROWANIA_X, 12);
	cout << "D - drzwi zamkniete.";
	gotoxy(POZYCJA_STEROWANIA_X, 13);
	cout << "O - drzwi otwarte.";
	gotoxy(POZYCJA_STEROWANIA_X, 14);
	cout << (char)ZNAK_STRZALKA_DOL << (char)ZNAK_STRZALKA_GORA << (char)ZNAK_STRZALKA_LEWO << (char)ZNAK_STRZALKA_PRAWO << " - pozycja gracza.";
	gotoxy(POZYCJA_STEROWANIA_X, 16);
	cout << "Nacisnij dowolny klawisz, aby powrocic do gry.";
	getch();
}

void Wyswietl_znak_strzalki(gracz_t gracz)
{
	switch (gracz.kierunek)
	{
	case GORA: {
		cout << (char)ZNAK_STRZALKA_GORA;
		break;
	}
	case DOL: {
		cout << (char)ZNAK_STRZALKA_DOL;
		break;
	}
	case LEWO: {
		cout << (char)ZNAK_STRZALKA_LEWO;
		break;
	}
	case PRAWO: {
		cout << (char)ZNAK_STRZALKA_PRAWO;
		break;
	}
	}
}

void Zmien_kierunek(gracz_t *gracz, int wprowadzenie)
{
	switch ((*gracz).kierunek)
	{
	case GORA: {
		if (wprowadzenie == STRZALKA_LEWO) (*gracz).kierunek = LEWO;
		else (*gracz).kierunek = PRAWO;
		break;
	}
	case LEWO: {
		if (wprowadzenie == STRZALKA_LEWO) (*gracz).kierunek = DOL;
		else (*gracz).kierunek = GORA;
		break;
	}
	case DOL: {
		if (wprowadzenie == STRZALKA_LEWO) (*gracz).kierunek = PRAWO;
		else (*gracz).kierunek = LEWO;
		break;
	}
	case PRAWO: {
		if (wprowadzenie == STRZALKA_LEWO) (*gracz).kierunek = GORA;
		else (*gracz).kierunek = DOL;
		break;
	}
	default: cout << "BLAD ZMIANA KIERUNKU\n";
	}
}

//Zwraca true gdy pole sasiadujace z graczem od strony "ktore" jest wolne
bool Pole_wolne(labirynt_t labirynt, gracz_t gracz, kierunek_t ktore)
{
	int indeks_pola;
	switch (ktore)
	{
	case GORA: {
		indeks_pola = ((gracz.polozenie.y - 2) * labirynt.rozmiar_x) + (gracz.polozenie.x - 1);
		if (labirynt.plansza[indeks_pola] == ' ' || labirynt.plansza[indeks_pola] == 'O' || labirynt.plansza[indeks_pola] == 'M')
			return true;
		else return false;
	}
	case DOL: {
		indeks_pola = (gracz.polozenie.y * labirynt.rozmiar_x) + (gracz.polozenie.x - 1);
		if ((gracz.polozenie.y != labirynt.rozmiar_y && labirynt.plansza[indeks_pola] == ' ') || 
			(gracz.polozenie.y != labirynt.rozmiar_y && labirynt.plansza[indeks_pola] == 'O') ||
			(gracz.polozenie.y != labirynt.rozmiar_y && labirynt.plansza[indeks_pola] == 'M'))
			return true;
		else return false;
	}
	case LEWO: {
		indeks_pola = ((gracz.polozenie.y - 1) * labirynt.rozmiar_x) + (gracz.polozenie.x - 2);
		if (labirynt.plansza[indeks_pola] == ' ' || labirynt.plansza[indeks_pola] == 'O' || labirynt.plansza[indeks_pola] == 'M') return true;
		else return false;
	}
	case PRAWO: {
		indeks_pola = ((gracz.polozenie.y - 1) * labirynt.rozmiar_x) + gracz.polozenie.x;
		if (labirynt.plansza[indeks_pola] == ' ' || labirynt.plansza[indeks_pola] == 'O' || labirynt.plansza[indeks_pola] == 'M') return true;
		else return false;
	}
	default: return false;
	}
}

void Rusz(labirynt_t labirynt, gracz_t *gracz, int wprowadzenie)
{
	bool wolne_gora = Pole_wolne(labirynt, (*gracz), GORA);
	bool wolne_dol = Pole_wolne(labirynt, (*gracz), DOL);
	bool wolne_lewo = Pole_wolne(labirynt, (*gracz), LEWO);
	bool wolne_prawo = Pole_wolne(labirynt, (*gracz), PRAWO);
	(*gracz).ilosc_ruchow++;
	switch ((*gracz).kierunek)
	{
	case GORA: {
		if (wprowadzenie == STRZALKA_GORA)
		{
			if (wolne_gora) (*gracz).polozenie.y--;
		}
		else if (wolne_dol) (*gracz).polozenie.y++;
		break;
	}
	case PRAWO: {
		if (wprowadzenie == STRZALKA_GORA)
		{
			if (wolne_prawo) (*gracz).polozenie.x++;
		}
		else if (wolne_lewo) (*gracz).polozenie.x--;
		break;
	}
	case DOL: {
		if (wprowadzenie == STRZALKA_GORA)
		{
			if (wolne_dol) (*gracz).polozenie.y++;
		}
		else if (wolne_gora) (*gracz).polozenie.y--;
		break;
	}
	case LEWO: {
		if (wprowadzenie == STRZALKA_GORA)
		{
			if (wolne_lewo) (*gracz).polozenie.x--;
		}
		else if (wolne_prawo) (*gracz).polozenie.x++;
		break;
	}
	}
}

void Wyswietl_licznik(clock_t czas_rozpoczecia, gracz_t gracz)
{
	gotoxy(WIDOK_2D_POZYCJAX, 1);
	int sekundy = 0, minuty = 0;
	sekundy = ((clock() - czas_rozpoczecia) / CLOCKS_PER_SEC) % 60;
	minuty = ((clock() - czas_rozpoczecia) / CLOCKS_PER_SEC) / 60;
	if (minuty < 10) cout << "0";
	cout << minuty << ":";
	if (sekundy < 10) cout << "0";
	cout << sekundy;
	gotoxy(WIDOK_2D_POZYCJAX, 2);
	cout << "Ilosc ruchow: " << gracz.ilosc_ruchow;
}

void Wyswietl_stan_gry(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia)

{
	Wyswietl_labirynt(labirynt, WIDOK_2D_POZYCJAX, WIDOK_2D_POZYCJAY);
	Wyswietl_licznik(czas_rozpoczecia, gracz);
	gotoxy(gracz.polozenie.x + WIDOK_2D_POZYCJAX - 1, gracz.polozenie.y + WIDOK_2D_POZYCJAY - 1);
	textcolor(GREEN);
	Wyswietl_znak_strzalki(gracz);
	textcolor(LIGHTGRAY);
	Ustaw_widok_3d(gracz, labirynt);
}

void Wykonaj_ruch(labirynt_t labirynt, gracz_t *gracz, int wprowadzenie)
{
	gracz_t gracz_kopia;
	gracz_kopia.kierunek = (*gracz).kierunek;
	gracz_kopia.polozenie.x = (*gracz).polozenie.x;
	gracz_kopia.polozenie.y = (*gracz).polozenie.y;
	gracz_kopia.ilosc_ruchow = (*gracz).ilosc_ruchow;

	if (wprowadzenie == STRZALKA_GORA || wprowadzenie == STRZALKA_DOL)
		Rusz(labirynt, &gracz_kopia, wprowadzenie);
	if (wprowadzenie == STRZALKA_LEWO || wprowadzenie == STRZALKA_PRAWO)
		Zmien_kierunek(&gracz_kopia, wprowadzenie);

	(*gracz).kierunek = gracz_kopia.kierunek;
	(*gracz).polozenie.x = gracz_kopia.polozenie.x;
	(*gracz).polozenie.y = gracz_kopia.polozenie.y;
	(*gracz).ilosc_ruchow = gracz_kopia.ilosc_ruchow;

}

void Resetuj_stan_gry(clock_t *czas_rozpoczecia, labirynt_t *labirynt, gracz_t *gracz)
{
	*czas_rozpoczecia = clock();
	(*gracz).polozenie.x = (*labirynt).wejscie.x;
	(*gracz).polozenie.y = (*labirynt).wejscie.y;
	(*gracz).ilosc_ruchow = 0;
	(*gracz).kierunek = GORA;
	//zmieniamy wszystkie drzwi otwarte na zamkniete
	for (int i = 0; i < (*labirynt).rozmiar_y * (*labirynt).rozmiar_x; i++)
	{
		if ((*labirynt).plansza[i] == 'O') (*labirynt).plansza[i] = 'D';
	}
}

void Zmien_stan_drzwi(labirynt_t *labirynt, gracz_t gracz, clock_t czas_rozpoczecia)
{
	int pole_przed = ((Pole_przed_Y(gracz) - 1) * (*labirynt).rozmiar_x) + (Pole_przed_X(gracz) - 1);
	if ((*labirynt).plansza[pole_przed] == 'D')
	{
		Wyswietl_animacje_otwierania((*labirynt), gracz, czas_rozpoczecia);
		(*labirynt).plansza[pole_przed] = 'O';
		return;
	}
	if ((*labirynt).plansza[pole_przed] == 'O')
	{
		Wyswietl_animacje_zamykania((*labirynt), gracz, czas_rozpoczecia);
		(*labirynt).plansza[pole_przed] = 'D';
	}
}

void Graj(labirynt_t labirynt, gracz_t gracz, clock_t czas_rozpoczecia)
{
	_setcursortype(_NOCURSOR);
	int kolor = LIGHTGRAY;
	int klawisz;

	do {
		clrscr();
		if (gracz.polozenie.x == labirynt.wyjscie.x && gracz.polozenie.y == labirynt.wyjscie.y)
		{
			gotoxy(1, 1);
			cout << "Gratulacje, ukonczyles labirynt!" << endl;
			cout << "Nacisnij dowolny klawisz, aby wrocic do menu glownego." << endl;
			break;
		}
		textcolor(kolor);
		Wyswietl_stan_gry(labirynt, gracz, czas_rozpoczecia);
		klawisz = getch();
		if (klawisz == 0) {
			klawisz = getch();
			Wykonaj_ruch(labirynt, &gracz, klawisz);
		}
		else
		{
			if (klawisz == 'd') Zmien_stan_drzwi(&labirynt, gracz, czas_rozpoczecia);
			if (klawisz == 'h') Wyswietl_sterowanie();
			if (klawisz == 'r')Resetuj_stan_gry(&czas_rozpoczecia, &labirynt, &gracz);
		}
	} while (klawisz != 'q');
	getch();
}


// FUNKCJE ZWIAZANE Z EDYTOREM LABIRYNTOW

void Ustaw_poprzedni(wspolrzedne_t pozycja_poprzedniego_znaku, char poprzedni_znak);
char *Realokuj(char *tablica, int *aktualny_rozmiar); //realokacja pamieci
char *Pobierz_nazwe_pliku(int *rozmiar);
bool Poprawny_format_nazwy(char *napis, int rozmiar);
void Wyswietl_pomoc_edytora();
void Zapisz_labirynt(labirynt_t labirynt);
void Porusz_kursor(int wprowadzenie, wspolrzedne_t *aktualne_wspolrzedne);
void Ustaw_znak(int wprowadzenie, wspolrzedne_t pozycja_kursora, labirynt_t *docelowy, char *znak);
char Losowy_znak(); //zwraca losowy znak z @ # $ % &
void Okresl_rozmiar_edytowanego(labirynt_t *labirynt);
void Skaluj_plansze_labiryntu(labirynt_t *labirynt, int x_poczatkowe);

void Edytor(labirynt_t *labirynt);
void Edytuj_istniejacy();
void Stworz_nowy();
void Zapisz_edytowany(labirynt_t edytowany);
void Graj_w_edytowany(labirynt_t labirynt);


void Otworz_edytor()
{
	int klawisz = NULL;
	clrscr();
	gotoxy(1, 1);
	cout << "Witaj w edytorze labiryntu. Aby uzyskac pomoc nascisnij h.";
	do
	{
		klawisz = getch();
		if (klawisz == 'c') Stworz_nowy();
		if (klawisz == 'h') Wyswietl_pomoc_edytora();
		if (klawisz == 'e') Edytuj_istniejacy();
		clrscr();
	} while (klawisz != 'q');
}

void Wyswietl_pomoc_edytora()
{
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y);
	cout << "Mozesz tutaj stworzyc nowy labirynt lub edytowac juz istniejacy.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 1);
	cout << "****STEROWANIE****";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 2);
	cout << "strzalki - zmiana pozycji kursora.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 3);
	cout << "1 - umieszczenie sciany.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 4);
	cout << "spacja - umieszczenie pustego pola(korytarz).";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 5);
	cout << "d - umieszczenie drzwi.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 6);
	cout << "s - umieszczenie wejscia.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 7);
	cout << "m - umieszczenie wyjscia.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 8);
	cout << "z - zapisanie stanu edytowanego labiryntu.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 9);
	cout << "e - otwarcie istniejacego labiryntu do edycji.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 10);
	cout << "c - stworzenie nowego labiryntu.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 11);
	cout << "p - rozpoczecie gry w edytowanym labiryncie.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 12);
	cout << "q - wyjscie z edytora.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 13);
	cout << "Tworzony labirynt powinien spelniac pewne wymagania:";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 14);
	cout << "- Powinien posiadac 1 wejscie oraz 1 wyjscie";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 15);
	cout << "- Nie powinien zawierac pustych przestrzeni 2x2 lub wiekszych.";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 16);
	cout << "- Maksymalne wymiary labiryntu: 30(szerokosc) x 22(dlugosc).";
	gotoxy(POZYCJA_POMOCY_EDYTORA_X, POZYCJA_POMOCY_EDYTORA_Y + 17);
	cout << "- Labirynt jest czworokatem.";
	getch();
	clrscr();
}

//Glowna funkcja pozwalajaca na edycje labiryntu podanego jako argument
void Edytor(labirynt_t *labirynt)
{
	int x, y, klawisz = NULL;
	char poprzedni_znak = NULL;
	_setcursortype(_NORMALCURSOR);
	wspolrzedne_t kursor = { POZYCJA_EDYTORA_X, POZYCJA_EDYTORA_Y }, pozycja_poprzedniego_znaku;
	labirynt_t labirynt_kopia = *labirynt;
	do
	{
		clrscr();
		Wyswietl_labirynt(labirynt_kopia, POZYCJA_EDYTORA_X, POZYCJA_EDYTORA_Y);
		if (poprzedni_znak != NULL && poprzedni_znak != ' ') Ustaw_poprzedni(pozycja_poprzedniego_znaku, poprzedni_znak);
		gotoxy(kursor.x, kursor.y);
		klawisz = getch();
		if (klawisz == 'p') Graj_w_edytowany(*labirynt);
		if (klawisz == 'h') Wyswietl_pomoc_edytora();
		if (klawisz == 'z')
		{

			Zapisz_edytowany(labirynt_kopia);
		}
		if (klawisz == 0) //strzalka
		{
			klawisz = getch();
			Porusz_kursor(klawisz, &kursor);
		}
		if (klawisz == '1' || klawisz == ' ' || klawisz == 'd' || klawisz == 's' || klawisz == 'm')
		{
			Ustaw_znak(klawisz, kursor, &labirynt_kopia, &poprzedni_znak);
			pozycja_poprzedniego_znaku = kursor;
		}
	} while (klawisz != 'q');
	*labirynt = labirynt_kopia;
}

void Stworz_nowy()
{
	//tworzymy nowy labirynt z pusta plansza
	labirynt_t nowy = { NULL, {1, 1}, {1, 1}, MAX_SZEROKOSC, MAX_DLUGOSC};
	nowy.plansza = (char*)malloc(MAX_SZEROKOSC * MAX_DLUGOSC);
	for (int i = 0; i < nowy.rozmiar_x * nowy.rozmiar_y; i++) nowy.plansza[i] = ' '; //ustawiamy pusta plansze do edycji
	
	Edytor(&nowy);
}

void Edytuj_istniejacy()
{
	char *nazwa_pliku = NULL;
	int rozmiar;
	nazwa_pliku = Pobierz_nazwe_pliku(&rozmiar);
	labirynt_t labirynt = { NULL,{ 1, 1 },{ 1, 1 } , 0, 0 };

	Pobierz_rozmiar(nazwa_pliku, &labirynt);
	labirynt.plansza = (char *)malloc(labirynt.rozmiar_y * labirynt.rozmiar_x);
	Pobierz_labirynt(nazwa_pliku, &labirynt);

	//tworzymy nowy labirynt ze zwiekszona plansza - na wypadek ewentualnego powiekszenia labiryntu podczas edycji
	labirynt_t nowy = { NULL,{ 1, 1 },{ 1, 1 } , MAX_SZEROKOSC, MAX_DLUGOSC };
	nowy.plansza = (char*)malloc(MAX_SZEROKOSC * MAX_DLUGOSC);
	for (int i = 0; i < nowy.rozmiar_x * nowy.rozmiar_y; i++) nowy.plansza[i] = ' '; // zerujemy plansze
	for (int i = 0; i < labirynt.rozmiar_y; i++)
	{
		for (int j = 0; j < labirynt.rozmiar_x; j++)
		{
			nowy.plansza[i * nowy.rozmiar_x + j] = labirynt.plansza[i * labirynt.rozmiar_x + j];
		}
	}
	Edytor(&nowy);
}

// Zwraca nazwe pliku do zapisu 
//zmienna rozmiar_nazwy przechowuje ilosc liter nazwy
char *Pobierz_nazwe_pliku(int *rozmiar_nazwy)
{
	gotoxy(1, 1);
	int rozmiar = 2, nr_litery = 0;
	char wprowadzony = NULL;
	char *napis = NULL;
	napis = (char *)malloc(rozmiar);
	do
	{
		clrscr();
		rozmiar = 2;
		nr_litery = 0;
		wprowadzony = NULL;
		free(napis);
		*napis = NULL;
		napis = (char *)malloc(rozmiar);

		cout << "Podaj nazwe pliku: ";
		do
		{
			wprowadzony = getch();
			napis[nr_litery] = wprowadzony;
			cout << wprowadzony;
			nr_litery++;
			if (nr_litery == rozmiar - 1)
			{
				napis = Realokuj(napis, &rozmiar);
			}
		} while (wprowadzony != 13);
		napis[nr_litery - 1] = '\0';
	} while (!Poprawny_format_nazwy(napis, nr_litery));
	
	*rozmiar_nazwy = nr_litery;
	clrscr();
	return napis;
}

void Zapisz_edytowany(labirynt_t edytowany)
{
	int rozmiar_x_poczatkowy = edytowany.rozmiar_x; //przechowuje szerokosc planszy przed edycja - do funkcji skalujacej
	Okresl_rozmiar_edytowanego(&edytowany);
	Skaluj_plansze_labiryntu(&edytowany, rozmiar_x_poczatkowy);
	char *nazwa_pliku = NULL;
	int rozmiar;
	nazwa_pliku = Pobierz_nazwe_pliku(&rozmiar);
	while (!Poprawny_format_nazwy(nazwa_pliku, rozmiar))
	{
		cout << "Niepoprawna nazwa pliku.\n";
		free(nazwa_pliku);
		nazwa_pliku = Pobierz_nazwe_pliku(&rozmiar);
	}
	nazwa_pliku[rozmiar - 1] = '\0';
	FILE *do_zapisu;
	do_zapisu = fopen(nazwa_pliku, "w");
	for (int i = 0; i < edytowany.rozmiar_y; i++)
	{
		for (int j = 0; j < edytowany.rozmiar_x + 1; j++)
		{
			if (j == edytowany.rozmiar_x && i == edytowany.rozmiar_y - 1)
			{
				fputc(EOF, do_zapisu);
				break;
			}
			if (j == edytowany.rozmiar_x)
				fputc('\n', do_zapisu);
			else {
				switch (edytowany.plansza[(i * edytowany.rozmiar_x) + j])
				{
				case ' ': {
					fputc('0', do_zapisu);
					break;
				}
				case 'D': {
					fputc('D', do_zapisu);
					break;
				}
				case 'S': {
					fputc('S', do_zapisu);
					break;
				}
				case 'M': {
					fputc('M', do_zapisu);
					break;
				}
				default: fputc('1', do_zapisu);
				}
			}
		}
	}
	fclose(do_zapisu);
	gotoxy(1, 1);
	cout << "Labirynt zostal zapisany w pliku " << nazwa_pliku << "\n";
	cout << "Nacisnij dowolny klawisz, aby powrocic do edytora.";
	getch();
}

//Zmienia pozycje kursora w edytorze
void Porusz_kursor(int wprowadzenie, wspolrzedne_t *aktualne_wspolrzedne)
{
	switch (wprowadzenie)
	{
	case STRZALKA_GORA: {
		if ((*aktualne_wspolrzedne).y > POZYCJA_EDYTORA_Y) (*aktualne_wspolrzedne).y--;
		break;
	}
	case STRZALKA_DOL: {
		if ((*aktualne_wspolrzedne).y < MAX_DLUGOSC + POZYCJA_EDYTORA_Y - 1) (*aktualne_wspolrzedne).y++;
		break;
	}
	case STRZALKA_LEWO: {
		if ((*aktualne_wspolrzedne).x > POZYCJA_EDYTORA_X) (*aktualne_wspolrzedne).x--;
		break;
	}
	case STRZALKA_PRAWO: {
		if ((*aktualne_wspolrzedne).x < MAX_SZEROKOSC + POZYCJA_EDYTORA_X - 1) (*aktualne_wspolrzedne).x++;
		break;
	}
	default: cout << "Error: poruszanie sie po edytorze";
	}
}

//Ustawia znak w planszy labiryntu
//zmienna *znak pozwala przechowywac poprzedni ustawiony znak
void Ustaw_znak(int wprowadzenie, wspolrzedne_t pozycja_kursora, labirynt_t *docelowy, char *znak)
{
	int indeks_pola = (pozycja_kursora.y - POZYCJA_EDYTORA_Y) * MAX_SZEROKOSC + pozycja_kursora.x - POZYCJA_EDYTORA_X;
	switch (wprowadzenie)
	{
	case '1': {
		(*docelowy).plansza[indeks_pola] = Losowy_znak();
		*znak = (*docelowy).plansza[indeks_pola];
		break;
	}
	case ' ': {
		(*docelowy).plansza[indeks_pola] = ' ';
		*znak = ' ';
		break;
	}
	case 'd': {
		(*docelowy).plansza[indeks_pola] = 'D';
		*znak = 'D';
		break;
	}
	case 's': {
		(*docelowy).plansza[indeks_pola] = 'S';
		*znak = 'S';
		break;
	}
	case 'm': {
		(*docelowy).plansza[indeks_pola] = 'M';
		*znak = 'M';
		break;
	}
	}
}

char Losowy_znak()
{
	srand(time(NULL));
	char znak = NULL;
	int liczba;
	liczba = rand() % 5;
	switch (liczba)
	{
	case 0: {
		return '@';
		break;
	}
	case 1: {
		return '#';
		break;
	}
	case 2: {
		return '$';
		break;
	}
	case 3: {
		return '%';
		break;
	}
	case 4: {
		return '&';
		break;
	}
	}
	return 0;
}

//Funkcje uruchamiamy przed zapisaniem aby sprawdzic rozmiar i przypisac go do zmiennej labirynt
void Okresl_rozmiar_edytowanego(labirynt_t *labirynt)
{
	// szukamy rozmiaru_x:
	int i = 0;
	while ((*labirynt).plansza[i] != ' ')
	{
		i++;
	}
	(*labirynt).rozmiar_x = i;
	//szukamy rozmiaru_y :
	i = 0;
	while ((*labirynt).plansza[i * MAX_SZEROKOSC] != ' ' && i != MAX_DLUGOSC)
	{
		i++;
	}
	(*labirynt).rozmiar_y = i;
}

//Funkcja pomniejsza plansze o nieuzywane obszary(w przypadku zmniejszenia labiryntu podczas edycji)
void Skaluj_plansze_labiryntu(labirynt_t *labirynt, int x_poczatkowe)
{
	char *nowa = NULL;
	nowa = (char *)malloc((*labirynt).rozmiar_x * (*labirynt).rozmiar_y);
	//kopiujemy plansze do nowej zmiennej o odpowiednich rozmiarach:
	for (int i = 0; i < (*labirynt).rozmiar_y; i++)
	{
		for (int j = 0; j < (*labirynt).rozmiar_x; j++)
		{
			nowa[i * (*labirynt).rozmiar_x + j] = (*labirynt).plansza[i * x_poczatkowe + j];
		}
	}
	(*labirynt).plansza = nowa;
}

//rozmiar - rozmiar lancucha znakow - razem ze znakiem konca lancucha
bool Poprawny_format_nazwy(char *napis, int rozmiar)
{
	if (rozmiar < 5) return false; // oznacza to, ze nie ma .txt na koncu
	//Sprawdzamy czy ma .txt na koncu
	if (napis[rozmiar - 5] != '.' || napis[rozmiar - 4] != 't' || napis[rozmiar - 3] != 'x' || napis[rozmiar - 2] != 't') return false;
	//sprawdzamy czy napis zawiera znaki niedozwolone w formacie nazwy pliku
	for (int i = 0; i < rozmiar - 1; i++)
	{
		if (napis[i] == 92 || napis[i] == 47 || napis[i] == 58 || napis[i] == 42 || napis[i] == 63 || napis[i] == 34 || napis[i] == 60
				|| napis[i] == 62 || napis[i] == 124) return false;
	}
	return true;
}

//Realokcja pamieci - do tablicy dynamicznej. Podwaja rozmiar podanej tablicy typu char
char *Realokuj(char *tablica, int *aktualny_rozmiar)
{
	char *nowy = NULL;
	nowy = (char *)malloc(2 * (*aktualny_rozmiar));
	for (int i = 0; i < *aktualny_rozmiar; i++)
	{
		nowy[i] = tablica[i];
	}
	free(tablica);
	*aktualny_rozmiar *= 2;
	return nowy;
}

void Ustaw_poprzedni(wspolrzedne_t pozycja_poprzedniego_znaku, char poprzedni_znak)
{
	gotoxy(pozycja_poprzedniego_znaku.x, pozycja_poprzedniego_znaku.y);
	textcolor(LIGHTRED);
	cout << poprzedni_znak;
	textcolor(LIGHTGRAY);
}

void Graj_w_edytowany(labirynt_t labirynt)
{
	int rozmiar_x_poczatkowy = labirynt.rozmiar_x; //przechowuje pierwotna szerokosc planszy - do funkcji skalujacej
	Okresl_rozmiar_edytowanego(&labirynt);
	Skaluj_plansze_labiryntu(&labirynt, rozmiar_x_poczatkowy);
	struct wspolrzedne_t wsp_gracza = { 1, 1 };
	struct gracz_t gracz = { GORA, wsp_gracza, 0 };
	clock_t czas_rozpoczecia;
	Ustaw_wartosci_poczatkowe(&labirynt, &gracz, &czas_rozpoczecia);
	Graj(labirynt, gracz, czas_rozpoczecia);
}