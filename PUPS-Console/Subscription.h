#pragma once
#include <string>

using namespace std;

class Subscription {
private:
	int id;
	string date_begining;
	string date_expiration;
public:
	void setId(int id) {
		this->id = id;
	}
	void setDateBegining(string date_begining) {
		this->date_begining = date_begining;
	}

	void setDateExpiration(string date_expiration) {
		this->date_expiration = date_expiration;
	}

	int getId() {
		return this->id;
	}

	string getDateBegining() {
		return this->date_begining;
	}

	string getDateExpiration() {
		return this->date_expiration;
	}

};
