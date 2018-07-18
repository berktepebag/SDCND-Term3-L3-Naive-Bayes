#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

class GNB {
public:

	vector<string> possible_labels = {"left","keep","right"};
	double left_prior;
	double right_prior;
	double keep_prior;

	vector<double> left_mean;
	vector<double> left_sigma;

	vector<double> right_mean;
	vector<double> right_sigma;

	vector<double> keep_mean;
	vector<double> keep_sigma;


	/**
  	* Constructor
  	*/
 	GNB();

	/**
 	* Destructor
 	*/
 	virtual ~GNB();

 	void train(vector<vector<double> > data, vector<string>  labels);

  	string predict(vector<double>);

};

#endif



