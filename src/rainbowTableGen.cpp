#include "src/rainbowTableGen.hpp"
#include "src/threadpool.hpp"

namespace rainbow
{
    RainbowTableGen::RainbowTableGen(float size):
        size_{size},
        rainbowTableFile_{}
    {
        if (size < getSizeOnMegaBytes(RTENTRY_SIZE)) // size of an entry of the table.
        {
            cerr << "[ERROR] : Size < 16 bytes not enougth to generate RainbowTable, increase the size please !" << endl;
            exit(EXIT_FAILURE);
        }
    }

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
        dg::utils::ThreadPool pool(num_cpus, true);
        string filesName[num_cpus]; // filesName containing the miniRainbowTable.
        const unsigned nbchain_table = ceil(getSizeOnBytes(this->size_) / RTENTRY_SIZE);
        const unsigned nbchain_by_cpu = nbchain_table / num_cpus;
        unsigned nbThread_launch = 0;
        const auto start = high_resolution_clock::now();
        filesName[0] = "miniRainbow1.txt";
        pool.enqueue(mem_fn(&RainbowTableGen::generateMiniTable), this,
                     ref(filesName[0]), nbchain_by_cpu + (nbchain_table % num_cpus)); //First thread takes the rest of nbr entries
        nbThread_launch++;

        if (nbchain_by_cpu > 0) // checks that they are works for other threads
        {
            for (unsigned i = 1; i < num_cpus; ++i)
            {
                filesName[i] = "miniRainbow" + to_string(i + 1) + ".txt";
                pool.enqueue(mem_fn(&RainbowTableGen::generateMiniTable), this,
                             ref(filesName[i]), nbchain_by_cpu);
                nbThread_launch++;
            }
        }

        cout << "[INFO] : Launch of " << nbThread_launch << " thread(s) for the generation" << endl;
        cout << "[INFO] : Generation of rainbowTable in progress ..." << flush;
        pool.join();
        set<rtEntry> tempSet;
        combineOrderedMiniTableIntoSet(filesName, tempSet, nbThread_launch);
        writePrecomputedValuesIntoTable(tempSet);
        tempSet.clear();
        const auto stop = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\33[2K\r[SUCCESS] : RainbowTable was generated on file Build/" << FILE_NAME_RTABLE  << endl;
        printDuration(duration);
    }

    void RainbowTableGen::generateMiniTable(const string &fileName, unsigned nbchain_by_cpu)
    {
        if (nbchain_by_cpu > 0)
        {
            ofstream table(fileName, ios_base::out | ios_base::binary | ios_base::trunc);

            if (!table.is_open())
            {
                cerr << "[ERROR] : Opening file " << fileName << " Thread : " <<
                     this_thread::get_id() << endl;
                return;
            }

            rtEntry precomputed;

            for (unsigned nbChain = 1; nbChain <= nbchain_by_cpu; ++nbChain)
            {
                buildPrecomputedHashChain(precomputed);
                table.write((char *) &precomputed, RTENTRY_SIZE);
            }

            table.close();
        }
    }


    void RainbowTableGen::combineOrderedMiniTableIntoSet(const string *filesName, set<rtEntry> &set_table, unsigned nb_table)
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
            else
            {
                cerr << "[ERROR] : Opening file " << *(filesName + i) << endl;
            }
        }
    }

    void RainbowTableGen::writePrecomputedValuesIntoTable(const set<rtEntry> &entries)
    {
        rainbowTableFile_.open(FILE_NAME_RTABLE,
                               ios::out | ios::binary | ios::trunc);

        if (rainbowTableFile_.fail())
        {
            cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << endl;
            exit(EXIT_FAILURE);
        }

        for (auto chain : entries)
        {
            this->rainbowTableFile_.write((char *) &chain, RTENTRY_SIZE);
        }

        rainbowTableFile_.close();
    }


}
