// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2015 The YiBitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>
#include "arith_uint256.h"
#include "crypto/scrypt.h"

using namespace std;

#include "chainparamsseeds.h"

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
 
 //VER(35):0x23:FG8kgu9976hDirBCvbus2LiJH558ELUGyZ
 
//#define CREATE_GENESIS

class CMainParams : public CChainParams {
protected:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        // Blocks 0 - 144999 are conventional difficulty calculation
        consensus.nSubsidyHalvingInterval = 200000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        //consensus.nPowTargetSpacing = 60; // 1 minute
		consensus.nPowTargetSpacing = 150; // 2.5 minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nAuxpowChainId = 0x0062; // 98 - Josh Wise!
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;
        consensus.nHeightEffective = 0;
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;

        // Blocks 0 - 0 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 0;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        //digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nPowTargetTimespan = 150; // post-digishield: 2.5 minute
        digishieldConsensus.nCoinbaseMaturity = 30;

        // Blocks 0+ are AuxPoW
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 0;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.fAllowAuxPow = true;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x46;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0x43;
        pchMessageStart[3] = 0x59;
        vAlertPubKey = ParseHex("0467180d30cce4738f65395f5d9e679e08fb14d717ec47d9465db8f9baa743dccddfb07463b49764b3596a5b428bb0b1d2d506d2ac26d968d04b3c6ee48aad080e");
        nDefaultPort = 11111;
        nMinerThreads = 0;
        nPruneAfterHeight = 100000;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database.
         *
         main：CBlock(hash=5e72ff644675cff4fede67fd3e03909aa01c09ec51c04bdc42e651dcedfe7e3b, ver=1, 
		 hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, 
		 hashMerkleRoot=33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b, 
		 nTime=1463069999, nBits=1e0ffff0, nNonce=1748189, vtx=1)
		  CTransaction(hash=33e104ceb5, ver=1, vin.size=1, vout.size=1, nLockTime=0)
			CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d01041c595443205969426974636f696e2e636f6d20323031362d30352d3132)
			CTxOut(nValue=25.00000000, scriptPubKey=0467180d30cce4738f65395f5d9e67)

		  vMerkleTree:  33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b
         */
        const char* pszTimestamp = "YTC YiBitcoin.com 2016-05-12";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 25 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0467180d30cce4738f65395f5d9e679e08fb14d717ec47d9465db8f9baa743dccddfb07463b49764b3596a5b428bb0b1d2d506d2ac26d968d04b3c6ee48aad080e") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion.SetGenesisVersion(1);
        genesis.nTime    = 1463069999;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 1748189;
		
#ifdef CREATE_GENESIS
		static bool bCreateGenesis = false;
		if (!bCreateGenesis) {
			bCreateGenesis = true;
			// create genesis block begin -----------------------------
			uint256 hash;		
			bool fNegative;
			bool fOverflow;
			arith_uint256 bnTarget;
			bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
			
			printf("%s：CREATE_GENESIS start ...\n", strNetworkID.c_str());
			while (true) {
				hash = genesis.GetPoWHash();			
				if (UintToArith256(hash) <= bnTarget)
				{
					// Found a solution
					assert(hash == genesis.GetPoWHash());

					LogPrintf("YiBitcoinMiner:\n");
					printf("proof-of-work found  \n  hash: %s  \ntarget: %s\n", hash.GetHex().c_str(), bnTarget.GetHex().c_str());
					break;
				}
				genesis.nNonce++;
			}
			printf("%s：%s\n", strNetworkID.c_str(), genesis.ToString().c_str());
			// create genesis block end
		}
#else
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x5e72ff644675cff4fede67fd3e03909aa01c09ec51c04bdc42e651dcedfe7e3b"));
        assert(genesis.hashMerkleRoot == uint256S("0x33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b"));
#endif

        vSeeds.push_back(CDNSSeedData("seed.yibitcoin.com", "seed.yibitcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed1.yibitcoin.com", "seed1.yibitcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed2.yibitcoin.com", "seed2.yibitcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed3.yibitcoin.com", "seed3.yibitcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed4.yibitcoin.com", "seed4.yibitcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed5.yibitcoin.com", "seed5.yibitcoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,78);  // 0x4e 'Y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,22);  // 0x16
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,158); // 0x9e
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0xfa)(0xca)(0xfd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0xfa)(0xc3)(0x98).convert_to_container<std::vector<unsigned char> >();

        //TODO: fix this for yibitcoin -- plddr
        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vFixedSeeds.clear();

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of

            (0, uint256S("0x5e72ff644675cff4fede67fd3e03909aa01c09ec51c04bdc42e651dcedfe7e3b")),

            0, // * UNIX timestamp of last checkpoint block
            0,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
private:
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        // Blocks 0 - 144999 are pre-Digishield
        consensus.nHeightEffective = 0;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nMajorityEnforceBlockUpgrade = 501;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.fStrictChainId = false;
        consensus.nHeightEffective = 0;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;

        // Reset links before we copy parameters
        consensus.pLeft = NULL;
        consensus.pRight = NULL;

        // Blocks 145000 - 157499 are Digishield without minimum difficulty on all blocks
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 0;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fPowAllowMinDifficultyBlocks = false;
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Blocks 0 - 0 are Digishield with minimum difficulty on all blocks
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = 0;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        // Enable AuxPoW at 0
        auxpowConsensus = minDifficultyConsensus;
        auxpowConsensus.nHeightEffective = 0;
        auxpowConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.fAllowAuxPow = true;

        // Assemble the binary search tree of parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &minDifficultyConsensus;
        minDifficultyConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0x47;
        pchMessageStart[1] = 0x55;
        pchMessageStart[2] = 0x44;
        pchMessageStart[3] = 0x60;
        vAlertPubKey = ParseHex("0467180d30cce4738f65395f5d9e679e08fb14d717ec47d9465db8f9baa743dccddfb07463b49764b3596a5b428bb0b1d2d506d2ac26d968d04b3c6ee48aad080e");
        nDefaultPort = 21111;
        nMinerThreads = 0;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1463069999;
        genesis.nNonce = 201621539;
		
		
		/*test：CBlock(hash=0b782b0bd6127295338e827374f3287fcfcb6592b5cdde23c5ca6685794e6bcb, ver=1, 
		hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, 
		hashMerkleRoot=33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b, 
		nTime=1463069999, nBits=1e0ffff0, nNonce=201621539, vtx=1)
		  CTransaction(hash=33e104ceb5, ver=1, vin.size=1, vout.size=1, nLockTime=0)
			CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d01041c595443205969426974636f696e2e636f6d20323031362d30352d3132)
			CTxOut(nValue=25.00000000, scriptPubKey=0467180d30cce4738f65395f5d9e67)

		  vMerkleTree:  33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b*/
		
#ifdef CREATE_GENESIS
		static bool bCreateGenesis = false;
		if (!bCreateGenesis) {
			bCreateGenesis = true;
			// create genesis block begin -----------------------------
			uint256 hash;		
			bool fNegative;
			bool fOverflow;
			arith_uint256 bnTarget;
			bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
			
			printf("%s：CREATE_GENESIS start ...\n", strNetworkID.c_str());		
			while (true) {
				hash = genesis.GetPoWHash();			
				if (UintToArith256(hash) <= bnTarget)
				{
					// Found a solution
					assert(hash == genesis.GetPoWHash());

					LogPrintf("YiBitcoinMiner:\n");
					printf("proof-of-work found  \n  hash: %s  \ntarget: %s\n", hash.GetHex().c_str(), bnTarget.GetHex().c_str());
					break;
				}
				genesis.nNonce++;
			}
			printf("%s：%s\n", strNetworkID.c_str(), genesis.ToString().c_str());
			// create genesis block end
		}
#else
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x0b782b0bd6127295338e827374f3287fcfcb6592b5cdde23c5ca6685794e6bcb"));
#endif


        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testnet.yibitcoin.com", "testnet.yibitcoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140); // 0x8c
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        //TODO: fix this for yibitcoin -- plddr
        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0b782b0bd6127295338e827374f3287fcfcb6592b5cdde23c5ca6685794e6bcb")),
            0, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
            0        // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;
        consensus.nCoinbaseMaturity = 60; // For easier testability in RPC tests

        // Reset links before we copy parameters
        consensus.pLeft = NULL;
        consensus.pRight = NULL;

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.nPowTargetTimespan = 1; // // regtest: also retarget every second in digishield mode, for conformity
        digishieldConsensus.fDigishieldDifficultyCalculation = true;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.fAllowAuxPow = true;
        auxpowConsensus.nHeightEffective = 20;

        // Assemble the binary search tree of parameters
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        pConsensusRoot = &digishieldConsensus;

        pchMessageStart[0] = 0x48;
        pchMessageStart[1] = 0x56;
        pchMessageStart[2] = 0x45;
        pchMessageStart[3] = 0x61;
        nMinerThreads = 1;
        genesis.nTime = 1463069999;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 3;
		
		/*
		regtest：CBlock(hash=7e3f07820e38a29e1da75123b638009c09567410bc258ee456ef87f4a6e24122, ver=1, 
		hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
		hashMerkleRoot=33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b, 
		nTime=1463069999, nBits=207fffff, nNonce=3, vtx=1)
		  CTransaction(hash=33e104ceb5, ver=1, vin.size=1, vout.size=1, nLockTime=0)
			CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d01041c595443205969426974636f696e2e636f6d20323031362d30352d3132)
			CTxOut(nValue=25.00000000, scriptPubKey=0467180d30cce4738f65395f5d9e67)

		  vMerkleTree:  33e104ceb5aefa2f3a73df5deb67dc213857bbc353201019b7ff7ae293d2415b
		  */
		
#ifdef CREATE_GENESIS
		static bool bCreateGenesis = false;
		if (!bCreateGenesis) {
			bCreateGenesis = true;
			// create genesis block begin -----------------------------
			uint256 hash;		
			bool fNegative;
			bool fOverflow;
			arith_uint256 bnTarget;
			bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
			
			printf("%s：CREATE_GENESIS start ...\n", strNetworkID.c_str());		
			while (true) {
				hash = genesis.GetPoWHash();			
				if (UintToArith256(hash) <= bnTarget)
				{
					// Found a solution
					assert(hash == genesis.GetPoWHash());

					LogPrintf("YiBitcoinMiner:\n");
					printf("proof-of-work found  \n  hash: %s  \ntarget: %s\n", hash.GetHex().c_str(), bnTarget.GetHex().c_str());
					break;
				}
				genesis.nNonce++;
			}
			printf("%s：%s\n", strNetworkID.c_str(), genesis.ToString().c_str());
			// create genesis block end 
		}
#else		
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x7e3f07820e38a29e1da75123b638009c09567410bc258ee456ef87f4a6e24122"));
#endif

        nDefaultPort = 31111;
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x7e3f07820e38a29e1da75123b638009c09567410bc258ee456ef87f4a6e24122")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
