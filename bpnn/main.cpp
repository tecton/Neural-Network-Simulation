/*******************************************************************
* Neural Network Simulation Example
* ------------------------------------------------------------------
* Ning Tang (2014)
* email: tecton69@gmail.com
*********************************************************************/

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include "BPNN.h"

using namespace std;

void simFile(BPNN &bpnn, int focus, int r);

void main()
{
	int inputLayerNodeNumber = 8;
	int hiddenLayerNodeNumber = 10;
	int outputLayerNodeNumber = 1;
	BPNN bpnn(inputLayerNodeNumber, hiddenLayerNodeNumber, outputLayerNodeNumber);
	bpnn.loadWeights("Data\\IW.txt", "Data\\LW.txt", "Data\\B1.txt", "Data\\B2.txt");

	Matrix *input = MatrixAlloc(inputLayerNodeNumber, 1);

	M_ELEMENT(input, 0, 0) = 149;
	M_ELEMENT(input, 1, 0) = 9626;
	M_ELEMENT(input, 2, 0) = 6989;
	M_ELEMENT(input, 3, 0) = 375;
	M_ELEMENT(input, 4, 0) = 2.05;
	M_ELEMENT(input, 5, 0) = 397;
	M_ELEMENT(input, 6, 0) = 564;
	M_ELEMENT(input, 7, 0) = 630825;
	//M_ELEMENT(input, 0, 0) = 0.0016e5;
	//M_ELEMENT(input, 1, 0) = 0.1078e5;
	//M_ELEMENT(input, 2, 0) = 0.0634e5;
	//M_ELEMENT(input, 3, 0) = 0.0039e5;
	//M_ELEMENT(input, 4, 0) = 0;
	//M_ELEMENT(input, 5, 0) = 0.0040e5;
	//M_ELEMENT(input, 6, 0) = 0.0057e5;
	//M_ELEMENT(input, 7, 0) = 5.7603e5;

	Matrix *output = bpnn.simulate(&input);

	cout << M_ELEMENT(output, 0, 0) << endl;

	MatrixFree(input);
	MatrixFree(output);

	system("pause");
}
