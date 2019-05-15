#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)

#include <set>
#include "Telegram.h"
#include "Entity.h"
#include "MessageTypes.h"
#include "EntityNames.h"
#include "EntityManager.h"
#include <iostream>
#include <time.h>
#include <Windows.h>

using std::cout;

using std::set;



class Entity;

//to make code easier to read
const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO = 0;

//to make life easier...
#define Dispatch MessageDispatcher::Instance()


class MessageDispatcher
{
private:

	//a std::set is used as the container for the delayed messages
	//because of the benefit of automatic sorting and avoidance
	//of duplicates. Messages are sorted by their dispatch time.
	std::set<Telegram> PriorityQ;

	//this method is utilized by DispatchMessage or DispatchDelayedMessages.
	//This method calls the message handling member function of the receiving
	//entity, pReceiver, with the newly created telegram
	void Discharge(Entity* pReceiver, const Telegram& msg);

	MessageDispatcher() {}

	//copy ctor and assignment should be private
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);

public:

	//this class is a singleton
	static MessageDispatcher* Instance();

	//send a message to another agent. Receiving agent is referenced by ID.
	void DispatchMessage(double  delay,
		int    sender,
		int    receiver,
		int    msg);

	//send out any delayed messages. This method is called each time through   
	//the main game loop.
	void DispatchDelayedMessages();

};


//------------------------------ Instance -------------------------------------

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}

void MessageDispatcher::Discharge(Entity* pReceiver,
	const Telegram& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		//telegram could not be handled
		cout << "Message not handled";
	}
}


void MessageDispatcher::DispatchMessage(double  delay,
	int    sender,
	int    receiver,
	int    msg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	//get pointers to the sender and receiver
	Entity* pSender = EntityMgr->GetEntityFromID(sender);
	Entity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//make sure the receiver is valid
	if (pReceiver == NULL)
	{
		cout << "\nWarning! No Receiver with ID of " << receiver << " found";

		return;
	}

	//create the telegram
	Telegram telegram(0, sender, receiver, msg);

	//if there is no delay, route telegram immediately                       
	if (delay <= 0.0f)
	{
		cout << "\nInstant telegram dispatched at time: " << clock()
			<< " by " << GetNameOfEntity(pSender->ID()) << " for " << GetNameOfEntity(pReceiver->ID())
			<< ". Msg is " << MsgToStr(msg) << endl;

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);

	}

	//else calculate the time when the telegram should be dispatched
	else
	{
		double CurrentTime = clock();

		telegram.DispatchTime = CurrentTime + delay;

		//and put it in the queue
		PriorityQ.insert(telegram);

		cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time "
			<< clock() << " for " << GetNameOfEntity(pReceiver->ID())
			<< ". Msg is " << MsgToStr(msg) << endl;

	}
}


void MessageDispatcher::DispatchDelayedMessages()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	//get current time
	double CurrentTime = clock();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while (!PriorityQ.empty() &&
		(PriorityQ.begin()->DispatchTime < CurrentTime) &&
		(PriorityQ.begin()->DispatchTime > 0))
	{
		//read the telegram from the front of the queue
		const Telegram& telegram = *PriorityQ.begin();

		//find the recipient
		Entity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

		cout << "\nQueued telegram ready for dispatch: Sent to "
			<< GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(telegram.Msg) << endl;

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}
#endif