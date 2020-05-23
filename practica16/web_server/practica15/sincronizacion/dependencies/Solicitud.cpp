#include "SocketDatagrama.h"
#include "Solicitud.h"
#include "mensaje.h"
#include <vector>
#include <string>

Solicitud::Solicitud(struct timeval timeout) {
	timeoutSocket = timeout;
	socketlocal = new SocketDatagrama(0,timeout);
}

char * Solicitud::doOperation(char* IP, int puerto, int operationId, vector<string> registros ) {
// char * Solicitud::doOperation(char* IP, int puerto, int operationId, char* arguments) {
	struct mensaje msj;
	char* resultado;
	int res;
	char aux[100];
	unsigned int id; 
	// unsigned int registros = atoi(arguments);
	
	// cout << "Votes to send: " << registros << endl;
	FILE *f = fopen("votes.txt", "r+");
	if (f==NULL) {
		cout << "Client error, no such file or directory" << endl;
		exit(-1);
	}

	id = 0;
	//for (int i = 0; i<len(registos); i++)
	for(auto vote: registros){

		char arguments[vote.size()+1];
		strcpy(arguments, vote.c_str());

		msj.messageType = 0;
		msj.requestId = id;
		memcpy(msj.IP, IP, 16);
		msj.puerto = puerto;
		msj.operationId = operationId;
		// msj.arguments = arguments;
		// fgets(msj.arguments, 100, f);

		for (int i = 0; i < 4000; i++) {
			msj.arguments[i] = arguments[i];

		}
		
		// cout<<"Arguments:" << msj.arguments;

		PaqueteDatagrama paq((char*) &msj, sizeof(msj), IP, puerto);
		socketlocal->envia(paq);
		PaqueteDatagrama paq1(sizeof(msj));
		res = socketlocal->recibeTimeout(paq1,timeoutSocket.tv_sec,timeoutSocket.tv_usec);
		if(res>=0){
			resultado  = paq1.obtieneDatos();
			// cout << resultado << endl;
			id++;
		} else {
			// cout << "Server error, fowarding message..." << endl;
			while (1) {
				PaqueteDatagrama paqReenvio((char*)&msj, sizeof(msj), IP, puerto);
				socketlocal->envia(paqReenvio);
				PaqueteDatagrama acuse(sizeof(msj));
				res = socketlocal->recibeTimeout(acuse,timeoutSocket.tv_sec,timeoutSocket.tv_usec);
				if(res >0 ){
					// cout << "Foward: " << acuse.obtieneDatos() << endl;
					break;
				}
			}
			id++;
		}
		//cout<<"Resultado: " <<res <<endl;
		/*if(res>=0){
			resultado = paq1.obtieneDatos();
			break;
		}*/
	}
	fclose(f);


	return resultado;
}
