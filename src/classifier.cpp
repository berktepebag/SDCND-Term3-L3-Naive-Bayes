#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include "classifier.h"
#include <assert.h>
#include <map>
#include <string>

/**
 * Initializes GNB
 */

using namespace std;

GNB::GNB() {

}

GNB::~GNB() {}

//Gathers labels and data into one map
struct MotionData{
	string direction;
	float s,d,s_d,d_d;


	MotionData(): direction(""), s(0.0), d(0.0), s_d(0.0), d_d(0.0){}
	//Constructor
	MotionData(string direction, float s, float d, float s_d, float d_d):
		direction(direction), s(s), d(d), s_d(s_d), d_d(d_d){}

	void printInfo(){

		cout << "Dir: " << direction << " s: " << s << " d: " << d
				<< " s_d: " << s_d << " d_d: " << d_d << endl;
	}

};

void GNB::train(vector<vector<double>> data, vector<string> labels)
{

	/*
		Trains the classifier with N data points and labels.

		INPUTS
		data - array of N observations
		  - Each observation is a tuple with 4 values: s, d,
		    s_dot and d_dot.
		  - Example : [
			  	[3.5, 0.1, 5.9, -0.02],
			  	[8.0, -0.3, 3.0, 2.2],
			  	...
		  	]

		labels - array of N labels
		  - Each label is one of "left", "keep", or "right".
	*/
	assert (data.size() == labels.size());

	vector< vector<double> > data_left;
	vector< vector<double> > data_keep;
	vector< vector<double> > data_right;

	map <int, MotionData> motionData;

	//Prepare the data according to labels
	for(int i=0; i < data.size(); i++){

	    string direction = labels[i];

	    double s = data[i][0];
        double d = data[i][1];
        double s_d = data[i][2];
        double d_d = data[i][3];

	    vector<double> row;

	    motionData[i] = MotionData(direction,s,d,s_d,d_d);

	    /*
        row.push_back(s);
        row.push_back(d);
        row.push_back(s_d);
        row.push_back(d_d);
	    if(direction == "left"){
	        data_left.push_back(row);
	    }
	    else if (direction == "right"){
	        data_right.push_back(row);
	    }
	    else{
	        data_keep.push_back(row);
	    }*/
	}

	for(map<int,MotionData>::iterator it = motionData.begin(); it != motionData.end(); it++){

		cout << it->first << ": " << flush;
		it->second.printInfo();
	}

	/*
	cout << "left size: " << data_left.size() << endl;
	cout << "keep size: " << data_keep.size() << endl;
	cout << "right size: " << data_right.size() << endl;*/

    double sum_s, sum_d, sum_sd, sum_dd;

    map<std::string, vector<double> > totals_by_label;
    map<std::string, int> example_map;

    example_map["bir"] = 1;

    //cout << example_map.find("bir") << endl;

    vector<double> sums;
	for(int i=0; i < data_left.size(); i++){

	    sum_s += data_left[i][0];
	    sum_d += data_left[i][1];
	    sum_sd += data_left[i][2];
	    sum_dd += data_left[i][3];
	    sums.push_back(sum_s);
	    sums.push_back(sum_d);
	    sums.push_back(sum_sd);
	    sums.push_back(sum_dd);
	}

	totals_by_label["left"] = sums;
	//string label = totals_by_label.find('left');
	//cout << "totals: " << label << endl;

	//cout << "left -> sum_s: "<< sum_s << " sum_d: "<< sum_d << "sum_sd: "<< sum_sd<< "sum_dd: "<< sum_dd<<endl;
    double mean_s, mean_d, mean_sd, mean_dd;

    vector<vector<double>> means;

    for(int i=0; i < data_left[0].size(); i++){
       vector<double> mean;

        mean_s = sum_s / data_left.size();
        mean_d = sum_d / data_left.size();
        mean_sd = sum_sd / data_left.size();
        mean_dd = sum_dd / data_left.size();

        mean.push_back(mean_s);
        mean.push_back(mean_d);
        mean.push_back(mean_sd);
        mean.push_back(mean_dd);
        means.push_back(mean);
    }

    for(int i=0; i<means[0].size(); i++){
        for(int j=0; j<means[i].size(); j++){
        cout << means[i][j];}
        cout << endl;
    }

	for(int i=0; i<labels.size(); i++){
	    //cout << labels[i] << endl;
	}

}

string GNB::predict(vector<double> sample)
{
	/*
		Once trained, this method is called and expected to return
		a predicted behavior for the given observation.

		INPUTS

		observation - a 4 tuple with s, d, s_dot, d_dot.
		  - Example: [3.5, 0.1, 8.5, -0.2]

		OUTPUT

		A label representing the best guess of the classifier. Can
		be one of "left", "keep" or "right".
		"""
		# TODO - complete this
	*/

	return this->possible_labels[1];

}
