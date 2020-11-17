#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <iomanip>

using namespace std;
#define MAX_PAISES 200

struct 
{
    unsigned short int dia;
    unsigned int cantidades[4];
}typedef tInfo; // ESTRUCTURA QUE REPRESENTA AL CAMPO "INFO" DE UN NODO

struct tNodoDia{ 
    tInfo info;
    tNodoDia* sgteDia;
}; // ESTRUCTURA QUE REPRESENTA CADA DIA DE DATOS CORRESPONDIENTES A UN PAIS Y MES

struct tsParDia 
{
    char nombrePais[21];
    unsigned short int mes;
    unsigned short int dia;
    unsigned int cantidades[4];
}; // ESTRUCTURA PARA LEER LOS REGISTROS DEL PARTE DIARIO

struct tsPais
{
    char nombrePais[21];
    char continente[21];
    unsigned int cantHabit;
}; // ESTRUCTURA PARA LEER LOS REGISTROS DE PAISES

typedef tsPais tvrPaises[MAX_PAISES]; // VECTOR DE PAISES
typedef tNodoDia** tmrPaisesMeses; // MATRIZ DE PAISES MESES

void Abrir(ifstream&,ifstream&);
void Cerrar(ifstream&,ifstream&);
void InicMat(tNodoDia**&);
void ProcPaises(ifstream&,tsPais[],int&);
void ProcParteDiario(ifstream&,tNodoDia**&,tsPais[],int);
void Listado(tNodoDia**,tsPais[],int);
void push(tNodoDia*&,tsParDia);
void pushHead(tNodoDia*&, tsParDia);
void pushMid(tNodoDia*&,tsParDia);
void pushNode(tNodoDia*&, tsParDia);
void Acumular(tNodoDia*&,tsParDia);
tNodoDia* ExisteNodo(tNodoDia*&,tsParDia);
void OrdXBur(tsPais[],int);
bool BusqBin(tsPais[],char[],int, int&);
bool compare(char[],char[]);



int main()
{
    ifstream Paises, ParteDiario;
    tmrPaisesMeses mPaisesMeses;
    tvrPaises vPaises;
    int cantPaises = 0;
   
    Abrir(Paises,ParteDiario);
    InicMat(mPaisesMeses);
    ProcPaises(Paises,vPaises,cantPaises);
    ProcParteDiario(ParteDiario,mPaisesMeses,vPaises,cantPaises);
    Listado(mPaisesMeses,vPaises,cantPaises);
    Cerrar(Paises,ParteDiario);
    
    return 0;
}

void Abrir(ifstream &arPaises, ifstream &arParDia){
    arPaises.open("Paises.dat",ios::binary);
    arParDia.open("ParteDiario.dat",ios::binary);
}

void Cerrar(ifstream &arPaises, ifstream &arParDia){
    arPaises.close();
    arParDia.close();
}

void InicMat(tNodoDia** &matriz){
    matriz = new tNodoDia*[200];
    for(int i=0;i<MAX_PAISES;i++){
       matriz[i] = new tNodoDia[12];
    }

    for(int i=0;i<MAX_PAISES;i++){
        for(int j=0;j<12;j++){

            matriz[i][j].sgteDia = nullptr;
        }
    }
}

void ProcPaises(ifstream &arPaises, tsPais tvrPaises[], int &cantPaises){
    tsPais registro;
    arPaises.read((char*)&registro,sizeof(registro));
    while(arPaises.good()){
        strcpy(tvrPaises[cantPaises].nombrePais,registro.nombrePais);
        strcpy(tvrPaises[cantPaises].continente,registro.continente);
        tvrPaises[cantPaises].cantHabit = registro.cantHabit;
        cantPaises++;
        arPaises.read((char*)&registro,sizeof(registro));
    }
    OrdXBur(tvrPaises,cantPaises);
}


void ProcParteDiario(ifstream &arParDia, tNodoDia**& matriz, tsPais paises[], int cantPaises){
    tsParDia registro;
    arParDia.read((char*)&registro,sizeof(registro));

    while(arParDia.good()){
        int index;
        if(BusqBin(paises,registro.nombrePais,cantPaises,index)){
            
            push(matriz[index][registro.mes-1].sgteDia,registro);
          
        }
        
        arParDia.read((char*)&registro,sizeof(registro));
    }
}

void Listado(tNodoDia** matriz, tsPais paises[], int cantPaises){
    
    cout<<setw(50)<<"======================================================="<<endl;
    cout<<setw(50)<<"Listado de casos Hisopados, Infectados, Recuperados y fallecidos"<<
    "ordenado por Pais, Mes y Dia"<<endl;
    cout<<setw(50)<<"======================================================="<<endl<<endl;
    
    for(int i=0;i<cantPaises;i++){
        cout<<"Pais: "<<paises[i].nombrePais<<"Cant.Hab.: "<<paises[i].cantHabit<<endl;
        for(int j=0;j<12;j++){
            if(matriz[i][j].sgteDia == NULL)continue;
            tNodoDia* pNodo = matriz[i][j].sgteDia;
            cout<<"Mes: "<<j+1<<setw(5)<<right
            <<"Dia"<<setw(8)<<"Hisop."<<setw(8)<<"Infec."<<setw(8)<<"Recup."<<
            setw(8) << "Fallec."<<endl;
            
            while(pNodo){
                cout<<setw(10)<<pNodo->info.dia
                <<setw(8)
                <<pNodo->info.cantidades[0]
                <<setw(8)
                <<pNodo->info.cantidades[1]
                <<setw(8)
                <<pNodo->info.cantidades[2]
                <<setw(8)
                <<pNodo->info.cantidades[3]<<endl;
                        
                pNodo = pNodo->sgteDia;
            }
        }
        cout<<endl<<endl;
    }
}

void push(tNodoDia *&p, tsParDia regPD){
    tNodoDia* act;
    
  
    act = ExisteNodo(p,regPD);
    if(act != NULL){ 
        Acumular(act,regPD);     
    }else{
        pushNode(p,regPD);
    }
}

void pushHead(tNodoDia* &head_ref, tsParDia regParDia){
    tNodoDia* pNodo = new tNodoDia();

    pNodo->info.dia = regParDia.dia;
    pNodo->info.cantidades[0] = regParDia.cantidades[0];
    pNodo->info.cantidades[1] = regParDia.cantidades[1];
    pNodo->info.cantidades[2] = regParDia.cantidades[2];
    pNodo->info.cantidades[3] = regParDia.cantidades[3];

    pNodo->sgteDia = head_ref;
    
    head_ref = pNodo;

}


void pushMid(tNodoDia* &head_ref, tsParDia new_data){
    
    // Allocate new nodes
    tNodoDia* new_node = new tNodoDia();
    tNodoDia* pos_node = head_ref;
    
    // Put in the data
    new_node->info.dia = new_data.dia;
    new_node->info.cantidades[0] = new_data.cantidades[0];
    new_node->info.cantidades[1] = new_data.cantidades[1];
    new_node->info.cantidades[2] = new_data.cantidades[2];
    new_node->info.cantidades[3] = new_data.cantidades[3];
    new_node->sgteDia = NULL;
    
    // Insert in order
    while(pos_node->sgteDia && new_data.dia > pos_node->sgteDia->info.dia){
        pos_node = pos_node->sgteDia;
    }
    
    new_node->sgteDia = pos_node->sgteDia;
    pos_node->sgteDia = new_node;
    
}

void pushNode(tNodoDia* &head_ref, tsParDia regParDia){
    if(!head_ref || regParDia.dia < head_ref->info.dia){ 
        pushHead(head_ref,regParDia);
    }
    else{
        pushMid(head_ref,regParDia);
    }
}

void Acumular(tNodoDia*&pos, tsParDia regPD){
    
    pos->info.cantidades[0] += regPD.cantidades[0];
    pos->info.cantidades[1] += regPD.cantidades[1];
    pos->info.cantidades[2] += regPD.cantidades[2];
    pos->info.cantidades[3] += regPD.cantidades[3];

}




tNodoDia* ExisteNodo(tNodoDia* &head_ref, tsParDia val){
    tNodoDia* puntAct = head_ref;
    
    while(puntAct && val.dia > puntAct->info.dia){
        
        puntAct = puntAct->sgteDia;
    }
  
    return (puntAct && val.dia == puntAct->info.dia) ? puntAct : NULL;
}

void OrdXBur(tsPais vec[],int cantPaises){

    int k = 0;
    bool ordenado;
    do{
        k++;
        ordenado = true;
        int i=0;
        while(i < cantPaises - k){
            if( strcmp(vec[i].nombrePais,vec[i+1].nombrePais) > 0 ){
                char aux[21];
                strcpy(aux,vec[i].nombrePais);
                strcpy(vec[i].nombrePais,vec[i+1].nombrePais);
                strcpy(vec[i+1].nombrePais,aux);
                ordenado = false;
            }
            i++;
        }
    }while(!ordenado);
}

bool BusqBin(tsPais paises[], char clv[], int cardPaises, int& index){
    int prim = 0;
    int der = cardPaises - 1;
    int medio = 0;
    
    while(prim <= der){
       
        
        medio = (prim+der)/2;
        
        if(compare(paises[medio].nombrePais,clv)){
            index = medio;
            return true;
        }else if( strcmp(paises[medio].nombrePais,clv) < 0 ){
            prim = medio + 1;
        }else{
            der = medio - 1;
        }
    }
    return false;
}

bool compare(char cad1[],char cad2[]){
    string cad1Str = "";
    string cad2Str = "";
    int i = 0;
    while(cad1[i] != ' '){
        cad1Str += cad1[i];
        i++;
    }
    i = 0;
    while(cad2[i] != ' '){
    
        cad2Str += cad2[i];
        i++;
    }
    if(cad1Str == cad2Str){
        return true;
    }else{
        return false;
    }

}