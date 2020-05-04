#include "src/rainbowTableGen.hpp"
#include "src/threadpool.hpp"
#include "src/passwd-utils.hpp"

using namespace std::chrono;
namespace rainbow
{
    RainbowTableGen::RainbowTableGen(const float size):
        size_{size},
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
        const float sizeTableOnByte = getSizeOnBytes(this->size_);
        float size_by_cpu = sizeTableOnByte / num_cpus;
        auto start = high_resolution_clock::now();

        for (unsigned int i = 0; i < num_cpus; ++i)
        {
            filesName[i] = "miniRainbow" + std::to_string(i + 1) + ".txt";
            pool.enqueue(std::mem_fn(&RainbowTableGen::generateMiniTable), this,
                         std::ref(filesName[i]), size_by_cpu);
        }

        pool.join();
        std::vector<RTChain> tempVec;
        tempVec.reserve(ceil(sizeTableOnByte /
                             RTCHAIN_SIZE)); // reserve memory to avoid a couple of allocations
        combineOrderedMiniTableIntoVec(filesName, tempVec);
        writePrecomputedValuesIntoTable(std::move(tempVec));
        tempVec.clear();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        printEndGenerator(duration);
    }

    void RainbowTableGen::generateMiniTable(const std::string &fileName,
                                            float TableSize)
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

        for (float currentSize = 0; currentSize < TableSize;
                        currentSize += RTCHAIN_SIZE,
                        this->currentSizeGeneration_ += RTCHAIN_SIZE)
        {
            buildPrecomputedHashChain(precomputed);
            table.write((char *) &precomputed, RTCHAIN_SIZE);
            printGenProgress(this->currentSizeGeneration_,
                             getSizeOnBytes(size_));
        }

        table.close();
    }


}
