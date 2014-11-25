#pragma once

#include "matrix.h"
#include <string>

using namespace std;

// single hidden layer BP neural network
class BPNN
{
public:
	BPNN(int inputNum, int hiddenNum, int outputNum);
	void loadWeights(string IWData, string LWData, string BiasData, string BiasData2);
	Matrix* simulate(Matrix **input);
	~BPNN(void);

private:
	void loadMatrix(ifstream& infile, Matrix **m);

	Matrix *IW, *LW, *B1, *B2;
	int inputNeurons, hiddenNeurons, outputNeurons;
};

