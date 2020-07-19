#pragma once 
#include <iostream>
#include <string>
#include <windows.h>

#define CURL_STATICLIB
#ifdef _DEBUG
#pragma comment (lib, "libcurl_a_debug.lib")
#else
#pragma comment (lib, "libcurl_a.lib")
#endif

#pragma comment(lib,"normaliz.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"advapi32.lib")

#include <rapidjson\document.h>
#include <curl\curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "User.h"

using namespace std;
using namespace rapidjson;

class Authorization {
	private:
		string host = "http://exodus-cheat.ru/";
		string prefix = "api/v1";
		string login = "";
		string password = "";
		string error = "";
		User* user;

		bool isVerified() {
			if (this->login == "" || this->password == "") {
				this->error = "Fill in the Email and password field";
				return false;
			} 

			return true;
		}

		void displayError() {
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h, FOREGROUND_RED);
			cout << this->error << endl;
			SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
		}

		static size_t write_data(char* ptr, size_t size, size_t nmemb, std::string* data)
		{
			if (data)
			{
				data->append(ptr, size * nmemb);
				return size * nmemb;
			}
			else return 0;  // будет ошибка
		}

		string escape(CURL* curl_handle, const std::string& text)
		{
			std::string result;
			char* esc_text = curl_easy_escape(curl_handle, text.c_str(), text.length());
			if (!esc_text) throw std::runtime_error("Can not convert string to URL");

			result = esc_text;
			curl_free(esc_text);

			return result;
			
		}

		bool queryAuth() {
			string queryUrl = host + prefix + "/user/auth";

			 curl_global_init(CURL_GLOBAL_ALL);
			 CURL* curl_handle = curl_easy_init();
		

			if (curl_handle) {
				string postData = "email=" + escape(curl_handle, this->login);
				postData += "&password=" + escape(curl_handle, this->password);
				//cout << queryUrl+postData.c_str() << endl;
				string content ="";


				curl_easy_setopt(curl_handle, CURLOPT_URL, "http://exodus-cheat.ru/api/v1/user/auth");
				curl_easy_setopt(curl_handle, CURLOPT_POST, true);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);
				curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData.c_str());
				curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, postData.length());

				CURLcode res = curl_easy_perform(curl_handle);

				if (res != CURLE_OK) {
					/*std::cout << curl_easy_strerror(res) << std::endl;
					cout << content << endl;*/
					curl_easy_cleanup(curl_handle);
					this->error = "Error";
					return false;
				}

				//cout << content << endl;

				Document document;
				document.Parse(content.c_str());
				string message =  document["message"].GetString();

				if (message != "OK") {
					this->error = message;
					return false;
				}
				

				string email = document["data"]["email"].GetString();
				int status = atoi(document["data"]["status"].GetString());
				int id = document["data"]["id"].GetInt();
				string name = document["data"]["name"].GetString();
				int idSub = document["data"]["id"].GetInt();
				string dateTo = document["data"]["dateTo"].GetString();
				string dateFrom = document["data"]["dateFrom"].GetString();				
				curl_easy_cleanup(curl_handle);
				this->user = new User();
				this->user->setId(id);
				this->user->setEmail(email);
				this->user->setName(name);
				this->user->setStatus(status);
				this->user->setSubscription(idSub, dateTo, dateFrom);
				//cout << nameDecode << endl;
				return true;
			}

		}


	public:
		Authorization(string login, string password) {
			this->login = login;
			this->password = password;
		}

		Authorization() {

		}

		void setLogin(string login) {
			this->login = login;
		}

		void setPassword(string password) {
			this->password = password;
		}

		User* getUser() {
			return this->user;
		}

		bool isAuth() {
			if (!isVerified()) {
				displayError();
				return false;
			}

			if (!queryAuth()) {
				displayError();
				return false;
			}

			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h, FOREGROUND_GREEN);
			cout << "Auth is OK" << endl;
			SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);

			return true;
				
		}

};