#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <list>
#include <set>
#include <cstring>
#include <utility>
#include <queue>

/* Las siguientes 3 funciones se implementaron con objeto de realizar el algoritmo
   de búsqueda A estrella tras realizar el implementado finalmente (Anchura). No he
   sido capaz de hacerlo funcionar pero igualmente me gustaría enseñar que lo intenté. 
*/

int ComportamientoJugador::distanciaManhattan(const estado &e, const estado &f){
	assert(e.fila < 100 and e.columna < 100 and f.fila < 100 and f.columna < 100);

	int distancia = abs(e.fila - f.fila) + abs(e.columna - f.columna);

	return distancia;
}

// Función que me permite saber el coste mínimo de los adyacentes
estado ComportamientoJugador::costeMinimo(const estado &origen, const estado &fin){
	estado minimo, posible;
	int coste_minimo = distanciaManhattan(origen, fin);
	
	minimo.fila = origen.fila;
	minimo.columna = origen.columna;
	
	// orientacion por defecto para el estado minimo
	minimo.orientacion = origen.orientacion;

	for (int i = origen.fila-1; i <= origen.fila+1; ++i){
		for (int j = origen.columna-1; j < origen.columna+1; ++j){
			if(i != origen.fila and j != origen.columna){
				posible.fila = i;
				posible.columna = j;
				if(distanciaManhattan(posible, fin) < coste_minimo){
					minimo.fila = posible.fila;		
					minimo.columna = posible.columna;
				}
			}
		}
	}

	return minimo;
}

std::vector<pair<estado, list<Action> > > ComportamientoJugador::nodosAdyacentes(pair<estado,list<Action> > &origen){
	std::vector<pair<estado, list<Action> > > v;
	std::pair<estado,list<Action>> sucesor;
	auto e = origen;

	sucesor = e;

	// Nodo delante
	switch(e.first.orientacion){
		case 0: 
			sucesor.first.fila = e.first.fila-1;
			break;
		case 1:
			sucesor.first.columna = e.first.columna+1;						
			break;
		case 2:
			sucesor.first.fila = e.first.fila+1;
			break;
		case 3:
			sucesor.first.columna = e.first.columna-1;
			break;
	}

	sucesor.second.push_back(actFORWARD);
	v.push_back(sucesor);		

	// Nodo a la izquierda
	sucesor = e;

	switch(e.first.orientacion){
		case 0: 
			sucesor.first.fila = e.first.columna-1;
			break;
		case 1:
			sucesor.first.columna = e.first.fila-1;						
			break;
		case 2:
			sucesor.first.fila = e.first.columna+1;
			break;
		case 3:
			sucesor.first.columna = e.first.fila+1;
			break;
	}

	sucesor.first.orientacion = (e.first.orientacion+3)%4;
	sucesor.second.push_back(actTURN_L);
	sucesor.second.push_back(actFORWARD);
	v.push_back(sucesor);

	// Nodo a la derecha
	sucesor = e;

	switch(e.first.orientacion){
		case 0: 
			sucesor.first.fila = e.first.columna+1;
			break;
		case 1:
			sucesor.first.columna = e.first.fila+1;						
			break;
		case 2:
			sucesor.first.fila = e.first.columna-1;
			break;
		case 3:
			sucesor.first.columna = e.first.fila-1;
			break;
	}

	sucesor.first.orientacion = (e.first.orientacion+1)%4;
	sucesor.second.push_back(actTURN_R);
	sucesor.second.push_back(actFORWARD);
	v.push_back(sucesor);

	// Nodo atrás
	sucesor = e;

	switch(e.first.orientacion){
		case 0: 
			sucesor.first.fila = e.first.fila+1;
			break;
		case 1:
			sucesor.first.columna = e.first.columna-1;						
			break;
		case 2:
			sucesor.first.fila = e.first.fila-1;
			break;
		case 3:
			sucesor.first.columna = e.first.columna+1;
			break;
	}

	if(e.first.orientacion < 2)
		sucesor.first.orientacion = sucesor.first.orientacion +2;

	// decido que gire 2 veces a la derecha
	sucesor.second.push_back(actTURN_R);
	sucesor.second.push_back(actTURN_R);
	sucesor.second.push_back(actFORWARD);
	v.push_back(sucesor);

	return v;
}

bool Find(list<pair<estado, list<Action> > > l, estado st){
	bool encontrado;

	for (auto it = l.begin(); it != l.end(); ++it){
		if(it->first.fila == st.fila and it->first.columna == st.columna and it->first.orientacion == st.orientacion)
			encontrado = true;
	}

	return encontrado;
}

// Va escribiendo sobre mapaResultado a medida que el jugador se va moviendo de forma
// que se va descubriendo el mapa
void ComportamientoJugador::PintaMapa(Sensores sensores){
	mapaResultado[fil][col] = sensores.terreno[0];
	switch(brujula){
		case 0:
			mapaResultado[fil-1][col-1] = sensores.terreno[1];
			mapaResultado[fil-1][col] = sensores.terreno[2];
			mapaResultado[fil-1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col-2] = sensores.terreno[4];
			mapaResultado[fil-2][col-1] = sensores.terreno[5];
			mapaResultado[fil-2][col] = sensores.terreno[6];
			mapaResultado[fil-2][col+1] = sensores.terreno[7];
			mapaResultado[fil-2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col-3] = sensores.terreno[9];
			mapaResultado[fil-3][col-2] = sensores.terreno[10];
			mapaResultado[fil-3][col-1] = sensores.terreno[11];
			mapaResultado[fil-3][col] = sensores.terreno[12];
			mapaResultado[fil-3][col+1] = sensores.terreno[13];
			mapaResultado[fil-3][col+2] = sensores.terreno[14];
			mapaResultado[fil-3][col+3] = sensores.terreno[15];
		 break;
		case 1:
			mapaResultado[fil-1][col+1] = sensores.terreno[1];
			mapaResultado[fil][col+1] = sensores.terreno[2];
			mapaResultado[fil+1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col+2] = sensores.terreno[4];
			mapaResultado[fil-1][col+2] = sensores.terreno[5];
			mapaResultado[fil][col+2] = sensores.terreno[6];
			mapaResultado[fil+1][col+2] = sensores.terreno[7];
			mapaResultado[fil+2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col+3] = sensores.terreno[9];
			mapaResultado[fil-2][col+3] = sensores.terreno[10];
			mapaResultado[fil-1][col+3] = sensores.terreno[11];
			mapaResultado[fil][col+3] = sensores.terreno[12];
			mapaResultado[fil+1][col+3] = sensores.terreno[13];
			mapaResultado[fil+2][col+3] = sensores.terreno[14];
			mapaResultado[fil+3][col+3] = sensores.terreno[15];
		 break;
		case 2:
			mapaResultado[fil+1][col+1] = sensores.terreno[1];
			mapaResultado[fil+1][col] = sensores.terreno[2];
			mapaResultado[fil+1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col+2] = sensores.terreno[4];
			mapaResultado[fil+2][col+1] = sensores.terreno[5];
			mapaResultado[fil+2][col] = sensores.terreno[6];
			mapaResultado[fil+2][col-1] = sensores.terreno[7];
			mapaResultado[fil+2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col+3] = sensores.terreno[9];
			mapaResultado[fil+3][col+2] = sensores.terreno[10];
			mapaResultado[fil+3][col+1] = sensores.terreno[11];
			mapaResultado[fil+3][col] = sensores.terreno[12];
			mapaResultado[fil+3][col-1] = sensores.terreno[13];
			mapaResultado[fil+3][col-2] = sensores.terreno[14];
			mapaResultado[fil+3][col-3] = sensores.terreno[15];
		 break;
		case 3:
			mapaResultado[fil+1][col-1] = sensores.terreno[1];
			mapaResultado[fil][col-1] = sensores.terreno[2];
			mapaResultado[fil-1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col-2] = sensores.terreno[4];
			mapaResultado[fil+1][col-2] = sensores.terreno[5];
			mapaResultado[fil][col-2] = sensores.terreno[6];
			mapaResultado[fil-1][col-2] = sensores.terreno[7];
			mapaResultado[fil-2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col-3] = sensores.terreno[9];
			mapaResultado[fil+2][col-3] = sensores.terreno[10];
			mapaResultado[fil+1][col-3] = sensores.terreno[11];
			mapaResultado[fil][col-3] = sensores.terreno[12];
			mapaResultado[fil-1][col-3] = sensores.terreno[13];
			mapaResultado[fil-2][col-3] = sensores.terreno[14];
			mapaResultado[fil-3][col-3] = sensores.terreno[15];
		 break;
	}
}

// PintaACiegas escribe sobre una matriz auxiliar que se usará mientras no estemos bien situados
void ComportamientoJugador::PintaACiegas(Sensores sensores){
	mapaResultadoaux[fil][col] = sensores.terreno[0];
	switch(brujula){
		case 0:
			mapaResultadoaux[filaux-1][colaux-1] = sensores.terreno[1];
			mapaResultadoaux[filaux-1][colaux] = sensores.terreno[2];
			mapaResultadoaux[filaux-1][colaux+1] = sensores.terreno[3];
			mapaResultadoaux[filaux-2][colaux-2] = sensores.terreno[4];
			mapaResultadoaux[filaux-2][colaux-1] = sensores.terreno[5];
			mapaResultadoaux[filaux-2][colaux] = sensores.terreno[6];
			mapaResultadoaux[filaux-2][colaux+1] = sensores.terreno[7];
			mapaResultadoaux[filaux-2][colaux+2] = sensores.terreno[8];
			mapaResultadoaux[filaux-3][colaux-3] = sensores.terreno[9];
			mapaResultadoaux[filaux-3][colaux-2] = sensores.terreno[10];
			mapaResultadoaux[filaux-3][colaux-1] = sensores.terreno[11];
			mapaResultadoaux[filaux-3][colaux] = sensores.terreno[12];
			mapaResultadoaux[filaux-3][colaux+1] = sensores.terreno[13];
			mapaResultadoaux[filaux-3][colaux+2] = sensores.terreno[14];
			mapaResultadoaux[filaux-3][colaux+3] = sensores.terreno[15];
		 break;
		case 1:
			mapaResultadoaux[filaux-1][colaux+1] = sensores.terreno[1];
			mapaResultadoaux[filaux][colaux+1] = sensores.terreno[2];
			mapaResultadoaux[filaux+1][colaux+1] = sensores.terreno[3];
			mapaResultadoaux[filaux-2][colaux+2] = sensores.terreno[4];
			mapaResultadoaux[filaux-1][colaux+2] = sensores.terreno[5];
			mapaResultadoaux[filaux][colaux+2] = sensores.terreno[6];
			mapaResultadoaux[filaux+1][colaux+2] = sensores.terreno[7];
			mapaResultadoaux[filaux+2][colaux+2] = sensores.terreno[8];
			mapaResultadoaux[filaux-3][colaux+3] = sensores.terreno[9];
			mapaResultadoaux[filaux-2][colaux+3] = sensores.terreno[10];
			mapaResultadoaux[filaux-1][colaux+3] = sensores.terreno[11];
			mapaResultadoaux[filaux][colaux+3] = sensores.terreno[12];
			mapaResultadoaux[filaux+1][colaux+3] = sensores.terreno[13];
			mapaResultadoaux[filaux+2][colaux+3] = sensores.terreno[14];
			mapaResultadoaux[filaux+3][colaux+3] = sensores.terreno[15];
		 break;
		case 2:
			mapaResultadoaux[filaux+1][colaux+1] = sensores.terreno[1];
			mapaResultadoaux[filaux+1][colaux] = sensores.terreno[2];
			mapaResultadoaux[filaux+1][colaux-1] = sensores.terreno[3];
			mapaResultadoaux[filaux+2][colaux+2] = sensores.terreno[4];
			mapaResultadoaux[filaux+2][colaux+1] = sensores.terreno[5];
			mapaResultadoaux[filaux+2][colaux] = sensores.terreno[6];
			mapaResultadoaux[filaux+2][colaux-1] = sensores.terreno[7];
			mapaResultadoaux[filaux+2][colaux-2] = sensores.terreno[8];
			mapaResultadoaux[filaux+3][colaux+3] = sensores.terreno[9];
			mapaResultadoaux[filaux+3][colaux+2] = sensores.terreno[10];
			mapaResultadoaux[filaux+3][colaux+1] = sensores.terreno[11];
			mapaResultadoaux[filaux+3][colaux] = sensores.terreno[12];
			mapaResultadoaux[filaux+3][colaux-1] = sensores.terreno[13];
			mapaResultadoaux[filaux+3][colaux-2] = sensores.terreno[14];
			mapaResultadoaux[filaux+3][colaux-3] = sensores.terreno[15];
		 break;
		case 3:
			mapaResultadoaux[filaux+1][colaux-1] = sensores.terreno[1];
			mapaResultadoaux[filaux][colaux-1] = sensores.terreno[2];
			mapaResultadoaux[filaux-1][colaux-1] = sensores.terreno[3];
			mapaResultadoaux[filaux+2][colaux-2] = sensores.terreno[4];
			mapaResultadoaux[filaux+1][colaux-2] = sensores.terreno[5];
			mapaResultadoaux[filaux][colaux-2] = sensores.terreno[6];
			mapaResultadoaux[filaux-1][colaux-2] = sensores.terreno[7];
			mapaResultadoaux[filaux-2][colaux-2] = sensores.terreno[8];
			mapaResultadoaux[filaux+3][colaux-3] = sensores.terreno[9];
			mapaResultadoaux[filaux+2][colaux-3] = sensores.terreno[10];
			mapaResultadoaux[filaux+1][colaux-3] = sensores.terreno[11];
			mapaResultadoaux[filaux][colaux-3] = sensores.terreno[12];
			mapaResultadoaux[filaux-1][colaux-3] = sensores.terreno[13];
			mapaResultadoaux[filaux-2][colaux-3] = sensores.terreno[14];
			mapaResultadoaux[filaux-3][colaux-3] = sensores.terreno[15];
		 break;
	}
}

void ComportamientoJugador::MarcaIntransitables(Sensores sensores){	
	switch(brujula){
		case 0: 
			if (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'A' 
				or sensores.terreno[2] == 'B' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M'){
				visitas_a_ciegas[filaux-1][colaux] = 7777;
			}
			break;
		case 1: 
			if (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'A' 
				or sensores.terreno[2] == 'B' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M'){
				visitas_a_ciegas[filaux][colaux+1] = 7777;
			}
			break;
		case 2: 
			if (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'A' 
				or sensores.terreno[2] == 'B' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M'){
				visitas_a_ciegas[filaux+1][colaux] = 7777;	
			}
			break;
		case 3:
			if (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'A' 
				or sensores.terreno[2] == 'B' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'M'){
				visitas_a_ciegas[filaux][colaux-1] = 7777;
			}
			break;
	}
}


// Función que devolverá la posición adyacente a la mía más prioritaria
Action ComportamientoJugador::Minimo(int a, int b, int c, int d){// Los 4 parámetros representan las posiciones delante, 
	Action accion;;						         //izquierda, derecha y detras mía, respectivamente
	int minimo; 
	int minimo1, minimo2;

	minimo1 = a < b ? a : b;
	minimo2 = c < d ? c : d;
	
	minimo = minimo1 < minimo2 ? minimo1 : minimo2;

	if (minimo == a ){ // and minimo != 7777...
		accion = actFORWARD;
	}
	else if (minimo == b or minimo == d){
		accion = actTURN_L;
	}
	else if(minimo == c)
		accion = actTURN_R;

	return accion; 
}

void ComportamientoJugador::MarcaK(unsigned char &a, unsigned char &b, unsigned char &c, unsigned char &d,
								int &pos1, int &pos2, int &pos3, int &pos4){
	if (a == 'K'){
		pos1 = -1;
	}
	else if(b == 'K'){ pos2 = -1; }
	else if(c == 'K'){ pos3 = -1;}
	else if (d == 'K'){ pos4 = -1;}
}


Action ComportamientoJugador::ProximaAccion(){
	Action accion;

	switch(brujula){
		case 0:
			MarcaK(mapaResultadoaux[filaux-1][colaux], mapaResultadoaux[filaux][colaux-1],mapaResultadoaux[filaux][colaux+1],mapaResultadoaux[filaux+1][colaux],
				visitas_a_ciegas[filaux-1][colaux], visitas_a_ciegas[filaux][colaux-1],visitas_a_ciegas[filaux][colaux+1],visitas_a_ciegas[filaux+1][colaux]);
			accion = Minimo(visitas_a_ciegas[filaux-1][colaux], visitas_a_ciegas[filaux][colaux-1],visitas_a_ciegas[filaux][colaux+1],visitas_a_ciegas[filaux+1][colaux]); 
			break;
		case 1:
			MarcaK(mapaResultadoaux[filaux][colaux+1], mapaResultadoaux[filaux-1][colaux],mapaResultadoaux[filaux+1][colaux],mapaResultadoaux[filaux][colaux-1],
				visitas_a_ciegas[filaux][colaux+1], visitas_a_ciegas[filaux-1][colaux],visitas_a_ciegas[filaux+1][colaux],visitas_a_ciegas[filaux][colaux-1]); 
			accion = Minimo(visitas_a_ciegas[filaux][colaux+1], visitas_a_ciegas[filaux-1][colaux],visitas_a_ciegas[filaux+1][colaux],visitas_a_ciegas[filaux][colaux-1]); 
			break;
		case 2: 
			MarcaK(mapaResultadoaux[filaux+1][colaux], mapaResultadoaux[filaux][colaux+1],mapaResultadoaux[filaux][colaux-1],mapaResultadoaux[filaux-1][colaux],
				visitas_a_ciegas[filaux+1][colaux], visitas_a_ciegas[filaux][colaux+1],visitas_a_ciegas[filaux][colaux-1],visitas_a_ciegas[filaux-1][colaux]);
			accion = Minimo(visitas_a_ciegas[filaux+1][colaux], visitas_a_ciegas[filaux][colaux+1],visitas_a_ciegas[filaux][colaux-1],visitas_a_ciegas[filaux-1][colaux]);
			break;
		case 3:
			MarcaK(mapaResultadoaux[filaux][colaux-1], mapaResultadoaux[filaux+1][colaux],mapaResultadoaux[filaux-1][colaux],mapaResultadoaux[filaux][colaux+1],
				visitas_a_ciegas[filaux][colaux-1], visitas_a_ciegas[filaux+1][colaux],visitas_a_ciegas[filaux-1][colaux],visitas_a_ciegas[filaux][colaux+1]); 
			accion = Minimo(visitas_a_ciegas[filaux][colaux-1], visitas_a_ciegas[filaux+1][colaux],visitas_a_ciegas[filaux-1][colaux],visitas_a_ciegas[filaux][colaux+1]);
			break;
	}
	
	return accion;
}

struct CompararNodo{
	bool operator()(const pair<estado,list<Action> > &a, const pair<estado, list<Action> > &b){
		if (a.first.fila < b.first.fila){
			return true;
		}
		else if (a.first.fila == b.first.fila and a.first.columna < b.first.columna){
			return true;
		}
		else if (a.first.fila == b.first.fila and a.first.columna == b.first.columna and a.first.orientacion < b.first.orientacion){
			return true;
		}
		else
			return false;
	}
};

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan){
	list<pair<estado,list<Action> > > lista_abiertos;
	set<pair<estado,list<Action> >, CompararNodo> cerrados;
	std::list<Action> v;
	bool objetivo_conseguido = false;

	plan.clear();
	auto aux = std::make_pair(origen, v);
	lista_abiertos.push_back(aux);
	
	// Mientras no esté vacía y no hayamos encontrado solución...
	while(!lista_abiertos.empty() and !objetivo_conseguido){
		std::pair<estado,list<Action>> sucesor;
		auto e = lista_abiertos.front();		

		lista_abiertos.pop_front();
		cerrados.insert(e);
		
		// Expando los sucesores de e

		// AVANZAR
		sucesor = e;

		switch(e.first.orientacion){
			case 0: 
				sucesor.first.fila = e.first.fila-1;
				break;
			case 1:
				sucesor.first.columna = e.first.columna+1;						
				break;
			case 2:
				sucesor.first.fila = e.first.fila+1;
				break;
			case 3:
				sucesor.first.columna = e.first.columna-1;
				break;
		}
		sucesor.second.push_back(actFORWARD);
		
		auto encontrado1 = cerrados.find(sucesor);
		bool encontrado = encontrado1 != cerrados.end();

		if (sucesor.first.fila >= 0 and sucesor.first.fila < mapaResultado.size() and 
			sucesor.first.columna >= 0 and sucesor.first.columna < mapaResultado.size() )
		{	
			if (!encontrado and mapaResultado[sucesor.first.fila][sucesor.first.columna] == 'T' or mapaResultado[sucesor.first.fila][sucesor.first.columna] == 'S'
				or mapaResultado[sucesor.first.fila][sucesor.first.columna] == 'K' or mapaResultado[sucesor.first.fila][sucesor.first.columna] == '?'){
				lista_abiertos.push_back(sucesor);
				cerrados.insert(sucesor);
			}
		} 

		// GIRAR IZQUIERDA
		sucesor = e;

		sucesor.first.orientacion = (e.first.orientacion+3)%4;
		sucesor.second.push_back(actTURN_L);
		encontrado1 = cerrados.find(sucesor);

		encontrado = encontrado1 != cerrados.end();
		
		if (!encontrado){ 
			lista_abiertos.push_back(sucesor);
			cerrados.insert(sucesor);
		}
		
		// GIRAR DERECHA
		sucesor = e;

		sucesor.first.orientacion = (e.first.orientacion+1)%4;
		sucesor.second.push_back(actTURN_R);

		encontrado1 = cerrados.find(sucesor);
		
		encontrado = encontrado1 != cerrados.end(); 
		
		if (!encontrado){
			lista_abiertos.push_back(sucesor);
			cerrados.insert(sucesor);
		}
		
		
		if(lista_abiertos.front().first.fila == destino.fila and lista_abiertos.front().first.columna == destino.columna){
			//cout << "conseguido" << endl;
			objetivo_conseguido = true;
		}
	}		

	if (objetivo_conseguido){
		auto e = lista_abiertos.front();
		plan = e.second;
		//cout << "plan encontrado" << endl;
		VisualizaPlan(origen, plan);
		objetivo_conseguido = true;
	}
	
	return objetivo_conseguido;
}


Action ComportamientoJugador::think(Sensores sensores) {
	Action sigAccion;
	visita++;
	
	// Actualizar el efecto de la ultima accion
	switch (ultimaAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			if (!sensores.colision){
				switch (brujula){
					case 0: 
						if (estoy_bien_situado){
							fil--;
						}
						else{
							visitas_a_ciegas[filaux][colaux] = visita;
							filaux--;
						}
						break;
					case 1: 
						if (estoy_bien_situado){
							col++;
						}
						else{
	           				visitas_a_ciegas[filaux][colaux] = visita;
	           				colaux++;
						}
						break;
					case 2: 
						if (estoy_bien_situado){
							fil++;
						}
						else{ 
							visitas_a_ciegas[filaux][colaux] = visita;
							filaux++;
						}
						break;
					case 3: 
						if (estoy_bien_situado){
							col--;
						}
						else{ 
							visitas_a_ciegas[filaux][colaux] = visita;
							colaux--;
						}
						break;
				}
			}
			//cout << "fil: " << filaux << "  col: " << colaux << " Or: " << brujula << endl;
			break;
	}


  	// Capturar fila y columna en las que se encuentra el agente y cambiar a bien situado
  	if (sensores.mensajeF != -1 and !estoy_bien_situado){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		estoy_bien_situado = true;		
	}
	
	unsigned char contenidoCasilla; // Almacenará el tipo de terreno que tengo justo delante
	
	if (estoy_bien_situado){		
		// Antes de nada, pinto lo que estoy viendo
		PintaMapa(sensores);		
	
		//  Determinar si ha cambiado el destino desde la ultima planificacion
		if (hayPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
			cout << "El destino ha cambiado\n";
			hayPlan = false;
		}

		//  Determinar si tengo que construir un plan
		if (!hayPlan){
			estado origen;
			origen.fila = fil;
			origen.columna = col;
			origen.orientacion = brujula;

			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;

			estado orig;
			orig.fila = fil; orig.columna = col ; orig.orientacion = brujula;
	    	hayPlan = pathFinding(orig,destino,plan);
		}

		// Ejecutar el plan; Control del plan
		if (hayPlan and plan.size()>0){
			
			switch(brujula){
				case 0: contenidoCasilla = mapaResultado[fil-1][col]; break;
				case 1: contenidoCasilla = mapaResultado[fil][col+1]; break;
				case 2: contenidoCasilla = mapaResultado[fil+1][col]; break;
				case 3: contenidoCasilla = mapaResultado[fil][col-1]; break;
			}

			sigAccion = plan.front();

			if(sensores.superficie[2] == 'a'){
				plan.push_front(actIDLE);
			}
			else if (contenidoCasilla == 'A' or contenidoCasilla == 'B' or contenidoCasilla == 'P' 
				     or contenidoCasilla == 'M' or contenidoCasilla == 'D' ){
				if (sigAccion==0){
					hayPlan = false;
					sigAccion = actIDLE;
				}
				else{
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}				
			}
			else{
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}
		}
		else {
			hayPlan = false; 
			sigAccion = actIDLE;
		}
	}
	else{ // !estoy_bien_situado
		PintaACiegas(sensores);		

		MarcaIntransitables(sensores);		

		// Decido a qué casilla me dirijo
		sigAccion = ProximaAccion();
	}

	// Actualizar y enviar la acción elegida
	ultimaAccion = sigAccion;
	return sigAccion;
}


void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}