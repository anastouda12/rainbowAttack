#include "src/rainbowTableGen.hpp"
#include "src/passwd-utils.hpp"
#include "src/threadpool.hpp"

using namespace std::chrono;
namespace rainbow
{


RainbowTableGen::RainbowTableGen(const float size):
    size_{size},
    rainbowTableFile_{}
{}

RainbowTableGen::~RainbowTableGen()
{
    if (this->rainbowTableFile_.is_open())
        this->rainbowTableFile_.close();
}

static void printStartGenerator(float sizeOnByteTable)
{
    std::cout << "[INFO] : Generation table of size +- : " <<
              sizeOnByteTable << " Bytes" << std::endl;
}

static void printGenerationProgression(double currentSize,
                                       double endSize)
{
    std::cout  << std::fixed << std::setprecision(5) << std::setw(17);
    std::cout << (double) ((double) currentSize / (double) endSize) * 100
              << "% \r";
    std::cout.flush();
}

static void printEndGenerator(std::chrono::milliseconds
                              duration)
{
    std::cout << "\r" << std::flush;
    std::cout << "100%... Loading finish !" << std::endl;
    std::cout <<
              "[LOCATION] : RainbowTable was generated on file 'Build/" <<
              FILE_NAME_RTABLE
              << "'"
              << std::endl;
    std::cout << "[DURATION] : Generated on " << std::fixed <<
              std::setprecision(5);
    std::cout << msToHours(duration.count()) << " hours : ";
    std::cout << msToMinute(duration.count()) << " minutes : ";
    std::cout << msToSecond(duration.count()) << " seconds" << std::endl;
}

void noon();
void RainbowTableGen::generateTable()
{
    dg::utils::ThreadPool pool(num_cpus, true);
    std::string filesName[num_cpus];
    const float sizeTableOnByte = getSizeOnBytes(this->size_);
    printStartGenerator(sizeTableOnByte);
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
    writePrecomputedValuesIntoTable(tempVec);
    tempVec.clear();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    printEndGenerator(duration);
}

void RainbowTableGen::buildPrecomputedHashChain(RTChain & rtchain)
const
{
    std::string password = generate_passwd(MAXIMAL_PASSWORD_LENGTH);
    char tail[9];
    calculTail(password, tail);
    strncpy(rtchain.head, password.c_str(), sizeof(RTChain().head));
    strncpy(rtchain.tail, tail, sizeof(RTChain().tail));
}


void RainbowTableGen::generateMiniTable(std::string & fileName,
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
    float currentSize = 0;
    for (currentSize = 0; currentSize < TableSize;
            currentSize += RTCHAIN_SIZE,
            this->currentSizeGeneration_ += RTCHAIN_SIZE)
    {
        buildPrecomputedHashChain(precomputed);
        table.write((char *) &precomputed, RTCHAIN_SIZE);
        printGenerationProgression(this->currentSizeGeneration_,
                                   TableSize * num_cpus);
    }
    table.close();
}

void RainbowTableGen::combineOrderedMiniTableIntoVec(std::string
        filesName[], std::vector<RTChain> & vecToFill)
{
    for (unsigned i = 0; i < num_cpus; ++i)
    {
        std::ifstream table(filesName[i],
                            std::ios_base::in | std::ios_base::binary);
        if (table.is_open())
        {
            RTChain chain;
            unsigned end = table.seekg(0, std::ios::end).tellg() / RTCHAIN_SIZE;

            table.seekg(0, std::ios::beg);

            for (unsigned i = 0; i <= end; i++)
            {
                table.seekg(i * RTCHAIN_SIZE);
                table.read((char *) &chain, RTCHAIN_SIZE);
                insertToPrecomputedOrdered(chain, vecToFill);
            }
            table.close();
        }
    }
}

}
