#include "src/rainbowTableGen.hpp"
#include "src/sha256.h"
#include "src/utils.hpp"
#include "src/passwd-utils.hpp"

using namespace std::chrono;
namespace rainbow
{

RainbowTableGen::RainbowTableGen(const float size,
                                 unsigned pwdLength):
    size_{size}, pwdLength_{pwdLength}, reduction_{Reduction(pwdLength)},
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
                              duration, unsigned nbChains)
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
    std::cout << "[STATS] : " << nbChains << " chains generated" <<
              std::endl;
}

void RainbowTableGen::generateTable()
{
    rainbowTableFile_.open(FILE_NAME_RTABLE,
                           std::ios::out | std::ios::binary | std::ios::trunc);
    if (this->rainbowTableFile_.is_open())
    {
        const float sizeTableOnByte = getSizeOnBytes(this->size_);
        float currentSize = 0;
        printStartGenerator(sizeTableOnByte);
        rtChain precomputed;
        unsigned counterEnregistrement = 0;
        std::vector<rtChain> tempVec;
        tempVec.reserve(ceil(sizeTableOnByte /
                             RTCHAIN_SIZE)); // reserve memory to avoid a couple of allocations
        auto start = high_resolution_clock::now();
        for (currentSize = 0; currentSize < sizeTableOnByte;
                currentSize += RTCHAIN_SIZE)
        {
            buildPrecomputedHashChain(precomputed);
            insertToPrecomputedOrdered(precomputed, tempVec);
            counterEnregistrement++;
            printGenerationProgression(currentSize, sizeTableOnByte);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        writePrecomputedValuesIntoTable(tempVec);
        tempVec.clear();
        printEndGenerator(duration, counterEnregistrement);
        this->rainbowTableFile_.close();
    }
    else
    {
        std::cerr << "[ERROR] : Opening file " << FILE_NAME_RTABLE <<
                  " failed" <<
                  std::endl;
    }
}

std::string * RainbowTableGen::calculTail(std::string & password)
const
{
    static std::string tail = password;
    for (unsigned step = 0; step < HASH_LEN; ++step)
    {
        tail = sha256(tail);
        tail = *this->reduction_.reduce(tail, step);

    }
    return &tail;
}

void RainbowTableGen::buildPrecomputedHashChain(
    rtChain & rtchain)
const
{
    std::string password = generate_passwd(random(
            this->pwdLength_, this->pwdLength_));
    strncpy(rtchain.head, password.c_str(), sizeof(rtChain().head));
    strncpy(rtchain.tail, calculTail(password)->c_str(),
            sizeof(rtChain().tail));
}


int RainbowTableGen::findIndexOrdered(const std::string & tail,
                                      std::vector<rtChain> & vec) const
{
    // Searches for tail using the binary search algorithm.
    int min = 0, max = vec.size() - 1;
    int mid = (min + max) / 2, comp;
    while (min <= max)
    {
        comp = tail.compare(vec[mid].tail);
        if (comp == 0)
        {
            return mid;
        }
        else if (comp < 0)
        {
            max = mid - 1;
        }
        else
        {
            min = mid + 1;
        }
        mid = (min + max) / 2;
    }
    return min;
}

}
