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
	left_prior = 0;
	right_prior = 0;
	keep_prior = 0;

	left_mean = vector<double>(4,0.0);
	left_sigma = vector<double> (4,0.0);

	right_mean = vector<double> (4,0.0);
	right_sigma = vector<double> (4,0.0);

	keep_mean = vector<double> (4,0.0);
	keep_sigma = vector<double> (4,0.0);
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

string label_names[] = {"left","right","keep"};

void GNB::train(vector<vector<double>> data, vector<string> labels)
{	/*
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
	for(unsigned int i=0; i < data.size(); i++){

		string direction = labels[i];

		double s = data[i][0];
		double d = data[i][1];
		double s_d = data[i][2];
		double d_d = data[i][3];

		vector<double> row;

		//Save to the map
		motionData[i] = MotionData(direction,s,d,s_d,d_d);
	}

	map<string, vector<double> > totals_by_label;

	vector< vector<double> > total_data(3,vector<double>(5,0.0));
	vector< vector<double> > mean_data(3,vector<double>(5,0.0));
	vector< vector<double> > sigma2_data(3,vector<double>(5,0.0));

	int total_labels;

	//Sum of the s,d,s_d and d_d for calculating mean for left,right,keep
	for(map<int,MotionData>::iterator it = motionData.begin(); it != motionData.end(); it++){

		if(false){
			cout << it->first << ": " << flush;
			it->second.printInfo();}

		for(unsigned int i = 0; i < sizeof(label_names)/sizeof(string); i++){
			int s= it->second.s;
			int d= it->second.d;
			int s_d = it->second.s_d;
			int d_d = it->second.d_d;


			if(it->second.direction == "left"){
				total_data[0][0] += s;
				total_data[0][1] += d;
				total_data[0][2] += s_d;
				total_data[0][3] += d_d;
				total_data[0][4] ++;
			}
			else if(it->second.direction == "right"){
				total_data[1][0] += s;
				total_data[1][1] += d;
				total_data[1][2] += s_d;
				total_data[1][3] += d_d;
				total_data[1][4] ++;
			}else if(it->second.direction == "keep"){
				total_data[2][0] += s;
				total_data[2][1] += d;
				total_data[2][2] += s_d;
				total_data[2][3] += d_d;
				total_data[2][4] ++;
			}else
			{
				cout << "Some wrong data here we have, huh?" << endl;
			}
		}

		total_labels = total_data[0][4]+total_data[1][4]+total_data[2][4];
	}

	//cout << "s: " << flush <<"d: " << flush <<"s_d: " << flush <<"d_d: " << "total count:" << endl;
	for(int i=0; i<3; i++){
		//cout << " " << label_names[i] << " : " << endl;
		for(int j=0; j<5; j++){
			//cout << total_data[i][j] << " , " << flush;

			//******************//
			//**Calculate Mean**//
			//*****************//
			mean_data[i][j]= total_data[i][j] / total_data[i][4];
		}

		cout << endl;
		//Print Mean
		if(false){
			for(int j=0; j<5; j++){
				cout << mean_data[i][j] << " , " << flush;
			}
			cout << endl;
		}
	}

	//Calculate Variance
	for(map<int,MotionData>::iterator it = motionData.begin(); it != motionData.end(); it++){

		if(false){
			cout << it->first << ": " << flush;
			it->second.printInfo();}

		for(unsigned int i = 0; i < sizeof(label_names)/sizeof(string); i++){
			int s= it->second.s;
			int d= it->second.d;
			int s_d = it->second.s_d;
			int d_d = it->second.d_d;

			if(it->second.direction == "left"){
				sigma2_data[0][0] += pow((s-mean_data[0][0]),2);
				sigma2_data[0][1] += pow((d-mean_data[0][1]),2);
				sigma2_data[0][2] += pow((s_d-mean_data[0][2]),2);
				sigma2_data[0][3] += pow((d_d-mean_data[0][3]),2);
			}
			else if(it->second.direction == "right"){
				sigma2_data[1][0] += pow((s-mean_data[1][0]),2);
				sigma2_data[1][1] += pow((d-mean_data[1][1]),2);
				sigma2_data[1][2] += pow((s_d-mean_data[1][2]),2);
				sigma2_data[1][3] += pow((d_d-mean_data[1][3]),2);
			}else if(it->second.direction == "keep"){
				sigma2_data[2][0] += pow((s-mean_data[2][0]),2);
				sigma2_data[2][1] += pow((d-mean_data[2][1]),2);
				sigma2_data[2][2] += pow((s_d-mean_data[2][2]),2);
				sigma2_data[2][3] += pow((d_d-mean_data[2][3]),2);
			}else
			{
				cout << "Some wrong data here we have, huh?" << endl;
			}
		}
	}

	//Calculate Sigma
	for(int i=0; i<3 ; i++){
		for(int j=0; j<4; j++){
			sigma2_data[i][j] = sqrt(sigma2_data[i][j]/total_data[i][4]);

			if(i==0) left_sigma[j] =  sigma2_data[i][j];
			if(i==1) right_sigma[j] =  sigma2_data[i][j];
			if(i==2) keep_sigma[j] =  sigma2_data[i][j];
		}

		//Print sigma
		if(false){
			cout << "Sigma: " << endl;
			cout << "s: " << flush <<"d: " << flush <<"s_d: " << flush <<"d_d: " << endl;
			for(int i=0; i<3; i++){
				cout << " " << label_names[i] << " : " << endl;
				for(int j=0; j<4; j++){
					cout << sigma2_data[i][j] << " , " << flush;
				}
				cout << endl;
			}
		}
	}

	//Probability of each label
	cout << "Probability of each label" << endl;
	left_prior = total_data[0][4]/total_labels;
	right_prior = total_data[1][4]/total_labels;
	keep_prior = total_data[2][4]/total_labels;

	cout << "total prob: " << (left_prior+right_prior+keep_prior)*100 << endl;

	//*********************************//
	//*****Copy mean data to array****//
	for(int i =0; i<4; i++){
		left_mean[i] =  mean_data[0][i];
		right_mean[i] =  mean_data[1][i];
		keep_mean[i] =  mean_data[2][i];
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

	double prob_left= 1.0;
	double prob_right= 1.0;
	double prob_keep= 1.0;

	//cout << "prob_left: " << prob_left << endl;

	cout <<  "//***********New Value**************//" << endl;

	for(int i=0; i<4; i++){

		cout << "right_sigma["<<i<<"]: " << right_sigma[i] << flush;
		cout << "right_mean["<<i<<"]: " << right_mean[i] << flush;
		cout << "sample["<<i<<"]: " << sample[i] << flush;

		prob_left *=  (1.0 / sqrt(2.0* M_PI* pow(left_sigma[i],2 ))) * exp(-0.5 * pow(sample[i]-left_mean[i],2)  / pow(left_sigma[i],2));
		prob_right *= (1.0 / sqrt(2.0* M_PI* pow(right_sigma[i],2))) * exp(-0.5 * pow(sample[i]-right_mean[i],2) / pow(right_sigma[i],2));
		prob_keep *=  (1.0 / sqrt(2.0* M_PI* pow(keep_sigma[i],2 ))) * exp(-0.5 * pow(sample[i]-keep_mean[i],2)  / pow(keep_sigma[i],2));
	    //left_p *= (1.0/sqrt(2.0 * M_PI * pow(left_sds[i], 2))) * exp(-0.5*pow(sample[i] - left_means[i], 2)/pow(left_sds[i], 2));

		cout << endl;
		cout << "prob_left 1: " << prob_left << endl;
		cout << "prob_right 1: " << prob_right << endl;
		cout << "prob_keep 1: " << prob_keep << endl;
	}


	prob_left *= left_prior;
	prob_right *= right_prior;
	prob_keep *= keep_prior;

	cout << "prob_left 2: " << prob_left << endl;
	cout << "prob_right 2: " << prob_right << endl;
	cout << "prob_keep 2: " << prob_keep << endl;

	double probs[3] = {prob_left, prob_right, prob_keep};
	double max = prob_left;
	double max_index = 0;
	for (int i=0; i<3; i++){
		if (probs[i] > max) {
			max = probs[i];
			max_index = i;
		}
	}

	cout << "turning: " << max_index << endl;
	return this->possible_labels[max_index];

}
