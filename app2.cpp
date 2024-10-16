#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#include "Shared\Classes\Headers\Jobs.h"
#include "Shared\Classes\Headers\Candidat.h"

using namespace std;
vector<Candidat *> getCandidati(const string &companie);

void print_Help()
{
    cout << "  _       _         _              _    ____          _     ___  \n"
         << " | |     (_)       | |            | |  / __ \\        | |   |__ \\ \n"
         << " | |      _  _ __  | | __ ___   __| | | |  | | _   _ | |_     ) |\n"
         << " | |     | || '_ \\ | |/ // _ \\ / _` | | |  | || | | || __|   / / \n"
         << " | |____ | || | | ||   <|  __/| (_| | | |__| || |_| || |_   / /_ \n"
         << " |______||_||_| |_||_|\\_\\\\___| \\__,_|  \\____/  \\__,_| \\__| |____|\n"
         << "                                                                 \n";

        
    cout << "Lista Comenzi: " << endl;
    cout << " -list_job\n";
    cout << " -sort_job <skills>\n";
    cout << " -apply_job <id_companie> <username> <email> \n";
}

vector<string> split(string input)
{
    vector<string> tokens;
    istringstream iss(input);
    string token;

    while (getline(iss, token, ','))
    {
        tokens.push_back(token);
    }

    return tokens;
}

vector<Job *> getJob()
{
    vector<Job *> vector_jobs;
    ifstream fin("Shared\\Files\\jobs.csv");
    if (fin.is_open())
    {
        string line;
        while (getline(fin, line))
        {
            vector<string> job_input = split(line);
            if (job_input.size() >= 7)
            {
                string id = job_input[0];
                vector<Candidat *> v = getCandidati(id);
                string companie = job_input[1];
                string pozitie = job_input[2];
                int salar = stoi(job_input[3]);
                string locatie = job_input[4];
                string remote = job_input[5];
                string skills = job_input[6];

                vector_jobs.push_back(new Job(id, companie, pozitie, salar, locatie, remote, v, skills));
            }
            else
            {
                cout << "Invalid line format: " << line << endl;
            }
        }
        fin.close();
    }
    else
    {
        cout << "File not open!" << endl;
    }
    return vector_jobs;
}
vector<Candidat *> getCandidati(const string &companie)
{
    vector<Candidat *> vector_cand;
    ifstream fin("Shared\\Files\\candidat.csv");
    if (fin.is_open())
    {
        string line;
        while (getline(fin, line))
        {
            vector<string> cand_input = split(line);
            if (cand_input.size() >= 3)
            {
                string id = cand_input[0];
                string username = cand_input[1];
                string email = cand_input[2];
                if (companie == id)
                {
                    vector_cand.push_back(new Candidat(id, username, email));
                }
            }
            else
            {
                cout << "Invalid line format: " << line << endl;
            }
        }
        fin.close();
    }
    else
    {
        cout << "File not open!" << endl;
    }
    return vector_cand;
}

void func_add_candidat(string id_comp, string username, string email)
{
    vector<Job *> vector_jobs = getJob();
    int ok = 0;
    for (auto *el : vector_jobs)
    {
        if (el->getId() == id_comp)
        {
            ok = 1;
        }
    }
    if (ok)
    {
        ofstream fout("Shared\\Files\\candidat.csv", ios_base::app);
        fout << "\n";
        Candidat c(id_comp, username, email);
        if (fout.is_open())
        {
            fout << c.getId() << "," << c.getNume() << "," << c.getEmail() << "\n";
            fout.close();
        }
        else
        {
            cout << "Unable to open file for writing\n";
        }
    }
    else
    {
        cout << "The company does not exist!";
    }
}
int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "-help") == 0)
    {
        print_Help();
    }
    else if (argc > 1 && strcmp(argv[1], "-list_job") == 0)
    { // JOB COUT
        if (argc != 2)
        {
            cout << "Syntax is invalid! Correct Syntax = -list_job\n";
        }
        else
        {
            vector<Job *> vector_jobs = getJob();
            if (!vector_jobs.empty())
            {
                cout << "\nID | COMPANIE | POZITIE | SALAR | LOCATIE | REMOTE | LIMBAJ |\n\n";
                for (auto &el : vector_jobs)
                    cout << *el;
            }
            else
            {
                cout << "There are no available job listings";
            }
        }
    }

    else if (argc > 1 && strcmp(argv[1], "-sort_job") == 0)
    {
        if (argc != 3)
        {
            cout << "Syntax is invalid! Correct Syntax = -sort_job <skills>\n";
        }
        else
        {
            string skill = argv[2];
            vector<Job *> vector_jobs = getJob();
            if (!vector_jobs.empty())
            {
                for (auto *el : vector_jobs)
                {
                    if (el->getSkills() == skill)
                    {
                        cout << *el;
                    }
                }
            }
            else
            {
                cout << "Entered Skill was not found!";
            }
        }
    }
    else if (argc > 1 && strcmp(argv[1], "-apply_job") == 0)
    {
        if (argc != 5)
        {
            cout << "Syntax is invalid! Correct Syntax = -apply_job <id_companie> <username> <email>\n";
        }
        else
        {
            string id_comp = argv[2];
            string username = argv[3];
            string email = argv[4];
            func_add_candidat(id_comp, username, email);
        }
    }
    return 0;
}