#define NAO_EH_TRI  0
#define ESCALENO    1
#define ISOSCELES   2
#define EQUILATERO  3

struct stTriangulo
{
    int     lado1;
    int     lado2;
    int     lado3;
    int     tipo;
    int     perimetro;
    double  semiperimetro;
    double  area;
};
