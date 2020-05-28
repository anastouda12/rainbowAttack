#include "src/rainbowAttack.hpp"
#include "threadpool.hpp"
#include <algorithm>
#include "src/sha256.h"
#include "src/reduction.hpp"

using namespace dg::utils;
namespace rainbow
{

    RainbowAttack::RainbowAttack(const string &hashToCrackFilePath):
        rainbowTableFile_{},
        passwordCracked_{},
        passwordHashes_{},
        INDEX_MIN_RT{getMinIndexTable()},
        INDEX_MAX_RT{getMaxIndexTable()}
    {
        passwordHashes_.open(hashToCrackFilePath, ios::in);

        if (passwordHashes_.fail())
        {
            cerr << "[ERROR] : Password hashes file : " << hashToCrackFilePath << " failed to opening" << endl;
            exit(EXIT_FAILURE);
        }
    }



    vector<vector<string>> RainbowAttack::getHashesToCrack()
    {
        vector<vector<string>> vecFather;
        vecFather.reserve(num_cpus);
        unsigned nbHashes = count(istreambuf_iterator<char>(passwordHashes_),
                                  istreambuf_iterator<char>(), '\n');
        passwordHashes_.clear();
        passwordHashes_.seekg(0, ios_base::beg);
        unsigned nbHashesPerVec = nbHashes / num_cpus;
        unsigned nbHashesVecOne = nbHashesPerVec + (nbHashes % num_cpus);

        for (unsigned i = 0; i < num_cpus; ++i)  //init vector of futur passwords inside vector vecFather
        {
            vector<string> vec;

            if (i == 0)
            {
                vec.reserve(nbHashesVecOne);        // first vector will take the rest of works nbHashes%num_cpus
            }
            else
            {
                vec.reserve(nbHashesPerVec);
            }

            vecFather.push_back(vec);
        }

        string hash(SHA256_BLOCKSIZE, ' ');
        unsigned cpt = 0; // cpt hashes treated
        unsigned numberVec = 1; // number of the current vector

        while (getline(passwordHashes_, hash))
        {
            if (cpt <= nbHashesVecOne) // first vector will take the rest of works
            {
                vecFather.at(numberVec - 1).push_back(move(hash)); // first vector works
                cpt++;

                if (cpt == nbHashesVecOne)
                {
                    numberVec++;
                }
            }
            else
            {
                if (cpt < (nbHashesPerVec * numberVec) + (nbHashesVecOne - nbHashesPerVec))
                {
                    vecFather.at(numberVec - 1).push_back(move(hash));
                }
                else
                {
                    numberVec++;
                    vecFather.at(numberVec - 1).push_back(move(hash));
                }

                cpt++;
            }

        }

        passwordHashes_.close();
        return vecFather;
    }

    string RainbowAttack::crackPassword(const string &hash, ifstream &rtable)
    {
        string current(SHA256_BLOCKSIZE, ' ');
        string possPassword(PASSWORD_SIZE, ' ');
        rtEntry chain;
        char tail[PASSWORD_SIZE + 1];

        for (unsigned i = HASH_LEN; i-- > 0;) // Search of chain
        {
            current = hash;

            for (unsigned j = i; j < HASH_LEN; ++j)
            {
                REDUCE(current, tail, j, PASSWORD_SIZE);
                current = sha256(tail);
            }

            if (pwdBinarySearch(tail, chain, rtable)) //  Possible chain found -> build up
            {
                if (buildUpPassword(hash, chain, possPassword))
                {
                    return possPassword;
                }
                else
                {
                    cout << "False alarm" << endl;
                }

            }
        }

        possPassword = "NOTFOUND"; // pwd not found :(
        return possPassword;
    }

    bool RainbowAttack::pwdBinarySearch(const char *tail, rtEntry &chain, ifstream &rtable)
    {

        if (rtable.is_open())
        {
            int min = INDEX_MIN_RT;
            int max = INDEX_MAX_RT;
            int mid =  mid = (min + max) >> 1;
            int comp = 0;
            rtable.clear();
            rtable.seekg(0, ios::beg);

            while (min <= max)
            {
                rtable.seekg(mid * RTENTRY_SIZE);
                rtable.read((char *) &chain, RTENTRY_SIZE);
                comp = strcmp(tail, chain.tail);

                if (comp == 0)
                {
                    return true;
                }
                else if (comp < 0)
                {
                    max = mid - 1;
                }
                else
                {
                    min = mid + 1;
                }

                mid = (min + max) >> 1;
            }

        }

        return false;
    }

    bool RainbowAttack::buildUpPassword(const string &hash, rtEntry &chain, string &password)
    {
        string current = chain.head;
        string previous = current;
        char reduction[PASSWORD_SIZE + 1];
        unsigned step = 0;

        while (step < HASH_LEN)
        {
            previous = current;
            current = sha256(current);

            if (current.compare(hash) == 0)
            {
                password = previous;
                return true;
            }

            REDUCE(current, reduction, step, PASSWORD_SIZE);
            current = reduction;
            ++step;
        }

        return false;
    }


    void RainbowAttack::crackSomePasswords(vector<string> &vec)
    {
        ifstream rtable;
        rtable.open(FILE_NAME_RTABLE, ios::in | ios::binary);

        if (rtable.fail())
        {
            cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << endl;
            exit(EXIT_FAILURE);
        }

        for (unsigned i = 0; i < vec.size(); ++i)
        {
            vec[i] = crackPassword(vec.at(i), rtable);
        }
    }

    void RainbowAttack::attack()
    {
        passwordCracked_.open(PASSWORD_CRACKED_FILE, ios_base::out | ios_base::trunc);

        if (passwordCracked_.fail())
        {
            cerr << "[ERROR] : Failed to opening : " << PASSWORD_CRACKED_FILE << endl;
            exit(EXIT_FAILURE);
        }

        cout << "[INFO] RainbowAttack started" << endl;
        cout << "[INFO] Launch of " << num_cpus << " threads for the attack" << endl;
        cout << "[INFO] RainbowAttack in progress ..." << endl;
        vector<vector<string>> &&hashes = getHashesToCrack();
        auto start = high_resolution_clock::now();
        ThreadPool pool(num_cpus, true);

        for (unsigned i = 0; i < num_cpus; ++i)
        {
            pool.enqueue(mem_fn(&RainbowAttack::crackSomePasswords), this,
                         ref(hashes.at(i)));
        }

        pool.join();

        for (auto thwork : hashes)
        {
            for (auto pwd : thwork)
            {
                passwordCracked_ << pwd << endl;
            }
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        passwordCracked_.close();
        cout << "\33[2K\r[SUCCES] Passwords cracked was generated on file Build/" << PASSWORD_CRACKED_FILE << endl;
        printDuration(duration);
    }

    RainbowAttack::~RainbowAttack()
    {
        if (rainbowTableFile_.is_open())
        {
            rainbowTableFile_.close();
        }

        if (passwordHashes_.is_open())
        {
            passwordHashes_.close();
        }

        if (passwordCracked_.is_open())
        {
            passwordCracked_.close();
        }
    }





} // end namespace rainbow

