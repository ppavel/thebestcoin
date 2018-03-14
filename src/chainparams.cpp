// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// [Genesis block info]
// START: uncomment when need to mine genesis block
// #include "uint256.h"
// #include "arith_uint256.h"

// #include "consensus/params.h"
// #include "validation.h"
// #include "primitives/block.h"
// #include "validation.h"
// #include "pow.h"
// #include "utilmoneystr.h"
// #include "validationinterface.h"

// #include <algorithm>
// #include <boost/thread.hpp>
// #include <boost/tuple/tuple.hpp>
// #include <queue>
// #include <utility>
// END: uncomment when need to mine genesis block

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

// [Genesis block info]
// START: uncomment when need to mine genesis block
//using namespace std;
// END: uncomment when need to mine genesis block

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlockMain(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "26/02/2018 Japan's Inpex Wins Abu Dhabi Oil Stake";
    const CScript genesisOutputScript = CScript() << ParseHex("04879b33e9940e57ab29c51e992deb19c9b347f379e664aba4db00e590d77368b9bcc62aa88a88b53cacf141b2b3f23f8292fb3d531e93eab744b18d08cab07412") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "08/08/2017 The Normandy Impressionist Festival";
    const CScript genesisOutputScript = CScript() << ParseHex("04879b33e9940e57ab29c51e992deb19c9b347f379e664aba4db00e590d77368b9bcc62aa88a88b53cacf141b2b3f23f8292fb3d531e93eab744b18d08cab07412") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

// [Genesis block info]
// Uncomment these functions to mine the genesis block.
// /**
// * Scans genesis hash.
// *
// * @param pblock
// * @param nNonce
// * @param phash
// * @param networkId
// * @return
// */
// bool static ScanGenesisHash(CBlockHeader *pblock, uint256 *phash, Consensus::Params consensus) {
//    while (true) {
//        ++pblock->nNonce;

//        lyra2re2_hash(BEGIN(pblock->nVersion), BEGIN(*phash));

//        if (CheckProofOfWork(*phash, pblock->nBits, consensus)) {
//            return true;
//        }

//        // Return the nonce if the hash has at least some zero bits,
//        // caller will check if it has enough to reach the target
//        if (((uint16_t *) phash)[15] == 0) {
//            return true;
//        }

//        // If nothing found after trying for a while, return -1
//        if ((pblock->nNonce & 0xfff) == 0) {
//            return false;
//        }

//        return false;
//    }
// }

// /**
// * Mines genesis block.
// *
// * @param genesis
// * @param consensus
// * @param chainparams
// * @param networkId
// */
// static void MineGenesisBlock(CBlock *genesis, Consensus::Params consensus, const CChainParams &chainparams) {
//    // Create genesis block
//    std::string netID = chainparams.NetworkIDString();

//    if (true && genesis->GetHash() != consensus.hashGenesisBlock) {
//        std::cout << "recalculating params for " + netID + "net." << std::endl;
//        std::cout << "old genesis nonce: " << genesis->nNonce << std::endl;
//        std::cout << "old genesis hash: " << consensus.hashGenesisBlock.ToString().c_str() << std::endl;
//        std::cout << "genesis->GetHash(): " << genesis->GetHash().ToString().c_str() << std::endl;

//        std::cout << "Mining of genesis block started" << std::endl;

//        bool fNegative;
//        bool fOverflow;

//        arith_uint256 hashTarget = arith_uint256().SetCompact(genesis->nBits, &fNegative, &fOverflow);
//        uint256 hash;

//        genesis->nNonce = 0;

//        while (true) {
//            if (ScanGenesisHash(genesis, &hash, consensus) & UintToArith256(hash) <= hashTarget) {
//                LogPrintf(
//                        "assert(%s == %s)\n",
//                        hash.GetHex(),
//                        genesis->GetPoWHash().GetHex()
//                );

//                assert(hash == genesis->GetPoWHash());

//                std::cout << "proof-of-work found!" << std::endl;
//                std::cout << "MERKLE ROOT: " << genesis->hashMerkleRoot.GetHex() << std::endl;
//                std::cout << "NONCE: " << genesis->nNonce << std::endl;
//                std::cout << "HASH (hash): " << hash.GetHex() << std::endl;
//                std::cout << "HASH (genesis): " << genesis->GetHash().GetHex() << std::endl;
//                std::cout << "POW HASH (genesis): " << genesis->GetPoWHash().GetHex() << std::endl;
//                std::cout << "TARGET: " << hashTarget.GetHex() << std::endl;

//                if (!(genesis->nNonce & 0xFFFF)) {
//                    std::cout << "\r"
//                              << "Nonce "
//                              << genesis->nNonce
//                              << " hash "
//                              << genesis->GetHash().ToString().c_str()
//                              << std::flush;
//                }

//                break;
//            }
//        }

//        std::cout << std::endl;
//        std::cout << "new " + netID + "net genesis merkle root: " << genesis->hashMerkleRoot.GetHex() << std::endl;
//        std::cout << "new " + netID + "net genesis nonce: " << genesis->nNonce << std::endl;
//        std::cout << "new " + netID + "net genesis hash: " << genesis->GetHash().GetHex() << std::endl;
//        std::cout << "new " + netID + "net genesis POW hash: " << genesis->GetPoWHash().GetHex() << std::endl;
//        std::cout << "new " + netID + "net genesis nTime: " << genesis->nTime << std::endl;
//        std::cout << "generated with difficulty: " << consensus.powLimit.ToString().c_str() << std::endl;
//    }
// }

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 110000; //840000;
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2 * 60; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.nMinNFactor = 10;
        consensus.nMaxNFactor = 30;
        consensus.nChainStartTime = 1519637444;
        consensus.nKGWInterval = 12;
        consensus.fRbfEnabled = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1890; // 75% of 2520
        consensus.nMinerConfirmationWindow = 2520; // 

        // Deployment of BIP65, BIP66, and BIP34.
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nTimeout = 1551173444;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1551173444;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1551173444;

        // The best chain should have at least this much work.
        // @TODO: this parameter is not used for a while until the chain work would get higher. For now validation is done using checkpoints.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000a87058cbe368be6");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xe7;
        pchMessageStart[1] = 0x7e;
        pchMessageStart[2] = 0x26;
        pchMessageStart[3] = 0xa5;
        nDefaultPort = 8801;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlockMain(1519637444, 208868, 0x1e0ffff0, 1, 10000 * COIN);

        // uncomment to mine genesis block
        //MineGenesisBlock(&genesis, consensus, *this);

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256S("0x3b2151763816660310a81a9bf0ff3ef3b55fbaa6247d7511618339c2c0904798"));
        assert(genesis.GetPoWHash() == uint256S("0x000004e8bde6f80bf359d8bff9e127fcc2bb1232169356a4e947c9844d0d54fb"));
        assert(consensus.hashGenesisBlock == uint256S("0xa2248b4862e80521db8e8d2ee90157bc82068bbf75ff7c6f8d2a485ea1b77142"));

        vSeeds.clear();
        vFixedSeeds.clear();

        vSeeds.push_back(CDNSSeedData("dnsseed.thebestcoin.io","dnsseed.thebestcoin.io", false));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,15);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,18);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,96);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0xAD)(0xB5)(0x2C).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0xAD)(0xC3)(0xA6).convert_to_container<std::vector<unsigned char> >();

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, uint256S("0xa2248b4862e80521db8e8d2ee90157bc82068bbf75ff7c6f8d2a485ea1b77142"))
            (10000, uint256S("0x2941fee171c8164a2a5d5822bfc984ac41a01ca4dafb76574c10e1f03eae7c29")),
            1520971592, // * UNIX timestamp of last checkpoint block
            10005,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            450.0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 110000;
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.nMinNFactor = 10;
        consensus.nMaxNFactor = 30;
        consensus.nChainStartTime = 1512002329;
        consensus.nKGWInterval = 12;
        consensus.fRbfEnabled = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 26; // ~51% of 50
        consensus.nMinerConfirmationWindow = 50; // nPowTargetTimespan / nPowTargetSpacing

        // Deployment of BIP65, BIP66, and BIP34.
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nTimeout = 1551173444;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =   1551173444;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1517591350;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1551173444;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        pchMessageStart[0] = 'b';
        pchMessageStart[1] = 'e';
        pchMessageStart[2] = 's';
        pchMessageStart[3] = 't';
        nDefaultPort = 18801;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1512002329, 595553,  0x1e0ffff0, 1, 10000 * COIN);

        // uncomment to mine genesis block
        //MineGenesisBlock(&genesis, consensus, *this);

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256S("0x5f62b7694d6f012aa3d5ec8616a862a9fc28414a39067423275ea9258c3538fc"));
        assert(genesis.GetPoWHash() == uint256S("0x00000f6d27257478a853bcd88f7895aa028a1a0b873393d1db177c7dc266e2ff"));
        assert(consensus.hashGenesisBlock == uint256S("0x3db44171221dfd42b388a65aa654283b66f79a7cc5922cbb841fc370c510b672"));

        vSeeds.clear();
        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,85);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,87);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,26);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3F)(0x23)(0x25)(0x3B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3F)(0x23)(0x26)(0x3A).convert_to_container<std::vector<unsigned char> >();

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, uint256S("0x3db44171221dfd42b388a65aa654283b66f79a7cc5922cbb841fc370c510b672"))
            (23000, uint256S("0xc2b99c138005178418ba3c7f1a43dc95cfeb43df938302f86eebf73ac08aa572")),
            1520968975,
            23027,
            100.0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        //consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        //consensus.BIP34Hash = uint256();
        //consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        //consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1512002328, 2, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x001")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
