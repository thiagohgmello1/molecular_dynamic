#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#include <windows.h >
#define iA 843314861
#define iB 453816693
#define iM 1073741824

FILE *f1;

/*****************declara��o  fun��es*****************************/
float rand0();
void sauveconf();

/****************vari�veis globais********************************/
int npa, igr, npap, idist;
int isem = 678912345;
int nq;
float som, frott;
static double xrp[10000], r[2][10000], rp[2][10000], xr[10000];

int main()
{
  float dh, aux, x, decale, dx;
  int i, j, ncpt, nbord;
  float hmax, hmin, xx, xmin, xmax;
  float rmin, rmax;

  printf("-------------------------------------");
  printf("\n numero total de graos = nq * nq");
  printf("\n digite nq?");
  scanf("%d", &nq);
  printf("\n--------------------");
  printf("\n Distribuicao de raios: \n");
  printf("\n (0) Gaussiana \n");
  printf("\n (1) Uniforme \n");
  printf("\n (2) Bidisperso \n");
  printf("\n (3) Monodisperso \n");

  scanf("%d", &idist);

  printf("\n Entre com o coeficiente de atrito :");
  scanf("%f", &frott);

  printf("\n semente para sorteio dos numeros aleatorios (inteiros)?");

  scanf("%d", &igr);
  printf("\n digite o raio min?\n");
  scanf("%f", &hmin);
  printf("\n o raio max deve ser menor que 0.5 \n");
  printf("\n digite o raio max?\n");
  scanf("%f", &hmax);
  if (hmax > 0.5)
  {
    printf("\n o raio max= 0.5");
    printf("\n o programa foi abortado\n");
    exit(0);
  }
  printf("\n------------------------");
  printf("\n %d %f %f", idist, hmin, hmax);
  npa = nq * nq;
  printf("\n numero total de graos : npa = %d", npa);
  printf("\n -------------------------");
  /****************************calculo do raio*****************************/

  dh = hmax - hmin;
  rmin = 1E20;
  rmax = 0.0;

  for (i = 1; i <= npa; i++)
  {
    x = rand0();
    //       xr[i]=hmin + dh*x;
    //        printf("x= %f\n",xr[i]);

    switch (idist)
    {
    case 0:
    {
      som = 0.0;
      for (j = 1; j < 1000; j++)
        som = som + rand0();

      aux = (som - 500.0) / 83.25;
      aux = (aux + 0.4084) / 0.8585;
    }
    break;

    case 1:
      aux = rand0();
      break;

    case 2:
    {
      aux = rand0();
      if (aux > (4.0 / 7.0))
      {
        aux = 0;
      }
      else
      {
        aux = 1;
      }
    }
    break;

    case 3:
      aux = 1;
      break;
    }
    xr[i] = hmin + dh * aux;

    rmin = (xr[i] < rmin) ? xr[i] : rmin;
    rmax = (xr[i] > rmax) ? xr[i] : rmax;
  }
  printf("\n raio min = %f\n", rmin);
  printf("\n raiomax = %f\n", rmax);
  printf("\n------------------------------------");

  /******************************calculo posi��o*************************/

  nbord = nq;
  ncpt = 0;
  xmin = 1E20;
  xmax = 0.0;
  for (i = 1; i <= 2 * nbord; i++)
  {
    for (j = 1; j <= nbord * 0.5; j++)
    {
      ncpt = ncpt + 1;
      r[0][ncpt] = (float)(i) + (j % 2) * 0.5; // Se for linha par, o grao vai para a posição i. Se a linha for impar, o grão ficará no meio do caminho entre dois grãos
      r[1][ncpt] = (float)(j);
      xmin = (r[0][ncpt] < xmin) ? r[0][ncpt] : xmin;
      xmax = (r[0][ncpt] > xmax) ? r[0][ncpt] : xmax;
    }
  }
  decale = xmax - (xmax - xmin) * 0.5;
  rmin = 1E20;
  rmax = 0.0;

  for (i = 1; i <= npa; i++)
  {
    r[0][i] = r[0][i] - decale;
    rmin = (r[0][i] < rmin) ? r[0][i] : rmin;
    rmax = (r[0][i] > rmax) ? r[0][i] : rmax;
  }
  printf("\n rmin=%f e rmax=%f", rmin, rmax);

  /***************Calculo da posi��o dos gr�os da parede**************/
  npap = 2 * nbord;
  dx = (rmax - rmin) / (1.0 * npap);
  xx = rmin - dx;
  rmin = 1E20;
  rmax = -1E20;
  npap = npap + 1;
  hmax = 0.5;
  hmin = 0.5;
  dh = hmax - hmin;
  /*igr = 1265*/
  for (i = 1; i <= npap; i++)
  {
    xx = xx + dx;
    rp[0][i] = xx;
    rp[1][i] = 0;
    //  x=rand0();
    switch (idist)
    {
    case 0:
    {
      som = 0.0;
      for (j = 1; j < 1000; j++)
        som = som + rand0();

      aux = (som - 500.0) / 83.25;
      aux = (aux + 0.4084) / 0.8585;
    }
    break;

    case 1:
      aux = rand0();
      break;

    case 2:
    {
      aux = rand0();
      if (aux > (4.0 / 7.0))
      {
        aux = 0;
      }
      else
      {
        aux = 1;
      }
    }
    break;

    case 3:
      aux = 1;
      break;
    }

    xrp[i] = hmin + dh * aux;
    rmin = (rp[0][i] < rmin) ? rp[0][i] : rmin;
    rmax = (rp[0][i] > rmax) ? rp[0][i] : rmax;
  }
  printf("\n rmin = %f e rmax = %f", rmin, rmax);

  sauveconf();
}

/**********************defini��o fun��o rand0()***********************/
float rand0()
{
  float aux, x;

  aux = 0.5 / (float)(iM);

  isem = isem * iA + iB;
  {
    if (isem < 0)
    {
      isem = (isem + iM) + iM;
    }
  }
  x = isem * aux;
  return x;
}

/*************SauveConf************************/
void sauveconf()
{
  float xmax, aux;
  int iq1, iq2, i;
  char datname[16], nnq[2], nom[3];

  xmax = 0.0;
  for (i = 1; i <= npa; i++)
  {
    aux = sqrt(r[0][i] * r[0][i]);
    xmax = (xmax > aux) ? xmax : aux;
  }
  xmax = xmax + 0.5;
  xmax = xmax * 2.0;

  printf("\n escreva o nome da configura��o a ser guardada (o nome deve conter 3 letras: ex: af1)");
  scanf("\n %s", &nom);
  iq1 = nq / 10;
  iq2 = nq - 10 * iq1;
  nnq[0] = (char)(iq1 + 48);
  nnq[1] = (char)(iq2 + 48);

  datname[0] = 'D';
  datname[1] = 'M';
  datname[2] = 'c';
  datname[3] = 'o';
  datname[4] = 'n';
  datname[5] = 'f';
  datname[6] = nom[0];
  datname[7] = nom[1];
  datname[8] = nom[2];
  datname[9] = nnq[0];
  datname[10] = nnq[1];
  datname[11] = '.';
  datname[12] = 'd';
  datname[13] = 'a';
  datname[14] = 't';
  datname[15] = '\0';

  printf("%s\n", datname);
  f1 = fopen(datname, "w");
  fprintf(f1, "%d %d \n", npa, npa + npap);
  for (i = 1; i <= npa; i++)
  {
    fprintf(f1, "%f %f %f \n", r[0][i] / xmax, r[1][i] / xmax, xr[i] / xmax);
  }
  for (i = 1; i <= npap; i++)
  {
    fprintf(f1, "%f %f %f \n", rp[0][i] / xmax, rp[1][i] / xmax, xrp[i] / xmax);
  }
  fclose(f1);

  return;
}
