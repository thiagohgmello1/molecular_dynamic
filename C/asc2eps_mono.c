
/*
        Allbens Atman
        Version de 03/03/2003
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define xdois 2.0


FILE *f1;
FILE *f2;
FILE *f3;

void help();

/******************************************************************************
programme principal

******************************************************************************/
void main(argc, argv)
int argc;
float **argv;
{
/*      DEFINItion De lAS VARIAbles         */

int     i,ii,j,k, ihp, iaux;
int     cl,ch,num,num0;

static float    xr[20000], yr[20000], r[20000], tfit;
static float    hh[20000];
float   lmax, lmin, hmax, hmin, dl, dh, rd, ru, td, tu,ymin;
float   dt, kn, ks, frott, gn, ivmasstot, lx1, lx2;
static float    fn, ft, fx, fy, c1, c2, f12[20000];
int     nubr, rea, il, fon, fon0, mult;
static int  p1[20000], p2[20000];
char    nbr,read, nome[21],nome2[21]="DMconfAAA00000.eps";
float   dif, rif, rA, rB, hp, hp1, lett;

if(argc!=2){
   help();
   exit(0);}

/*      ouverture du fichier   */
f2=fopen(argv[1],"r");

/*      lecture du fichier d'entr�     */
fscanf(f2, "%d %d \n", &num0, &num);

printf("Read forces? 0 (yes), 1 (no), 2 (prepare)\n");
scanf("%d", &rea);

printf("Write numbers? 0 (yes), 1 (no)\n");
scanf("%d", &nubr);

if(nubr == 0)
{
    printf("Letter size? 0 - 1 \n");
    scanf("%f", &lett);
}

printf("Multiple files? 0 (yes), 1 (no)\n");
scanf("%d", &mult);

if(mult!=0) 
{
    switch(rea)
    {
        case 0:
        {
         /*  printf("go %d \n",rea); */ 
             for(i=1;i<=num;i++) 
             { 
                 fscanf(f2, "%f %f %f %f %f\n", &xr[i], &yr[i], &r[i], &lx1, &lx2); 
             } 
          
             fscanf(f2, "%f %f %f %f %f %f \n", &dt, &kn, &gn, &frott, &ks, &ivmasstot);
          
             fscanf(f2, "%d %d \n", &fon0, &fon); 
             for(i=1;i<=fon;i++) 
             { 
                 fscanf(f2, "%d %d %d %f %f %f %f \n", &il, &p1[i], &p2[i], &fn, &ft, &c1, &c2); 
          
                 fx = fn*c1 + ft*(-1.0*c2); 
                 fy = fn*c2 + ft*c1; 
          
                 f12[i] = sqrt(fx*fx + fy*fy); 
             } 
          
         } 
         break; 
         case 1: 
         { 
             for(i=1;i<=num;i++) 
             { 
                 fscanf(f2, "%f %f %f %f %f \n", &xr[i], &yr[i], &r[i], &lx1, &lx2); 
             } 
         } 
         break; 
         case 2: 
         { 
             for(i=1;i<=num;i++) 
             { 
                 fscanf(f2, "%f %f %f \n", &xr[i], &yr[i], &r[i]); 
             } 
         } 
    }

fclose(f2);

f1 = fopen("out.eps","w");
fprintf(f1, "%%!PS-Adobe-2.0 EPSF-3.0 \n");
fprintf(f1, "%%%%BoundingBox: -200 -200 200 200 \n");
fprintf(f1, " /rescale { sc mul exch sc mul exch } def \n");
fprintf(f1, " /sc 1 def \n");
fprintf(f1, " /lr { gsave \n");
fprintf(f1, "   1 0 0 setrgbcolor \n");
fprintf(f1, "         setlinewidth \n");
fprintf(f1, "         newpath \n");
fprintf(f1, " rescale moveto \n");
fprintf(f1, " rescale lineto  \n");
fprintf(f1, " stroke  \n");
fprintf(f1, "      grestore } bind def \n");
fprintf(f1, " /lb { gsave \n");
fprintf(f1, "   0 0 1 setrgbcolor \n");
fprintf(f1, "         setlinewidth \n");
fprintf(f1, "         newpath \n");
fprintf(f1, " rescale moveto \n");
fprintf(f1, " rescale lineto  \n");
fprintf(f1, " stroke  \n");
fprintf(f1, "      grestore } bind def \n");
fprintf(f1, " /l { gsave \n");
fprintf(f1, "    0 setgray \n");
fprintf(f1, "      setlinewidth \n");
fprintf(f1, "      newpath \n");
fprintf(f1, " rescale moveto  \n");
fprintf(f1, " rescale lineto \n");
fprintf(f1, " stroke \n");
fprintf(f1, "      grestore } bind def \n");
fprintf(f1, " %% 10 10 rescale translate  \n");
fprintf(f1, "/Times-Roman findfont \n");
fprintf(f1, "%f scalefont \n", lett);
fprintf(f1, "setfont \n");
fprintf(f1, "%%%%%%%%%%%%%%%%LIGNES%%%%%%%%%%%%%%%%%%%%%%%%%%  \n");

for(i=1;i<=num;i++)
{
    xr[i] = 250.0*xr[i];
    yr[i] = -125.0 + 250.0*yr[i];
    r[i] = 250.0*r[i];

    fprintf(f1, "0.1 setlinewidth \n");
    fprintf(f1,"newpath %f %f %f 0 360 arc\n", xr[i], yr[i], r[i]);
    
/*    printf("%f %f %f \n", xr[i], yr[i], r[i]);*/

    fprintf(f1, "stroke \n");
    if(nubr == 0)
    {
        fprintf(f1, "%f %f moveto \n", (xr[i]-r[i]), (yr[i]-0.5*r[i]));
        fprintf(f1, "( %d )show \n", i);
    }
    if(i>num0)
    {
        if(i==num0+1)
            fprintf(f1, "%f setgray \n", 0.5);
        ihp = (i - num0)%250;
        if(ihp==0)
        {
            ihp = ((i - num0)/250)%2;
            if(ihp==0)  
                fprintf(f1, "%f setgray \n", 0.5);
            else
                fprintf(f1, "%f setgray \n", 1.0);
        }
        fprintf(f1, "0.1 setlinewidth \n");
        fprintf(f1, "newpath %f %f %f 0 360 arc\n", xr[i], yr[i], r[i]);
        fprintf(f1, "fill \n");          
    }        
}
if(rea == 0)
{
    hp = (xr[num] - xr[num0+1]) + 0.5*(xr[num]-xr[num-1]);
    hp=250.0;
    for(i=1; i<=fon; i++)
    {
        dif = sqrt((xr[p1[i]] - xr[p2[i]])*(xr[p1[i]] - xr[p2[i]]));
        rif = r[p1[i]] + r[p2[i]];

        if(dif > (2.0*rif))
        {
            if(xr[p1[i]]<0)
            {
                rA = xr[p1[i]] + hp;
                rB = xr[p2[i]] - hp;
            }
            else
            {
                rA = xr[p1[i]] - hp;
                rB = xr[p2[i]] + hp;
            }
            
            fprintf(f1, "0.5 setgray \n");
            fprintf(f1, "0.1 setlinewidth \n");
            fprintf(f1,"newpath %f %f %f 0 360 arc\n", rA, yr[p1[i]], r[p1[i]]);
            fprintf(f1, "stroke \n");
            if(nubr == 0)
            {
                fprintf(f1, "%f %f moveto \n", (rA-r[p1[i]]), (yr[p1[i]]-0.5*r[p1[i]]));
                fprintf(f1, "( %d )show \n", p1[i]);
            }
            fprintf(f1, "0.1 setlinewidth \n");
            fprintf(f1,"newpath %f %f %f 0 360 arc\n", rB, yr[p2[i]], r[p2[i]]);
            fprintf(f1, "stroke \n");
            if(nubr == 0)
            {
                fprintf(f1, "%f %f moveto \n", (rB-r[p2[i]]), (yr[p2[i]]-0.5*r[p2[i]]));
                fprintf(f1, "( %d )show \n", p2[i]);
            }
            if(f12[i]>0.0)
            {
                fprintf(f1, "%f %f %f %f %f l\n",rA, yr[p1[i]],xr[p2[i]], yr[p2[i]], 0.03*f12[i]);
                fprintf(f1, "%f %f %f %f %f l\n",xr[p1[i]], yr[p1[i]],rB, yr[p2[i]], 0.03*f12[i]);
            }
            if(f12[i]<0.0)
            {
                fprintf(f1, "%f %f %f %f %f lr\n",rA, yr[p1[i]],xr[p2[i]], yr[p2[i]], 0.03*f12[i]);
                fprintf(f1, "%f %f %f %f %f lr\n",xr[p1[i]], yr[p1[i]],rB, yr[p2[i]], 0.03*f12[i]);
            }
        }
        else
        {
            if(f12[i]>0.0)
                fprintf(f1, "%f %f %f %f %f l\n",xr[p1[i]], yr[p1[i]],xr[p2[i]], yr[p2[i]],0.005*f12[i]);
            if(f12[i]<0.0)
                fprintf(f1, "%f %f %f %f %f lr\n",xr[p1[i]], yr[p1[i]],xr[p2[i]], yr[p2[i]],0.005*f12[i]);  
        }
    }
}

fclose(f1);

}
else
{
    fclose(f2);
    
    printf("Number of files? \n");
    scanf("%d", &iaux);
    
    printf("File prefix? (e.g. DMconfaa1 - max.9 caracters)\n");
    scanf("%s", &nome);
    
        
    for(ii=1;ii<=iaux;ii++)
    {
        nome[9]=ii/100 + 48;
        nome[10]=(ii - (ii/100)*100)/10 + 48;
        nome[11]=(ii - (ii/10)*10) + 48;
//        nome[12]=(ii - (ii/10)*10) + 48;
//        nome[13]=(ii - (ii/10)*10) + 48;
//        nome[14]='.';
//        nome[15]='d';        
//        nome[16]='a';        
//        nome[17]='t';        
        
        for(i=0;i<20000;i++)
        {
            xr[i]=0.0;
            yr[i]=0.0;
            r[i]=0.0;
            xr[i]=0.0;
            p1[i]=0;
            p2[i]=0;
        }
    
    printf("%s %d\n", nome, iaux);
        f2=fopen(nome,"r");
        
        /*      lecture du fichier d'entr�     */
        fscanf(f2, "%d %d \n", &num0, &num);

        switch(rea)
        {
            case 0:
            {
             /*  printf("go %d \n",rea); */ 
                  
                  for(i=1;i<=num;i++) 
                  { 
                    fscanf(f2, "%f %f %f %f %f\n", &xr[i], &yr[i], &r[i], &lx1, &lx2); 
                  } 
          
                fscanf(f2, "%f %f %f %f %f %f \n", &dt, &kn, &gn, &frott, &ks, &ivmasstot);
          
                   fscanf(f2, "%d %d \n", &fon0, &fon); 
                   for(i=1;i<=fon;i++) 
                   { 
                        fscanf(f2, "%d %d %d %f %f %f %f \n", &il, &p1[i], &p2[i], &fn, &ft, &c1, &c2); 
          
                        fx = fn*c1 + ft*(-1.0*c2); 
                        fy = fn*c2 + ft*c1; 
          
                        f12[i] = sqrt(fx*fx + fy*fy); 
                    } 
          
            } 
            break; 
            case 1: 
            { 
                for(i=1;i<=num;i++) 
                { 
                    fscanf(f2, "%f %f %f %f %f \n", &xr[i], &yr[i], &r[i], &lx1, &lx2); 
                } 
            } 
            break; 
            case 2: 
            { 
                for(i=1;i<=num;i++) 
                { 
                    fscanf(f2, "%f %f %f \n", &xr[i], &yr[i], &r[i]); 
                } 
            } 
        }

        fclose(f2);
        nome2[0]='i';
        nome2[1]='m';
        nome2[2]='a';
        nome2[3]='g';
        nome2[4]='e';        
        nome2[5]=nome[5];
        nome2[6]=nome[6];
        nome2[7]=nome[7];
        nome2[8]=nome[8];
        nome2[9]=nome[9];
        nome2[10]=nome[10];
        nome2[11]=nome[11];
//        nome2[12]=nome[12];
//        nome2[13]=nome[13];
        nome2[14]='.';                        
        nome2[15]='e';
        nome2[16]='p';
        nome2[17]='s';                        
        
        f1 = fopen(nome2,"w");
        fprintf(f1, "%%!PS-Adobe-2.0 EPSF-3.0 \n");
        fprintf(f1, "%%%%BoundingBox: -200 -200 200 200 \n");
        fprintf(f1, " /rescale { sc mul exch sc mul exch } def \n");
        fprintf(f1, " /sc 1 def \n");
        fprintf(f1, " /lr { gsave \n");
        fprintf(f1, "   1 0 0 setrgbcolor \n");
        fprintf(f1, "         setlinewidth \n");
        fprintf(f1, "         newpath \n");
        fprintf(f1, " rescale moveto \n");
        fprintf(f1, " rescale lineto  \n");
        fprintf(f1, " stroke  \n");
        fprintf(f1, "      grestore } bind def \n");
        fprintf(f1, " /lb { gsave \n");
        fprintf(f1, "   0 0 1 setrgbcolor \n");
        fprintf(f1, "         setlinewidth \n");
        fprintf(f1, "         newpath \n");
        fprintf(f1, " rescale moveto \n");
        fprintf(f1, " rescale lineto  \n");
        fprintf(f1, " stroke  \n");
        fprintf(f1, "      grestore } bind def \n");
        fprintf(f1, " /l { gsave \n");
        fprintf(f1, "    0 setgray \n");
        fprintf(f1, "      setlinewidth \n");
        fprintf(f1, "      newpath \n");
        fprintf(f1, " rescale moveto  \n");
        fprintf(f1, " rescale lineto \n");
        fprintf(f1, " stroke \n");
        fprintf(f1, "      grestore } bind def \n");
        fprintf(f1, " %% 10 10 rescale translate  \n");
        fprintf(f1, "/Times-Roman findfont \n");
        fprintf(f1, "%f scalefont \n", lett);
        fprintf(f1, "setfont \n");
        fprintf(f1, "%%%%%%%%%%%%%%%%LIGNES%%%%%%%%%%%%%%%%%%%%%%%%%%  \n");
        
        ymin = yr[num0+2];
        
        for(i=1;i<=num;i++)
        {
            xr[i] = 250.0*xr[i];
            yr[i] = -125.0 + 250.0*(yr[i]);
            r[i] = 250.0*r[i];

            fprintf(f1, "0.1 setlinewidth \n");
            fprintf(f1,"newpath %f %f %f 0 360 arc\n", xr[i], yr[i], r[i]);
            
/*            printf("%f %f %f \n", xr[i], yr[i], r[i]); */

            fprintf(f1, "stroke \n");
            if(nubr == 0)
            {
                fprintf(f1, "%f %f moveto \n", (xr[i]-r[i]), (yr[i]-0.5*r[i]));
                fprintf(f1, "( %d )show \n", i);
            }
            if(i>num0)
            {
                if(i==num0+1)
                    fprintf(f1, "%f setgray \n", 0.5);
                ihp = (i - num0)%250;
                if(ihp==0)
                {
                    ihp = ((i - num0)/250)%2;
                    if(ihp==0)  
                        fprintf(f1, "%f setgray \n", 0.5);
                    else
                        fprintf(f1, "%f setgray \n", 1.0);
                }
                fprintf(f1, "0.1 setlinewidth \n");
                fprintf(f1, "newpath %f %f %f 0 360 arc\n", xr[i], yr[i], r[i]);
                fprintf(f1, "fill \n");          
            }        
        }
        if(rea == 0)
        {
            for(i=1; i<=fon; i++)
            {
                dif = sqrt((xr[p1[i]] - xr[p2[i]])*(xr[p1[i]] - xr[p2[i]]));
                rif = r[p1[i]] + r[p2[i]];

                if(dif > (2.0*rif))
                {
                    if(xr[p1[i]]<0)
                    {
                        hp = (xr[num] - xr[num0+1]) + 0.5*(xr[num]-xr[num-1]);
                        rA = xr[p1[i]] + hp;
                        rB = xr[p2[i]] - hp;
                    }
                    else
                    {
                        hp = (xr[num] - xr[num0+1]) + 0.5*(xr[num]-xr[num-1]);
                        rA = xr[p1[i]] - hp;
                        rB = xr[p2[i]] + hp;
                    }
                    fprintf(f1, "0.5 setgray \n");
                    fprintf(f1, "0.1 setlinewidth \n");
                    fprintf(f1,"newpath %f %f %f 0 360 arc\n", rA, yr[p1[i]], r[p1[i]]);
                    fprintf(f1, "stroke \n");
                    if(nubr == 0)
                    {
                        fprintf(f1, "%f %f moveto \n", (rA-r[p1[i]]), (yr[p1[i]]-0.5*r[p1[i]]));
                        fprintf(f1, "( %d )show \n", p1[i]);
                    }
                    fprintf(f1, "0.1 setlinewidth \n");
                    fprintf(f1,"newpath %f %f %f 0 360 arc\n", rB, yr[p2[i]], r[p2[i]]);
                    fprintf(f1, "stroke \n");
                    if(nubr == 0)
                    {
                        fprintf(f1, "%f %f moveto \n", (rB-r[p2[i]]), (yr[p2[i]]-0.5*r[p2[i]]));
                        fprintf(f1, "( %d )show \n", p2[i]);
                    }
                    if(f12[i]>0.0)
                    {
                        fprintf(f1, "%f %f %f %f %f l \n",rA, yr[p1[i]],xr[p2[i]], yr[p2[i]], 0.0005*f12[i]);
                        fprintf(f1, "%f %f %f %f %f l \n",xr[p1[i]], yr[p1[i]],rB, yr[p2[i]], 0.0005*f12[i]);
                    }
                    if(f12[i]<0.0)
                    {
                        fprintf(f1, "%f %f %f %f %f lr \n",rA, yr[p1[i]],xr[p2[i]], yr[p2[i]], 0.0005*f12[i]);
                        fprintf(f1, "%f %f %f %f %f lr \n",xr[p1[i]], yr[p1[i]],rB, yr[p2[i]], 0.0005*f12[i]);
                    }
                }
                else
                {
                    if(f12[i]>0.0)
                        fprintf(f1, "%f %f %f %f %f l \n",xr[p1[i]], yr[p1[i]],xr[p2[i]], yr[p2[i]],0.0005*f12[i]);
                    if(f12[i]<0.0)
                        fprintf(f1, "%f %f %f %f %f l \n",xr[p1[i]], yr[p1[i]],xr[p2[i]], yr[p2[i]],0.0005*f12[i]);
                }
            }
        }

        fclose(f1);
    }
}
}
/*****************************************************************************/
void help()
{
 fprintf(stderr,"*Erro: asc2eps [Nom du fichier d'entre]\n");
}







