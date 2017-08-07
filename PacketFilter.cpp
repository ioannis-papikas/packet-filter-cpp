#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class PC : public cSimpleModule
{private:
    int udpnum;		// Μετρητής για τα πακέτα UDP.
	int tcpnum;		// Μετρητής για τα πακέτα TCP.
  protected:
    virtual void forwardMessage(cMessage *msg,int);
    virtual void handleMessage(cMessage *msg);
	virtual void initialize();
	virtual void updateDisplay();
};

Define_Module(PC);

void PC::initialize()
{
	udpnum = 0;
	tcpnum = 0;
	int i=5;	// Ο Αριθμός των πακέτων που θα σταλούν απο κάθε κόμβο προς τον κομβο για έλεγχο.
	while (i>0)
	{
		if (index()==0)		// Αποστολή απο τον κόμβο PC[0]
		{
			int prot = 1 + intrand(2003);	// Τυχαία επιλογή για τον καθορισμό του "πακέτου".
			if (prot%2==0)
			{
				cMessage *pck = new cMessage("TCP",6);//,0,0,0);
				scheduleAt(0.0, pck);
			}
			else
			{
				cMessage *pck = new cMessage("UDP",17);//,0,0,0);
				scheduleAt(0.0, pck);
			}
		}
		if (index()==1)		// Αποστολή απο τον κόμβο PC[1]
		{
			int prot = 1 + intrand(500);
			if (prot%2==0)
			{
				cMessage *pck = new cMessage("TCP",6);//,0,0,0);
				scheduleAt(0.0, pck);
			}
			else
			{
				cMessage *pck = new cMessage("TCP",17);//,0,0,0);
				scheduleAt(0.0, pck);
			}
		}
		i--;
	}
}

void PC::handleMessage(cMessage *msg)
{
	if (index()==0 || index()==1)
	{
		forwardMessage(msg,0);		// Απλή προώθηση του πακέτου για τα PC[0] & PC[1].
	}
	if (index()==2)					// Το λεγόμενο φιλτράρισμα στον κόμβο PC[2].
	{
		int k = msg->kind();
		if (k==6)
		{
			ev<<"TCP packet arrived, forwading packet..."<<endl;
			tcpnum++;
			forwardMessage(msg,2);
		}
		else if (k==17)
		{
			ev<<"UDP packet arrived, deleting packet..."<<endl;
			udpnum++;
			delete msg;
		}
		if (ev.isGUI())
            updateDisplay();		// Εμφάνιση ενός κειμένου πάνω απο τον κόμβο.
	}
	if (index()==3)					// Το πακέτο φτάνει στον τελικό κόμβο.
	{
		bubble("TCP ARRIVED");
		ev << " TCP Packet Arrived...";
	}
}

void PC::forwardMessage(cMessage *msg,int k)	// Συνάρτηση για την προώθηση του πακέτου (αν και δεν είναι απαραίτητη).
{												// Χρήση της μεταβλητής k για τον καθορισμό της πύλης.
    send(msg,"out",k);
}

void PC::updateDisplay()			// Εμφάνιση του αριθμού των πακέτων που περνάνε απο τον κόμβο.
{
    char buf[40];
    sprintf(buf, "TCP: %ld UDP: %ld", tcpnum, udpnum);
    displayString().setTagArg("t",0,buf);
}