#include <iostream>
#include <vector>
#include <string>
#include <fstream> // pour les lecture-ecriture dans les fichiers

using namespace std;

typedef struct {
  string nom;
  vector<vector<int>> grille;
} Grille;

typedef struct {
  size_t lig;
  size_t col;
} Indices;


// insérer ici les fonctions que vous appelez avant que leurs definitions
// apparaissent dans le fichier
void imprime(const Grille &g); // definie en fin de fichier.


/* Renvoie true ssi la grille a la bonne forme et ne contient que des valeurs
 * de 0 à 9
 */

 /** Fonction bienFormee
 *@param g une grille de Sudoku
 *@return true si la grille à la bonne forme et que des valeurs acceptées
 **/
bool bienFormee(const Grille &g) {
    if (g.grille.size()!=9){
        return false;
    }
    else{
        for(int i=0;i<g.grille.size();i++){
            if(g.grille.at(i).size()!=9){
                return false;
            }
        }
    }
    for (int i=0;i<g.grille.size();i++){
        for(int j=0;j<g.grille.at(i).size();j++){
            if(g.grille.at(i).at(j)>9 or g.grille.at(i).at(j)<0){
                return false;
            }
        }
    }
  return true;
}


/* Verifie si la grille est valide: chaque valeur de 1 à 9 apparait au plus
 * une fois dans chque ligne, colonne et région
 */

/** Fonction valcol
 *@param g une grille de Sudoku
 *@return true si les colones de la grille sont valides
 **/
bool valcol(const Grille &g){
    vector<int> col;
    col=vector<int>(10);
    for(int i=0;i<g.grille.size();i++){
        for(int j=0;j<col.size();j++){
            col.at(j)=0;
        }
        for(int j=0;j<g.grille.at(i).size();j++){
            col.at(g.grille.at(j).at(i))+=1;
            if(col.at(g.grille.at(j).at(i))>1 and g.grille.at(j).at(i)!=0){
                return false;
            }
        }
    }
    return true;
}

/** Fonction vallig
 *@param g une grille de Sudoku
 *@return true si les lignes de la grille sont valides
 **/
bool vallig(const Grille &g){
    vector<int> lig;
    lig=vector<int>(10);
    for(int i=0;i<g.grille.size();i++){
        for(int j=0;j<lig.size();j++){
            lig.at(j)=0;
        }
        for(int j=0;j<g.grille.at(i).size();j++){
            lig.at(g.grille.at(i).at(j))+=1;
            if(lig.at(g.grille.at(i).at(j))>1 and g.grille.at(i).at(j)!=0){
                return false;
            }
        }
    }
    return true;
}

/** Fonction val1car
 *@param g une grille de Sudoku
 *@param n le numéro du carré
 *@return true si le carré de la grille est valide
 **/
bool val1car(const Grille &g,int n){
    int i,j,l,c;
    vector<int> nbr;
    nbr=vector<int>(10);
    for(int k=0;k<nbr.size();k++){
        nbr.at(k)=0;
    }
    switch (n){
    case 1: i=0;
            j=0;
            break;
    case 2: i=3;
            j=0;
            break;
    case 3: i=6;
            j=0;
            break;
    case 4: i=0;
            j=3;
            break;
    case 5: i=3;
            j=3;
            break;
    case 6: i=6;
            j=3;
            break;
    case 7: i=0;
            j=6;
            break;
    case 8: i=3;
            j=6;
            break;
    case 9: i=6;
            j=6;
            break;
    }
    c=i+3;
    l=j+3;
    for(int k=i;k<c;k++){
        for(int m=j;m<l;m++){
            nbr.at(g.grille.at(k).at(m))+=1;
            if(nbr.at(g.grille.at(k).at(m))>1 and g.grille.at(k).at(m)!=0){
                return false;
            }
        }
    }
    return true;
}

/** Fonction valcar
 *@param g une grille de Sudoku
 *@return true si les carrés de la grille sont valides
 **/
bool valcar(const Grille &g){
    for(int i=1;i<10;i++){
        if(val1car(g,i)==false){
            return false;
        }
    }
    return true;
}

/** Fonction valide
 *@param g une grille de Sudoku
 *@return true si les carré, les colonnes et les lignes de la grille sont valides
 **/
bool valide(const Grille &g) {
    if(valcol(g) and vallig(g) and valcar(g)){
        return true;
    }
    return false;
}




/* renvoie le vecteur des valeurs disponibles pour le couple d'indices ind,
 * compte-tenu des valeurs placées sur sa ligne, colonne et sa région
*/
/** Fonction valide
 *@param g une grille de Sudoku
 *@param ind la position dont on veut connaitre les valeurs possibles
 *@return le vecteur de toutes les possibilités à cette case
 **/
vector<int> possibles(const Grille &g, Indices ind) {
    vector<int> res;
    res=vector<int>(0);
    for(int i=1;i<10;i++){
        Grille gri=g;
        gri.grille.at(ind.lig).at(ind.col)=i;
        if(valide(gri)){
            res.push_back(i);
        }
    }
    return res;
}


/* recoit en argument le vecteur des valeurs possibles pour la case
 * d'indices ij. Parcourt les valeurs en cherchant si pour l'une d'entre
 * elles il existe un seul emplacement valide dans la région.
 * Dans ce cas, stocke la valeur dans la case et renvoie true.
 * Renvoie false si pour chaque valeur possible il existe au moins deux
 * cases où on pourrait potentiellement la ranger. Dans ce cas ne
 * modifie pas la grille.
 */

 /** Fonction forceli
 *@param g une grille de Sudoku
 *@param ij la position dont on veut connaitre si il existe une valeur possible où cette case est la seule sur la ligne à l'avoir
 *@param vpossibles vecteur des possibles à la case ij
 *@return l'unique valeur probable qu'elle possède, si non -1
 **/
int forceli(const Grille &g, Indices ij, vector<int> vpossibles){
    int r;
    vector<int>compte;
    compte=vector<int> (vpossibles.size());
    if(g.grille.at(ij.lig).at(ij.col)==0){
        for(int j=0;j<g.grille.at(ij.lig).size();j++){
                if(j!=ij.col){
                vector<int>tmp;
                if(g.grille.at(ij.lig).at(j)!=0){
                    tmp=vector<int>(1);
                    tmp.at(0)=g.grille.at(ij.lig).at(j);
                }
                else{
                    Indices t={ij.lig,j};
                    tmp=possibles(g,t);
                }
                for(int k=0;k<vpossibles.size();k++){
                    for(int l=0;l<tmp.size();l++){
                        if(vpossibles.at(k)==tmp.at(l)){
                            compte.at(k)+=1;
                        }
                    }
                }
            }
        }
        int pas=0;
        for(int i=0;i<compte.size();i++){
            if(compte.at(i)==0){
                r=vpossibles.at(i);
                pas+=1;
            }
        }
        if(pas==1){
            return r;
        }
    }
    return -1;
}

/** Fonction forceco
 *@param g une grille de Sudoku
 *@param ij la position dont on veut connaitre si il existe une valeur possible où cette case est la seule sur la colonne à l'avoir
 *@param vpossibles vecteur des possibles à la case ij
 *@return l'unique valeur probable qu'elle possède, si non -1
 **/
int forceco(const Grille &g, Indices ij, vector<int> vpossibles){
    int r;
    vector<int>compte;
    compte=vector<int> (vpossibles.size());
    if(g.grille.at(ij.lig).at(ij.col)==0){
        for(int i=0;i<g.grille.at(ij.lig).size();i++){
                if(i!=ij.lig){
                vector<int>tmp;
                if(g.grille.at(i).at(ij.col)!=0){
                    tmp=vector<int>(1);
                    tmp.at(0)=g.grille.at(i).at(ij.col);
                }
                else{
                    Indices t={i,ij.col};
                    tmp=possibles(g,t);
                }
                for(int k=0;k<vpossibles.size();k++){
                    for(int l=0;l<tmp.size();l++){
                        if(vpossibles.at(k)==tmp.at(l)){
                            compte.at(k)+=1;
                        }
                    }
                }
            }
        }
        int pas=0;
        for(int i=0;i<compte.size();i++){
            if(compte.at(i)==0){
                r=vpossibles.at(i);
                pas+=1;
            }
        }
        if(pas==1){
            return r;
        }
    }
    return -1;
}

/** Fonction forceca
 *@param g une grille de Sudoku
 *@param ij la position dont on veut connaitre si il existe une valeur possible où cette case est la seule sur le carré à l'avoir
 *@param vpossibles vecteur des possibles à la case ij
 *@return l'unique valeur probable qu'elle possède, si non -1
 **/
int forceca(const Grille &g, Indices ij, vector<int> vpossibles){
    int r,i,j,c,l;
    vector<int>compte;
    compte=vector<int>(vpossibles.size());
    if(g.grille.at(ij.lig).at(ij.col)==0){
        if(ij.lig>=0 and ij.lig<3){
            i=0;
        }
        else if(ij.lig>=3 and ij.lig<6){
            i=3;
        }
        else if(ij.lig>=6 and ij.lig<9){
            i=6;
        }

        if(ij.col>=0 and ij.col<3){
            j=0;
        }
        else if(ij.col>=3 and ij.col<6){
            j=3;
        }
        else if(ij.col>=6 and ij.col<9){
            j=6;
        }
        c=i+3;
        l=j+3;
        for(int k=i;k<c;k++){
            for(int m=j;m<l;m++){
                if(k!=ij.lig or m!=ij.col){
                    vector<int>tmp;
                    if(g.grille.at(k).at(m)!=0){
                        tmp=vector<int>(1);
                        tmp.at(0)=g.grille.at(k).at(m);
                    }
                    else{
                        Indices t={k,m};
                        tmp=possibles(g,t);
                    }
                    for(int v=0;v<vpossibles.size();v++){
                        for(int t=0;t<tmp.size();t++){
                            if(vpossibles.at(v)==tmp.at(t)){
                                compte.at(v)+=1;
                            }
                        }
                    }
                }
            }
        }
        int pas=0;
        for(int w=0;w<compte.size();w++){
            if(compte.at(w)==0){
                r=vpossibles.at(w);
                pas+=1;
            }
        }
        if(pas==1){
            return r;
        }
    }
    return -1;
}

/** Fonction force
 *@param g une grille de Sudoku
 *@param ij la position dont on veut connaitre si il existe une valeur possible où cette case est la seule sur la ligne à l'avoir
 *@param vpossibles vecteur des possibles à la case ij
 *@return true si on peut modifier de manière sure la valeur de la case
 **/
bool force(Grille &g, Indices ij, vector<int> vpossibles) {
    if(vpossibles.size()==1){
        cout<<"Case("<<ij.lig+1<<";"<<ij.col+1<<") 0->"<<vpossibles.at(0)<<endl;
        g.grille.at(ij.lig).at(ij.col)=vpossibles.at(0);
        return true;
    }
    else if (forceli(g,ij,vpossibles)!=-1){
        cout<<"Case("<<ij.lig+1<<";"<<ij.col+1<<") 0->"<<forceli(g,ij,vpossibles)<<endl;
        g.grille.at(ij.lig).at(ij.col)=forceli(g,ij,vpossibles);
        return true;
    }
    else if (forceco(g,ij,vpossibles)!=-1){
        cout<<"Case("<<ij.lig+1<<";"<<ij.col+1<<") 0->"<<forceco(g,ij,vpossibles)<<endl;
        g.grille.at(ij.lig).at(ij.col)=forceco(g,ij,vpossibles);
        return true;
    }
    else if (forceca(g,ij,vpossibles)!=-1){
        cout<<"Case("<<ij.lig+1<<";"<<ij.col+1<<") 0->"<<forceca(g,ij,vpossibles)<<endl;
        g.grille.at(ij.lig).at(ij.col)=forceca(g,ij,vpossibles);
        return true;
    }
    else{
        return false;
    }
}

/** Fonction userSuggest
*@param g une grille de Sudoku
*@return true si l'utilisateur choisir manuellement une valeur
**/
bool userSuggest(Grille &g){
    char alors;
    int lig,col;
    cout<<"Grille "<<g.nom<<endl<<"Voulez-vous suggerer une valeur ? (o/n)"<<endl;
    do{
        cin>>alors;
        if(alors!='o' and alors!='n'){
            cout<<"Erreur : choisir oui(o) ou non(n)"<<endl;
        }
    }while(alors!='o' and alors!='n');
    if(alors=='o'){

        cout<<"Quelle case voulez vous remplir ?"<<endl;
        int cn=0;
        do{
            if(cn>0){
                    cout<<"Erreur : rentrer une position libre"<<endl;
                }
                cn+=1;
            int cl=0,cr=0;
            cout<<"Ligne : ";
            do{
                if(cl>0){
                    cout<<"Erreur : rentrer une ligne valide"<<endl;
                }
                cl+=1;
                cin>>lig;
            }while(lig>9 or lig<1);
            cout<<"Colonne : ";
            do{
                if(cr>0){
                    cout<<"Erreur : rentrer une colonne valide"<<endl;
                }
                cr+=1;
                cin>>col;
            }while(col>9 or col<1);
        }while(g.grille.at(lig-1).at(col-1)!=0);
        int val,cv=0;
        do{
            if(cv>0){
                cout<<"Erreur : la grille n'est plus valide"<<endl;
            }
            cv+=1;
            Indices t={lig-1,col-1};
            vector<int>pos=possibles(g,t);
            cout<<"Quelle valeur ?"<<endl;
            for(int i=0;i<pos.size();i++){
                cout<<pos.at(i)<<'|';
            }
            cout<<endl<<"Valeur : ";
            do{
                cin>>val;
                if(val>9 or val<1){
                    cout<<"Erreur : rentrer une valeur valide"<<endl;
                }
            }while(val>9 or val<1);
            g.grille.at(lig-1).at(col-1)=val;
        }while(valide(g)==false);
        return true;

    }
    return false;
}

/** Fonction possible
*@param g une grille de Sudoku
*@return true s'il est encore possible de jouer
**/
bool possibilites(Grille g){
    for(int i=0;i<g.grille.size();i++){
                    for(int j=0;j<g.grille.at(i).size();j++){
                        Indices t={i,j};
                        if(g.grille.at(i).at(j)==0){
                            if(possibles(g,t).size()==0){
                                return false;
                            }
                        }
                    }
                }
            return true;
}

/** Fonction joue
*@param g une grille de Sudoku
*@return true si la grille est résolu
**/
bool joue(Grille &g) {
    bool res = false, change=true;
    if (bienFormee(g)){
        if(valide(g)){
            cout << "Debut du traitement de la grille " << g.nom << endl;
            imprime(g);
            do{
                res=true;
                for(int i=0;i<g.grille.size();i++){
                    for(int j=0;j<g.grille.at(i).size();j++){
                        if(g.grille.at(i).at(j)==0){
                            res=false;
                        }
                    }
                }
                if(res){
                    cout<<"Grille "<<g.nom<<" resolue"<<endl;
                    change=false;
                }
                while(change and possibilites(g)){
                    change=false;
                    for(int i=0;i<g.grille.size();i++){
                        for(int j=0;j<g.grille.at(i).size();j++){
                            if(g.grille.at(i).at(j)==0){
                                Indices t={i,j};
                                vector<int>pos=possibles(g,t);
                                if(force(g,t,pos)){
                                    change=true;
                                }
                            }
                        }
                    }
                    imprime(g);
                }
                res=true;
                for(int i=0;i<g.grille.size();i++){
                    for(int j=0;j<g.grille.at(i).size();j++){
                        if(g.grille.at(i).at(j)==0){
                            res=false;
                        }
                    }
                }
                if(res){
                    cout<<"Grille "<<g.nom<<" resolue"<<endl;
                    change=false;
                }
                if(change==false and res==false and possibilites(g)){
                    if(userSuggest(g)){
                        change=true;
                    }
                    if (change){
                        imprime(g);
                    }
                }


            }while(possibilites(g) and change);
        }
    }
    if(res==false){
        cout<<endl<<"Grille "<<g.nom<<" non-resolue";
    }
    return res;
}


void test4() {
  Grille g = { "test4",
	       {
		 { 9, 2, 0, 0, 0, 0, 0, 6, 0 },
		 { 5, 3, 0, 0, 0, 0, 0, 9, 0 },
		 { 8, 0, 0, 0, 4, 7, 0, 0, 0 },
		 { 0, 0, 0, 0, 1, 0, 0, 0, 0 },
		 { 3, 5, 0, 0, 0, 0, 0, 0, 0 },
		 { 0, 6, 0, 0, 0, 0, 8, 5, 0 },
		 { 0, 0, 0, 5, 0, 6, 0, 0, 0 },
		 { 0, 0, 9, 8, 0, 0, 0, 0, 4 },
		 { 0, 0, 0, 3, 0, 0, 0, 0, 2 }
	       }
  };
  joue(g);
}

void test3() {
  Grille g = { "test3",
	       {
		 { 0, 0, 0, 0, 0, 0, 5, 0, 4 },
		 { 0, 0, 8, 0, 9, 3, 0, 0, 0 },
		 { 4, 0, 0, 0, 0, 1, 0, 0, 0 },
		 { 7, 0, 6, 0, 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 0, 0, 0, 1, 0, 8 },
		 { 0, 0, 5, 0, 1, 0, 0, 0, 2 },
		 { 0, 8, 0, 2, 0, 0, 0, 0, 0 },
		 { 0, 9, 0, 0, 0, 0, 0, 3, 0 },
		 { 0, 0, 0, 5, 0, 6, 0, 7, 0 }
	       }
  };
  joue(g);
  // Aide:
  // 7 -> (2, 9)
  // 5 -> (2, 2)
  // 6 -> (6, 4)

}


void test2() {
  Grille g = { "test2",
	       { { 0, 5, 0, 0, 0, 0, 0, 0, 3 },
		 { 0, 0, 0, 1, 0, 4, 0, 0, 0 },
		 { 0, 0, 0, 0, 0, 8, 0, 5, 6 },
		 { 6, 0, 1, 0, 0, 0, 5, 0, 0 },
		 { 0, 0, 0, 0, 7, 6, 4, 0, 0 },
		 { 0, 4, 7, 0, 0, 3, 8, 0, 0 },
		 { 3, 0, 0, 0, 0, 2, 0, 9, 0 },
		 { 2, 0, 9, 0, 0, 0, 0, 0, 0 },
		 { 0, 0, 0, 4, 0, 1, 0, 0, 0 }
	       }
  };
  joue(g);
  /* par exemple pour aider:
     1 -> (3, 7)
     8 -> (1, 3)
     5 -> (5, 4)
     8 -> (5, 1)
  */
}


void test1() {
  Grille g = { "test1",
	       { { 0, 0, 0, 0, 0, 4, 3, 0, 0 },
		 { 0, 0, 0, 0, 6, 5, 0, 0, 2 },
		 { 1, 2, 0, 0, 0, 0, 4, 0, 9 },
		 { 6, 0, 0, 0, 0, 0, 1, 3, 5 },
		 { 3, 9, 0, 0, 2, 0, 0, 0, 8 },
		 { 0, 0, 8, 0, 7, 0, 0, 2, 0 },
		 { 0, 8, 0, 1, 4, 9, 5, 0, 0 },
		 { 5, 0, 7, 0, 0, 3, 0, 0, 0 },
		 { 0, 0, 4, 0, 0, 0, 6, 8, 0 }
	       }
  };
  joue(g);  // pas besoin d'aide supplementaire
}

/* quelques grilles prédéfinies */
int main() {
  // Les grilles G2 à G3 peuvent se résoudre sans aide de l'utilisateur
  Grille g2 = { "G2",
		{ { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
		  { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
		  { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
		  { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
		  { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
		  { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
		  { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
		  { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
		  { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
		}};
  Grille g3 = { "G3",
		{ { 0, 2, 7, 0, 6, 5, 0, 4, 9 },
		  { 3, 5, 0, 0, 0, 9, 0, 0, 8 },
		  { 8, 9, 0, 0, 0, 0, 0, 0, 0 },
		  { 2, 0, 0, 0, 9, 0, 0, 0, 0 },
		  { 0, 0, 0, 6, 8, 7, 0, 0, 0 },
		  { 0, 0, 0, 0, 3, 0, 0, 0, 5 },
		  { 0, 0, 0, 0, 0, 0, 0, 6, 3 },
		  { 7, 0, 0, 2, 0, 0, 0, 8, 4 },
		  { 4, 8, 0, 9, 7, 0, 1, 5, 0}
		}};
  Grille g4 = { "G4",
		{ { 8, 0, 0, 0, 0, 0, 1, 0, 0 },
		  { 0, 3, 4, 0, 2, 0, 6, 0, 0 },
		  { 0, 9, 0, 4, 0, 0, 0, 0, 2 },
		  { 5, 1, 0, 0, 4, 2, 9, 6, 0 },
		  { 0, 0, 0, 0, 6, 0, 0, 0, 0 },
		  { 0, 6, 7, 1, 5, 0, 0, 2, 3 },
		  { 9, 0, 0, 0, 0, 4, 0, 3, 0 },
		  { 0, 0, 2, 0, 1, 0, 7, 4, 0 },
		  { 0, 0, 8, 0, 0, 0, 0, 0, 1 }
		}};
  // cette grille admet deux solutions différentes mais demande une aide
  // de l'utilisateur si on se limite aux deux autres stratégies définies
  Grille g5 = { "G5",
		{ { 0, 0, 8, 0, 0, 0, 2, 0, 0 },
		  { 0, 0, 4, 3, 0, 0, 0, 0, 1 },
		  { 0, 5, 0, 0, 0, 7, 0, 8, 9 },
		  { 0, 0, 5, 7, 4, 0, 0, 0, 2 },
		  { 0, 0, 0, 1, 0, 3, 0, 0, 0 },
		  { 4, 0, 0, 0, 2, 0, 6, 0, 0 },
		  { 2, 6, 0, 5, 0, 0, 0, 7, 0 },
		  { 5, 0, 0, 0, 0, 2, 9, 0, 0 },
		  { 0, 0, 1, 0, 0, 0, 5, 0, 0 }
		}};
  joue(g2);
  joue(g3);
  joue(g4);
  joue(g5);
  test1();
  test2();
  test3();
  test4();
  return 0;
}

		// Fonctions fournies, rien à modifier a priori.


/* Imprime une ligne de séparation formée du caractère passé en argument */
void imprimeL(char c) {
  cout << ' ';
  for(size_t i = 0; i < 9; i += 1) { cout << c << c << c << c; }
  /* pour prendre en compte les espaces ajoutés prèsb des '+' */
  cout << c << c << c << c << c << c;
  cout << endl;
}

/* suppose la grille bien formee: 9 lignes de 9 colonnes de chiffres de 0 à 9 */
void imprime(const Grille &g) {
  for(size_t i = 0; i < 9; i += 1) {
    imprimeL(i % 3 == 0 ? '+' : '-'); // ligne d'en-tete
    for(size_t j = 0; j < 9; j += 1) {
      char c = '0' + g.grille.at(i).at(j);
      cout << (j % 3 == 0 ? " + " : " " ) << ' ' << c << ' ';
    }
    cout << "+" << endl;
  }
  imprimeL('+'); // ligne finale du bas
  cout << endl;
}
