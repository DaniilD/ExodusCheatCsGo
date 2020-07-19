#pragma once
#include <string>
#include "Subscription.h"

using namespace std;
class User {
private:
	int id, status;
	string name, email;
	
public:
	Subscription* subscription;
	//geters and seters
	void setId(int id) {
		this->id = id;
	}

	void setStatus(int status) {
		this->status = status;
	}

	void setName(string name) {
		this->name = name;
	}

	void setEmail(string email) {
		this->email = email;
	}

	int getId() {
		return this->id;
	}

	int getStatus() {
		return this->status;
	}

	string getName() {
		return this->name;
	}

	string getEmail() {
		return this->email;
	}

	void setSubscription(int id, string dateTo, string dateFrom) {
		this->subscription = new Subscription();
		this->subscription->setId(id);
		this->subscription->setDateBegining(dateTo);
		this->subscription->setDateExpiration(dateFrom);
	}

	

};
