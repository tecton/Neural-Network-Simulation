/*
	We've defined two types: a 'Matrix' type,
	and a 'Vector' type.

	To allocate space for a new matrix, do

		Matrix	*m = MatrixAlloc(i,j)

	to get an (i by j) matrix.
	Do
		MatrixFree(m)
	to free up the space. Don't call MatrixFree on an already
	free'd matrix.

	Similarly,
		Vector	*v = VectorAlloc(n)

	to get an n-vector.

	To access elements, do

		M_ELEMENT(m,i,j) = foo

	or
		foo = M_ELEMENT(m,i,j)

	to access the (i,j) entry of m. Similarly,

		V_ELEMENT(v,i) = foo

			or

		foo = V_ELEMENT(v,i)

	gets the ith entry of v.

	MatrixTimesMatrix(a, b, c)
		writes the product (a * b) into c. *YOU* must have
	allocated the correct size c before calling this routine.
	Similary,

	MatrixTimesVector(a, v, c)
		writes (a * v) into the vector c. Again, you allocate c.

	MatrixTranspose(a, b)
		puts the transpose of a into b. If a has size (n,m)
	allocate b to have size (m,n).

	Finally, ChSolve(a, b, c)
		computes c as the solution of  a * c = b,
	writing into c. The assumption is that a is symmetric
	positive definite.

	WARNING WARNING WARNING
	ChSolve will overwrite a as it solves a * c = b. If you
	need to use a after calling ChSolve, store a copy
	of it somewhere!

	Please look at the code if you have a question, and if
	that doesn't help, send me mail. --David

	A sample test program of ChSolve is in the file mtest.c
*/

#ifndef MATRIX_H
#define MATRIX_H

struct matrix {
	int	nrows, ncols;
	double	*elem;
};

struct vector {
	int	len;
	double	*elem;
};

typedef struct matrix Matrix;
typedef struct vector VectorN;

#define M_ELEMENT(m,i,j)	*((m)->elem + (m)->ncols * (i) + (j))
#define V_ELEMENT(v,i)		*(v->elem + (i))

void	MatrixTimesMatrix(Matrix *a, Matrix *b, Matrix *c);

void	MatrixTansig(Matrix *a, Matrix *b);

// A*B+C=D
void	MatrixTimesAddMatrix(Matrix *a, Matrix *b, Matrix *c, Matrix *d);

void	MatrixTimesVector(Matrix *a, VectorN *v, VectorN *c);

void	ChSolve(Matrix *a, VectorN *b, VectorN *c);

void	MatrixTranspose(Matrix *a, Matrix *b);

Matrix	*MatrixAlloc(int, int);
VectorN	*VectorAlloc(int);
void	MatrixFree(Matrix *);
void	VectorFree(VectorN *);

#endif