#ifndef __UDP_RECEIVER__
#define __UDP_RECEIVER__

#include "cThread.h"
#include "UDPDataSets.h"

/**
 * CLASS UDPReceiver
 * 
 * Erstellt eien Empfaenger, der UDP-Daten auf einem Port empfaengt
 * 
 * Starten des Threads mit start()
 * 
 */
class UDPReceiver: public cThread {

// Attribute

public:
	/* Empfangsport */
	Port port;
	/* Wait-Variable */
	bool done;
	bool kill;
	
private:
	/* Gebundener Socket */
	SOCKET sok;
	/* Pointer zum Zielort der Daten */
	void* data;
	/* Laenge der Daten */
	unsigned int bytes;

// Operationen
public:
	UDPReceiver();
	virtual ~UDPReceiver();
	/* Empfaenger initialisieren (Daten, laenge, empfangs-Port) */
	bool initReceiver(void* data, unsigned int bytes, Port port);
	
private:
	/* Thread starten */ 
	virtual void run();
	/* Daten Empfangen */
	void ReceiveData();

};

#endif
