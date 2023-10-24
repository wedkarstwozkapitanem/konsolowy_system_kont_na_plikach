#include<iostream>
#include <locale.h>
#include <string>
#include <limits>
#include <map>
#include <fstream>
#include <list>
#include <time.h>
#include <cstdlib>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
#define endl '\n'; //własna stała nowej lini



class interfejs_logowania {
private:
	std::map<std::string, std::string> baza_loginow;

	short czy_nowy{ 0 }, limit_prob{ 0 };
	struct {
		std::string email{ "" }, powtorzony_email{ "" }, haslo{ "" }, powtorzone_haslo{ "" };
	} login;

	struct {
		int id{ 0 };
		std::string imie{ "" }, nawisko{ "" };
		int  wiek;
		std::vector<std::string> hobby;
	} dane_profilu;

	int zliczanie_wartosci(std::string email, char znak) {
		int wynik{ 0 };
		for (int i{ 0 }; i < email.length(); i++)	if (email[i] == znak) wynik++;
		return wynik;
	}

	bool czy_walidowany(std::string email) {
		if (email.find('@') == -1 || email[0] == '@' || email[email.length() - 1] == '@' || zliczanie_wartosci(email, '@') != 1 || email.length() <= 4) {
			return false;
		}
		return true;
	}

	bool czy_duza_litera(std::string h) {
		for(auto &i : h) {
			if ((i > 64) && (i < 91)) return true;
		}
		return false;
	}

	bool czy_liczba(std::string h) {
		for (auto& i : h) {
			if ((i > 47) && i < 58) return true;
		}
		return false;
	}

	bool czy_walidowane_haslo(std::string haslo) {
		if (haslo.length() < 4 || !czy_duza_litera(haslo) || !czy_liczba(haslo) ) {
			return false;
		}
		return true;
	}


	std::string podzial(std::string tekst,int start) {
		std::string tresc{ "" };
		for (int i{ start }; i < tekst.length(); i++) {
			if (tekst[i] == ' ') {
				return tresc;
				break;
			}
			tresc += tekst[i];
		}

		return tresc;
	}

	bool pobierz_z_bazy() {
		std::ifstream baza_danych("baza_danych.txt");
		if (baza_danych.good()) {
			std::string login{ "" }, email{ "" }, haslo{ "" };
			int id{ 0 };
			while (getline(baza_danych, login)) {
				email = podzial(login, 0);
				haslo = podzial(login, email.length() + 3);
				if (podzial(login, haslo.length() + email.length() + 3) != "") {
					id = (int)std::stoi(podzial(login, haslo.length() + email.length() + 3));

				//	cout << podzial(login, haslo.length() + 3) << endl;
				}
			//	cout << "-" << haslo << endl;
				baza_loginow[email] = haslo;
			}
		}
		baza_danych.close();
		return true;
	}


	bool dodaj_uzytkownika(std::string email,std::string haslo,std::string imie) {
		std::fstream baza_danych("baza_danych.txt", std::ios::app);
		if (!baza_danych.is_open()) {
			baza_danych.open("baza_danych.txt");
		}

		srand(time(NULL));
		int losowy_id = std::rand();


		if (baza_danych.is_open() && baza_danych.good()) {
			baza_danych << email << " | " << haslo << " | " << losowy_id << endl;
			baza_danych.close();

			std::fstream dane_uzytkownika;
				dane_uzytkownika.open(email + ".txt", std::ios::out);
				if (dane_uzytkownika.is_open() && dane_uzytkownika.good()) {
				dane_uzytkownika << imie << endl;
				dane_uzytkownika.close();
				}
				else {
					cerr << "Nie udało się pobrać danych profilowych";
				}
			
		}
		else {
			cerr << "Wystąpił błąd podczas odczytu pliku\n";
		}
		
		return true;
	}


	bool czy_email_istnieje(std::string e) {
		pobierz_z_bazy();
		for (const auto& i : baza_loginow) {
		//	cout << i.first << endl;
			if (i.first == e) return true;
		}
		return false;
	}

	void pobierz_dane_profilu() {
		pobierz_id();
		std::fstream dane(login.email + ".txt", std::ios::in);

		if (dane.good() && dane.is_open()) {
			std::string linia;
			//while (getline(dane, linia)) {
			    dane >> linia;
			    dane_profilu.imie = linia;
				//cout << linia << endl;
			//}
		}
		else {
			cerr << "Błąd";
		}
	}
	void pobierz_dane_profilu(int id) {
		std::fstream dane(login.email + ".txt", std::ios::in);

		if (dane.good() && dane.is_open()) {
			std::string linia;
			//while (getline(dane, linia)) {
				dane >> linia;
			    dane_profilu.imie = linia;
				//cout << linia << endl;
				
			//}
		}
		else {
			cerr << "Błąd";
		}
	}

	int pobierz_id() {
		int id{ 0 };
		std::ifstream baza_danych("baza_danych.txt");
		if (baza_danych.good()) {
			std::string login{ "" }, email{ "" }, haslo{ "" };
			int id{ 0 };
			while (getline(baza_danych, login)) {
				if (podzial(login, 0) == this->login.email) {
						haslo = podzial(login, email.length() + 3);
						if (podzial(login, haslo.length() + email.length() + 3) != "") {
							dane_profilu.id = (int)std::stoi(podzial(login, haslo.length() + email.length() + 3));
							return dane_profilu.id;
						}
				}
			}
		}
		baza_danych.close();
		return 0;
	}
public:
	interfejs_logowania(){
		cout << "Chcesz się zarejestrować (1) czy masz już konto? (2) ";
		while (!(cin >> this->czy_nowy)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			cout << "Opcją musi być liczba z zakresu 1-2 Ponowna próba: ";
		}
		if (this->czy_nowy == 1) rejestracja();
		else logowanie();
	}

	void rejestracja() {
		cout << "Wprowadz email ";
		do {
			while (!(cin >> this->login.email)) {
				if (cin.fail()) {
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					cerr << "Wystąpił problem.Spróbuj wpisać email jeszcze raz! ";
				}
			}
			if (czy_email_istnieje(this -> login.email)) {
				cout << "Podany email już u nas istnieje, więc możesz się zalogować" << endl;
				logowanie();
				break;
				return;
			}
			if (!czy_walidowany(this -> login.email)) cout << "Email nie jest walidowany!\n**Musi zawierać id użytkownika @ domene poczty oraz zawierać minimium 4 znaki **\n";
		} while (!czy_walidowany(this -> login.email));
	//	system("cls");
		powtorz_email();
	}

	void powtorz_email() {
		cout << "Powtórz email ";
		while (!(cin >> this->login.powtorzony_email)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "Wystąpił problem.Spróbuj wpisać powtórzony email jeszcze raz! ";
			}
		}
		while (this->login.email != this->login.powtorzony_email) {
			cout << "Emaile nie są zgodne\nPowtórz emaila jeszcze raz ";
			cin >> this->login.powtorzony_email;
		}
	//	system("cls");
		wprowadz_haslo();
	}

	void wprowadz_haslo() {
		cout << "Wprowadz hasło ";
		do {
			while (!(cin >> this->login.haslo)) {
				if (cin.fail()) {
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					cerr << "Wystąpił problem.Spróbuj wpisać hasło jeszcze raz! ";
				}
			}
			if (!czy_walidowane_haslo(this->login.haslo)) cout << "Hasło nie jest walidowane!\n**Musi zawierać przyjnamniej 4 znaki,liczby oraz wielkie litery **\n";
		} while (!czy_walidowane_haslo(this->login.haslo));
		powtorz_haslo();
	}

	void powtorz_haslo() {
		cout << "Powtórz hasło ";
		while (!(cin >> this->login.powtorzone_haslo)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "Wystąpił problem.Spróbuj wpisać powtórzony hasło jeszcze raz! ";
			}
		}
		while (this->login.haslo != this->login.powtorzone_haslo) {
			cout << "Hasła nie są zgodne\nPowtórz hasło jeszcze raz ";
			cin >> this->login.powtorzone_haslo;
		}
		//	system("cls");
		cout << "Jak masz na imie? ";
		while (!(cin >> this->dane_profilu.imie)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "Wystąpił problem.Spróbuj ponownie wpisać pole imie! " << endl;
			}
		}
		if (dodaj_uzytkownika(this -> login.email, this ->login.haslo,this -> dane_profilu.imie)) cout << "Zarejestrowano pomyślnie!\n Witaj " << dane_profilu.imie;
		else cout << "Wystąpił błąd";
	}


	void logowanie() {
		pobierz_z_bazy();
//		for (const auto& i : baza_loginow) cout << i.first << endl;
		cout << "Zaloguj się \nWprowadż email ";
		while(!(cin >> login.email)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "Wystąpił błąd.Wprowadż jeszcze raz email";
			}
		}
		cout << "Wprowadż hasło ";

		while (!(cin >> login.haslo)) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cerr << "Wystąpił błąd.Wprowadż jeszcze raz hasło";
			}
		}

		if (baza_loginow[login.email] == login.haslo) {
			cout << "\nPomyślnie zalogowano\nWitaj ";
			this -> pobierz_dane_profilu(pobierz_id());
			cout << this->dane_profilu.imie;
			cout << "\nDalsza część serwisu w budowie";
		}
		else {
			if (czy_email_istnieje(login.email)) {
				while (baza_loginow[login.email] != login.haslo) {
					cout << "Błedne dane logowania.Wprowadż jeszcze raz hasło: ";
					while (!(cin >> login.haslo)) {
						if (cin.fail()) {
							cin.clear();
							cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							cerr << "Wystąpił błąd.Wprowadż jeszcze raz hasło";
						}
					}
				}
			cout << "\nPomyślnie zalogowano\nWitaj ";
			this -> pobierz_dane_profilu(pobierz_id());
			this -> dane_profilu.imie;
			
			cout << "\nDalsza część serwisu w budowie";
			}
			else {
				cout << "Nie znaleziono danego loginu!";
			}
		}
	}
};

int main() {
	setlocale(LC_CTYPE, "Polish");
	interfejs_logowania logowanie;
	return 0;
}
