#include "src/rainbowTableGen.hpp"
#include "src/threadpool.hpp"
#include "src/passwd-utils.hpp"

using namespace std::chrono;
namespace rainbow
{
    RainbowTableGen::RainbowTableGen(float &&size):
        size_{std::move(size)},
        currentSizeGeneration_{0},
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
        dg::utils::ThreadPool pool(num_cpus, true);
        std::string filesName[num_cpus];
        const int NBCHAIN_TABLE = ceil(getSizeOnBytes(this->size_) / RTCHAIN_SIZE);
        const int NBCHAIN_BY_CPU = NBCHAIN_TABLE / num_cpus;
        auto start = high_resolution_clock::now();
        //First thread takes the rest of nbr entries modulo nbr threads + his share of the work
        filesName[0] = "miniRainbow1.txt";
        pool.enqueue(std::mem_fn(&RainbowTableGen::generateMiniTable), this,
                     std::ref(filesName[0]), NBCHAIN_BY_CPU + NBCHAIN_TABLE % num_cpus);

        for (unsigned int i = 1; i < num_cpus; ++i)
        {
            filesName[i] = "miniRainbow" + std::to_string(i + 1) + ".txt";
            pool.enqueue(std::mem_fn(&RainbowTableGen::generateMiniTable), this,
                         std::ref(filesName[i]), NBCHAIN_BY_CPU);
        }

        pool.join();
        std::vector<RTChain> tempVec;
        tempVec.reserve(NBCHAIN_TABLE); // reserve memory to avoid a couple of allocations
        combineOrderedMiniTableIntoVec(filesName, tempVec);
        writePrecomputedValuesIntoTable(tempVec);
        tempVec.clear();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        printEndGenerator(duration.count());
    }

    void RainbowTableGen::generateMiniTable(const std::string &fileName,
                                            const int nbchain_by_cpu)
    {
        if (nbchain_by_cpu > 0)
        {
            std::fstream table(fileName,
                               std::ios::trunc | std::ios_base::in | std::ios_base::out |
                               std::ios_base::binary);

            if (!table.is_open())
            {
                std::cout << "[ERROR] : Opening file " << fileName << "Thread : " <<
                          std::this_thread::get_id() << std::endl;
                return;
            }

            RTChain precomputed;

            for (int nbChain = 1; nbChain <= nbchain_by_cpu; ++nbChain)
            {
                buildPrecomputedHashChain(precomputed);
                table.write((char *) &precomputed, RTCHAIN_SIZE);
                //printGenProgress(this->currentSizeGeneration_,
                //               getSizeOnBytes(size_));
            }

            table.close();
        }
    }


}
