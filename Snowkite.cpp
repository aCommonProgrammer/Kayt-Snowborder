/*!
    \file Snowkite.cpp
    \brief calcolo delle distanze percorse dai partecipanti e del podio finale
    \author aCommonProgrammer
    \version 1.0
*/

#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <ctime>

/*! \var CARATTERE_SEPARATORE
    \brief carattere per separare i dati nel file*/
#define CARATTERE_SEPARATORE '-'

/*! \var PARTECIPANTI
    \brief numero partecipanti nella gara*/
int PARTECIPANTI = 15;

/*! \var PARTECIPANTI
    \brief numero partecipanti nella gara*/
int GRANDEZZA_PODIO = 3;

/*! \var MINUTI_GARA
    \brief tempo massimo in minuti per una corsa dell'atleta*/
#define MINUTI_GARA 30

using namespace std;

//-------------------------------------------FUNZIONI----------------------------------------------------------
/*!
    \param[in] delay tempo da aspettare
    \brief ferma il programma per il tempo indicato
*/
void timeout(double delay)
{
    delay *= CLOCKS_PER_SEC;
    clock_t now = clock();
    while(clock() - now <delay);
}

/*!
    \brief modifica i parametri di generazione del registro di gara
*/
void modificaGenerazione()
{
    int scelta;
        do{
            system("cls");
            cout<<"\nSelezionare le impostazioni di modifica:\n"
                  "1)Numero partecipanti\n"
                  "2)Grandezza podio\n"
                  "3)Salva le impostazioni correnti\n"
                  ">>"; cin>>scelta;
            switch (scelta)
            {
            case 1: system("cls");
                    cout<<"Inserire il numero dei partecipanti: "; cin>>PARTECIPANTI;
                    break;
            case 2: system("cls");
                    cout<<"Inserire la grandezza del podio "; cin>>GRANDEZZA_PODIO;
                    if (GRANDEZZA_PODIO > PARTECIPANTI)
                    {
                        cout<<"La grandezza del podio e' maggiore del numero dei partecipanti! E' stata automaticamente portata a "<<PARTECIPANTI;
                        GRANDEZZA_PODIO = PARTECIPANTI;
                    }
                    break;
            }
        }while(scelta != 3);
}
//-------------------------------------------------------------------------------------------------------------


//------------------------------------------STRUCT-SNOWKITERIDER---------------------------------------------
/*!
    \struct SnowkiteRider
    \brief contiene il nome e i punti rilevati di un atleta
*/
struct SnowkiteRider
{
/*! \var nome
    \brief nome dell'atleta*/
    string nome;

/*! \var minuti
    \brief tempo in minuti della prestazione*/
    int minuti = 0;

/*! \var misurazione
    \brief contiene i punti rilevati durante il percorso dell'atleta*/
    int misurazione[MINUTI_GARA][2];

/*! \var distanza
    \brief contiene la distanza percorsa dall'atleta*/
    int distanza = 0;
};
//-------------------------------------------------------------------------------------------------------------

//------------------------------------------CLASSE-GENERA-DISTANZE---------------------------------------------
/*!
    \class GeneraDistanze
    \brief simula i dati che dovrebbero essere rilevati in una gara
*/
class GeneraDistanze
{
    public:
    /*!
        \brief restituisce il nome del file di registro
    */
    string getFileName()
    {
        return file_name;
    }

    /*!
        \param[in] name nome del file
        \brief imposta il nome del file di registro
    */
    void setFileName(string name)
    {
        file_name = name;
    }

    /*!
        \brief genera i dati degli atleti e li passa uno ad uno alla funzione writeSnowkiteRider()
    */
    void creaRegistroGara()
    {
        ofstream f(file_name);
        f.close();
        for (int c=0; c<PARTECIPANTI; c++)
        {
            int m=rand()%MINUTI_GARA;
            writeSnowkiteRider(generaGiro(generaNome(), m), m);
        }
    }

    /*!
        \param[in] atleta atleta da inserire nel registro
        \param[in] minuti tempo della prestazione in minuti
        \brief scrive nel file del registro l'ateta passato come parametro
        \note Il file viene scritto nel formato "nome cognome-minuti-x0-y0-x1-y1-x2-y2...-"
    */
    void writeSnowkiteRider(SnowkiteRider atleta, int minuti)
    {
        fstream fout(file_name, ios::app);
        fout<<atleta.nome<<CARATTERE_SEPARATORE<<atleta.minuti<<CARATTERE_SEPARATORE;
            for (int t=0; t<minuti; t++)
            {
                fout<<atleta.misurazione[t][0]<<CARATTERE_SEPARATORE<<atleta.misurazione[t][1]<<CARATTERE_SEPARATORE;
            }
        fout<<'\n';
        fout.close();
    }

    private:
/*! \var file_name
    \brief nome del file dove è contenuto il registro gara*/
    string file_name = "RegistroGara.txt";

/*!
    \param[in] nome nome dell'atleta
    \param[in] n_minuti tempo effettuato dall'atleta
    \brief genera la prestazione di un atleta
    \return atleta con prestazione genrata
*/
    SnowkiteRider generaGiro(string nome, int n_minuti)
    {
       if(n_minuti > MINUTI_GARA)
                n_minuti = MINUTI_GARA;

        SnowkiteRider atleta;
        atleta.nome = nome;
        atleta.minuti = n_minuti;
        for(int m=0; m<n_minuti; m++)
        {
            atleta.misurazione[m][0] = rand()%101;
            atleta.misurazione[m][1] = rand()%101;
        }
        return atleta;
    }

/*!
    \brief genera un nome e un cognome
    \return nome e cognome
*/
    string generaNome()
    {
        ifstream fin_nome("Nomi.txt");
        ifstream fin_cognome("Cognomi.txt");
        string nome, cognome;
        for (int c=0, r=rand()%9224; c<r; c++)
        {
            getline(fin_nome, nome);
        }
        for (int c=0, r=rand()%872; c<r; c++)
        {
            getline(fin_cognome, cognome);
        }
        fin_nome.close(); fin_cognome.close();
        return nome+" "+cognome;
    }
};
//-------------------------------------------------------------------------------------------------------------

//--------------------------------------CLASSE-CALCOLA-PODIO---------------------------------------------------
/*!
    \class CalcolaPodio
    \brief calcola il podio degli atleti vincitori
*/
class CalcolaPodio
{
    public:
/*!
    \brief restituisce il nome del file di registro
*/
    string getFileName()
    {
        return file_name;
    }

/*!
    \param[in] name nome del file
    \brief imposta il nome del file di registro
*/
    void setFileName(string name)
    {
        file_name = name;
    }

/*!
    \param[in] vettore vettore che viene riempito con i dati nel file di registro
    \brief legge il file di registro e lo trascrive in un vettore
*/
    void leggiRegistro(SnowkiteRider vettore[])
    {
        ifstream conta_righe(file_name);
        int c = 0;
        string app;
        while(conta_righe)
        {
            getline(conta_righe, app);
            c++;
        }
        conta_righe.close();

        PARTECIPANTI = c-1;

        ifstream fin(file_name);
        for (int i=0; i<PARTECIPANTI; i++)
        {
            getline(fin, vettore[i].nome, CARATTERE_SEPARATORE);
            getline(fin, app, CARATTERE_SEPARATORE);
            vettore[i].minuti = stoi(app);
            int m;
            for (m=0; m<vettore[i].minuti; m++)
            {
                getline(fin, app, CARATTERE_SEPARATORE);
                vettore[i].misurazione[m][0] = stoi(app);
                getline(fin, app, CARATTERE_SEPARATORE);
                vettore[i].misurazione[m][1] = stoi(app);
            }
        }
        fin.close();
    }

/*!
    \param[in] atleti vettore contenente il registro gara
    \param[in] vettore vettore che verra riempito con il podio
    \brief calcola il podio dei vincitori
*/
    void podio(SnowkiteRider atleti[])
    {
        calcolaDistanze(atleti);
        SnowkiteRider appoggio;

        for (int i=0; i<PARTECIPANTI; i++)
            for (int j=1; j<PARTECIPANTI;j ++)
                if (atleti[i].distanza < atleti[j].distanza)
                {
                    appoggio = atleti[i];
                    atleti[i] = atleti[j];
                    atleti[j] = appoggio;
                }

        for (int c=0, d=PARTECIPANTI; c<GRANDEZZA_PODIO; c++, d--)
        {
            if(c==0)
                cout<<"\a\n\t"<<c+1<<(char) -8<<"    "<<atleti[c].nome<<"    ("<<atleti[c].distanza<<"m)";
            else
                cout<<"\a\n\t"<<c+1<<(char) -8<<"    "<<atleti[d].nome<<"    ("<<atleti[d].distanza<<"m)";
            timeout(1);
        }
    }

/*!
    \param[in] atleti vettore contenente il registro gara
    \brief calcola le distanze percorse dagli atleti
*/
    void calcolaDistanze(SnowkiteRider atleti[])
    {
        for (int c=0; c<PARTECIPANTI; c++)
         {
            atleti[c].distanza = 0;
            for (int m=0; m<atleti[c].minuti-1; m++)
                atleti[c].distanza += (int) sqrt((int) (((atleti[c].misurazione[m][0]-atleti[c].misurazione[m+1][0])*(atleti[c].misurazione[m][0]-atleti[c].misurazione[m+1][0]))+(atleti[c].misurazione[m][1]-atleti[c].misurazione[m+1][1])*(atleti[c].misurazione[m][1]-atleti[c].misurazione[m+1][1])));

         }
    }

 private:
    /*! \var file_name
        \brief nome del file dove è contenuto il registro gara*/
        string file_name = "RegistroGara.txt";


};
//-------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN------------------------------------------------------------
//! \brief Generazione e o calcolo della partita e del podio
int main()
{
    //Impostazione della randomizzazione
    srand(time(NULL));

    cout<<"    _____                         _     _  _         "<<endl
        <<"   / ____|                       | |   (_)| |        "<<endl
        <<"  | (___   _ __    ___ __      __| | __ _ | |_  ___  "<<endl
        <<"   \\___ \\ | '_ \\  / _ \\\\ \\ /\\ / /| |/ /| || __|/ _ \\ "<<endl
        <<"   ____) || | | || (_) |\\ V  V / |   < | || |_|  __/ "<<endl
        <<"  |_____/ |_| |_| \\___/  \\_/\\_/  |_|\\_\\|_| \\__|\\___| "<<endl;
    timeout(3);
    system("cls");

    string risposta;
    cout<<"Generare una nuova partita? "; cin>>risposta;

    if (risposta == "si" || risposta == "SI" || risposta == "Si" || risposta == "sI")
    {
        //Menu' di modifica dei parametri di generazione
        modificaGenerazione();
        //Generazione del registro di gara
        GeneraDistanze Generatore;
        Generatore.creaRegistroGara();
        cout<<"Partita generata correttemente";
        timeout(3);
    }
    system("cls");

    cout<<"Caricamento podio";
    for (int c=0; c<5; c++)
    {
        timeout((double) (rand()%4+2)/10);
        cout<<".";
    }

    //Dichiarazione dei vettori della lista partecipanti e atleti sul podio
    SnowkiteRider atleta[PARTECIPANTI];

    //Lettura del registro e calcolo del podio
    CalcolaPodio Calcolatore;
    Calcolatore.leggiRegistro(atleta);
    Calcolatore.podio(atleta);

    return 0;
}
//-------------------------------------------------------------------------------------------------------------
