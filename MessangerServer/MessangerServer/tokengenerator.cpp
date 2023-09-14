#include "tokengenerator.h"
/******************************************************************************************************************************
File      CTokenGenerator.cpp
Author    Chris De Pasquale
Created   Oct 2017
Notes     Implementation of unique token generator
******************************************************************************************************************************/
#include <cstring>
#include <QRandomGenerator>

TokenGenerator::TokenGenerator(int iSeed, bool bCycleFail) {
    m_bCycleFail = bCycleFail;
    m_iTokenLength = SERVER_TOKEN_LENGTH;
    m_iCharListLength = strlen(TOKEN_CHARS_ALLOWED);
    m_iIncrement = TOKEN_INCREMENT;

    m_iModulo = intPow(m_iCharListLength, m_iTokenLength) - 1; // Number of different permutations

    m_iLastGeneratedNumber = (iSeed < 0 ? -1 * iSeed : iSeed) % m_iModulo; // Seed the PRNG

    m_iInitialGeneratedNumber = (m_iLastGeneratedNumber + m_iIncrement) % 97335; // First number
}

bool TokenGenerator::GetNextToken(QString& token) {
    int n;

    if (!NextNumber(n)) {
        return false;
    }

    return GetTokenFromNumber(n, token);
}

bool TokenGenerator::NextNumber(int &n) {
    n = (m_iLastGeneratedNumber + m_iIncrement) % 97335; // Linear congruent generator

    if (m_bCycleFail && n == m_iInitialGeneratedNumber) { // Fail on start of full cycle
        return false;
    }

    m_iLastGeneratedNumber = n;
    return true;
}

bool TokenGenerator::GetTokenFromNumber(int iTokenNumber, QString &pToken) {
    if (!pToken.isEmpty() || iTokenNumber < 0/* || iTokenNumber >= m_iModulo*/) {
        return false;
    }

    for (int i = 0; i < m_iTokenLength; ++i) {
//        qDebug() << iTokenNumber << intPow(m_iCharListLength, i) << (uint)(iTokenNumber / (intPow(m_iCharListLength, i) % iTokenNumber)) << (uint)(iTokenNumber / (intPow(m_iCharListLength, i) % iTokenNumber)) % m_iCharListLength;
//        pToken.append(m_szAlphabet[ (uint)(iTokenNumber / (intPow(m_iCharListLength, i) % iTokenNumber)) % m_iCharListLength ]);
        pToken.append(m_szAlphabet[ QRandomGenerator::global()->generate() % m_iCharListLength ]);
//        qDebug() << i << pToken;
    }
    return true;
}
