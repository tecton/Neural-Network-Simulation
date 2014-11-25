#include "BPNN.h"
#include <fstream>
#include <iostream>

BPNN::BPNN(int inputNum, int hiddenNum, int outputNum)
	:inputNeurons(inputNum), hiddenNeurons(hiddenNum), outputNeurons(outputNum)
{
	IW = MatrixAlloc(hiddenNeurons, inputNeurons);
	LW = MatrixAlloc(1, hiddenNeurons);
	B1 = MatrixAlloc(hiddenNeurons, 1);
	B2 = MatrixAlloc(outputNeurons, 1);
}

BPNN::~BPNN(void)
{
	MatrixFree(IW);
	MatrixFree(LW);
	MatrixFree(B1);
	MatrixFree(B2);
}

void BPNN::loadWeights(string IWData, string LWData, string BiasData, string BiasData2)
{
	ifstream infile;
	string data;

	infile.open(IWData);
	loadMatrix(infile, &IW);
	infile.close();

	infile.open(LWData);
	loadMatrix(infile, &LW);
	infile.close();

	infile.open(BiasData);
	loadMatrix(infile, &B1);
	infile.close();

	infile.open(BiasData2);
	loadMatrix(infile, &B2);
	infile.close();

	cout << "load successful.\n";
}

void BPNN::loadMatrix(ifstream& infile, Matrix **m)
{
	string line;
	size_t pos = 0;
	string token;
	string delimiter = ",";
	int i = 0, j = 0;

	while (infile >> line)
	{
		while ((pos = line.find(delimiter)) != string::npos)
		{
			token = line.substr(0, pos);
			M_ELEMENT(*m, i, j) = atof(token.c_str());
			line.erase(0, pos + delimiter.length());
			j++;
		}
		M_ELEMENT(*m, i, j) = atof(line.c_str());
		i++;
		j = 0;
	}
}

Matrix* BPNN::simulate(Matrix **input)
{
	Matrix *y1 = MatrixAlloc(hiddenNeurons, 1);
	Matrix *y1a = MatrixAlloc(hiddenNeurons, 1);
	Matrix *y1t = MatrixAlloc(hiddenNeurons, 1);
	Matrix *result = MatrixAlloc(outputNeurons, 1);

	//y1 = IW * input + B1
	MatrixTimesAddMatrix(IW, *input, B1, y1);
	
	MatrixTansig(y1, y1t);

	MatrixTimesAddMatrix(LW, y1t, B2, result);

	return result;
}