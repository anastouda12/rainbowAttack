#include "src/rainbowTableGen.hpp"
#include "src/threadpool.hpp"
#include "src/sha256.h"
#include "passwd-utils.hpp"
#include "src/reduction.hpp"


namespace rainbow
{
    RainbowTableGen::RainbowTableGen(const float size):
        size_{size},
        rainbowTableFile_{}
    {}

    RainbowTableGen::~RainbowTableGen()
    {
        if (this->rainbowTableFile_.is_open())
        {
            this->rainbowTableFile_.close();
        }
    }

    void RainbowTableGen::generateTable()
    {
        cout << "[INFO] : Generation of rainbowTable started" << endl;
        cout << "[INFO] : Launch of " << num_cpus << " threads for the generation" << endl;
        cout << "[INFO] : Generation of rainbowTable in progress ..." << flush;
        dg::utils::ThreadPool pool(num_cpus, true);
        string filesName[num_cpus];
        const int nbchain_table = ceil(getSizeOnBytes(this->size_) / RTENTRY_SIZE);
        const int nbchain_by_cpu = nbchain_table / num_cpus;
        auto start = high_resolution_clock::now();
        filesName[0] = "miniRainbow1.txt";         //First thread takes the rest of nbr entries k
        pool.enqueue(mem_fn(&RainbowTableGen::generateMiniTable), this,
                     ref(filesName[0]), nbchain_by_cpu + nbchain_table % num_cpus);

        for (unsigned int i = 1; i < num_cpus; ++i)
        {
            filesName[i] = "miniRainbow" + to_string(i + 1) + ".txt";
            pool.enqueue(mem_fn(&RainbowTableGen::generateMiniTable), this,
                         ref(filesName[i]), nbchain_by_cpu);
        }

        pool.join();
        set<rtEntry> tempSet;
        combineOrderedMiniTableIntoSet(filesName, tempSet, num_cpus);
        writePrecomputedValuesIntoTable(tempSet);
        tempSet.clear();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\33[2K\r[SUCCESS] : RainbowTable was generated on file Build/" << FILE_NAME_RTABLE  << endl;
        printDuration(duration);
    }

    void RainbowTableGen::generateMiniTable(const string &fileName,
                                            const int nbchain_by_cpu)
    {
        if (nbchain_by_cpu > 0)
        {
            ofstream table(fileName, ios_base::out | ios_base::trunc | ios_base::binary);

            if (!table.is_open())
            {
                cout << "[ERROR] : Opening file " << fileName << "Thread : " <<
                     this_thread::get_id() << endl;
                return;
            }

            rtEntry precomputed;

            for (int nbChain = 1; nbChain <= nbchain_by_cpu; ++nbChain)
            {
                buildPrecomputedHashChain(precomputed);
                table.write((char *) &precomputed, RTENTRY_SIZE);
            }

            table.close();
        }
    }


    void RainbowTableGen::combineOrderedMiniTableIntoSet(const string *filesName, set<rtEntry> &set_table, const unsigned nb_table)
    {
        ifstream table;

        for (unsigned i = 0; i < nb_table; ++i)
        {
            table.open(*(filesName + i), ios_base::in | ios_base::binary);

            if (table.is_open())
            {
                rtEntry chain;
                unsigned end = table.seekg(0, ios::end).tellg() / RTENTRY_SIZE;
                table.seekg(0, ios::beg);

                for (unsigned j = 0; j <= end; ++j)
                {
                    table.seekg(j * RTENTRY_SIZE);
                    table.read((char *) &chain, RTENTRY_SIZE);
                    set_table.insert(move(chain));
                }

                table.close();
                remove((*(filesName + i)).c_str());

            }
        }
    }

    inline void RainbowTableGen::writePrecomputedValuesIntoTable(
            const set<rtEntry> &entries)
    {
        rainbowTableFile_.open(FILE_NAME_RTABLE,
                               ios::out | ios::trunc | ios::binary);

        if (rainbowTableFile_.is_open())
        {

            for (auto chain : entries)
            {
                this->rainbowTableFile_.write((char *) &chain, RTENTRY_SIZE);
            }

            rainbowTableFile_.close();
        }

    }

    inline void RainbowTableGen::calculTail(string &&password, char *tail)
    {
        string hash = move(password);

        for (unsigned step = 0; step < HASH_LEN; ++step)
        {
            hash = sha256(hash);
            REDUCE(hash, tail, step, PASSWORD_SIZE);
            hash = tail;
        }

    }

    inline void RainbowTableGen::buildPrecomputedHashChain(rtEntry &chain)
    {
        string &&password = generate_passwd(PASSWORD_SIZE);
        copyArrays(password.c_str(), chain.head, PASSWORD_SIZE);
        calculTail(move(password), chain.tail);
    }


}
