#include "Common.h"

// Convert JSON to INT
int jsonToInteger(json j) {
	string j_str = j;
	int i = stoi(j_str);
	return i;
}

struct Transaction
{
	string timestamp;
	int record_id;
	float amount;
};

map<string, set<string> >social_network;
map<string, vector<Transaction> >purchase_history;
map<int, float>record_amount;

void addPurchase(string id, string timestamp, int record_id, float amount) {
	Transaction transac;
	transac.timestamp = timestamp;
	transac.record_id = record_id;
	transac.amount = amount;

	purchase_history[id].push_back(transac);
}

set<string> searchNeighbors(string id, int D) {
	int layer = 1;
	queue<string> q;
	q.push(id);

	set<string>friend_set;
	while (!q.empty()) {
		string cur_id = q.front();
		q.pop();
		for (set<string>::iterator it = social_network[cur_id].begin();
			it != social_network[cur_id].end(); it++) {
			friend_set.insert(*it);
			q.push(*it);
		}
		++layer;
		if (layer > D)
			break;
	}

	return friend_set;
}


bool flagAnomaly(float amount, set<string>& friend_set, int T, float& mean, float& sd) {
	vector<int> friend_record_vec;
	vector<float> friend_amount_vec;

	for (set<string>::iterator frd_it = friend_set.begin(); frd_it != friend_set.end(); frd_it++) {
		for (vector<Transaction>::iterator transac_it = purchase_history[*frd_it].begin();
			transac_it != purchase_history[*frd_it].end(); transac_it++) {
			Transaction transac = *transac_it;

			friend_record_vec.push_back(transac.record_id);
		}
	}

	sort(friend_record_vec.begin(), friend_record_vec.end());
	if (friend_record_vec.size() <= T) {
		for (int i = 0; i < friend_record_vec.size(); i++) {
			int friend_record = friend_record_vec[i];
			float friend_amount = record_amount[friend_record];
			friend_amount_vec.push_back(friend_amount);
			mean += friend_amount;
		}
	}
	else {
		for (int i = 0; i < T; i++) {
			int friend_record = friend_record_vec[i];
			float friend_amount = record_amount[friend_record];
			friend_amount_vec.push_back(friend_amount);
			mean += friend_amount;
		}
	}
	mean /= friend_amount_vec.size();

	float sum_of_squares = 0.0;
	for (vector<float>::iterator vec_it = friend_amount_vec.begin();
		vec_it != friend_amount_vec.end(); vec_it++) {
		sum_of_squares += (*vec_it - mean) * (*vec_it - mean);
	}
	sd = sqrt(sum_of_squares / friend_amount_vec.size());

	if (amount > mean + 3 * sd)
		return true;

	return false;
}