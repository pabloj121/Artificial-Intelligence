#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <cstring>

struct estado {
  int fila;
  int columna;
  int orientacion;

  estado():fila(0), columna(0),orientacion(0){}
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = filaux= colaux = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      visita = 0;
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      estoy_bien_situado = false;
      hayPlan = false;

      memset(mapaResultadoaux, '?', sizeof(mapaResultadoaux));
      memset(visitas_a_ciegas, 0, sizeof(visitas_a_ciegas));
    }

    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = filaux = colaux= 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      visita = 0;
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      estoy_bien_situado = false;
      hayPlan = false;
      memset(mapaResultadoaux, '?', sizeof(mapaResultadoaux));
      memset(visitas_a_ciegas, 0, sizeof(visitas_a_ciegas));
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    int distanciaManhattan(const estado &e, const estado &f);
    estado costeMinimo(const estado &origen, const estado &fin);
    std::vector<pair<estado, list<Action> > > nodosAdyacentes(pair<estado,list<Action> > &origen);
    void PintaMapa(Sensores sensores);
    void MarcaIntransitables(Sensores sensores);
    void PintaACiegas(Sensores sensores);
    Action Minimo(int a, int b, int c, int d);
    void MarcaK(unsigned char &a, unsigned char &b, unsigned char &c, unsigned char &d,
                                    int &pos1, int &pos2, int &pos3, int &pos4);
    Action ProximaAccion();
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, filaux, colaux, brujula;
    estado destino;
    list<Action> plan;

    // Nuevas Variables de Estado
    Action ultimaAccion;
    bool hayPlan;
    bool estoy_bien_situado;
    int visitas_a_ciegas[200][200];
    unsigned char mapaResultadoaux[200][200];
    int visita;

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
};

#endif
