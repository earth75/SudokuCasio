#include "fxlib.h"
#include "sudoku.h"


unsigned char* intToStrClk(unsigned char* c, int n);
unsigned char* intToStrCase(unsigned char* c, int n);

DISPBOX Quadrillage; // zone du quadrillage
DISPBOX Menu; //zone du menu
int temp1, temp2, temp3, temp4;//buffers
long clkbuff1, clkbuff2, time, minutes, secondes, debut; //buffer d'horloge 1&2, le temps écoulé en tick, minutes, secondes, le temps du début de la partie
unsigned char* mint, scd, nbr;
int d, x, y, r, n, gamefinished; //?, ?, ?, ?, ?, état du jeu (en cours ou fini)
int exit, restart, posx, posy, mode; //
int casex, casey, combin, rnd, rndx, rndy, result, aide, f, done, lvl;
int key, trig, carrex, carrey; //buffer de touche, ?, l'abscsse et l'ordinnée du carré utilisé
int grille[9][9]; //contient la grille affichée
int cache[9][9]; //contiient les cases à afficher
int combi[9][9]; //contient le nombre de chiffres possibles dans chaque case
int feuille[9][9]; //contient le tableau à résoudre
Possib chiffre[9][9]; //contient les différentes posibilités pour chaque case

//----------------------------------------------------------------------------
//MAIN  #################################################
//----------------------------------------------------------------------------
int AddIn_main(int isAppli, unsigned short OptionNum)
{
exit = 0;
do
{
Bdisp_AllClr_DDVRAM();
srand(time_getTicks());
init_graph();
newTableau();
PrintMini( 78, 1, "~SUDOKU~", MINI_OR); 
PrintMini( 65, 7, "1:JOUER", MINI_OR); 
PrintMini( 65, 13, "2:SOLVEUR", MINI_OR); 
key = 0;
while (key != KEY_CHAR_1 && key != KEY_CHAR_2)
{
GetKey(&key);
}
if (key == KEY_CHAR_1){mode = 1;}
else if (key == KEY_CHAR_2){mode = 2;}

if (mode == 1) 
{
Bdisp_AreaClr_VRAM(&Menu);
PrintMini( 65, 7, "GENERATION...", MINI_OR); 
Bdisp_PutDisp_DD();
SudoEngine();
PrintMini( 65, 13, "TERMINE!", MINI_OR); 
PrintMini( 65, 24, "DIFFICULTE :", MINI_OR);
PrintMini( 65, 30, "1:FACILE", MINI_OR); 
PrintMini( 65, 36, "2:MOYEN", MINI_OR); 
PrintMini( 65, 42, "3:DIFFICILE", MINI_OR); 
PrintMini( 65, 48, "4:CHUCK NORRIS", MINI_OR); 

key = 0;
while (key != KEY_CHAR_0 && key != KEY_CHAR_1 && key != KEY_CHAR_2 && key != KEY_CHAR_3)
{
GetKey(&key);
}
if (key == KEY_CHAR_0) { lvl=0; }
else if (key == KEY_CHAR_1) { lvl=1;}
else if (key == KEY_CHAR_2) {lvl=2;}
else if (key == KEY_CHAR_3) {lvl=3;}
Cacher(lvl);

gamefinished = 0;
do
{
newTableau();
DebutPartie();
Bdisp_AreaClr_VRAM(&Menu);
Bdisp_DrawLineVRAM(66, 13, 125, 13);
Bdisp_DrawLineVRAM(66, 13 , 66, 33);
Bdisp_DrawLineVRAM(66, 33, 125, 33);
Bdisp_DrawLineVRAM(125, 13, 125, 33);
Bdisp_DrawLineVRAM(68, 15, 123, 15);
Bdisp_DrawLineVRAM(68, 15, 68, 31);
Bdisp_DrawLineVRAM(68, 31, 123, 31);
Bdisp_DrawLineVRAM(123, 15, 123, 31);
PrintMini( 70, 17, "#", MINI_OVER); 
PrintMini( 66, 44, "VERIFIER", MINI_OR); 
PrintMini( 66, 51, "NOUVEAU", MINI_OR);
PrintMini( 66, 58, "MENU", MINI_OR); 
InitClk();
if (posx < 9) { Bdisp_AreaReverseVRAM(7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );  Aide();}
else { Bdisp_AreaReverseVRAM(64, 7*posy+1, 100, 7*posy+6  );}
Bdisp_PutDisp_DD();

restart = 0;
aide = 1;
do
{
controle(); 
}while (restart == 0 );

}while (gamefinished == 0);
}

else if (mode == 2)
{
  do
{
Bdisp_AreaClr_VRAM(&Menu);
 InitData();
 newTableau();

PrintMini( 66, 44, "RESOUDRE", MINI_OR); 
PrintMini( 66, 51, "NOUVEAU", MINI_OR);
PrintMini( 66, 58, "MENU", MINI_OR); 
if (posx < 9) { Bdisp_AreaReverseVRAM(7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );}
else { Bdisp_AreaReverseVRAM(64, 7*posy+1, 100, 7*posy+6  );}
Bdisp_PutDisp_DD();
do
{
controle(); 
}while (restart == 0 );

}while(gamefinished == 0);
}
}while ( exit == 0);
exit = 0;
restart = 0;
gamefinished = 0;
return 0;
}


//-----------------------------------------------------------------------------
//GRAPHICS  ##############################################
//-----------------------------------------------------------------------------

int newTableau()  //############################################
{
Bdisp_AreaClr_VRAM(&Quadrillage);
for (temp1 = 0; temp1<4; temp1++)
{
Bdisp_DrawLineVRAM( 21*temp1, 0, 21*temp1, 63);
Bdisp_DrawLineVRAM( 0, 21*temp1, 63, 21*temp1); 
} 
for (temp1 = 0; temp1 < 3; temp1++)
{

Bdisp_SetPoint_VRAM( 62, 21*temp1+7, 1);
Bdisp_SetPoint_VRAM( 62, 21*temp1+14, 1);
Bdisp_SetPoint_VRAM( 21*temp1+7, 62, 1);
Bdisp_SetPoint_VRAM( 21*temp1+14, 62, 1);

for (temp2 = 0; temp2 < 3; temp2++)
{
for (temp3 = 0; temp3 < 3; temp3++)
{
for (temp4 = 0; temp4 < 3; temp4++)
{

Bdisp_DrawLineVRAM( 7*temp3-1+21*temp1, 7*temp4+21*temp2 , 7*temp3+1+21*temp1, 7*temp4+21*temp2 );
Bdisp_DrawLineVRAM( 7*temp3+21*temp1, 7*temp4-1+21*temp2 , 7*temp3+21*temp1, 7*temp4+1+21*temp2 ); 

}
}
}
}

}


int init_graph()//#############################################
{
x=0;
y=0;
posx = 0;
posy = 0;
Quadrillage.left = 0;
Quadrillage.top = 0;
Quadrillage.right = 63;
Quadrillage.bottom = 63;
Menu.left = 64;
Menu.top = 7;
Menu.right = 127;
Menu.bottom = 63;
return 1;
}


int curseur( int x, int y)//#####################################
{
if (posx < 9)
{
Bdisp_AreaReverseVRAM(7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );
}
else
{
 Bdisp_AreaReverseVRAM(64, 7*posy+1, 100, 7*posy+6  );
}

posy = (posy + y);
if (posy <0) { posy = 8; }
if (posy >8) { posy = 0; }

posx = (posx + x);
if (posx < 0 && posy < 6) { posx = 8; }
else if (posx < 0 && posy > 5) { posx = 9; }
if (posx > 8 && posy <6) { posx = 0; }
else if (posx >9) {posx = 0; }

if (posx < 9)
{
Bdisp_AreaReverseVRAM(7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );
}
else
{
 Bdisp_AreaReverseVRAM(64, 7*posy+1, 100, 7*posy+6  );
}

Sleep(150);
}

int EffacerMenu() //########################################
{ 
Bdisp_AreaClr_VRAM(&Menu);

Bdisp_PutDisp_DD();
}





//-----------------------------------------------------------------------------
//SOLVEUR ###########################################
//-----------------------------------------------------------------------------
int rempli, places;

int Solveur()
{
  rempli = 0;
PrintMini( 86, 7, "00/81", MINI_OR); 
Bdisp_PutDisp_DD();
Sleep (150);
  for (temp1 = 0 ; temp1 < 9 ; temp1++)
  {
  for (temp2 = 0 ; temp2 < 9 ; temp2++)
  {
  chiffre[temp1][temp2].val[0] = 0;
  for (temp3 = 1; temp3 <10; temp3++)
  {
  chiffre[temp1][temp2].val[temp3] = 1;
  }}}
  
  for (temp1 = 0; temp1 < 9; temp1++)
  {
     for (temp2 = 0; temp2 < 9; temp2++)
  {
    if (feuille [temp1][temp2] != 0)
    {
   rempli ++;
    recalcprobfeuille(temp1,temp2);
  } }}

intToStrClk(&nbr, rempli);
PrintMini( 86, 7, &nbr , MINI_OVER);
 Bdisp_PutDisp_DD();
Sleep (150);

  do // boucle de résolution
  {


    for (temp1 = 0; temp1 < 9; temp1++)
  {
     for (temp2 = 1; temp2 < 10; temp2++)
  {
  
  places = 0;
    for (temp3 = 0; temp3 < 9; temp3++) //lignes
  {
    if (chiffre[temp1][temp3].val[temp2] == 1) { places++;}
  }

  if (places == 1)
  {
    temp3 = 0;
    while(chiffre[temp1][temp3].val[temp2] == 0) {temp3++;}
    feuille[temp1][temp3] = temp2;
    rempli++;
    recalcprob(temp1, temp3);
    intToStrCase(&nbr, feuille[temp1][temp3]);
    PrintMini(7*temp1+2, 7*temp3+2, &nbr, MINI_OR);
    Bdisp_PutDisp_DD();
  }
  
places = 0;
  for (temp3 = 0; temp3 < 9; temp3++) // colonnes
  {

    if (chiffre[temp3][temp1].val[temp2] == 1) { places++;}
  }

  if (places == 1)
  {
    temp3 = 0;
    while(chiffre[temp3][temp1].val[temp2] == 0)
    {temp3++;}
    feuille[temp3][temp1] = temp2;
    rempli++;
    recalcprobfeuille(temp3, temp1);
		intToStrCase(&nbr, feuille[temp3][temp1]);
		PrintMini(7*temp3+2, 7*temp1+2, &nbr, MINI_OR);
		Bdisp_PutDisp_DD();
  }
  
   places = 0;
  for (temp3 = 0; temp3 < 9; temp3++) // carrÃ©s
  {
 
    if (chiffre[(temp3 - temp3 / 3) + (temp1 - temp1 / 3)][temp3 / 3 + temp1 / 3].val[temp2] == 1) { places++;}
  }

  if (places == 1)
  {
    temp3 = 0;
    while(chiffre[(temp3 - temp3 / 3) + (temp1 - temp1 / 3)][temp3 / 3 + temp1 / 3].val[temp2] == 0)
    {temp3++;}
    grille[(temp3 - temp3 / 3) + (temp1 - temp1 / 3)][temp3 / 3 + temp1 / 3] = temp2;
    rempli++;
    recalcprobfeuille(((temp3 - temp3 / 3) + (temp1 - temp1 / 3)), (temp3 / 3 + temp1 / 3));
    intToStrCase(&nbr, feuille[(temp3 - temp3 / 3) + (temp1 - temp1 / 3)][temp3 / 3 + temp1 / 3]);
		PrintMini(7*((temp3 - temp3 / 3) + (temp1 - temp1 / 3)), 7*(temp3 / 3 + temp1 / 3)+2, &nbr, MINI_OR);
		Bdisp_PutDisp_DD();
  }
  
  }}
		intToStrClk(&nbr, rempli);
		PrintMini( 86, 7, &nbr , MINI_OVER);
 		Bdisp_PutDisp_DD();
  }while (Verificationf() == 0);
  
  } 


//-----------------------------------------------------------------------------
//Clock  ###########################################
//-----------------------------------------------------------------------------

int InitClk() // initiates timer and rand. ############################
{
debut = time_getTicks();
clkbuff1 = 0;
clkbuff2 = debut;
time = 0;
PrintMini( 86, 7, "00:00", MINI_OR); 
Bdisp_PutDisp_DD();
}

unsigned char* intToStrClk(unsigned char* c, int n) //####################
{
 d = n/10;
c[0] = d+'0';
c[1] = n-10*d+'0';
c[2] = 0;
        
return c;
}

void Refreshclk() //############################################
{
clkbuff1 = clkbuff2;
clkbuff2 = time_getTicks();

if (clkbuff2>clkbuff1) { time = time +clkbuff2-clkbuff1; }
if (clkbuff2<clkbuff1) { time = time +clkbuff2 + 11059200 - clkbuff1; }

minutes = time/(7680);
secondes = (time - (minutes*7680))/128;
intToStrClk(&mint, minutes);
intToStrClk(&scd, secondes);

PrintMini( 86, 7, &mint , MINI_OVER);
PrintMini( 97, 7, &scd, MINI_OVER);
if (minutes >= 60) { PrintMini( 71, 7, "TOO SLOW! XD", MINI_OVER); }

Bdisp_PutDisp_DD();
}




//-----------------------------------------------------------------------------
//GAME ENGINE  ###########################################
//-----------------------------------------------------------------------------




int SudoEngine()  //###########################################
{

do
{
InitData();
Generation();
done = Verification();
}while(done == 0);

return 1;
}

int Verification() //##########################################
{
f = 1;
for (temp1 = 0 ; temp1 < 9 ; temp1++)
{
for (temp2 = 0 ; temp2 < 9 ; temp2++)
{
if (grille[temp1][temp2] == 0) 
{
f = 0;
}
}
}

return f;
}


int Verificationf() //##########################################
{
f = 1;
for (temp1 = 0 ; temp1 < 9 ; temp1++)
{
for (temp2 = 0 ; temp2 < 9 ; temp2++)
{
if (feuille[temp1][temp2] == 0) 
{
f = 0;
}
}
}

return f;
}


int Generation()  //###########################################
{

f =1;

while (f == 1)
{

f = 0;
combin = 10;
 for (temp2 = 0 ; temp2 < 9 ; temp2++)
  {
  for (temp1 = 0 ; temp1 < 9 ; temp1++)
  {
  if (combi[temp1][temp2] < combin && combi[temp1][temp2] > 0)
  {
  combin = combi[temp1][temp2];
  casex = temp1;
  casey = temp2;
  f = 1;
   }}}

if (f == 1)
{

do 
{
rnd = rand() % 9 + 1;
}
while (chiffre[casex][casey].val[rnd] == 0);

grille[casex][casey] = rnd;
recalcprob (casex, casey);
}}
return 1;
}


int tempc1, tempc2;

int recalcprobfeuille(casex, casey) //###################################
{
if (feuille[casex][casey] > 0)
{
for (tempc1 =0; tempc1 <9; tempc1++)
{
tempc2 = tempc1 + 1;
if(chiffre[casex][casey].val[tempc2] != 0 ) { chiffre[casex][casey].val[tempc2] = 0; combi[casex][casey]--; }
if(chiffre[casex][tempc1].val[feuille[casex][casey]] != 0 ) { chiffre[casex][tempc1].val[feuille[casex][casey]] = 0; combi[casex][tempc1]--; }
if(chiffre[tempc1][casey].val[feuille[casex][casey]] != 0 ) { chiffre[tempc1][casey].val[feuille[casex][casey]] = 0; combi[tempc1][casey]--; }
}

carrex = 3 * (casex / 3);
carrey = 3 * (casey / 3);

for (tempc1 = 0; tempc1 < 3; tempc1++)
{
for (tempc2 = 0; tempc2 < 3; tempc2++)
{

if (chiffre[tempc1 + carrex][tempc2 + carrey].val[feuille[casex][casey]] != 0 )
{
chiffre[tempc1 + carrex][tempc2 + carrey].val[feuille[casex][casey]] = 0;
combi[tempc1 + carrex][tempc2 + carrey]--;
}
}}
}
}

int recalcprob(casex, casey) //#########################################"
{
if (grille[casex][casey] > 0)
{
for (tempc1 =0; tempc1 <9; tempc1++)
{
tempc2 = tempc1 + 1;
if(chiffre[casex][casey].val[tempc2] != 0 ) { chiffre[casex][casey].val[tempc2] = 0; combi[casex][casey]--; }
if(chiffre[casex][tempc1].val[grille[casex][casey]] != 0 ) { chiffre[casex][tempc1].val[grille[casex][casey]] = 0; combi[casex][tempc1]--; }
if(chiffre[tempc1][casey].val[grille[casex][casey]] != 0 ) { chiffre[tempc1][casey].val[grille[casex][casey]] = 0; combi[tempc1][casey]--; }
}

carrex = 3 * (casex / 3);
carrey = 3 * (casey / 3);

for (tempc1 = 0; tempc1 < 3; tempc1++)
{
for (tempc2 = 0; tempc2 < 3; tempc2++)
{

if (chiffre[tempc1 + carrex][tempc2 + carrey].val[grille[casex][casey]] != 0 )
{
chiffre[tempc1 + carrex][tempc2 + carrey].val[grille[casex][casey]] = 0;
combi[tempc1 + carrex][tempc2 + carrey]--;
}
}}
}
}

int Cacher(lvl) //############################################
{
if (lvl != 0)
{
for (temp1 = 0; temp1 < (15*lvl+15); temp1++)
{

do
{
rndx = rand() % 9 ;
rndy = rand() % 9;
}while (cache[rndx][rndy] == 0);

cache[rndx][rndy] = 0;
}
}
}
int InitData() //##############################################
{
for (temp1 = 0 ; temp1 < 9 ; temp1++)
{
for (temp2 = 0 ; temp2 < 9 ; temp2++)
{
grille[temp1][temp2] = 0;
cache[temp1][temp2] = 1;
combi[temp1][temp2] = 9;
}
}

for (temp1 = 0 ; temp1 < 9 ; temp1++)
{
for (temp2 = 0 ; temp2 < 9 ; temp2++)
{
chiffre[temp1][temp2].val[0] = 0;
for (temp3 = 1; temp3 <10; temp3++)
{
chiffre[temp1][temp2].val[temp3] = 1;
}
}
}
return 1;
}

int DebutPartie() //##############################
{
for (temp1 = 0; temp1 < 9; temp1++)
{
for (temp2 = 0; temp2 < 9; temp2++)
{
feuille[temp1][temp2] = cache[temp1][temp2] * grille[temp1][temp2];

if(feuille[temp1][temp2] != 0)
{
intToStrCase(&nbr, feuille[temp1][temp2]);
PrintMini(7*temp1+2, 7*temp2+2, &nbr, MINI_OR);
}
}
}

for (temp1 = 0 ; temp1 < 9 ; temp1++)
{
for (temp2 = 0 ; temp2 < 9 ; temp2++)
{
chiffre[temp1][temp2].val[0] = 0;
for (temp3 = 1; temp3 <10; temp3++)
{
chiffre[temp1][temp2].val[temp3] = temp3;
}}}

for (temp1 = 0; temp1 < 9; temp1++)
{
for (temp2 = 0; temp2 < 9; temp2++)
{
recalcprobfeuille(temp1, temp2);
}}


return 1;
}

int Aide()
{

intToStrCase(&nbr, chiffre[posx][posy].val[1]);
PrintMini(77, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[2]);
PrintMini(85, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[3]);
PrintMini(93, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[4]);
PrintMini(101, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[5]);
PrintMini(109, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[6]);
PrintMini(117, 17, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[7]);
PrintMini(77, 25, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[8]);
PrintMini(85, 25, &nbr, MINI_OVER);
intToStrCase(&nbr, chiffre[posx][posy].val[9]);
PrintMini(93, 25, &nbr, MINI_OVER);
}

int finpartie() {return 0;}

int controle()  //#################################################
{
temp1 = 0;
while (temp1 == 0 )
{
//if (IsKeyDown(KEY_CTRL_MENU)){GetKey(&key);} //pour retourner au menu j'ai pas trouv? mieux
if (mode == 1) {Refreshclk();}
temp1 = Interrupt();
}

if (posx == 9 && r == 1)
{
 if (posy == 6)
 {
   if (mode == 1)
   {
     if (finpartie() == 0)
     {
      Bdisp_AreaClr_VRAM(&Menu);
      PrintMini( 65, 36, "FAUX!", MINI_OR);
      key = 0;
      Sleep (150);
      PrintMini( 66, 51, "EXE POUR", MINI_OR);
      PrintMini( 66, 58, "RECOMMENCER", MINI_OR); 
      Bdisp_PutDisp_DD();
      while (key != KEY_CTRL_EXE) { GetKey(&key); }
     }
     restart = 1;
   }
   else if (mode ==2)
   {
      Solveur();
      key = 0;
      Sleep (150);
      PrintMini( 66, 51, "EXE POUR", MINI_OR);
      PrintMini( 66, 58, "RECOMMENCER", MINI_OR); 
      Bdisp_PutDisp_DD();
      while (key != KEY_CTRL_EXE) { GetKey(&key); }
      restart = 1;
   }
 }
 if (posy == 7)
 {
  restart = 1;
  gamefinished = 1;
 }
 if (posy == 8)
 {
  restart = 1;
  gamefinished = 1;
  exit = 1;
 }
}
 
if ( x != 0 || y != 0)
{
curseur( x, y);

}

if (aide == 1 &&  posx<9) {Aide();}


if ( n>0 && n<10  && posx < 9 && ( (cache[posx][posy] == 0 && mode == 1) || mode == 2 ) )
{
feuille[posx][posy] = n;
recalcprobfeuille(posx, posy);
intToStrCase(&nbr, feuille[posx][posy]);
Bdisp_AreaReverseVRAM( 7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );
for (temp1 = 0; temp1 <5; temp1++)
{
Bdisp_SetPoint_VRAM( 7*posx+2, 7*posy+2+temp1, 0);
Bdisp_SetPoint_VRAM( 7*posx+3, 7*posy+2+temp1, 0);
Bdisp_SetPoint_VRAM( 7*posx+4, 7*posy+2+temp1, 0);
}
PrintMini( 7*posx+2, 7*posy+2, &nbr, MINI_OR);
Bdisp_AreaReverseVRAM( 7*posx+1, 7*posy+1, 7*posx+6, 7*posy+6  );
}

if ( n == 10 && posx < 9 && ( (cache[posx][posy] == 0 && mode == 1) || mode == 2 ))
{
feuille[posx][posy] = 0;
for (temp1 = 0; temp1 <5; temp1++)
{
Bdisp_SetPoint_VRAM( 7*posx+2, 7*posy+2+temp1, 1);
Bdisp_SetPoint_VRAM( 7*posx+3, 7*posy+2+temp1, 1);
Bdisp_SetPoint_VRAM( 7*posx+4, 7*posy+2+temp1, 1);
}
}
Bdisp_PutDisp_DD();
return 1;
}

int Interrupt()  //##################################################
{
x=0;
y=0;
n = 0;
r = 0;
x=x+IsKeyDown(KEY_CTRL_RIGHT);
x=x-IsKeyDown(KEY_CTRL_LEFT);
y=y+IsKeyDown(KEY_CTRL_DOWN);
y=y-IsKeyDown(KEY_CTRL_UP);
if (IsKeyDown(KEY_CHAR_1) != 0){n = IsKeyDown(KEY_CHAR_1);}
if (IsKeyDown(KEY_CHAR_2) != 0){n = 2*IsKeyDown(KEY_CHAR_2);}
if (IsKeyDown(KEY_CHAR_3) != 0){n = 3*IsKeyDown(KEY_CHAR_3);}
if (IsKeyDown(KEY_CHAR_4) != 0){n = 4*IsKeyDown(KEY_CHAR_4);}
if (IsKeyDown(KEY_CHAR_5) != 0){n = 5*IsKeyDown(KEY_CHAR_5);}
if (IsKeyDown(KEY_CHAR_6) != 0){n = 6*IsKeyDown(KEY_CHAR_6);}
if (IsKeyDown(KEY_CHAR_7) != 0){n = 7*IsKeyDown(KEY_CHAR_7);}
if (IsKeyDown(KEY_CHAR_8) != 0){n = 8*IsKeyDown(KEY_CHAR_8);}
if (IsKeyDown(KEY_CHAR_9) != 0){n = 9*IsKeyDown(KEY_CHAR_9);}
if (IsKeyDown(KEY_CHAR_0) != 0){n = 10*IsKeyDown(KEY_CHAR_0);}
if (IsKeyDown(KEY_CTRL_EXE) != 0){r = IsKeyDown(KEY_CTRL_EXE);}

if (x == 0 && y == 0 && n == 0 && r == 0) { return 0; }
else { return 1; }
}

unsigned char* intToStrCase(unsigned char* c, int n) //##################
{
if(n>9)
 {
return 0;
}
else
 {
c[0] = n+'0';
c[1] = 0;
return c;
}

}


//###############################################################


//****************************************************************************
//**************                                                                             ****************
//**************                 Notice!                                               ****************
//**************                                                                             ****************
//**************Please do not change the following source. ****************
//**************                                                                             ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section
