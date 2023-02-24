/*!
    \file Snowkite.cpp
    \brief calcolo delle distanze percorse dai partecipanti e del podio finale
    \author aCommonProgrammer
    \version 1.0
*/

#include <iostream>
#include <fstream>
#include <time.h>

#define CARATTERE_SEPARATORE '-'
#define PARTECIPANTI 40
#define MINUTI_GARA 30

using namespace std;

//------------------------------------------STRUCT-KITESNOWBOARDER---------------------------------------------
/*!
    \struct KiteSnowboarder
    \brief contiene il nome e i punti rilevati di un atleta
*/
struct KiteSnowboarder
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
        \brief genera i dati degli atleti e li passa uno ad uno alla funzione writeKiteSnowboarder()
    */
        void creaRegistroGara()
        {
            ofstream f(file_name);
            f.close();
            int n = rand()%PARTECIPANTI;
            for (int c=0; c<n; c++)
            {
                int m=rand()%MINUTI_GARA;
                writeKiteSnowboarder(generaGiro(generaNome(), m), m);
            }
        }

    /*!
        \fn writeKiteSnowboarder
        \param[in] athlete atleta da inserire nel registro
        \param[in] minuti tempo della prestazione in minuti
        \brief scrive nel file del registro l'ateta passato come parametro
        \note Il file viene scritto nel formato "nome cognome-minuti-x0-y0-x1-y1-x2-y2...-"
    */
        void writeKiteSnowboarder(KiteSnowboarder athlete, int minuti)
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
        KiteSnowboarder generaGiro(string nome, int n_minuti)
        {
            if(n_minuti > MINUTI_GARA)
                n_minuti = MINUTI_GARA;

            KiteSnowboarder atleta;
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
    \brief calcola il podio dei primi 3 atleti
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
        \brief legge il file di registro e lo trascrive in un vettore
        \return vettore contenente i dati del file
    */
    KiteSnowboarder* leggiRegistro()
    {
        KiteSnowboarder vettore[PARTECIPANTI];
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
        return vettore;
    }

    /*!
        \fn podio
        \brief calcola il podio dei vincitori
        \return vettore contenente il podio
    */
    KiteSnowboarder* podio()
    {

    }

 private:
    /*! \var file_name
        \brief nome del file dove è contenuto il registro gara*/
        string file_name = "RegistroGara.txt";


};
//-------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN------------------------------------------------------------
int main()
{
    srand(time(NULL));
    GeneraDistanze Generatore;
    Generatore.creaRegistroGara();
    CalcolaPodio Calcolatore;
    KiteSnowboarder* partecipanti = Calcolatore.leggiRegistro();


    return 0;
}
//-------------------------------------------------------------------------------------------------------------
