#include "stdafx.h"
#include "Instruction.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "tstring.h"

// 初始化指令库
bool Instruction::m_bInit = false;


const char* skipspace( const char* pStr ) {
	while( *pStr != 0 && (* pStr == ' ' || *pStr == '\t' || *pStr == '\n' || *pStr == '\r') )
		++pStr;
	return pStr;
}
const char* nextWord( const char* pStr ) {
	while( *pStr == ',' || *pStr != ' ' || *pStr == '\t' || *pStr == '\n' || *pStr == '\r'  )
	{
		if( *pStr == 0 )
			break;
		++pStr;
	}
	return pStr;
}


map<unsigned int , INSTRUCTION_CLASS> Instruction::m_insList;


unsigned int hashCode( const char* pStr ) {
	const unsigned int p = 16777619;
	unsigned int hash = 2166136261L;
	for( int i = 0; i < pStr[ i ] != '\0'; i++ )
		hash = ( hash ^ pStr[ i ] ) * p;

	hash += hash << 13;
	hash ^= hash >> 7;
	hash += hash << 3;
	hash ^= hash >> 17;
	hash += hash << 5;
	return hash;
}

Instruction::Instruction( ) 
	:m_insClass( e_none ) , m_operatorCount()
{
	for(auto & i : m_operator)
		i[0] = 0;
	m_mnemonic[ 0 ] = 0;
}

Instruction::Instruction( const char* pIns ) 
	: m_insClass( e_none ) ,
	m_operatorCount( )
{
	for(auto & i : m_operator)
		i[ 0 ] = 0;
	m_mnemonic[ 0 ] = 0;

	setInstruction( pIns );
}



bool Instruction::setInstruction( const char* pIns ) 
{
	if (pIns == nullptr)return false;
	// 截取操作数
	pIns = skipspace( pIns );
		
	const char* pNextWord = nextWord( pIns );
	if( pNextWord - pIns <= MAX_INS )
	{
		strncpy_s( m_mnemonic , pIns , pNextWord - pIns );
		pIns = pNextWord;
	}
	else {
		return false;
	}
	
	
	_strupr( m_mnemonic );
	// 获取操作数
	auto itr = m_insList.find( hashCode( m_mnemonic ) );
	if (itr != m_insList.end())
	{
		m_insClass = itr->second;
	}


	m_operatorCount = 0;
	for( ; *pIns != 0 && m_operatorCount<MAX_OPERAOTCOUNT; ) {
		// mov eax , 1
		pIns = skipspace( pNextWord );
		pNextWord = strstr( pIns , "," );
		if( pNextWord == nullptr ) {
			if( *pIns != 0 ) {
				strcpy_s( m_operator[ m_operatorCount++ ] , pIns );
			} 
			break;
		}

		strncpy_s( m_operator[ m_operatorCount++ ] , pIns , pNextWord - pIns );
		++pNextWord;

		pIns = skipspace(pNextWord);
	}
	
	return true;
}

bool Instruction::initInstructionLibrary( ) 
{
	if (m_bInit)
		return false;
	
	m_bInit = true;

	struct INSSTRUCT{
		const char* pIns;
		INSTRUCTION_CLASS	type;
	}pIns[] =
	{
		{ "MOV" , e_tran } ,
		{ "PUSH" , e_tran } ,
		{ "POP" , e_tran } ,
		{ "SUB" , e_tran } ,
		{ "ADD" , e_tran } ,
		{ "IMUL" , e_tran } ,
		{ "MUL" , e_tran } ,
		{ "DIV" , e_tran } ,
		{ "IDIV" , e_tran } ,
		{ "XCHG" , e_tran } ,
		{ "SHL" , e_bit } ,
		{ "SHR" , e_bit } ,
		{ "ROL" , e_bit } ,
		{ "ROR" , e_bit } ,
		{ "XOR" , e_bit } ,
		{ "AND" , e_bit } ,
		{ "OR" , e_bit } ,
		{ "NOT" , e_bit } ,
		{ "JMP" , e_jmp } ,
		{ "CALL" , e_call } ,
		{"JZ",e_jcc},
		{ "JNZ",e_jcc },
		{ "JE" , e_jcc } ,
		{ "JNE" , e_jcc } ,
		{ "JG" , e_jcc } ,
		{ "JGE" , e_jcc } ,
		{ "JL" , e_jcc } ,
		{ "JLE" , e_jcc } ,
		{ "JA" , e_jcc } ,
		{ "JAE" , e_jcc } ,
		{ "JE" , e_jcc }
	};
	unsigned int hash = 0;
	for(auto & pIn : pIns) {
		hash = hashCode(pIn.pIns);
		m_insList.insert( std::pair<unsigned int , INSTRUCTION_CLASS>(hash, pIn.type ) );
	}

	m_bInit = true;
	return true;
}

INSTRUCTION_CLASS Instruction::checkInstructClass(const char* mnemonic)
{
	if (mnemonic == nullptr)
		return e_none;
	auto itr = m_insList.find(hashCode(mnemonic));
	if (itr == m_insList.end())
		return e_none;
	return itr->second;
}

INSTRUCTION_CLASS Instruction::type( ) const {
	return m_insClass;
}

TString Instruction::instruction( ) const {

	TString buff;

	buff = m_mnemonic;
	buff += ' ';

	int i = 0;
	for( ; i < m_operatorCount - 1; ++i ) {
		buff += m_operator[ i ];
		buff += ',';
	}
	buff += m_operator[ i ];

	return buff;
}

const char* Instruction::mnemonic( ) const {
	return m_mnemonic;
}

const char* Instruction::operator1( ) const {
	return m_operator[ 0 ];
}

const char* Instruction::operator2( ) const {
	return m_operator[ 1 ];
}

const char* Instruction::operator3( ) const {
	return m_operator[ 2 ];
}

bool Instruction::addInstruct( const char* pIns ,INSTRUCTION_CLASS type) {

	char* pBuff = _strdup( pIns );
	pBuff = _strupr( pBuff );
	
	
	auto bRet = m_insList.insert( std::pair<unsigned int , INSTRUCTION_CLASS>( hashCode( pBuff ) , type ) );
	
	free( pBuff );

	return bRet.second;
}

void Instruction::setMnemonic( const char* mnemonic ) {
	strcpy_s( m_mnemonic , mnemonic );
	m_operatorCount = 0;
	m_insClass = checkInstructClass(mnemonic);
}

void Instruction::setOperator1( const char* pOperator ) {
	strcpy_s( m_operator[ 0 ] , pOperator );
	m_operatorCount = 1;
}

void Instruction::setOperator2( const char* pOperator ) {
	strcpy_s( m_operator[ 1 ] , pOperator );
	m_operatorCount = 2;
}

void Instruction::setOperator3( const char* pOperator ) {
	strcpy_s( m_operator[ 2 ] , pOperator );
	m_operatorCount = 3;
}

std::ostream& operator<<(std::ostream& o, const Instruction& ins)
{
	o << ins.mnemonic();

	for (int i =0; i < ins.m_operatorCount - 1; ++i)
	{
		o << ins.m_operator[i]<<',';
	}
	o << "\b ";
	return o;
}

std::ofstream& operator<<(std::ofstream& o, const Instruction& ins)
{
	o << ins.instruction();
	return o;
}

void InstructionGroup::addInstruction(const Instruction& ins)
{
	push_back(ins);
}

TString InstructionGroup::toString() const
{
	TString s;
	for (auto&i : *this)
	{
		s << i.instruction() << ';';
	}
	return s;
}

InstructionGroup::InstructionGroup(const TString& pIns)
{
	TStringList insList;
	insList = pIns.split(';');
	for (auto&i : insList)
	{
		push_back(Instruction(i.c_str()));
	}
}

InstructionGroup::InstructionGroup(const char* pIns)
{
	TStringList insList;
	if (pIns)
	{
		insList = TString(pIns).split(';');
		for (auto&i : insList)
		{
			push_back(Instruction(i.c_str()));
		}
	}
}

