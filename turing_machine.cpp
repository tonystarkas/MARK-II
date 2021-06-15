#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include <mutex>
using namespace std;
mutex mt;


struct Kintamieji {
	string Dabartinebusena, Sekantibusena;
	char Esamassimbolis, Busimassimbolis, Kryptis;
};

void Rodykle(int x, int y) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void IvedimasSkaiciavimas(string Failas, int Vieta) {
	//Nuskaitymas
	int gijos;
	Kintamieji K[1000];
	int halt = 0;
	string Juosta, laikinojibusena = "0";
	int Busena;
	ifstream fd;
	fd.open(Failas);
	fd >> Busena;
	fd >> Juosta;
	int a = 0;
	while (!fd.eof()) {
		fd >> K[a].Dabartinebusena;
		fd >> K[a].Esamassimbolis;
		fd >> K[a].Busimassimbolis;
		fd >> K[a].Kryptis;
		fd >> K[a].Sekantibusena;
		a++;
	}
	fd.close();
	//Skaiciavimas
	while (halt == 0) {
		if (_kbhit()) {
			break;
		}
		int Apribojimas = 1;//tikrina ar atitinka ifa
		mt.lock();
		Rodykle(0, (Vieta * 6) + 1);
		cout << Failas << endl << endl;
		cout << Juosta << endl;
		for (int i = 0; i < Juosta.length(); i++) {
			if (i == (Busena - 1)) {
				cout << "^";
			}
			else cout << " ";
		}
		cout << endl;
		mt.unlock();
		Sleep(10);
		for (int i = 0; i < a; i++) {
			if (K[i].Dabartinebusena == laikinojibusena && K[i].Esamassimbolis == Juosta[Busena - 1])
			{
				Apribojimas = 0;
				Juosta[Busena - 1] = K[i].Busimassimbolis;
				if (K[i].Kryptis == 'R') {
					Busena++;
				}
				else Busena--;

				laikinojibusena = K[i].Sekantibusena;

				if (laikinojibusena == "X" || Busena == 0 || Busena > (Juosta.length())) {//tikrina ar yra ribose
					halt = 1;
				}
				if (Apribojimas == 1) {
					halt = 1;
				}
			}
		}
	}
	mt.lock();
	Rodykle(0, (Vieta * 6) + 1);
	cout << Failas << endl << "Baigta" << endl;
	cout << Juosta << endl << endl;
	mt.unlock();
}

int main()
{
	int sk;
	string Failas[100];
	cout << "Kiek failu paleisti?" << endl;
	cin >> sk;
	for (int i = 0; i < sk; i++) {
		cout << i + 1 << "-ojo" << " " << "failo pavadinimas" << endl;
		cin >> Failas[i];
		cout << endl;
	}
	system("cls");
	cout << "~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
	thread thrd[20];
	for (int i = 0; i < sk; i++) {
		thrd[i] = thread(IvedimasSkaiciavimas, Failas[i], i);
	}
	for (int i = 0; i < sk; i++) {
		thrd[i].join();
	}
}