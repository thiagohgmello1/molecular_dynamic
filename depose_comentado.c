#include <stdio.h>
#include <math.h>
//#include <windows.h>
#define iA 843314861 /* N�meros m�gicos*/
#define iB 453816693 /* N�meros m�gicos*/
#define iM 1073741824 /* N�meros m�gicos*/

FILE *f1;
FILE *f2;

/*****************declara��o  fun��es*****************************/
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

/****************vari�veis globais********************************/

char nom[3],nvieux[3],nnq[2];
int it,isem = 98765431;
int nq,npa,npap,ncont,ntotal,nl,nlt,ifreqv,iaff,isauve,mlh,ncell,icont0,igliss;
static double rot2[10000],rot3[10000],rot1[10000];
float kn,ks,dt2s2,frott,c1,xrmax,tolff,ivmasstot,gn,erreurtotal,viol,equil;
double dt;
static double xr[10000],r[2][10000],rp[2][10000], vp[2][10000];
static int   ior[40000],iex[40000],liste[2][300000],io[300000];
static double rot1p[10000],rot2p[10000],rot3p[10000];
static double gammma[10000];
static double ap[2][10000],xrp[10000],nnvois[10000],eij[40000],xnij[2][40000];
static double fp[2][10000],fpp[2][10000];
static double ivmass[10000],ivmominit[10000],reacn[40000], react0[40000],react[40000],a[2][10000];
static double v[2][10000];
 

int main ()
{
    inicio(); /*� feita a inicializa��o das vari�veis*/
    recherche_Voisins1(); /*(procura de vizinhos)*/
    it=0; /*Contador de itera��es- Numero de passos da dinamica molecular*/
    equil=1; /*Indica se o sistema chegou na consi��o de equil�brio*/
    while(equil!=0) /*Efetivamente, a dinamica molecular (um passo)*/
    {
                it++;
                if((it%ifreqv)==0) recherche_Voisins1();
                preditor();
                detectcontacts();
                calculoforcas();
                archivagereaction();
                corretor();

                if((it%iaff)==0)
                {
                                verifequilibre(); /*Verifica se chegou na condi��o de parada*/
                                affichage();
                }
                 if ((it%isauve)==0)  sauvconf(); /*Salva a configura��o de tempos em tempos*/
    }

    printf("'EQUILIBRE !'\n");

    affichage(); /*Mostrar resultados*/
    sauvconf(); /*Salvar a configura��o*/

//    Sleep(8000);
}

void inicio()
{
int iq1,iq2,i,j,irep,lecforces,ill,il;
float xmax,zmass; /*Calcula a maior massa e a posi��o mais afastada das bordas*/
char datname[16]; /* Nome do arquivo no qual ser� guardada a configura��o*/

    printf(" Digite nq \n"); /*Raiz quadrada do n�mero de graos do sistema N = nq*nq em que N � o n�mero de graos do sistema*/
    scanf("\n %d",&nq);
    printf(" Digite o nome da configura��o de inicio (o nome deve conter 3 letras: ex: af0\n)"); /*O padr�o � DMconf(3 letras)(nq).dat*/
    scanf( "\n %s",&nvieux); 
    printf(" digite kn \n"); /*Constante de mola na dire��o normal. Lei de Hooke (F=kn*x). Tipicamente 1000*/
    scanf("\n %f",&kn);
    printf(" Digite ks\n"); /*Constante de mola na dire��o tangencial. Tipicamente 750*/
    scanf("\n %f",&ks);
    printf("Digite o coeficiente de atrito \n"); /*Tipicamente 0.5*/
    scanf("%f",&frott);
    printf(" Digite a frequencia de exibi�ao\n"); /*Frequ�ncia de exibi��o. Tipicamente sao 1000 passos*/
    scanf("\n%d",&iaff);
    printf("Digite a frequencia calculada vizinhos\n"); /*Tipicamente 100 passos*/
    scanf("\n%d",&ifreqv);
    printf(" Digite a frequencia a ser guardada et verifEQUILIBRE\n"); /*Tipicamente 1000 passos*/ 
    scanf("\n%d",&isauve);
    printf(" Havera leitura das for�as iniciais? (0=sim)(1=nao)\n"); /*Quando se deseja continuar a simula��o com uma amostra anterior*/
    scanf("\n %d",&lecforces);
    printf("Digite o erro sobre o peso\n"); /*Tipicamente, um valor pequeno. Utilizar 10^-5*/
    scanf("\n %f",&tolff);

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
    printf(" Digite o nome da configura��o a ser guardada (o nome deve conter 3 letras: ex: af1\n)");
    scanf( "\n %s",&nom);


   f1=fopen(datname,"r");
   fscanf(f1,"%d %d \n",&npa,&npap); /*Arquivo de configura��o. npa � o numero de graos livres do sistema. npap � o numero de graos livres mais os graos da parede*/
   for(i=1;i<=npap;i++)
   {
                       fscanf(f1,"%le %le %le",&r[0][i],&r[1][i],&xr[i]); /*r[0][i] posi��o x. r[1][i] posi��o y. xr[i] raio do grao*/

   }
   if (lecforces==0) /*Se for pedido para ler as for�as do arquivo de entrada*/
   {
                      fscanf(f1,"%d %d",&ncont,&ntotal); /*ncont numero de for�as entre graos livres. ntotal numero de for�as dos graos livres mais os da parede*/
                      printf("ncont, ntotal = %d %d",ncont, ntotal);
                      for (ill=1;ill<=ntotal;ill++)
                      {
                          fscanf(f1,"%d %d %d %le %le ",&il,&ior[il],&iex[il],&reacn[il],&react0[il]);
                      }/*il � o numero da for�a, ior[il] � uma lista das primeiras parciculas em contato. iex[il] � uma lista das segundas particulas em contato. react0[il] � a for�a tangencial do contato*/
   }
    fclose(f1);

/*********-----Numero de celulas para a pesquisa dos vizinhos**********/
      mlh = (int)(sqrt((float)(npa)/10.0));
      if (mlh<=3)
      {
         printf("\n Aten��o n�o basta celula mlh = %d \n",mlh);
         mlh=3;
      }
      ncell = mlh*mlh;
      printf("Numero de celulas para pesquisa dos vizinhos = %d\n ",ncell);

/*********----Massa de particulas na amostra, invertido
-----A maior particula tem massa de 1 adim*************/
      zmass = 0.0;
      ivmasstot = 0.0;
      xrmax = -1E20;
      for(i =1;i<=npap;i++)
      {
         xrmax = (xr[i]>xrmax)?xr[i]:xrmax; /*C�lculo do maior raio do sistema*/
      }
      for (i=1;i<=npa;i++)
      {
         ivmass[i] = xrmax*xrmax / (xr[i]*xr[i]); /*Calcula o inverso da massa*/
         ivmominit[i] = 2E0 * ivmass[i] /(xr[i]*xr[i]); /*Calcula o inverso do momento*/
         zmass = (ivmass[i]>zmass)?ivmass[i]:zmass; /*Menor massa do sistema */
         ivmasstot = ivmasstot + (1/ivmass[i]); /*inverso da massa total do sistema*/
      }
      ivmasstot = 1.0/ivmasstot; /*inverse de la masse totale;*/
/************-----Amortecimento***************/
      printf(" gn ? < %f \n ",2.0*sqrt(kn/zmass)); /*Calcula o amortecimento do sistema.*/
      scanf("%f",&gn);
/************-----Determina�ao du pas de tempo*************/
      dt = 1/sqrt(kn*zmass)/50; /* ! xx/50 -> arbitraire*/  /*Tempo de colisao*/
      printf("dt = %f \n",dt);
/**********-----Coeficiente de preditor-corretor**************/
      dt2s2 = dt*dt/2;
      c1 = dt2s2/dt;
/***********-----Inicializa��o multipla**************/
      printf("Iniciar com velocidades aleatorias? (0=sim)(1=nao)\n");
      scanf("%d",&irep); /*Por padrao, escolher nao*/
      if (irep==0)
      {
        isem = 987654321;
         for (i=1;i<=npap;i++)
         {
	    v[0][i] = pow(0.05*(-1),i) * rand0();
        v[1][i] = pow(0.05*(-1),(i+1))* rand0();
        }
     }
     else
     {
             for(i=1;i<=npap;i++)
           {
            v[0][i] = 0.0;
            v[1][i] = 0.0;
            }

    }
     for( i=1;i<=npap;i++)
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
Uso de \ amostras muito pequenas (npa < 1000)**************/

int icon, npoint,i,j;
double x1,x12,xi,yi, xij,yij,dij;


      x1 =  2.2*xrmax; /*raio de busca (2.2 vezes o raio do maior grao)*/
      x12 = x1*x1;
      ior[ntotal+1] = 0; /*Lista que define se os gr�os estavam em contato com algum outro antes de executar a rotina*/
      icon = 1;
      npoint = 0; /*Contator com o n�mero de vizinhos*/

/***********vizinhan�a entre gr�os livres ***********/
      for (i=1;i<=npa;i++) /*npa � o numero de graos livres*/
      {
         xi = r[0][i];
         yi = r[1][i];

         for (j=i+1;j<=npa;j++)
        {
            xij = r[0][j] - xi; /*Deslocamento x entre as particulas i e j*/
            yij = r[1][j] - yi; /*Deslocamento y entre as particulas i e j*/
            xij = xij - anint(xij); /*Condi��es periodicas de contorno*/
            dij = (xij*xij + yij*yij); /*Dist�ncia quadr�tica*/
           if (dij < x12) /*Se o grao desejado est� dentro do raio de busca*/
           {
               npoint = npoint + 1; /*Contador de pares de vizinhos*/
               liste[0][npoint] = i;
               liste[1][npoint] = j;

               if ((ior[icon]==i)&&(iex[icon]==j)) /*Testa se a vizinhan�a ja existia anteriormente*/
               {
                  io[npoint] = 0; /*Indica que o contato ja existia*/
                  icon = icon + 1; 
               }
               else
               {
                  io[npoint] = 1; /*Indica que � um novo contato*/
               }
          }
        }
      }
      nl = npoint;
/***********vizinhan�a entre gr�os e gr�os livres fixos*************/

     for (i=1;i<=npa;i++)
     {
         xi = r[0][i];
         yi = r[1][i];


        for (j =npa+1;j<=npap;j++)
        {
            xij = r[0][j] - xi;
            yij = r[1][j] - yi;
            xij = xij - anint(xij);
            dij = (xij*xij + yij*yij);
            
             if (dij < x12)
            {
               npoint = npoint + 1;
               liste[0][npoint] = i;
               liste[1][npoint] = j;
               if((ior[icon]==i)&&(iex[icon]==j))
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
     nlt = npoint;

      return;

}
void preditor()

{
     int i;
     double vi1, vi2,ai1, ai2,rt2,rt3;
 /************Predictions sur les particules libres***********/

     for  (i =1;i<=npa;i++) /*Para cada um dos gr�os*/
     {
         vi1 = v[0][i]; /*Velocidade do grao i em x*/
         vi2 = v[1][i]; /*Velocidade do grao i em y*/
         ai1 = a[0][i]; /*Acelera��o do grao i em x*/
         ai2 = a[1][i]; /*Aceleracao do grao i em y*/
         rp[0][i] = r[0][i] + dt*vi1 + dt2s2*ai1; /*Equa��o da cinem�tica em x*/
		 r[0][i] = r[0][i] - anint(r[0][i]); /*Garante a condi��o peri�dica de contorno*/
         rp[1][i] = r[1][i] + dt*vi2 + dt2s2*ai2; /*Equa��o da cinem�tica em y*/
         vp[0][i] = vi1 + dt*ai1 ; /*Previsao da velocidade em x*/
         vp[1][i] = vi2 + dt*ai2 ; /*Previsao da velocidade em y*/
         ap[0][i] = ai1 ; /*Acelera��o em x constante durante o passo de tempo*/
         ap[1][i] = ai2 ; /*Acelera��o em y constante durante o passo de tempo*/
         rt2 = rot2[i]; /*Velocidade angular*/
         rt3 = rot3[i]; /*Acelera��o angular*/
         rot1p[i] = rot1[i] + dt*rt2 + dt2s2*rt3 ; /*Posi��o angular*/
         rot2p[i] = rt2 + dt*rt3 ; /*Velocidade angular*/
         rot3p[i] = rt3; /*Acelera��o angular constante*/
      }


      return;
  }
     /**********************defini��o fun��o rand0()***********************/
float rand0()
{
      float aux,x,rand0;

          aux = 0.5/(float) (iM);

          isem = isem*iA + iB;
          {
               if(isem < 0)
               {
                        isem = (isem + iM )+iM;
               }
          }
          x = isem*aux;
          rand0=x;


          printf("% f %f \n",x,aux);
          return x;
}
/***************************defini�ao fun��o anint ************************************************/
int anint(float x) /*Calcula o inteiro mais pr�ximo de x*/
{
      int ix;

      if(x>=0)    x=x+0.5;
      else        x=x-0.5;

      ix = (int)x;

      return (ix);

}
/******************detecta contatos************************/

void detectcontacts()

 {
     int i,j,il;
    double x1,x12,xi,yi,xij,yij,dij,xr2,hij;

/***********Inicializa�oes*******************/
      ncont = 0;
      icont0 = 0;
      for(i = 1;i<=npa; i++)
      {
         nnvois[i] = 0; /*Cria uma lista para contabilizar o n�mero de contatos*/ 
       }

/**************Contato entre gr�os livres******************/
      for(il=1;il<=nl;il++)
{
         i=liste[0][il]; /*Seleciona a particula i da lista de contatos do recheche voisins*/
         j=liste[1][il]; /*Seleciona a particula j da lista de contatos do recheche voisins*/
         xij = rp[0][j] - rp[0][i]; /*Dist�ncia x entre os dois graos*/
         yij = rp[1][j] - rp[1][i]; /*Dist�ncia y entre os dois gr�os*/
         xij = xij - anint(xij); /***! CL periodique***/ /*Calcula a condi��o peri�dica de contorno*/
         dij = xij*xij + yij*yij; /*Dist�ncia quadr�tica entre part�culas*/
         xr2 = (xr[i]+xr[j])*(xr[i]+xr[j]); /*Soma dos raios dos gr�os ao quadrado*/
         if(io[il]) icont0 = icont0 + 1;/***!compteur pour recuperation react**/

         if (dij<xr2) /*Verifica se a dist�ncia entre os gr�os � menor que a soma dos raios ao quadrado (penetra��o)*/
         {
            nnvois[i] = nnvois[i] + 1;
            nnvois[j] = nnvois[j] + 1;
            ncont = ncont + 1; /*N�mero de contatos*/
            ior[ncont] = i; /*Lista de contatos. Salva a part�cula i em ior*/
            iex[ncont] = j; /*Lista de contatos. Salva a part�cula j em iex*/
            dij = sqrt(dij); /*dist�ncia entre gr�os*/
            hij = dij - xr[i] - xr[j]; /*Valor da interpenetra��o*/
            eij[ncont] = hij; /*Lista de interpenetra��es para calculo das for�as*/
            xnij[0][ncont] = xij / dij; /*Cosseno do angulo de contato*/
            xnij[1][ncont] = yij / dij; /*Seno do angulo de contato*/

/***recuperation des forces tangentielles***/
            if (io[il]) /*Verifica se o contato j� existia anteriormente*/
            {
               react[ncont] = react0[icont0]; /*Se existia, retorna o valor da for�a tangencial anterior*/
            }
            else
            {
               react[ncont] = 0; /*Se nao existia, a for�a tangencial � nula*/ 
            }

            io[il] =0;
         }
         else /*Se nao houver contato*/
         {
            io[il] = 1;
         }
}
      ntotal = ncont; /*N�mero total de contatos*/
/***Contact entre grains libres et paroi***/
     for(il =nl+1;il<=nlt;il++)
     {
         i=liste[0][il];
         j=liste[1][il];
         xij = r[0][j] - rp[0][i];
         xij = xij - anint(xij);/** CL periodique**/
         yij = r[1][j] - rp[1][i];
         dij = xij*xij + yij*yij;
         xr2 = (xr[i]+xr[j])*(xr[i]+xr[j]);
        if (io[il]) icont0 = icont0 + 1; /***!compteur pour recuperation react***/
        {
         if (dij<xr2)
         {
            nnvois[i] = nnvois[i] + 1;
            ntotal = ntotal + 1;
            ior[ntotal]= i;
            iex[ntotal] = j;
            dij = sqrt(dij);
            hij = dij - xr[i] - xr[j];
            eij[ntotal] = hij ;
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
/******************* Calculo das For�as******************/

     void calculoforcas()
    {
      int i,j,il;
      double xn1,xn2,xt1,xt2;
      double fn,ft,fx,fy,vijn,vijt,ftest,vp1i,vp1j,vp2i,vp2j,fnel,fnvi ;

/***********Inicializa��es*********/

      for ( i =1;i<=npa;i++)  /*Inicializa as for�as previstas*/
      {
         fp[0][i] = 0; /*For�a prevista em x*/
         fp[1][i] = - ivmasstot / ivmass[i]; // Gravit\'e// /*For�a peso normalizado para cada grao livre. Assim, a soma de todos os pesos � 1*/
         gammma[i] = 0;
      }
      for ( i = npa+1;i<=npap;i++) /*Gr�os da parede. Soma das for�as � zero uma vez que os gr�os est�o parados*/
      {
         fpp[0][i] = 0; /*For�a prevista na parede em x*/
         fpp[1][i] = 0; /*For�a prevista na parede em y*/
     }
      igliss = 0; /*N�mero de contatos deslizantes (for�a tangencial > mu*Normal). A part�cula est� em movimento de rota��o*/

/**************Contatos entre gr�os livres**************/
     for (il = 1;il<= ncont;il++)
     {
         i = ior[il]; /*Seleciona a particula i em contato*/
         j = iex[il]; /*Seleciona a part�cula j em contato*/
         vp1i = vp[0][i]; /*Velocidade de i em x*/
         vp1j = vp[0][j]; /*Velocidade de j em x*/
         vp2i = vp[1][i]; /*Velocidade de i em y*/
         vp2j = vp[1][j]; /*Velocidade de j em y*/
/********For�as normais***********/
         xn1 = xnij[0][il]; /*Cosseno do �ngulo (theta) de dire��o de contato*/
         xn2 = xnij[1][il]; /*Seno do �ngulo (theta)*/
         vijn = xn1*(vp1j-vp1i)+xn2*(vp2j-vp2i); /*Velocidade relativa na dire��o normal do contato. Velocidade de aproxima��o ou afastamento entre as part�culas*/
         fnel = - eij[il]*kn; /*For�a normal el�stica. Relaciona a interpenetra��o e a constante kn*/
         fnvi = - vijn*gn; /*For�a normal viscosa. For�a de dissipa��o*/
         fn = fnel + fnvi; /*For�a normal total*/
         reacn[il] = fn; /*Resultante na dire��o normal*/
/*************For�as tangenciais****************/
         xt1 = -xnij[1][il];
         xt2 =  xnij[0][il];
         vijt = xt1*(vp1j-vp1i)+xt2*(vp2j-vp2i) -  rot2p[i]*xr[i] - rot2p[j]*xr[j]; /*Velocidade relativa tangencial*/
         ft = react[il]; /*For�a tangencial. Indica o tanto que um gr�o girou dentro do outro*/
         ft = ft - ks*vijt*dt;
         ftest = frott*fnel; /*For�a de teste. mu*Normal*/
         if ((fabs(ft)>ftest) && (r[1][i] > 0.0)) /*Verificar se a for�a tangencial � maior que a for�a de teste. Se for verdade, o contato desliza*/
         {
           igliss=igliss+1;
            if (ft>0)
            {
               ft =  ftest;
            }
            else
            {
               ft = -ftest;
            }
          }

         react[il] = ft;
/*******Resultantes das for�as**********/
         fx = fn*xn1 + ft*xt1; /*Decomposi��o das for�as na dire��o x*/
         fy = fn*xn2 + ft*xt2; /*Decomposi��o das for�as na dire��o y*/
         fp[0][i] = fp[0][i] - fx; /*For�a prevista resultante*/
         fp[1][i] = fp[1][i] - fy;
         fp[0][j] = fp[0][j] + fx;
         fp[1][j] = fp[1][j] + fy;
         gammma[i] = gammma[i] - ft*xr[i]; /*Torque previsto resultante*/
         gammma[j] = gammma[j] - ft*xr[j];
     }

/**************Contatos gr�os livres - graos fixos (paroi)************/
      for( il=ncont+1;il<=ntotal;il++)
      {
         i = ior[il];
         j = iex[il];
         vp1i = vp[0][i];
         vp2i = vp[1][i];
/*********For�as normais**********/
         xn1 = xnij[0][il];
         xn2 = xnij[1][il];
         vijn = xn1*(-vp1i)+xn2*(-vp2i);
         fnel = - eij[il]*kn;
         fnvi = - vijn*gn;
         fn = fnel + fnvi;
         reacn[il] = fn;
/*************For�as tangenciais************/
         xt1 = -xnij[1][il];
         xt2 =  xnij[0][il];
         vijt = xt1*(-vp1i)+xt2*(-vp2i) - rot2p[i]*xr[i];
         ft = react[il];
         ft = ft - ks*vijt*dt;
         ftest = frott*fnel;

         if(fabs(ft)>ftest)
         {
             igliss=igliss+1;
            if (ft>0)
            {
               ft =  ftest;
            }
            else
            {
               ft = -ftest;
            }
            }

         react[il] = ft;
/***************Bilan des forces***************/
         fx = fn*xn1 + ft*xt1;
         fy = fn*xn2 + ft*xt2;
         fp[0][i] = fp[0][i] - fx;
         fp[1][i] = fp[1][i] - fy;
         fpp[0][j] = fpp[0][j] + fx;
         fpp[1][j] = fpp[1][j] + fy;
         gammma[i] = gammma[i] - ft*xr[i];

      }

      return;
      }


/**********************Correcteur*****************/
void corretor()
{
     int i;
/*******************Correction sur les particules libres****************/
    for (i = 1;i<=npa;i++)
    {
         a[0][i] = fp[0][i]*ivmass[i]; /*Acelera��o corrigida em x*/	
         a[1][i] = fp[1][i]*ivmass[i]; /*Acelera��o corrigida em y*/
         v[0][i] = vp[0][i] + c1*(a[0][i]-ap[0][i]); /*Corre��o da velocidade. c1 � um fator de corre��o*/
         v[1][i] = vp[1][i] + c1*(a[1][i]-ap[1][i]);
         r[0][i] = rp[0][i]; /*As posi��es permanecem as mesmas*/
         r[1][i] = rp[1][i];
         rot3[i] = gammma[i]*ivmominit[i]; /*Acelera��o angular*/
         rot2[i] = rot2p[i] + c1*(rot3[i]-rot3p[i]); /*Velocidade angular corrigida*/
         rot1[i] = rot1p[i]; /*As posi��es angulares sao mantidas*/
      }

      return;
      }


/***************************verificando equilibrio *********************/
void verifequilibre()
{
  int i,il,j;
 double  tolForce, fnel,fx,fy, res1,resrel,ff,aux;
  static double Rmax[10000],fve[2][40000];


      erreurtotal = 0;
      tolForce = 1E-6;

/*******************Calcul des forces Elastiques (sans amortissement)**************/
    for (i = 1;i<=npa;i++)
    {
         Rmax[i] = 0.0;
         fve[0][i] = 0.0;
         fve[1][i]  = - ivmasstot / ivmass[i];// ! Gravit\'e//
      }
     for (il = 1;il<=ncont;il++)
     {
         i = ior[il];
         j = iex[il];
         fnel = - eij[il]*kn;
         Rmax[i] = (fnel>Rmax[i])?fnel:Rmax[i];
         Rmax[j] =(fnel>Rmax[j])?fnel:Rmax[j];
         fx = fnel*xnij[0][il] - react[il]*xnij[1][il];
         fy = fnel*xnij[1][il] + react[il]*xnij[0][il];
         fve[0][i] = fve[0][i] - fx;
         fve[1][i] = fve[1][i] - fy;
         fve[0][j] = fve[0][j] + fx;
         fve[1][j] = fve[1][j]+ fy;
   }
     for( il = ncont+1;il<=ntotal;il++)
     {
         i = ior[il];
         fnel = - eij[il]*kn;
         Rmax[i] = (fnel>Rmax[i])?fnel:Rmax[i];
         fx = fnel*xnij[0][il] - react[il]*xnij[1][il];
         fy = fnel*xnij[1][il] + react[il]*xnij[0][il];
         fve[0][i] = fve[0][i] - fx;
         fve[1][i] = fve[1][i] - fy;
    }

      res1 = 0;
      resrel = 0;
     for( i = 1;i<=npa;i++)
     {
        if(nnvois[i]>1)
        {
            ff = sqrt(fve[0][i]*fve[0][i] + fve[1][i]*fve[1][i]);
            res1 = (fabs(gammma[i])>res1)?fabs(gammma[i]):res1;
            res1=(ff>res1)?ff:res1;

           if(Rmax[i]>0)
           {
               aux = 1.0*ff/Rmax[i];
               resrel = (aux>resrel)?aux:resrel;
            }
         }
     }

      erreurtotal = (res1>erreurtotal)?res1:erreurtotal;
      erreurtotal = (resrel>erreurtotal)?resrel:erreurtotal;

      return;

         }
void profon()
{

      int i,il;

/******************Calcul de l'interp\'en\'etration maximale*****************/
      viol = 0.0;
      for( il = 1;il<= ntotal;il++)
      {
         viol = (eij[il]<viol)?eij[il]:viol;
     }
      viol = -viol/(2*xrmax);

      return;
}

/**********************affichage*******************************/

void affichage()
{

double cineti,ymax,vqrt,ff;
int i,nsi,nsi1;

/******************Test des violations ( = profondeurs de penetration)****************/
profon();
/***********Energie cinetique totale***********/
      cineti = 0.0;
      ymax = 0.0;
     for( i = 1;i<=npa;i++)
     {
         ymax = (r[1][i]>ymax)?r[1][i]:ymax;
         vqrt = v[0][i]*v[0][i] + v[1][i]*v[1][i];
         cineti = cineti + vqrt;
      }

      nsi = 0;
      nsi1 = 0;
      for( i=1;i<=npa;i++)
      {
        if (nnvois[i]>0) nsi = nsi + 1;
         if (nnvois[i]<=1) nsi1 = nsi1 + 1;
     }

      ff = 0.0;
      for( i = npa+1;i<=npap;i++)
      {
         ff = ff - fpp[1][i];
     }
 f2=fopen ("saida.dat","a");
     fprintf(f2,"%d %d %d %d %le %le %le %le %le %d \n", it,ntotal,nsi,nsi1,viol,cineti,ff,erreurtotal,ymax,igliss);
/*itera��o, numero total de contatos, numero de graos com pelo menos 2 vizinhos, numero de graos com 1 vizinho, -, energia cin�tica, for�a medida no fundo, erro total m�ximo permitido, posi��o mais alta, numero de graos que estao deslizando*/
     fclose(f2);
printf("%d %d %d %d %le %le %le %le %le %d \n", it,ntotal,nsi,nsi1,viol,cineti,ff,erreurtotal,ymax,igliss);

/*********Equilibre atteind ?**************/ /*Testes de equil�brio*/
      equil = 1;
       if (igliss<1)
       {
        if (nsi1==0)
        {
            if (cineti<1E-7)
            {
               if (fabs(ff-1.0)<=tolff)
               {
                   equil = 0;
               }
            }
        }
      }

      return;
      }



      /**************Subroutine ArchivageReaction********************/


/**********Archivage des r\'eactions tangentielles*************/
int archivagereaction()
{
      int il;


      for ( il = 1;il<=ntotal;il++)
      {
         react0[il] = react[il];
      }


      }


/*************SauveConf************************/
 void sauvconf()
{
    float xmax, aux;
    int iq1,iq2,i,il,j;
    char datname[16];

    xmax = 0.0;
    for (i = 1; i<=npa; i++)
    {
         aux = sqrt(r[0][i]*r[0][i]);
         xmax = (xmax>aux)?xmax:aux;
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



      f1=fopen(datname,"w");
      fprintf(f1,"%d %d \n", npa,npap);
      for ( i = 1; i<=npap; i++)
      {
         fprintf(f1,"%f %f %f %f %f \n", r[0][i], r[1][i], xr[i],ivmass[i],ivmominit[i]);
      }
      fprintf(f1,"%f %f %f %f %f %f \n",dt,kn,gn,frott,ks,ivmasstot);
      fprintf(f1,"%d %d \n",ncont,ntotal);
      for(il=1;il<=ntotal;il++)
      {
          i=ior[il];
          j=iex[il];
          fprintf(f1,"%d %d %d %f %le %f %f \n",il,i,j,reacn[il],react[il],xnij[0][il],xnij[1][il]);
      }
                 fclose(f1);

      return;
     }

