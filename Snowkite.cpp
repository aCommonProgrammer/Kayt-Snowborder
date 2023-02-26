/*!
    \file Snowkite.cpp
    \brief calcolo delle distanze percorse dai partecipanti e del podio finale
    \author aCommonProgrammer
    \version 1.0
*/

#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <ctime>

#define CARATTERE_SEPARATORE '-'
#define PARTECIPANTI 40
#define GRANDEZZA_PODIO 3
#define MINUTI_GARA 30

using namespace std;

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
    int misurazione[MINUTI_GARA][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};

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
        \fn getFileName
        \brief restituisce il nome del file di registro
    */
    string getFileName()
    {
        return file_name;
    }

    /*!
        \fn setFileName
        \param[in] name nome del file
        \brief imposta il nome del file di registro
    */
    void setFileName(string name)
    {
        file_name = name;
    }

    /*!
        \fn creaRegistroGara
        \brief genera i dati degli atleti e li passa uno ad uno alla funzione writeSnowkiteRider()
    */
    void creaRegistroGara()
    {
        ofstream f(file_name);
        f.close();
        int n = rand()%PARTECIPANTI;
        for (int c=0; c<n; c++)
        {
            int m=rand()%MINUTI_GARA;
            writeSnowkiteRider(generaGiro(generaNome(), m), m);
        }
    }

    /*!
        \fn writeSnowkiteRider
        \param[in] athlete atleta da inserire nel registro
        \param[in] minuti tempo della prestazione in minuti
        \brief scrive nel file del registro l'ateta passato come parametro
        \note Il file viene scritto nel formato "nome cognome-minuti-x0-y0-x1-y1-x2-y2...-"
    */
    void writeSnowkiteRider(SnowkiteRider athlete, int minuti)
    {
        fstream fout(file_name, ios::app);
        fout<<athlete.nome<<CARATTERE_SEPARATORE<<athlete.minuti<<CARATTERE_SEPARATORE;
            for (int t=0; t<minuti; t++)
            {
                fout<<athlete.misurazione[t][0]<<CARATTERE_SEPARATORE<<athlete.misurazione[t][1]<<CARATTERE_SEPARATORE;
                if (t == minuti-1)
                    fout<<'\n';
            }
        fout.close();
    }

    private:
/*! \var file_name
    \brief nome del file dove è contenuto il registro gara*/
    string file_name = "RegistroGara.txt";

/*!
    \fn generaGiro
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
    \fn generaNome
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
    \fn getFileName
    \brief restituisce il nome del file di registro
*/
    string getFileName()
    {
        return file_name;
    }

/*!
    \fn setFileName
    \param[in] name nome del file
    \brief imposta il nome del file di registro
*/
    void setFileName(string name)
    {
        file_name = name;
    }

/*!
    \fn leggiRegistro
    \param[in] vettore vettore che viene riempito con i dati nel file di registro
    \brief legge il file di registro e lo trascrive in un vettore
*/
    void leggiRegistro(SnowkiteRider vettore[])
    {
        ifstream fin(file_name);
        string app_interi;
        int contatore = 0;
        while(fin)
        {
            getline(fin, vettore[contatore].nome, CARATTERE_SEPARATORE);
            getline(fin, app_interi, CARATTERE_SEPARATORE);
            vettore[contatore].minuti = stoi(app_interi);
            for (int m=0; m<vettore[contatore].minuti; m++)
            {
                getline(fin, app_interi, CARATTERE_SEPARATORE);
                vettore[contatore].misurazione[0][m] = stoi(app_interi);
                getline(fin, app_interi, CARATTERE_SEPARATORE);
                vettore[contatore].misurazione[1][m] = stoi(app_interi);
            }
            contatore++;
        }
        fin.close();
    }

/*!
    \fn podio
    \param[in] atleti vettore contenente il registro gara
    \param[in] vettore vettore che verra riempito con il podio
    \brief calcola il podio dei vincitori
*/
    void podio(SnowkiteRider atleti[], SnowkiteRider vettore[])
    {
        calcolaDistanze(atleti);
        int posizione_vincitori[GRANDEZZA_PODIO] = {0,0,0};
        for (int c=0; c<GRANDEZZA_PODIO; c++)
        {
            for (int i=1; i<PARTECIPANTI; i++)
                if (atleti[posizione_vincitori[c]].distanza > atleti[i].distanza)
                    posizione_vincitori[c] = i;
            vettore[c] = atleti[posizione_vincitori[c]];
            atleti[posizione_vincitori[c]].distanza = c * -1;
        }
    }

/*!
    \fn calcolaDistanza
    \param[in] atleti vettore contenente il registro gara
    \brief calcola le distanze percorse dagli atleti
*/
    void calcolaDistanze(SnowkiteRider atleti[])
    {
        for (int c=0; c<PARTECIPANTI; c++)
            for (int m=0; m<atleti[c].minuti-1; m++)
                atleti[c].distanza += (int) sqrt(pow(atleti[c].misurazione[m][0]-atleti[c].misurazione[m+1][0],2)+pow(atleti[c].misurazione[m][1]-atleti[c].misurazione[m+1][1],2));
    }

 private:
    /*! \var file_name
        \brief nome del file dove è contenuto il registro gara*/
        string file_name = "RegistroGara.txt";


};
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------FUNZIONI----------------------------------------------------------
/*!
    \fn timeout
    \param[in] delay tempo da aspettare
    \brief ferma il programma per il tempo indicato
*/
void timeout(double delay)
{
    delay *= CLOCKS_PER_SEC;
    clock_t now = clock();
    while(clock() - now <delay);
}
//-------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN------------------------------------------------------------
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
        timeout((double) (rand()%4+3)/10);
        cout<<".";
    }

    //Dichiarazione dei vettori della lista partecipanti e atleti sul podio
    SnowkiteRider atleta[PARTECIPANTI], podio[GRANDEZZA_PODIO];

    //Lettura del registro e calcolo del podio
    CalcolaPodio Calcolatore;
    Calcolatore.leggiRegistro(atleta);
    Calcolatore.podio(atleta, podio);

    cout<<"\n\t\tPODIO\n";
    for (int c=0; c<GRANDEZZA_PODIO; c++)
    {
        cout<<"\a\n\t"<<c+1<<(char) -8<<"\t"<<podio[c].nome<<"    ("<<podio[c].distanza<<"m)";
        timeout(1);
    }

    return 0;
}
//-------------------------------------------------------------------------------------------------------------
