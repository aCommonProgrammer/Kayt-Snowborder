/*!
    \file Snowkite.cpp
    \brief calcolo delle distanze percorse dai partecipanti e del podio finale
    \author aCommonProgrammer
    \version 1.0
*/

#include <iostream>
#include <fstream>
#include <time.h>

#define CARATTERE_SEPARATORE '.'

using namespace std;

//------------------------------------------CLASSE-GENERA-DISTANZE---------------------------------------------

/*!
    \struct KiteSnowboarder
    \brief contiene il nome e i punti rilevati di un atleta
*/

struct KiteSnowboarder
{
/*!
    \var nome
    \brief nome dell'atleta
*/
    string nome;
/*!
    \var misurazione
    \brief contiene i punti rilevati durante il percorso dell'atleta
*/
    int misurazione[30][2];
};

/*!
    \class GeneraDistanze
    \brief simula i dati che dovrebbero essere rilevati in una gara
*/
class GeneraDistanze
{
    public:
    /*!
        \var file_name
        \brief nome del file dove � contenuto il registro gara
    */
        string file_name = "RegistroGara.txt";

    /*!
        \fn creaRegistroGara
        \brief genera i dati degli atleti e li passa uno ad uno alla funzione writeKiteSnowboarder(KiteSnowboarder athlete)
    */
        void creaRegistroGara()
        {
            int n = rand()%40;
            for (int c=0; c<n; c++)
            {
                int m=rand()%30;
                writeKiteSnowboarder(generaGiro(generaNome(), m), m);
            }
        }

        /*!
        \fn writeKiteSnowboarder
        \param[in] athlete atleta da inserire nel registro
        \param[in] minuti tempo della prestazione in minuti
        \brief scrive nel file del registro l'ateta passato come parametro
        */
        void writeKiteSnowboarder(KiteSnowboarder athlete, int minuti)
        {
            ofstream fout(file_name);
            fout<<athlete.nome<<CARATTERE_SEPARATORE;
                for (int t=0; t<minuti; t++)
                {
                    fout<<athlete.misurazione[t][0]<<CARATTERE_SEPARATORE<<athlete.misurazione[t][1];
                    if (t != minuti-1)
                        fout<<CARATTERE_SEPARATORE;
                    else
                        fout<<'\n';
                }
                fout.close();
        }

    private:
        /*!
        \fn generaGiro
        \param[in] nome nome dell'atleta
        \param[in] n_minuti tempo effettuato dall'atleta (max 30)
        \brief genera la prestazione di un atleta
        \return atleta con prestazione genrata
        */
        KiteSnowboarder generaGiro(string nome, int n_minuti)
        {
            if(n_minuti > 30)
                n_minuti = 30;

            KiteSnowboarder atleta;
            atleta.nome = nome;
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
                fin_nome>>nome;
            }
            for (int c=0, r=rand()%872; c<r; c++)
            {
                fin_cognome>>cognome;
            }
            fin_nome.close(); fin_cognome.close();
            return nome+" "+cognome;
        }
};
//-------------------------------------------------------------------------------------------------------------


//---------------------------------------------MAIN------------------------------------------------------------
int main()
{
    srand(time(NULL));


    return 0;
}
//-------------------------------------------------------------------------------------------------------------
