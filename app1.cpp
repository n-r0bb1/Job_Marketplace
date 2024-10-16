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
vector<string> split(const string &input);

void print_Help()
{
    cout << "  _       _         _              _    ____          _     __ \n"
         << " | |     (_)       | |            | |  / __ \\        | |   /_ |\n"
         << " | |      _  _ __  | | __ ___   __| | | |  | | _   _ | |_   | |\n"
         << " | |     | || '_ \\ | |/ // _ \\ / _` | | |  | || | | || __|  | |\n"
         << " | |____ | || | | ||   <|  __/| (_| | | |__| || |_| || |_   | |\n"
         << " |______||_||_| |_||_|\\_\\\\___| \\__,_|  \\____/  \\__,_| \\__|  |_|\n"
         << "                                                               \n"
         << "                                                              \n";

    cout << "Lista Comenzi: " << endl;
    cout << " -add_job <id> <nume_companie> <pozitie> <salar> <locatie> <remote> <skills>\n";
    cout << " -mod_job <id> <pozitie> <salar> <remote> \n";
    cout << " -delete_job <id_job>\n";
    cout << " -list_cand <id_job>\n";
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

vector<Candidat *> getCandidati2(const string &companie)
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
                if (companie != id)
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

void func_add_job(const string &id, const string &companie, const string &pozitie, int salar, const string &locatie, const string &remote, const string &skills)
{
    cout << "Job listing added succesfully!\n"
         << endl;
    vector<Candidat *> v = getCandidati(id);
    Job j(id, companie, pozitie, salar, locatie, remote, v, skills);
    ofstream fout("Shared\\Files\\jobs.csv", ios_base::app);
    fout << "\n";
    if (fout.is_open())
    {
        fout << j.getId() << "," << j.getCompanie() << "," << j.getPozitie() << "," << j.getSalar() << "," << j.getLocatie() << "," << j.getRemote() << "," << j.getSkills() << "\n";
        fout.close();
    }
    else
    {
        cout << "Unable to open file for writing\n";
    }
}

vector<string> split(const string &input)
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

int main(int argc, char **argv)
{

    if (argc > 1 && strcmp(argv[1], "-help") == 0)
    {
        print_Help();
    }
    else if (argc > 1 && strcmp(argv[1], "-add_job") == 0)
    {
        if (argc != 9)
        {
            cout << "Syntax is invalid! Correct Syntax = -add_job <id> <nume_companie> <pozitie> <salar> <locatie> <remote> <skills>\n";
        }
        else
        {
            string id = argv[2];
            string companie = argv[3];
            string pozitie = argv[4];
            int salar = atoi(argv[5]);
            string locatie = argv[6];
            string remote = argv[7];
            string skills = argv[8];
            func_add_job(id, companie, pozitie, salar, locatie, remote, skills);
        }
    }
    else if (argc > 1 && strcmp(argv[1], "-mod_job") == 0)
    {
        if (argc != 6)
        {
            cout << "Syntax is invalid! Correct Syntax = -mod_job <id> <pozitie> <salar> <remote> \n";
        }
        else
        {
            string id = argv[2];
            string new_pozitie = argv[3];
            int new_salar = atoi(argv[4]);
            string new_remote = argv[5];

            vector<Job *> vector_jobs = getJob();
            for (auto &el : vector_jobs)
            {
                if (el->getId() == id)
                {
                    el->setPozitie(new_pozitie);
                    el->setSalar(new_salar);
                    el->setRemote(new_remote);
                }
                else
                {
                    cout << "The company does not exist!\n";
                }
            }

            ofstream fout_del("Shared\\Files\\jobs.csv", ios_base::trunc);
            fout_del.close();

            ofstream fout("Shared\\Files\\jobs.csv", ios_base::app);

            if (fout.is_open())
            {
                for (auto &j : vector_jobs)
                {
                    fout << j->getId() << "," << j->getCompanie() << "," << j->getPozitie() << "," << j->getSalar() << "," << j->getLocatie() << "," << j->getRemote() << "," << j->getSkills() << "\n";
                }
            }
            else
            {
                cout << "Unable to open file for writing\n";
            }
            fout.close();
        }
    }
    else if (argc > 1 && strcmp(argv[1], "-delete_job") == 0)
    {
        if (argc != 3)
        {
            cout << "Syntax is invalid! Correct Syntax = -delete_job <id_job>\n";
        }
        else
        {
            string delete_id = argv[2];
            vector<Job *> vector_jobs = getJob();
            vector<Candidat *> vector_cand = getCandidati2(delete_id);
            int count = 0;
            for (auto &el : vector_jobs)
            {
                if (el->getId() == delete_id)
                {
                    vector_jobs.erase(vector_jobs.begin() + count);
                    break;
                }
                count++;
            }

            ofstream fout_del("Shared\\Files\\jobs.csv", ios_base::trunc);
            fout_del.close();

            ofstream fout("Shared\\Files\\jobs.csv", ios_base::app);
            if (fout.is_open())
            {
                for (auto &el : vector_jobs)
                    fout << el->getId() << "," << el->getCompanie() << "," << el->getPozitie() << "," << el->getSalar() << "," << el->getLocatie() << "," << el->getRemote() << "," << el->getSkills() << "\n";
                fout.close();
                cout << "Job with ID " << delete_id << " deleted successfully.\n";
            }
            else
            {
                cout << "Unable to open file for writing\n";
            }

            ofstream fout_del_cand("Shared\\Files\\candidat.csv", ios_base::trunc);
            fout_del_cand.close();

            ofstream fout_cand("Shared\\Files\\candidat.csv", ios_base::app);
            if (fout_cand.is_open())
            {
                for (auto &el : vector_cand)
                    fout_cand << el->getId() << "," << el->getNume() << "," << el->getEmail() << "\n";
                fout_cand.close();
            }
            else
            {
                cout << "Unable to open file for writing\n";
            }
        }
    }
    else if (argc > 1 && strcmp(argv[1], "-list_cand") == 0)
    {
        if (argc != 3)
        {
            cout << "Syntax is invalid! Correct Syntax = -list_cand <id_job>\n";
        }
        else
        {
            string id = argv[2];
            vector<Candidat *> candidati = getCandidati(argv[2]);
            if (!candidati.empty())
            {
                for (auto &el : candidati)
                {
                    cout << *el;
                }
            }
            else
            {
                cout << "There are no available candidates for this company!\n";
            }
        }
    }
    else
    {
        cout << "Invalid command. Use -help for list of commands.\n";
    }

    return 0;
}
