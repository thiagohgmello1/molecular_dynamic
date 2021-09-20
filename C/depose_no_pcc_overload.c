#include <stdio.h>
#include <math.h>
//#include <windows.h>
#define iA 843314861
#define iB 453816693
#define iM 1073741824
#define STEP 1000.0

FILE *f1;
FILE *f2;

/*****************declaracao  funcoes*****************************/
float rand0();
void sauveconf();
int anint(float x);
void recherche_Voisins1();
void inicio();
void preditor();
void detectcontacts();
void calculoforcas();
void corretor();
void verifequilibre();
void profon();
void affichage();
void sauvconf();
int archivagereaction();
double possible_weigth_calc();

/****************variaveis globais********************************/

char nom[3], nvieux[3], nnq[2];
int it, isem = 98765431;
int nq, npa, npap, ncont, ntotal, nl, nlt, ifreqv, iaff, isauve, mlh, ncell, icont0, igliss;
static double rot2[10000], rot3[10000], rot1[10000];
float kn, ks, dt2s2, frott, c1, xrmax, tolff, ivmasstot, gn, erreurtotal, viol, equil;
double dt;
static double xr[10000], r[2][10000], rp[2][10000], vp[2][10000];
static int ior[40000], iex[40000], liste[2][300000], io[300000];
static double rot1p[10000], rot2p[10000], rot3p[10000];
static double gammma[10000];
static double ap[2][10000], xrp[10000], nnvois[10000], eij[40000], xnij[2][40000];
static double fp[2][10000], fpp[2][10000];
static double ivmass[10000], ivmominit[10000], reacn[40000], react0[40000], react[40000], a[2][10000];
static double v[2][10000];
int i_y_max = 0, overload = 0, periodic = 0;
double f_overload = 0.0, y_max = 0.0;

int main()
{
   inicio();
   recherche_Voisins1();
   it = 0;
   equil = 1;
   while (equil != 0)
   {
      it++;
      if ((it % ifreqv) == 0)
      {
         recherche_Voisins1();
      }
         
      preditor();
      detectcontacts();
      calculoforcas();
      archivagereaction();
      corretor();

      if ((it % iaff) == 0)
      {
         verifequilibre();
         affichage();
      }
      if ((it % isauve) == 0)
      {
         sauvconf();
      }
         
   }

   printf("'EQUILIBRE !'\n");
   printf("i_y_max = %d\n", i_y_max);

   affichage();
   sauvconf();

   //    Sleep(8000);
}

void inicio()
{
   int iq1, iq2, i, j, irep, lecforces, ill, il, lx1, lx2;
   float xmax, zmass;
   char datname[16];

   printf(" Digite nq \n");
   scanf("\n %d", &nq);
   printf(" Digite o nome da configuracao de inicio (o nome deve conter 3 letras: ex: af0\n)");
   scanf("\n %s", &nvieux);
   printf(" digite kn \n");
   scanf("\n %f", &kn);
   printf(" Digite ks\n");
   scanf("\n %f", &ks);
   printf("Digite o coeficiente de atrito \n");
   scanf("%f", &frott);
   printf(" Digite a frequencia de exibicao\n");
   scanf("\n%d", &iaff);
   printf("Digite a frequencia calculada vizinhos\n");
   scanf("\n%d", &ifreqv);
   printf(" Digite a frequencia a ser guardada et verifEQUILIBRE\n");
   scanf("\n%d", &isauve);
   printf(" Havera leitura das forcas iniciais? (0=sim)(1=nao)\n");
   scanf("\n %d", &lecforces);
   printf("Digite o erro sobre o peso\n");
   scanf("\n %f", &tolff);
   printf(" Havera sobrecarga? (1=sim)(0=nao)\n");
   scanf("\n %d", &overload);
   printf(" Havera condicoes periodicas de contorno? (1=sim)(0=nao)\n");
   scanf("\n %d", &periodic);

   iq1 = nq / 10;
   iq2 = nq - 10 * iq1;
   nnq[0] = (char)(iq1 + 48);
   nnq[1] = (char)(iq2 + 48);
   ncont = 0;
   ntotal = 0;

   /***************-----leitura dados iniciais**************/
   datname[0] = 'D';
   datname[1] = 'M';
   datname[2] = 'c';
   datname[3] = 'o';
   datname[4] = 'n';
   datname[5] = 'f';
   datname[6] = nvieux[0];
   datname[7] = nvieux[1];
   datname[8] = nvieux[2];
   datname[9] = nnq[0];
   datname[10] = nnq[1];
   datname[11] = '.';
   datname[12] = 'd';
   datname[13] = 'a';
   datname[14] = 't';
   datname[15] = '\0';

   printf("%s\n", datname);
   printf(" Digite o nome da configuracao a ser guardada (o nome deve conter 3 letras: ex: af1\n)");
   scanf("\n %s", &nom);

   f1 = fopen(datname, "r");
   fscanf(f1, "%d %d \n", &npa, &npap);
   if (lecforces==0)
   {
		for(i=1;i<=npap;i++)
   		{
            fscanf(f1,"%le %le %le %le %le",&r[0][i],&r[1][i],&xr[i], &lx1, &lx2);
   		}
   }
   else{
   		for(i=1;i<=npap;i++)
   		{
            fscanf(f1,"%le %le %le",&r[0][i],&r[1][i],&xr[i]);

   		}
   }
   fclose(f1);

   /*********-----Numero de celulas para a pesquisa dos vizinhos**********/
   mlh = (int)(sqrt((float)(npa) / 10.0));
   if (mlh <= 3)
   {
      printf("\n Atencao nao basta celula mlh = %d \n", mlh);
      mlh = 3;
   }
   ncell = mlh * mlh;
   printf("Numero de celulas para pesquisa dos vizinhos = %d\n ", ncell);

   /*********----Massa de particulas na amostra, invertido
-----A maior particula tem massa de 1 adim*************/
   zmass = 0.0;
   ivmasstot = 0.0;
   xrmax = -1E20;
   for (i = 1; i <= npap; i++)
   {
      xrmax = (xr[i] > xrmax) ? xr[i] : xrmax;
   }
   for (i = 1; i <= npa; i++)
   {
      ivmass[i] = xrmax * xrmax / (xr[i] * xr[i]);
      ivmominit[i] = 2E0 * ivmass[i] / (xr[i] * xr[i]);
      zmass = (ivmass[i] > zmass) ? ivmass[i] : zmass;
      ivmasstot = ivmasstot + (1 / ivmass[i]);
   }
   ivmasstot = 1.0 / ivmasstot; /*inverse de la masse totale;*/
                                /************-----Amortecimento***************/
   printf(" gn ? < %f \n ", 2.0 * sqrt(kn / zmass));
   scanf("%f", &gn);
   /************-----Determinacao du pas de tempo*************/
   dt = 1 / sqrt(kn * zmass) / 50; /* ! xx/50 -> arbitraire!*/
   printf("dt = %f \n", dt);
   /**********-----Coeficiente de preditor-corretor**************/
   dt2s2 = dt * dt / 2;
   c1 = dt2s2 / dt;
   /***********-----Inicializacao multipla**************/
   printf("Iniciar com velocidades aleatorias? (0=sim)(1=nao)\n");
   scanf("%d", &irep);
   if (irep == 0)
   {
      isem = 987654321;
      for (i = 1; i <= npap; i++)
      {
         v[0][i] = pow(0.05 * (-1), i) * rand0();
         v[1][i] = pow(0.05 * (-1), (i + 1)) * rand0();
      }
   }
   else
   {
      for (i = 1; i <= npap; i++)
      {
         v[0][i] = 0.0;
         v[1][i] = 0.0;
      }
   }
   for (i = 1; i <= npap; i++)
   {
      a[0][i] = 0.0;
      a[1][i] = 0.0;
      rot2[i] = 0.0;
      rot3[i] = 0.0;
   }
}

void recherche_Voisins1()
{
   /**************** Pesquisa vizinhos 1 *************/
   /***********Pesquisa abrupta vizinhos! (en N**2)
Uso de \ amostras muito pequenas (npa < 50)**************/

   int icon, npoint, i, j;
   double x1, x12, xi, yi, xij, yij, dij;

   x1 = 2.2 * xrmax;
   x12 = x1 * x1;
   ior[ntotal + 1] = 0;
   icon = 1;
   npoint = 0;

   /***********vizinhanca entre graos livres ***********/
   for (i = 1; i <= npa; i++)
   {
      xi = r[0][i];
      yi = r[1][i];

      if (yi > 0.0)
      {
         for (j = i + 1; j <= npa; j++)
         {
            if (r[1][j] > 0.0)
            {
               xij = r[0][j] - xi;
               yij = r[1][j] - yi;
               xij = xij - periodic * anint(xij);
               dij = (xij * xij + yij * yij);
               if (dij < x12)
               {
                  npoint = npoint + 1;
                  liste[0][npoint] = i;
                  liste[1][npoint] = j;

                  if ((ior[icon] == i) && (iex[icon] == j))
                  {
                     io[npoint] = 0;
                     icon = icon + 1;
                  }
                  else
                  {
                     io[npoint] = 1;
                  }
               }
            }
         }
      }
   }
   nl = npoint;
   /***********vizinhanca entre graos e graos livres fixos*************/

   for (i = 1; i <= npa; i++)
   {
      xi = r[0][i];
      yi = r[1][i];

      if (yi > 0.0)
      {
         for (j = npa + 1; j <= npap; j++)
         {
            xij = r[0][j] - xi;
            yij = r[1][j] - yi;
            xij = xij - periodic * anint(xij);
            dij = (xij * xij + yij * yij);

            if (dij < x12)
            {
               npoint = npoint + 1;
               liste[0][npoint] = i;
               liste[1][npoint] = j;
               if ((ior[icon] == i) && (iex[icon] == j))
               {
                  io[npoint] = 0;
                  icon = icon + 1;
               }
               else
               {
                  io[npoint] = 1;
               }
            }
         }
      }
   }
   nlt = npoint;

   return;
}
void preditor()

{
   int i;
   double vi1, vi2, ai1, ai2, rt2, rt3;
   /************Predictions sur les particules libres***********/

   for (i = 1; i <= npa; i++)
   {
      if (r[1][i] > 0.0)
      {
         vi1 = v[0][i];
         vi2 = v[1][i];
         ai1 = a[0][i];
         ai2 = a[1][i];
         //         r[0][i] = r[0][i] - anint(r[0][i]);
         rp[0][i] = r[0][i] + dt * vi1 + dt2s2 * ai1;
         rp[1][i] = r[1][i] + dt * vi2 + dt2s2 * ai2;
         vp[0][i] = vi1 + dt * ai1;
         vp[1][i] = vi2 + dt * ai2;
         ap[0][i] = ai1;
         ap[1][i] = ai2;
         rt2 = rot2[i];
         rt3 = rot3[i];
         rot1p[i] = rot1[i] + dt * rt2 + dt2s2 * rt3;
         rot2p[i] = rt2 + dt * rt3;
         rot3p[i] = rt3;
      }
   }

   return;
}
/**********************definicao funcao rand0()***********************/
float rand0()
{
   float aux, x, rand0;

   aux = 0.5 / (float)(iM);

   isem = isem * iA + iB;
   {
      if (isem < 0)
      {
         isem = (isem + iM) + iM;
      }
   }
   x = isem * aux;
   rand0 = x;

   printf("% f %f \n", x, aux);
   return x;
}
/***************************definicao funcao anint ************************************************/
int anint(float x)
{
   int ix;

   if (x >= 0)
      x = x + 0.5;
   else
      x = x - 0.5;

   ix = (int)x;

   return (ix);
}
/******************detecta contatos************************/

void detectcontacts()

{
   int i, j, il;
   double x1, x12, xi, yi, xij, yij, dij, xr2, hij;

   /***********Inicializacoes*******************/
   ncont = 0;
   icont0 = 0;
   for (i = 1; i <= npa; i++)
   {
      nnvois[i] = 0;
   }

   /**************Contato entre graos livres******************/
   for (il = 1; il <= nl; il++)
   {
      i = liste[0][il];
      j = liste[1][il];
      xij = rp[0][j] - rp[0][i];
      yij = rp[1][j] - rp[1][i];
      xij = xij - periodic * anint(xij); /***! CL periodique***/
      dij = xij * xij + yij * yij;
      xr2 = (xr[i] + xr[j]) * (xr[i] + xr[j]);
      if (io[il])
         icont0 = icont0 + 1; /***!compteur pour recuperation react**/

      if (dij < xr2)
      {
         nnvois[i] = nnvois[i] + 1;
         nnvois[j] = nnvois[j] + 1;
         ncont = ncont + 1;
         ior[ncont] = i;
         iex[ncont] = j;
         dij = sqrt(dij);
         hij = dij - xr[i] - xr[j];
         eij[ncont] = hij;
         xnij[0][ncont] = xij / dij;
         xnij[1][ncont] = yij / dij;

         /***recuperation des forces tangentielles***/
         if (io[il])
         {
            react[ncont] = react0[icont0];
         }
         else
         {
            react[ncont] = 0;
         }

         io[il] = 0;
      }
      else
      {
         io[il] = 1;
      }
   }
   ntotal = ncont;
   /***Contact entre grains libres et paroi***/
   for (il = nl + 1; il <= nlt; il++)
   {
      i = liste[0][il];
      j = liste[1][il];
      xij = r[0][j] - rp[0][i];
      xij = xij - periodic * anint(xij);/** CL periodique**/
      yij = r[1][j] - rp[1][i];
      dij = xij * xij + yij * yij;
      xr2 = (xr[i] + xr[j]) * (xr[i] + xr[j]);
      if (io[il])
         icont0 = icont0 + 1; /***!compteur pour recuperation react***/
      {
         if (dij < xr2)
         {
            nnvois[i] = nnvois[i] + 1;
            ntotal = ntotal + 1;
            ior[ntotal] = i;
            iex[ntotal] = j;
            dij = sqrt(dij);
            hij = dij - xr[i] - xr[j];
            eij[ntotal] = hij;
            xnij[0][ntotal] = xij / dij;
            xnij[1][ntotal] = yij / dij;

            /************recuperation des forces tangentielles***********/
            if (io[il])
            {
               react[ntotal] = react0[icont0];
            }
            else
            {
               react[ntotal] = 0;
            }
            io[il] = 0;
         }
         else
         {
            io[il] = 1;
         }
      }
   }

   return;
}
/******************* Calculo das Forcas******************/

void calculoforcas()
{
   int i, j, il;
   double xn1, xn2, xt1, xt2;
   double fn, ft, fx, fy, vijn, vijt, ftest, vp1i, vp1j, vp2i, vp2j, fnel, fnvi;

   /***********Inicializacoes*********/
   for (i = 1; i <= npa; i++)
   {
      if ((r[1][i] > y_max) && (it == 1))
      {
         y_max = r[1][i];
         i_y_max = i;
      }
      fp[0][i] = 0;
      fp[1][i] = -ivmasstot / ivmass[i]; // Gravit\'e//
      gammma[i] = 0;
   }
   f_overload = overload*(ivmasstot / ivmass[i_y_max]) * fmin(it / STEP, 1.);
   fp[1][i_y_max] -= f_overload;
   for (i = npa + 1; i <= npap; i++)
   {
      fpp[0][i] = 0;
      fpp[1][i] = 0;
   }
   igliss = 0;

   /**************Contatos entre graos livres**************/
   for (il = 1; il <= ncont; il++)
   {
      i = ior[il];
      j = iex[il];
      vp1i = vp[0][i];
      vp1j = vp[0][j];
      vp2i = vp[1][i];
      vp2j = vp[1][j];
      /********Forcas normais***********/
      xn1 = xnij[0][il];
      xn2 = xnij[1][il];
      vijn = xn1 * (vp1j - vp1i) + xn2 * (vp2j - vp2i);
      fnel = -eij[il] * kn;
      fnvi = -vijn * gn;
      fn = fnel + fnvi;
      reacn[il] = fn;
      /*************Forcas tangenciais****************/
      xt1 = -xnij[1][il];
      xt2 = xnij[0][il];
      vijt = xt1 * (vp1j - vp1i) + xt2 * (vp2j - vp2i) - rot2p[i] * xr[i] - rot2p[j] * xr[j];
      ft = react[il];
      ft = ft - ks * vijt * dt;
      ftest = frott * fnel;
      if ((fabs(ft) > ftest) && (r[1][i] > 0.0))
      {
         igliss = igliss + 1;
         if (ft > 0)
         {
            ft = ftest;
         }
         else
         {
            ft = -ftest;
         }
      }

      react[il] = ft;
      /*******Resultantes das forcas**********/
      fx = fn * xn1 + ft * xt1;
      fy = fn * xn2 + ft * xt2;
      fp[0][i] = fp[0][i] - fx;
      fp[1][i] = fp[1][i] - fy;
      fp[0][j] = fp[0][j] + fx;
      fp[1][j] = fp[1][j] + fy;
      gammma[i] = gammma[i] - ft * xr[i];
      gammma[j] = gammma[j] - ft * xr[j];
   }

   /**************Contatos graos livres - graos fixos (paroi)************/
   for (il = ncont + 1; il <= ntotal; il++)
   {
      i = ior[il];
      j = iex[il];
      vp1i = vp[0][i];
      vp2i = vp[1][i];
      /*********Forcas normais**********/
      xn1 = xnij[0][il];
      xn2 = xnij[1][il];
      vijn = xn1 * (-vp1i) + xn2 * (-vp2i);
      fnel = -eij[il] * kn;
      fnvi = -vijn * gn;
      fn = fnel + fnvi;
      reacn[il] = fn;
      /*************Forcas tangenciais************/
      xt1 = -xnij[1][il];
      xt2 = xnij[0][il];
      vijt = xt1 * (-vp1i) + xt2 * (-vp2i) - rot2p[i] * xr[i];
      ft = react[il];
      ft = ft - ks * vijt * dt;
      ftest = frott * fnel;

      if (fabs(ft) > ftest)
      {
         igliss = igliss + 1;
         if (ft > 0)
         {
            ft = ftest;
         }
         else
         {
            ft = -ftest;
         }
      }

      react[il] = ft;
      /***************Bilan des forces***************/
      fx = fn * xn1 + ft * xt1;
      fy = fn * xn2 + ft * xt2;
      fp[0][i] = fp[0][i] - fx;
      fp[1][i] = fp[1][i] - fy;
      fpp[0][j] = fpp[0][j] + fx;
      fpp[1][j] = fpp[1][j] + fy;
      gammma[i] = gammma[i] - ft * xr[i];
   }

   return;
}

/**********************Correcteur*****************/
void corretor()
{
   int i;
   /*******************Correction sur les particules libres****************/
   for (i = 1; i <= npa; i++)
   {
      if (r[1][i] > 0.0)
      {
         a[0][i] = fp[0][i] * ivmass[i];
         a[1][i] = fp[1][i] * ivmass[i];
         v[0][i] = vp[0][i] + c1 * (a[0][i] - ap[0][i]);
         v[1][i] = vp[1][i] + c1 * (a[1][i] - ap[1][i]);
         r[0][i] = rp[0][i];
         r[1][i] = rp[1][i];
         rot3[i] = gammma[i] * ivmominit[i];
         rot2[i] = rot2p[i] + c1 * (rot3[i] - rot3p[i]);
         rot1[i] = rot1p[i];
      }
   }

   return;
}

/***************************verificando equilibrio *********************/
void verifequilibre()
{
   int i, il, j;
   double tolForce, fnel, fx, fy, res1, resrel, ff, aux;
   static double Rmax[10000], fve[2][40000];

   erreurtotal = 0;
   tolForce = 1E-6;

   /*******************Calcul des forces Elastiques (sans amortissement)**************/
   for (i = 1; i <= npa; i++)
   {
      Rmax[i] = 0.0;
      fve[0][i] = 0.0;
      fve[1][i] = -ivmasstot / ivmass[i]; // ! Gravit\'e//
   }
   for (il = 1; il <= ncont; il++)
   {
      i = ior[il];
      j = iex[il];
      fnel = -eij[il] * kn;
      Rmax[i] = (fnel > Rmax[i]) ? fnel : Rmax[i];
      Rmax[j] = (fnel > Rmax[j]) ? fnel : Rmax[j];
      fx = fnel * xnij[0][il] - react[il] * xnij[1][il];
      fy = fnel * xnij[1][il] + react[il] * xnij[0][il];
      fve[0][i] = fve[0][i] - fx;
      fve[1][i] = fve[1][i] - fy;
      fve[0][j] = fve[0][j] + fx;
      fve[1][j] = fve[1][j] + fy;
   }
   for (il = ncont + 1; il <= ntotal; il++)
   {
      i = ior[il];
      fnel = -eij[il] * kn;
      Rmax[i] = (fnel > Rmax[i]) ? fnel : Rmax[i];
      fx = fnel * xnij[0][il] - react[il] * xnij[1][il];
      fy = fnel * xnij[1][il] + react[il] * xnij[0][il];
      fve[0][i] = fve[0][i] - fx;
      fve[1][i] = fve[1][i] - fy;
   }

   res1 = 0;
   resrel = 0;
   for (i = 1; i <= npa; i++)
   {
      if (nnvois[i] > 1)
      {
         ff = sqrt(fve[0][i] * fve[0][i] + fve[1][i] * fve[1][i]);
         res1 = (fabs(gammma[i]) > res1) ? fabs(gammma[i]) : res1;
         res1 = (ff > res1) ? ff : res1;

         if (Rmax[i] > 0)
         {
            aux = 1.0 * ff / Rmax[i];
            resrel = (aux > resrel) ? aux : resrel;
         }
      }
   }

   erreurtotal = (res1 > erreurtotal) ? res1 : erreurtotal;
   erreurtotal = (resrel > erreurtotal) ? resrel : erreurtotal;

   return;
}
void profon()
{

   int i, il;

   /******************Calcul de l'interp\'en\'etration maximale*****************/
   viol = 0.0;
   for (il = 1; il <= ntotal; il++)
   {
      viol = (eij[il] < viol) ? eij[il] : viol;
   }
   viol = -viol / (2 * xrmax);

   return;
}

/**********************affichage*******************************/

void affichage()
{

   double cineti, ymax, vqrt, ff;
   int i, nsi, nsi1;
   double weigth = 0.0;
   
   weigth = possible_weigth_calc();

   /******************Test des violations ( = profondeurs de penetration)****************/
   profon();
   /***********Energie cinetique totale***********/
   cineti = 0.0;
   ymax = 0.0;
   for (i = 1; i <= npa; i++)
   {
      if (r[1][i] > 0.0)
      {
         ymax = (r[1][i] > ymax) ? r[1][i] : ymax;
         vqrt = v[0][i] * v[0][i] + v[1][i] * v[1][i];
         cineti = cineti + vqrt;
      }
   }

   nsi = 0;
   nsi1 = 0;
   for (i = 1; i <= npa; i++)
   {
      if (nnvois[i] > 0)
         nsi = nsi + 1;
      if ((nnvois[i] <= 1) && (r[1][i] > 0.0))
         nsi1 = nsi1 + 1;
   }

   ff = 0.0;
   for (i = npa + 1; i <= npap; i++)
   {
      ff = ff - fpp[1][i];
   }
   f2 = fopen("saida.dat", "a");
   fprintf(f2, "%d %d %d %d %le %le %le %le %le %d \n", it, ntotal, nsi, nsi1, viol, cineti, ff, erreurtotal, ymax, igliss);
   fclose(f2);
   printf("%d %d %d %d %le %le %le %le %le %d \n", it,ntotal,nsi,nsi1,viol,cineti,ff,erreurtotal,ymax,igliss);
   //printf("Energia cinetica= %le \n", cineti);

   /*********Equilibre atteind ?**************/
   equil = 1;
   if (igliss < 1)
   {
      if (nsi1 == 0)
      {
         if (cineti < 1E-7)
         {
            // equil = 0;
               if (fabs(ff - weigth) <= tolff)
               {
                  equil = 0;
               }
         }
      }
   }

   return;
}

double possible_weigth_calc()
{
   double weigth = 0.0;
   for (int i = 1; i <= npa; i++)
   {
      if (r[1][i] > 0.0){
         weigth += ivmasstot / ivmass[i];
      }
   }
   weigth += f_overload;

   return weigth;
}


/**************Subroutine ArchivageReaction********************/

/**********Archivage des r\'eactions tangentielles*************/
int archivagereaction()
{
   int il;

   for (il = 1; il <= ntotal; il++)
   {
      react0[il] = react[il];
   }
}

/*************SauveConf************************/
void sauvconf()
{
   float xmax, aux;
   int iq1, iq2, i, il, j;
   char datname[16];

   xmax = 0.0;
   for (i = 1; i <= npa; i++)
   {
      aux = sqrt(r[0][i] * r[0][i]);
      xmax = (xmax > aux) ? xmax : aux;
   }
   xmax = xmax + 0.5;
   xmax = xmax * 2.0;

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

   f1 = fopen(datname, "w");
   fprintf(f1, "%d %d \n", npa, npap);
   for (i = 1; i <= npap; i++)
   {
      fprintf(f1, "%f %f %f %f %f \n", r[0][i], r[1][i], xr[i], ivmass[i], ivmominit[i]);
   }
   fprintf(f1, "%f %f %f %f %f %f \n", dt, kn, gn, frott, ks, ivmasstot);
   fprintf(f1, "%d %d \n", ncont, ntotal);
   for (il = 1; il <= ntotal; il++)
   {
      i = ior[il];
      j = iex[il];
      fprintf(f1, "%d %d %d %f %le %f %f \n", il, i, j, reacn[il], react[il], xnij[0][il], xnij[1][il]);
   }
   fclose(f1);

   return;
}
