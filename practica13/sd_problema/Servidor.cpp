#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include "./dependencies/Respuesta.h"

using namespace std;

int main(int argc, char const *argv[]) {

	Respuesta respuesta(atoi(argv[1]));
  	char confirmacion[] = "Nuevo registro almacenado";

	while(1) {
		struct mensaje msj;
    	struct mensaje m1;

		FILE *archivo = NULL;
	  	archivo = fopen(argv[2], "a+");
	  	if (archivo == NULL) {
	    	cout << "Server error: No se encontro el archivo " << argv[2] << endl;
	  	}

		cout << "\nEscuchando... " << endl;
		// Request info
		memcpy(&msj, respuesta.getRequest(), sizeof(struct mensaje));

		// cout <<"From:"<< msj.IP <<" Port:" <<msj.puerto<< " OpId:" << msj.operationId << endl;
  		// cout << "Data: " << msj.arguments << endl;

		switch(msj.operationId) {

			case 1: // Op:1:Guarda datos
				fputs(msj.arguments, archivo);
				fputs("\n", archivo);
				fclose(archivo);

				/*
					zona para solucion de errores de escritura
				*/

				// Prepara respuesta m1
		        m1.messageType = 1;
		        m1.puerto = msj.puerto;
		        m1.requestId = msj.requestId;
        		memcpy(m1.arguments, confirmacion, strlen(confirmacion)+1);
		        memcpy(m1.IP, msj.IP, 16);

		        // Envia respuesta
				respuesta.sendReply((char*) m1.arguments,m1.IP, msj.puerto);

				cout << ": Operacion terminada ---" << endl;
        		break;

			default:
				cout << "Server error: No existe operacion " << msj.operationId  << ", conexion ignorada."<< endl;
				
		}
	}


	return 0;
}
