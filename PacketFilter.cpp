#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class PC : public cSimpleModule
{private:
    int udpnum;		// �������� ��� �� ������ UDP.
	int tcpnum;		// �������� ��� �� ������ TCP.
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
	int i=5;	// � ������� ��� ������� ��� �� ������� ��� ���� ����� ���� ��� ����� ��� ������.
	while (i>0)
	{
		if (index()==0)		// �������� ��� ��� ����� PC[0]
		{
			int prot = 1 + intrand(2003);	// ������ ������� ��� ��� ��������� ��� "�������".
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
		if (index()==1)		// �������� ��� ��� ����� PC[1]
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
		forwardMessage(msg,0);		// ���� �������� ��� ������� ��� �� PC[0] & PC[1].
	}
	if (index()==2)					// �� �������� ����������� ���� ����� PC[2].
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
            updateDisplay();		// �������� ���� �������� ���� ��� ��� �����.
	}
	if (index()==3)					// �� ������ ������ ���� ������ �����.
	{
		bubble("TCP ARRIVED");
		ev << " TCP Packet Arrived...";
	}
}

void PC::forwardMessage(cMessage *msg,int k)	// ��������� ��� ��� �������� ��� ������� (�� ��� ��� ����� ����������).
{												// ����� ��� ���������� k ��� ��� ��������� ��� �����.
    send(msg,"out",k);
}

void PC::updateDisplay()			// �������� ��� ������� ��� ������� ��� ������� ��� ��� �����.
{
    char buf[40];
    sprintf(buf, "TCP: %ld UDP: %ld", tcpnum, udpnum);
    displayString().setTagArg("t",0,buf);
}