# CrazyBuyer

This program is for Insight data engineer challenge of "Anomaly Detection".

More details about the problem set up please refer to: https://github.com/InsightDataScience/anomaly_detection.

Any of the detected "anomaly" who purchases much more than the average of his/her friends, is really a "CrazyBuyer" :)

## Summary of approach
### Data structure
At the very beginning I globalize the following variables:

1. map<string, set\<string\> >social_network;

- In the log file, each id is considered as a string type, and can connect to multiple other ids to form a social network.

- Therefore here I use a C++ STL map<string, set\<string\> > structure to store social network information.

2. map<string, vector\<Transaction\> >purchase_history;

- First I construct a struct named "Transaction", for each id in the log file, including three memberships: timestamp (what time this id make the purchase), record_id (the order of the purchases I defined by myself, from the earliest one to the latest one, to differentiate transactions with the same timestamps. Since I assume the batch log file is already ordered by timestamp, i.e., from the earliest one to the latest, so here the record_id would be just from 1,2,3,..., to the total number of lines), and the amount of the purchase.

        struct Transaction
     
        {
  
          string timestamp;
	
	      int record_id;
	
	      float amount;
	
        };

- Since each id can make multiple transactions, so here I use a C++ STL map<string, vector\<Transaction\> > structure to store for each id, its historical purchase information.


3. map<int, float>record_amount;


The step-by-step implementations of CrazyBuyer is as follows:
Step 0: At the very beginning initialize
Step 1: Read in the "batch_log.json":
-- The first line is "D" and "T"
-- For each of other lines,
---- if "event_type" is "befriend", then 

## Algorithm



## Functions of files in the "src" folder

-- main.cpp

-- utils.cpp

-- json.hpp

-- Common.h

-- CrazyBuyer

-- Makefile


1. Read in batch log data:





## Acknowledgement

Many thanks to the powerful "JSON for Modern C++" library: https://github.com/nlohmann/json, whose whole code consists of a single header file "json.hpp", which I cloned and added to my source code folders.
