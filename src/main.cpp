// main.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "Common.h"
#include "utils.cpp"

int main(int argc, char* argv[])
{
	char* batch_filename = argv[1];
	char* stream_filename = argv[2];
	char* output_filename = argv[3];

	// 1. initializeSocialNetwork
	ifstream batch_file(batch_filename);

	// 1.1 Parse first line and get D and T
	string first_line;
	getline(batch_file, first_line);
	if (first_line.empty()) {
		cerr << "Empty batch log file!" << endl;
	}

	json j;
	istringstream stream(first_line);
	stream >> j;
	int D = jsonToInteger(j["D"]);
	int T = jsonToInteger(j["T"]);

	// 1.2 Build network (from the second line)
	string batch_line;
	int record_id = 0;
	while (getline(batch_file, batch_line)) {
		if (batch_line.empty())
			break;

		++record_id;
		istringstream stream(batch_line);
		json j;
		stream >> j;
		string event_type = j["event_type"];
		
		if (!event_type.compare("befriend")) {
			string id1 = j["id1"];
			string id2 = j["id2"];
			social_network[id1].insert(id2);
			social_network[id2].insert(id1);
		}
		else if (!event_type.compare("unfriend")) {
			string id1 = j["id1"];
			string id2 = j["id2"];

			// delete id2 from id1's friend set
			set<string>::iterator it = social_network[id1].begin();
			it = social_network[id1].find(id2);
			if (it != social_network[id1].end()) {
				social_network[id1].erase(it);
			}
			// delete id1 from id2's friend set
			it = social_network[id2].begin();
			it = social_network[id2].find(id1);
			if (it != social_network[id2].end()) {
				social_network[id2].erase(it);
			}
		}
		else if (!event_type.compare("purchase")) {
			string id = j["id"];
			string timestamp = j["timestamp"];
			string amount_str = j["amount"];
			float amount = stof(amount_str);
			record_amount[record_id] = amount;
			addPurchase(id, timestamp, record_id, amount);
		}
		else {
			cerr << "Wrong Event_Type: None of befriend, unfriend, or purchase! Check!";
		}

	}

	// 2. readStreamLog
	ifstream stream_file(stream_filename);
	ofstream of(output_filename);
	string stream_line;
	while (getline(stream_file, stream_line)) {
		if (stream_line.empty())
			break;
		++record_id;
		istringstream stream(stream_line);
		json j;
		stream >> j;

		string event_type = j["event_type"];

		if (!event_type.compare("befriend")) {
			string id1 = j["id1"];
			string id2 = j["id2"];
			social_network[id1].insert(id2);
			social_network[id2].insert(id1);
		}
		else if (!event_type.compare("unfriend")) {
			string id1 = j["id1"];
			string id2 = j["id2"];

			// delete id2 from id1's friend set
			set<string>::iterator it = social_network[id1].begin();
			it = social_network[id1].find(id2);
			if (it != social_network[id1].end()) {
				social_network[id1].erase(it);
			}
			// delete id1 from id2's friend set
			it = social_network[id2].begin();
			it = social_network[id2].find(id1);
			if (it != social_network[id2].end()) {
				social_network[id2].erase(it);
			}
		}
		else if (!event_type.compare("purchase")) {
			string id = j["id"];
			string timestamp = j["timestamp"];
			string amount_str = j["amount"];
			float amount = stof(amount_str);
			
			// Search D-layer neighbors of current id
			set<string>friend_set = searchNeighbors(id, D);

			// flagAnomaly
			float mean = 0.0, sd = 0.0;
			bool flag = flagAnomaly(amount, friend_set, T, mean, sd);

			if (flag) {
				ofstream of(output_filename, ios::app);
				//of << id << "\t" << amount << "\t" << mean << "\t" << sd << endl;
				of << "{\"event_type\":\"purchase\", \"timestamp\":\""
					<< timestamp << "\", \"id\": \""
					<< id << "\", \"amount\": \""
					<< amount_str << "\", \"mean\": \""
					<< fixed << setprecision(2) << (int)(mean * 100) / 100.0 << "\", \"sd\": \""
					<< fixed << setprecision(2) << (int)(sd * 100) / 100.0 << "\"}";
				if(!stream_file.eof())
					of << endl;
			}

			// update purchase_history
			addPurchase(id, timestamp, record_id, amount);
			record_amount[record_id] = amount;
		}
		else {
			cerr << "Wrong Event_Type: None of befriend, unfriend, or purchase! Check!";
		}
	}
	
    return 0;
}


