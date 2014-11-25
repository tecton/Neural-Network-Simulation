#include	<stdio.h>
#include	<math.h>
#include	<malloc.h>
#include	<stdlib.h>
#include	<memory.h>
#include	"matrix.h"

char *malloc_test(int size, char *s)
{
    char    *p = (char *)malloc(size);

    if(p == NULL)
    {
		fprintf(stderr, "Sorry, malloc error in '%s'\n", s);
		exit(-1);
    }

	memset(p, 0, size);
    return p;
}

Matrix	*MatrixAlloc(int n, int m)
{
    Matrix	*a = (Matrix *)malloc_test(sizeof(Matrix), "MatrixAlloc");
    a->nrows = n;
    a->ncols = m;
    a->elem = (double *)malloc_test(sizeof(double) * n * m,
				    "MatrixAlloc 2");

    return a;
}

VectorN	*VectorAlloc(int n)
{
    VectorN	*v = (VectorN *)malloc_test(sizeof(VectorN), "VectorAlloc");
    v->len = n;
    v->elem = (double *)malloc_test(sizeof(double) * n,
				    "VectorAlloc 2");

    return v;
}

void MatrixFree(Matrix *m)
{
    free((char *)m->elem);
    free((char *)m);
}

void VectorFree(VectorN *v)
{
    free((char *)v->elem);
    free((char *)v);
}

void MatrixTranspose(Matrix *a, Matrix *b)
{
    int	i, j;

    if(a->nrows != b->ncols || a->ncols != b->nrows)
    {
	fprintf(stderr, "MatrixTranspose: bad sized matrix\n");
	exit(-1);
    }

    for(i = 0; i < a->nrows; i++)
	for(j = 0; j < a->ncols; j++)
	    M_ELEMENT(b,j,i) = M_ELEMENT(a,i,j);
}

void MatrixTansig(Matrix *a, Matrix *b)
{
	int i,j;

	for(i = 0; i < a->nrows; i++)
	for(j = 0; j < a->ncols; j++)
		M_ELEMENT(b,i,j) = 2 / (1 + exp(-2 * M_ELEMENT(a,i,j))) - 1;
}

void MatrixTimesMatrix(Matrix *a, Matrix *b, Matrix *c)
{
    int	i, j, k;

    if(a->ncols != b->nrows)
    {
	fprintf(stderr, "MatrixTimesMatrix: bad sized matrices A,B\n");
	exit(-1);
    }
    else if(a->nrows != c->nrows || b->ncols != c->ncols)
    {
	fprintf(stderr, "MatrixTimesMatrix: bad sized C matrix\n");
	exit(-1);
    }

    for(i = 0; i < a->nrows; i++)
	for(j = 0; j < b->ncols; j++)
	{
	    double	sum = 0;
	    for(k = 0; k < a->ncols; k++)
		sum += M_ELEMENT(a,i,k) * M_ELEMENT(b,k,j);
	    M_ELEMENT(c,i,j) = sum;
	}
}

void MatrixTimesAddMatrix(Matrix *a, Matrix *b, Matrix *c, Matrix *d)
{
    int	i, j, k;

    if(a->ncols != b->nrows)
    {
	fprintf(stderr, "MatrixTimesMatrix: bad sized matrices A,B\n");
	exit(-1);
    }
    else if(a->nrows != d->nrows || b->ncols != d->ncols)
    {
	fprintf(stderr, "MatrixTimesMatrix: bad sized D matrix\n");
	exit(-1);
    }
	else if(c->nrows != d->nrows || c->ncols != d->ncols)
    {
	fprintf(stderr, "MatrixTimesMatrix: bad sized C matrix\n");
	exit(-1);
    }

    for(i = 0; i < a->nrows; i++)
	for(j = 0; j < b->ncols; j++)
	{
	    double	sum = 0;
	    for(k = 0; k < a->ncols; k++)
		sum += M_ELEMENT(a,i,k) * M_ELEMENT(b,k,j);
	    M_ELEMENT(d,i,j) = sum + M_ELEMENT(c,i,j);
	}
}

void MatrixTimesVector(Matrix *a, VectorN *b, VectorN *c)
{
    int	i, j;

    if(a->ncols != b->len)
    {
	fprintf(stderr, "MatrixTimesVector: bad sizes for A and B\n");
	exit(-1);
    }
    else if(c->len != a->nrows)
    {
	fprintf(stderr, "MatrixTimesVector: bad sized C vector\n");
	exit(-1);
    }

    for(i = 0; i < a->nrows; i++)
    {
	double	sum = 0;
	for(j = 0; j < a->ncols; j++)
	    sum += M_ELEMENT(a,i,j) * V_ELEMENT(b,j);
	V_ELEMENT(c,i) = sum;
    }
}

static	double p[256];

void ChSolve(Matrix *a, VectorN *b, VectorN *x)
{
    double	sum;
    int	i, j, k, n;

    if(a->nrows > 255)
    {
	fprintf(stderr, "ChSolve: matrix too big\n");
	exit(0);
    }

    if(a->nrows != a->ncols)
    {
	fprintf(stderr, "ChSolve: non-square matrix\n");
	exit(-1);
    }

    if(a->nrows != b->len || a->nrows != x->len)
    {
	fprintf(stderr, "ChSolve: bad sized B or X vector\n");
	exit(-1);
    }

    n = a->nrows;

    for(i = 0; i < n; i++)
	for(j = i; j < n; j++)
	{
	    for(sum = M_ELEMENT(a,i,j), k = i - 1; k >= 0; k--)
		sum -= M_ELEMENT(a,i,k) * M_ELEMENT(a,j,k);
	    if(i == j)
	    {
		if(sum <= 0.)
		{
		    fprintf(stderr, "ChSolve: singular matrix! -- sorry!\n");
		    exit(0);
		}
		p[i] = sqrt(sum);
	    }
	    else
		M_ELEMENT(a,j,i) = sum / p[i];
	}

    for(i = 0; i < n; i++)
    {
	for(sum = V_ELEMENT(b,i), k = i - 1; k >= 0; k--)
	    sum -= M_ELEMENT(a,i,k) * V_ELEMENT(x,k);
	V_ELEMENT(x,i) = sum / p[i];
    }

    for(i = n - 1; i >= 0; i--)
    {
	for(sum = V_ELEMENT(x,i), k = i + 1; k < n; k++)
	    sum -= M_ELEMENT(a,k,i) * V_ELEMENT(x,k);
	V_ELEMENT(x,i) = sum / p[i];
    }
}
